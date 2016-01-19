//////////////////////////////////////////////////////////////////////////////////////////
/// \file pfslist.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Source of the pfslist program.
///
/// pfslist is used to show a list of the files stored in a PFS file system image.
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pfs.h"

void printHelp(char *argv[])
{
    printf("How to : %s <image_file>\n", argv[0]);
    printf("With   : image_file : Path to the file system image file.\n");
}

int main(int argc, char *argv[])
{
    // Check number of arguments
    if (argc < 2)
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

    // Iterate over each file entry
    for (uint32_t fileEntryIndex = 0; fileEntryIndex < fs->superblock->nbFileEntries; fileEntryIndex++)
    {
        // If the file entry is used
        if (fs->fileEntries[fileEntryIndex].fileName[0] != 0)
        {
            printf("%s %d [byte(s)]\n", fs->fileEntries[fileEntryIndex].fileName, fs->fileEntries[fileEntryIndex].fileSize);
        }
    }

    // Free the resources
    fclose(fsFile);
    destroyPFS(fs);

    return 0;
}
