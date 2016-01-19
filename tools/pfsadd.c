//////////////////////////////////////////////////////////////////////////////////////////
/// \file pfsadd.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Source of the pfsadd program.
///
/// pfsadd is used to add a file to a PFS file system image.
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pfs.h"

void printHelp(char *argv[])
{
    printf("How to : %s <image_file> <file>\n", argv[0]);
    printf("With   : image_file : Path to the file system image file.\n");
    printf("         file       : Path to the file to be added in the file system.\n");
}

int main(int argc, char *argv[])
{
    // Check number of arguments
    if (argc < 3)
    {
        printf("Error: Not enough arguments.\n");
        printHelp(argv);
        return 1;
    }

    FILE *fsFile = fopen(argv[1], "r+b");
    FILE *dataFile = fopen(argv[2], "rb");

    // Check arguments validity
    if (fsFile == NULL)
    {
        printf("Error: Couldn't read the file system image file '%s'.\n", argv[1]);
        printHelp(argv);
        return 1;
    }
    if (dataFile == NULL)
    {
        printf("Error: Couldn't read the file '%s'.\n", argv[2]);
        printHelp(argv);
        return 1;
    }

    // Load the file system from the file
    PFS *fs = loadPFS(fsFile);
    uint32_t blockSize = fs->superblock->sectorsPerBlock * SECTOR_SIZE;

    // Check the file name length
    if (strlen(argv[2]) > 31)
    {
        argv[2][31] = '\0';
        printf("Warning: The file name is to long and has been truncated to '%s'\n", argv[2]);
    }

    // Check if file name already exists
    if (findFile(fs, (uint8_t*)argv[2]) != -1)
    {
        printf("Error: File name '%s' already exists.\n", argv[2]);
        return 1;
    }

    // Find an empty file entry in the file system
    uint32_t fileEntryIndex;
    for (fileEntryIndex = 0; fileEntryIndex < fs->superblock->nbFileEntries; fileEntryIndex++)
    {
        if (fs->fileEntries[fileEntryIndex].fileName[0] == 0)
        {
            break;
        }
    }

    // If no file entry is available
    if (fileEntryIndex == fs->superblock->nbFileEntries)
    {
        printf("Error: No file entry available.\n");
        return 1;
    }

    // Load the data file into the file system
    FileEntry *fe = &(fs->fileEntries[fileEntryIndex]);
    memcpy(fe->fileName, argv[2], strlen(argv[2]) + 1);
    fe->fileSize = 0;

    int maxNbDataBlocks = (fs->superblock->fileEntrySize - FILE_ENTRY_HEADER_SIZE) / FILE_ENTRY_BYTES_PER_BLOCK_INDEX;

    for (int i = 0; ! feof(dataFile); i++)
    {
        // Check if there still is an available data block in the file entry
        if (i == maxNbDataBlocks)
        {
            printf("Error: File is to big.\n");
            return 1;
        }

        // Allocate a data block from the file system
        uint16_t dataBlockIndex = allocDataBlock(fs);
        if (dataBlockIndex == 0)
        {
            printf("Error: Not enough data blocks available.\n");
            return 1;
        }
        // Add its index to the file entry
        fe->dataBlocks[i] = dataBlockIndex;

        // Copy the the data file into the file system
        fe->fileSize += fread(fs->data + dataBlockIndex * blockSize, 1, blockSize, dataFile);
    }

    // Write the updated file system
    rewind(fsFile);
    writePFS(fs, fsFile);

    // Free the resources
    fclose(dataFile);
    fclose(fsFile);
    destroyPFS(fs);

    return 0;
}
