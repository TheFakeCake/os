//////////////////////////////////////////////////////////////////////////////////////////
/// \file pfsdel.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Source of the pfsdel program.
///
/// pfsdel is used to remove a file from a PFS file system image.
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pfs.h"

void printHelp(char *argv[])
{
    printf("How to : %s <image_file> <file>\n", argv[0]);
    printf("With   : image_file : Path to the file system image file.\n");
    printf("         file       : Nae of the file to be deleted in the file system.\n");
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

    // Check arguments validity
    if (fsFile == NULL)
    {
        printf("Error: Couldn't read the file system image file '%s'.\n", argv[1]);
        printHelp(argv);
        return 1;
    }

    // Load the file system from the file
    PFS *fs = loadPFS(fsFile);

    // Check if the file to be deleted exists
    uint32_t fileEntryIndex = findFile(fs, (uint8_t*)argv[2]);
    if (fileEntryIndex == -1)
    {
        printf("Warning: The file '%s' doesn't exist.\n", argv[2]);
        return 1;
    }

    FileEntry *fe = &(fs->fileEntries[fileEntryIndex]);

    // Set the first byte of the file entry to 0
    fe->fileName[0] = 0;

    // Iterate over each data block index of the file
    for (uint32_t i = 0; fe->dataBlocks[i] != 0 && i * 2 < fs->superblock->fileEntrySize - FILE_ENTRY_HEADER_SIZE; i++)
    {
        fs->bitmap[fe->dataBlocks[i] / 8] ^= 1 << (7 - (fe->dataBlocks[i] % 8));
        fe->dataBlocks[i] = 0;
    }

    // Write the updated file system
    rewind(fsFile);
    writePFS(fs, fsFile);

    // Free the resources
    fclose(fsFile);
    //destroyPFS(fs);

    return 0;
}
