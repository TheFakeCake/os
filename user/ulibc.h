#ifndef _ULIB_H_
#define _ULIB_H_

#include "../common/types.h"
#include "../common/string.h"

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) file_iterator_t
/// \brief File iterator structure.
///
/// A file_iterator_t is used to iterate over the files of the file system with the
/// next_file() function.
//////////////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((packed))
{
    uint32_t fileEntriesPerSector;
    uint32_t firstSector;
    uint32_t lastSector;

    uint32_t index;
    uint32_t indexInSector;
    uint32_t sector;

    uint8_t boundToFile;
} file_iterator_t;

//////////////////////////////////////////////////////////////////////////////////////////
/// \struct __attribute__((packed)) stat_t
/// \brief Statistics of a file.
//////////////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((packed))
{
    uint32_t size;
} stat_t;

//Fonctions d'accès aux fichiers
int read_file(char *filename, uchar *buf);
int get_stat(char *filename, stat_t *stat);
int remove_file(char *filename);
file_iterator_t get_file_iterator();
int get_next_file(char *filename, file_iterator_t *it);

//Fonctions de contrôle de processus (tâche) :
int exec(char *filename);
extern void exit();

//Fonctions d'entrées/sorties :
int getc();
void gets(char* buffer);
void putc(char c);
void puts(char *str);
void printf(char *frmt, ...);
void clear_display();
void set_cursor(int ligne, int colonne);

//Fonctions liées au temps :
void sleep(uint ms);
uint get_ticks();

#endif
