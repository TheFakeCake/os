//////////////////////////////////////////////////////////////////////////////////////////
/// \file pfscreate.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Source of the pfscreate program.
///
/// pfscreate is used to create an empty PFS image file.
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include "pfs.h"

void printHelp(char *argv[])
{
    printf("How to : %s <image_file> <block_size> <nb_file_entries> <nb_data_blocks>\n", argv[0]);
    printf("With   : image_file      : Path to the image file to create.\n");
    printf("         block_size      : Size of blocks in bytes (greater than zero and divisible by 512).\n");
    printf("         nb_file_entries : Number of file entries that can exists in the file system (greater than zero).\n");
    printf("         nb_data_blocks  : Number of blocks in which data can be stored (greater than zero).\n");
}

int main(int argc, char *argv[])
{
    // Check number of arguments
    if (argc < 5)
    {
        printf("Error: Not enough arguments.\n");
        printHelp(argv);
        return 1;
    }

    // Retreive arguments
    FILE *file = fopen(argv[1], "wb");
    uint32_t blockSize = atoi(argv[2]);
    uint32_t nbFileEntries = atoi(argv[3]);
    uint32_t nbDataBlocks = atoi(argv[4]);

    // Check arguments validity
    if (file == NULL)
    {
        printf("Error: Couldn't create a file.\n");
        printHelp(argv);
        return 1;
    }
    if (blockSize <= 0 || blockSize % SECTOR_SIZE > 0)
    {
        printf("Error: block_size must be greater than zero and divisible by 512.\n");
        printHelp(argv);
        return 1;
    }
    if (nbFileEntries <= 0)
    {
        printf("Error: nb_file_entries must be greater than zero.\n");
        printHelp(argv);
        return 1;
    }
    if (nbDataBlocks <= 0)
    {
        printf("Error: nb_data_blocks must be greater than zero.\n");
        printHelp(argv);
        return 1;
    }

    // Create an empty PFS and write it into a file
    Superblock *sb = createSuperblock(blockSize, nbFileEntries, nbDataBlocks);
    PFS *fs = createPFS(sb);
    writePFS(fs, file);

    // Free the resources
    fclose(file);
    destroyPFS(fs);

    return 0;
}
