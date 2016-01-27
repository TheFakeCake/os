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
extern int read_file(char *filename, uchar *buf);
extern int get_stat(char *filename, stat_t *stat);
extern int remove_file(char *filename);
extern file_iterator_t get_file_iterator();
extern int get_next_file(char *filename, file_iterator_t *it);

//Fonctions de contrôle de processus (tâche) :
extern int exec(char *filename);
extern void exit();

//Fonctions d'entrées/sorties :
extern int getc();
extern unsigned int gets(char *buffer, unsigned int bufferSize);
extern void putc(char c);
extern void puts(char *str);
extern void printf(char *frmt, ...);
extern void clear_display();
extern void set_cursor(int ligne, int colonne);

//Fonctions liées au temps :
extern void sleep(uint ms);
extern uint get_ticks();

#endif
