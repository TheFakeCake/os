//////////////////////////////////////////////////////////////////////////////////////////
/// \file pfs.h
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 11 december 2015
/// \brief Declaration of structures and functions to interact with a PFS file system.
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PFS_H_
#define _PFS_H_

#include <stdint.h>
#include <stdio.h>

#define SECTOR_SIZE     512
#define FILE_ENTRY_SIZE 256
#define FILE_ENTRY_HEADER_SIZE 36
#define FILE_ENTRY_BYTES_PER_BLOCK_INDEX 2
#define FILE_ENTRY_NB_BLOCK_INDEX ((FILE_ENTRY_SIZE - FILE_ENTRY_HEADER_SIZE) / FILE_ENTRY_BYTES_PER_BLOCK_INDEX)

// Typedefs and forward definition of the structures
typedef struct __attribute__((packed)) Superblock   Superblock;
typedef struct __attribute__((packed)) PFS          PFS;
typedef struct __attribute__((packed)) FileEntry    FileEntry;

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) Superblock
/// \brief Superblock structure.
//////////////////////////////////////////////////////////////////////////////////////////
struct __attribute__((packed)) Superblock
{
    uint8_t     signature[8];       ///< Signature du système de fichiers (8 bytes) : PFSv0100
    uint32_t    sectorsPerBlock;    ///< Nombre de secteurs par block (32 bits)
    uint32_t    bitmapSize;         ///< Taille du bitmap en blocks (32 bits)
    uint32_t    nbFileEntries;      ///< Nombre de file entries (32 bits)
    uint32_t    fileEntrySize;      ///< Taille d'une file entry en bytes (32 bits)
    uint32_t    nbDataBlocks;       ///< Nombre de data blocks (32 bits)
};

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) PFS
/// \brief PFS structure.
///
/// This structure contains several pointers to the element of the file system.
//////////////////////////////////////////////////////////////////////////////////////////
struct __attribute__((packed)) PFS
{
    Superblock *superblock;
    uint8_t    *bitmap;             ///< Bitmap size = Superblock.bitmapSize * Superblock.sectorsPerBlock * SECTOR_SIZE
    FileEntry  *fileEntries;
    uint8_t    *data;
};

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) FileEntry
/// \brief File entry structure.
//////////////////////////////////////////////////////////////////////////////////////////
struct __attribute__((packed)) FileEntry
{
    uint8_t     fileName[32];
    uint32_t    fileSize;
    uint16_t   *dataBlocks;
};

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn PFS* createPFS(Superblock *sb)
/// \brief Create and initialize a PFS.
///
/// The structure is dynamically allocated by this function and returned.
/// The created PFS is empty (no files).
///
/// \param sb : Pointer to the Superblock of the file system.
///             The PFS is initialized following the data from the Superblock.
///
/// \return Pointer to the created PFS structure.
//////////////////////////////////////////////////////////////////////////////////////////
PFS* createPFS(Superblock *sb);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn Superblock* createSuperblock(uint32_t blockSize, uint32_t nbFileEntries, uint32_t nbDataBlocks)
/// \brief Create and initialize a Superblock structure.
///
/// The structure is dynamically allocated by this function and returned.
///
/// \param blockSize : Size of blocks in the file system.
/// \param nbFileEntries : Maximum number of file entries the file system can store.
/// \param nbDataBlocks : Number of data blocks stored in the file system.
///
/// \return Pointer to the created Superblock structure.
//////////////////////////////////////////////////////////////////////////////////////////
Superblock* createSuperblock(uint32_t blockSize, uint32_t nbFileEntries, uint32_t nbDataBlocks);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void destroyPFS(PFS *fs)
/// \brief Destroy a PFS.
///
/// All the memory of the PFS is deallocated, including the Superblock and the fs argument.
///
/// \param fs : Pointer to the PFS to be destroyed.
//////////////////////////////////////////////////////////////////////////////////////////
void destroyPFS(PFS *fs);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn PFS* loadPFS(FILE *file)
/// \brief Load a PFS from a file.
/// \param file : File from which the PFS is loaded. The file must be opened in read binary mode.
/// \return Pointer to the PFS.
//////////////////////////////////////////////////////////////////////////////////////////
PFS* loadPFS(FILE *file);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void writePFS(PFS *fs, FILE *file)
/// \brief Write a PFS into a file.
/// \param fs : Pointer to the PFS to be written.
/// \param file : File into which the PFS is written. The file must be opened in write binary mode.
//////////////////////////////////////////////////////////////////////////////////////////
void writePFS(PFS *fs, FILE *file);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint16_t allocDataBlock(PFS *fs)
/// \brief Allocate a data block from a PFS.
///
/// A free data block is searched in the bitmap of the PFS and its number is returned.
/// If no available data block is found, 0 is returned.
///
/// \param fs : Pointer to the PFS in which the data block should be allocated.
/// \return : The number of the allocated data block or 0 if no data block is free.
//////////////////////////////////////////////////////////////////////////////////////////
uint16_t allocDataBlock(PFS *fs);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn int32_t findFile(PFS *fs, uint8_t* fileName)
/// \brief Find a file into a PFS.
///
/// Search the file entry with the matching fileName and return its index number.
/// If no file is found, -1 is returned.
///
/// \param fs : Pointer to the PFS in which the file is searched.
/// \param fileName : File name (string) to be found in the PFS.
///
/// \return : The index of the FileEntry of the found file. -1 if no matching file.
//////////////////////////////////////////////////////////////////////////////////////////
int32_t findFile(PFS *fs, uint8_t* fileName);

#endif
