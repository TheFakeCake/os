//////////////////////////////////////////////////////////////////////////////////////////
/// \file ide.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Implementation of functions that interact with a PFS file system.
//////////////////////////////////////////////////////////////////////////////////////////

#include "pfs.h"

#include "ide.h"
#include "../common/string.h"
#include "io.h"

//////////////////////////////////// STATIC GLOBALS //////////////////////////////////////

static Superblock sb;

/////////////////////////////////// STATIC FUNCTIONS /////////////////////////////////////

static inline uint32_t ceil(uint32_t a, uint32_t b)
{
    uint32_t q = a / b;
    if (a % b == 0)
    {
        return q;
    }
    return q + 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
void superblock_init()
{
    // Read the first sector and copy it into the Superblock
    uint8_t buffer[SECTOR_SIZE];
    read_sector(0, buffer);
    memcpy(&sb, buffer, sizeof(Superblock));
}

//////////////////////////////////////////////////////////////////////////////////////////
file_iterator_t find_file(char *fileName)
{
    file_iterator_t it = file_iterator();
    char name[32];

    // Iterate over the files
    while (file_next(name, &it))
    {
        // Return the iterator if the file is found
        if (strncmp(fileName, name, 32) == 0)
        {
            return it;
        }
    }
    return it;
}

//////////////////////////////////////////////////////////////////////////////////////////
int file_stat(char *filename, stat_t *stat)
{
    file_iterator_t it = find_file(filename);

    // If the file isn't found, return an error
    if (! it.boundToFile)
    {
        return -1;
    }

    // Read the file entry sector
    uint8_t buffer[SECTOR_SIZE];
    read_sector(it.sector, buffer);

    // Retreive the file size from the file entry
    stat->size = *((uint32_t*)(buffer + it.indexInSector * sb.fileEntrySize + 32));

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
int file_read(char *filename, void *buf)
{
    file_iterator_t it = find_file(filename);

    // If the file isn't found, return an error
    if (! it.boundToFile)
    {
        return -1;
    }

    // Read the sector of the file entry
    uint8_t fileEntrySector[SECTOR_SIZE];
    read_sector(it.sector, fileEntrySector);

    // Retrieve the file entry
    FileEntry *fe = (FileEntry*)&fileEntrySector[it.indexInSector * sb.fileEntrySize];

    // Calculate the number of sectors and data blocks that are used by the file
    uint32_t nbSectors = ceil(fe->fileSize, SECTOR_SIZE);
    uint32_t nbBlocks = ceil(nbSectors, sb.sectorsPerBlock);

    uint8_t buffer[SECTOR_SIZE];
    uint16_t blockIndex;
    uint32_t j;

    // Iterate over the blocks
    for (uint32_t i = 0, sectorCount = 0; i < nbBlocks; i++)
    {
        blockIndex = fe->dataBlocks[i] + 1 + sb.bitmapSize
                   + ceil(sb.nbFileEntries * sb.fileEntrySize, SECTOR_SIZE * sb.sectorsPerBlock);

        // Iterate over the sectors, AVOIDING THE LAST ONE
        for (j = 0; j < sb.sectorsPerBlock && sectorCount < nbSectors - 1; j++, sectorCount++)
        {
            read_sector(blockIndex * sb.sectorsPerBlock + j, buffer);
            memcpy(buf, buffer, SECTOR_SIZE);
            buf += SECTOR_SIZE;
        }
    }
    // Read the last sector
    uint32_t lastSectorSize = SECTOR_SIZE;
    if (fe->fileSize % SECTOR_SIZE > 0)
    {
        lastSectorSize = fe->fileSize % SECTOR_SIZE;
    }
    read_sector(blockIndex * sb.sectorsPerBlock + j, buffer);
    memcpy(buf, buffer, lastSectorSize);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
int file_remove(char *filename)
{
    file_iterator_t it = find_file(filename);

    // If the file isn't found, return an error
    if (! it.boundToFile)
    {
        return -1;
    }

    // Read the sector of the file entry
    uint8_t fileEntrySector[SECTOR_SIZE];
    read_sector(it.sector, fileEntrySector);

    // Retrieve the file entry
    FileEntry *fe = (FileEntry*)&fileEntrySector[it.indexInSector * sb.fileEntrySize];

    // Calculate the number of sectors and data blocks that are used by the file
    uint32_t nbSectors = ceil(fe->fileSize, SECTOR_SIZE);
    uint32_t nbBlocks = ceil(nbSectors, sb.sectorsPerBlock);

    // Clears the file entry and writes it
    fe->fileName[0] = 0;
    write_sector(it.sector, fileEntrySector);

    // Deallocate the data blocks in the bitmap
    uint8_t buffer[SECTOR_SIZE];
    for (uint32_t i = 0; i < nbBlocks; ++i)
    {
        // Read the bitmap sector
        read_sector(sb.sectorsPerBlock + fe->dataBlocks[i] / (8 * SECTOR_SIZE * sb.sectorsPerBlock), buffer);

        // Edit the bit
        uint16_t byte = buffer[fe->dataBlocks[i] / 8];
        byte ^= 1 << (7 - (fe->dataBlocks[i] % 8));
        buffer[fe->dataBlocks[i] / 8] = byte;

        // Write the bitmap sector
        write_sector(sb.sectorsPerBlock + fe->dataBlocks[i] / (8 * SECTOR_SIZE * sb.sectorsPerBlock), buffer);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
int file_exists(char *filename)
{
    file_iterator_t it = find_file(filename);
    return it.boundToFile;
}

//////////////////////////////////////////////////////////////////////////////////////////
file_iterator_t file_iterator()
{
    file_iterator_t it;

    it.fileEntriesPerSector = SECTOR_SIZE / sb.fileEntrySize;
    it.firstSector = (1 + sb.bitmapSize) * sb.sectorsPerBlock;
    it.lastSector = it.firstSector + ceil(sb.nbFileEntries, it.fileEntriesPerSector);

    it.index = 0;
    it.indexInSector = 0;
    it.sector = it.firstSector;

    it.boundToFile = 0;

    return it;
}

//////////////////////////////////////////////////////////////////////////////////////////
int file_next(char *filename, file_iterator_t *it)
{
    uint8_t buffer[SECTOR_SIZE];

    // If the iterator is already bound to a file, increments its index
    if (it->boundToFile)
    {
        it->index++;

        if (++(it->indexInSector) == it->fileEntriesPerSector)
        {
            it->indexInSector = 0;
            it->sector++;
        }
    }

    // Iterate over the remaining sectors of file entries
    while (it->sector <= it->lastSector)
    {
        read_sector(it->sector, buffer);

        // Iterate over the file entries in the sector
        while (it->indexInSector < it->fileEntriesPerSector && it->index < sb.nbFileEntries)
        {
            // If the file entry is used
            if (buffer[it->indexInSector * sb.fileEntrySize] != 0)
            {
                memcpy(filename, (char*)&buffer[it->indexInSector * sb.fileEntrySize], 32);
                it->boundToFile = 1;
                return 1;
            }
            it->index++;
            it->indexInSector++;
        }

        it->indexInSector = 0;
        it->sector++;
    }

    // Reset the iterator
    it->index = 0;
    it->sector = it->firstSector;
    it->boundToFile = 0;

    return 0;
}
