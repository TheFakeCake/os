//////////////////////////////////////////////////////////////////////////////////////////
/// \file pfs.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Implementation of the PFS functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "pfs.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////////
PFS* createPFS(Superblock *sb)
{
    uint32_t blockSize = sb->sectorsPerBlock * SECTOR_SIZE;

    // Allocation of the PFS
    PFS *fs = malloc(sizeof(PFS));

    // Set the suberblock
    fs->superblock = sb;

    // Allocation of the bitmap
    fs->bitmap = calloc(sb->bitmapSize * blockSize, sizeof(uint8_t));

    // Alocation of the files entries
    fs->fileEntries = calloc(sb->nbFileEntries, sizeof(FileEntry));
    for (int i = 0; i < sb->nbFileEntries; i++)
    {
        fs->fileEntries[i].dataBlocks = calloc(sb->fileEntrySize - FILE_ENTRY_HEADER_SIZE, sizeof(uint8_t));
    }
    // Allocation of data blocks
    fs->data = calloc(sb->nbDataBlocks * blockSize, sizeof(uint8_t));

    return fs;
}

//////////////////////////////////////////////////////////////////////////////////////////
Superblock* createSuperblock(uint32_t blockSize, uint32_t nbFileEntries, uint32_t nbDataBlocks)
{
    Superblock *sb = malloc(sizeof(Superblock));

    memcpy(sb->signature, "PFSv0100", 8);
    sb->sectorsPerBlock = blockSize / SECTOR_SIZE;
    sb->bitmapSize      = (uint32_t)ceil(ceil(nbDataBlocks / 8.0) / blockSize);
    sb->nbFileEntries   = nbFileEntries;
    sb->fileEntrySize   = FILE_ENTRY_SIZE;
    sb->nbDataBlocks    = nbDataBlocks;

    return sb;
}

//////////////////////////////////////////////////////////////////////////////////////////
void destroyPFS(PFS *fs)
{
    for (int i = 0; i < fs->superblock->nbFileEntries; i++)
    {
        free(fs->fileEntries[i].dataBlocks);
    }
    free(fs->fileEntries);
    free(fs->bitmap);
    free(fs->data);
    free(fs->superblock);
    free(fs);
}

//////////////////////////////////////////////////////////////////////////////////////////
PFS* loadPFS(FILE *file)
{
    PFS *fs = malloc(sizeof(PFS));
    fs->superblock = malloc(sizeof(Superblock));

    // Load the superblock
    fread(fs->superblock, sizeof(Superblock), 1, file);

    uint32_t blockSize = fs->superblock->sectorsPerBlock * SECTOR_SIZE;

    // Put the cursor at the begining of the second block and load the bitmap
    fseek(file, blockSize, SEEK_SET);
    fs->bitmap = malloc(fs->superblock->bitmapSize * blockSize);
    fread(fs->bitmap, blockSize, fs->superblock->bitmapSize, file);

    // Load the file entries
    fs->fileEntries = malloc(sizeof(FileEntry) * fs->superblock->nbFileEntries);
    for (int i = 0; i < fs->superblock->nbFileEntries; i++)
    {
        fs->fileEntries[i].dataBlocks = malloc(fs->superblock->fileEntrySize - FILE_ENTRY_HEADER_SIZE);
        fread(&(fs->fileEntries[i]), FILE_ENTRY_HEADER_SIZE, 1, file);
        fread(fs->fileEntries[i].dataBlocks, sizeof(uint8_t), fs->superblock->fileEntrySize - FILE_ENTRY_HEADER_SIZE, file);
    }
    fseek(file, (1 + fs->superblock->bitmapSize +
                (uint32_t)ceil(fs->superblock->nbFileEntries * fs->superblock->fileEntrySize / (double)blockSize))
                * blockSize, SEEK_SET);

    // Load data blocks
    fs->data = malloc(fs->superblock->nbDataBlocks * blockSize);
    fread(fs->data, blockSize, fs->superblock->nbDataBlocks, file);

    return fs;
}

//////////////////////////////////////////////////////////////////////////////////////////
void writePFS(PFS *fs, FILE *file)
{
    uint32_t blockSize = fs->superblock->sectorsPerBlock * SECTOR_SIZE;
    uint8_t *padding = calloc(blockSize, sizeof(uint8_t));

    // Write the superblock (padding needs to be added after to make a full block)
    fwrite(fs->superblock, sizeof(Superblock), 1, file);
    fwrite(padding, sizeof(uint8_t), blockSize - sizeof(Superblock), file);

    // Write the bitmap
    fwrite(fs->bitmap, sizeof(uint8_t), fs->superblock->bitmapSize * blockSize, file);

    // Write the file entries (padding may be added to make a full block)
    for (int i = 0; i < fs->superblock->nbFileEntries; i++)
    {
        fwrite(fs->fileEntries[i].fileName, sizeof(uint8_t), 32, file);
        fwrite(&(fs->fileEntries[i].fileSize), sizeof(uint32_t), 1, file);
        fwrite(fs->fileEntries[i].dataBlocks, sizeof(uint8_t), fs->superblock->fileEntrySize - FILE_ENTRY_HEADER_SIZE, file);
    }
    // Add padding if the last block of file entries isn't full
    if (fs->superblock->nbFileEntries * fs->superblock->fileEntrySize % blockSize > 0)
    {
        fwrite(padding, sizeof(uint8_t),
            blockSize - fs->superblock->nbFileEntries * fs->superblock->fileEntrySize % blockSize, file);
    }

    // Write the data blocks
    fwrite(fs->data, sizeof(uint8_t), fs->superblock->nbDataBlocks * blockSize, file);

    free(padding);
}

//////////////////////////////////////////////////////////////////////////////////////////
uint16_t allocDataBlock(PFS *fs)
{
    uint16_t byte = 0;
    uint16_t bit = 6;

    // Iterate over each bit of the bitmap, ignoring the first one (data block 0)
    while (byte * 8 + (7 - bit) < fs->superblock->nbDataBlocks)
    {
        // Jump to the next byte if the current one is full
        if (fs->bitmap[byte] == 0xFF)
        {
            byte++;
            continue;
        }
        // If the bit is 0 (free), allocate the block
        if ((fs->bitmap[byte] & (1 << bit)) == 0)
        {
            fs->bitmap[byte] |= (1 << bit);
            return byte * 8 + (7 - bit);
        }
        // Move the cursors
        if (bit == 0)
        {
            byte++;
            bit = 7;
        }
        else
        {
            bit--;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
int32_t findFile(PFS *fs, uint8_t* fileName)
{
    // Iterate over each file entry
    for (int32_t i = 0; i < fs->superblock->nbFileEntries; i++)
    {
        // If the file name matches
        if (strcmp((char*)fileName, (char*)(fs->fileEntries[i].fileName)) == 0)
        {
            return i;
        }
    }
    return -1;
}
