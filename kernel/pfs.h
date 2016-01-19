//////////////////////////////////////////////////////////////////////////////////////////
/// \file ide.h
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Declaration of structures and functions that interact with a PFS file system.
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PFS_H_
#define _PFS_H_

#include "../common/types.h"

// Typedefs and forward declaration for the structures
typedef struct __attribute__((packed)) Superblock Superblock;
typedef struct __attribute__((packed)) file_iterator_t file_iterator_t;
typedef struct __attribute__((packed)) stat_t stat_t;

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) Superblock
/// \brief Superblock structure.
//////////////////////////////////////////////////////////////////////////////////////////
struct __attribute__((packed)) Superblock
{
    uint8_t  signature[8];       // Signature du système de fichiers (8 bytes) : PFSv0100
    uint32_t sectorsPerBlock;    // Nombre de secteurs par block (32 bits)
    uint32_t bitmapSize;         // Taille du bitmap en blocks (32 bits)
    uint32_t nbFileEntries;      // Nombre de file entries (32 bits)
    uint32_t fileEntrySize;      // Taille d'une file entry en bytes (32 bits)
    uint32_t nbDataBlocks;       // Nombre de data blocks (32 bits)
};

typedef struct __attribute__((packed))
{
    uint8_t  fileName[32];
    uint32_t fileSize;
    uint16_t dataBlocks[];
} FileEntry;

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) file_iterator_t
/// \brief File iterator structure.
///
/// A file_iterator_t is used to iterate over the files of the file system with the
/// next_file() function.
//////////////////////////////////////////////////////////////////////////////////////////
struct __attribute__((packed)) file_iterator_t
{
    uint32_t fileEntriesPerSector;
	uint32_t firstSector;
    uint32_t lastSector;

    uint32_t index;
	uint32_t indexInSector;
	uint32_t sector;

	uint8_t boundToFile;
};

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) stat_t
/// \brief Statistics of a file.
//////////////////////////////////////////////////////////////////////////////////////////
struct __attribute__((packed)) stat_t
{
	uint32_t size;
};

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern void superblock_init()
/// \brief Initialize the superblock.
///
/// This function must be called before using the other functions that interacts with the
/// file system.
//////////////////////////////////////////////////////////////////////////////////////////
extern void superblock_init();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern file_iterator_t find_file(char *fileName)
/// \brief Searches a file in the file system.
///
/// \param fileName : Name of the file to be found.
/// \return A file_iterator_t on the file.
//////////////////////////////////////////////////////////////////////////////////////////
extern file_iterator_t find_file(char *fileName);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern int file_stat(char *filename, stat_t *stat)
/// \brief Returns the statistics of a file.
///
/// \param fileName : Name of the file (string).
/// \param stat : Pointer to a stat_t structure where the results have to be stored.
/// \return 0 on success or -1 if error.
//////////////////////////////////////////////////////////////////////////////////////////
extern int file_stat(char *filename, stat_t *stat);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern int file_read(char *filename, void *buf)
/// \brief Reads a file from the file system.
///
/// \param fileName : Name of the file (string).
/// \param buf : Buffer in which the file is stored.
/// \return 0 on success or -1 if error.
//////////////////////////////////////////////////////////////////////////////////////////
extern int file_read(char *filename, void *buf);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern int file_remove(char *filename)
/// \brief Remove a file from the file system.
///
/// \param fileName : Name of the file to be removed (string).
/// \return 0 on success or -1 if error.
//////////////////////////////////////////////////////////////////////////////////////////
extern int file_remove(char *filename);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern int file_exists(char *filename)
/// \brief Check if a file exists in the file system.
///
/// \param fileName : Name of the file (string).
/// \return 1 if the file exists, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////////////////
extern int file_exists(char *filename);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern file_iterator_t file_iterator()
/// \brief Returns a file_iterator_t on the begining of the file system.
///
/// \return The file_iterator_t initialized at the begining of the file system.
//////////////////////////////////////////////////////////////////////////////////////////
extern file_iterator_t file_iterator();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn extern int file_next(char *filename, file_iterator_t *it)
/// \brief Returns a file_iterator_t on the begining of the file system.
///
/// \return The file_iterator_t initialized at the begining of the file system.
//////////////////////////////////////////////////////////////////////////////////////////
extern int file_next(char *filename, file_iterator_t *it);

#endif
