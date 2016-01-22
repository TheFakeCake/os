#include "ulibc.h"

#include "../common/syscall_nb.h"
#include "../common/common_io.h"

extern int syscall(uint32_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

//Fonctions d'accès aux fichiers
int read_file(char *filename, uchar *buf)
{
	return syscall(SYSCALL_FILE_READ, (uint32_t) filename, (uint32_t) buf, 0, 0);
}
int get_stat(char *filename, stat_t *stat)
{
	return syscall(SYSCALL_FILE_STAT, (uint32_t) filename, (uint32_t) stat, 0, 0);
}
int remove_file(char *filename)
{
	return syscall(SYSCALL_FILE_REMOVE, (uint32_t) filename, 0, 0, 0);
}
file_iterator_t get_file_iterator()
{
	file_iterator_t result;
	syscall(SYSCALL_FILE_ITERATOR, (uint32_t) &result, 0, 0, 0);
	return result;
}
int get_next_file(char *filename, file_iterator_t *it)
{
	return syscall(SYSCALL_FILE_NEXT, (uint32_t) filename, (uint32_t) it, 0, 0);
}

//Fonctions de contrôle de processus (tâche) :
int exec(char *filename)
{
	return syscall(SYSCALL_EXEC, (uint32_t) filename, 0, 0, 0);
}

//Fonctions d'entrées/sorties :
int getc()
{
	return syscall(SYSCALL_GETC, 0, 0, 0, 0);
}

void gets(char* buffer)
{
    int indexbuffer = 0;
    while((buffer[indexbuffer] = getc()) != '\n')
    {
        if((buffer[indexbuffer] == '\b' && indexbuffer == 0) || (buffer[indexbuffer] == '\t'))
        {
            buffer[indexbuffer] = 0;
            continue;
        }

        putc(buffer[indexbuffer]);

        if(buffer[indexbuffer] == '\b' && indexbuffer != 0)
        {
            buffer[indexbuffer] = 0;
            indexbuffer--;
        }
        else
        {
        indexbuffer++;
        }
    }
    buffer[indexbuffer] = 0;
    putc('\n');
}

void putc(char c)
{
	syscall(SYSCALL_PUTC, (uint32_t) c, 0, 0, 0);
}
void puts(char *str)
{
	syscall(SYSCALL_PUTS, (uint32_t) str, 0, 0, 0);
}
void printf(char *frmt, ...)
{
	__genericPrintFormat(putc, puts, &frmt);
}

void clear_display()
{
    syscall(SYSCALL_CLEAR_SCREEN, 0, 0, 0, 0);
}

void set_cursor(int ligne, int colonne)
{
	syscall(SYSCALL_SET_CURSOR, (uint32_t) ligne, (uint32_t) colonne, 0, 0);
}

//Fonctions liées au temps :
void sleep(uint ms)
{
	syscall(SYSCALL_SLEEP, (uint32_t) ms, 0, 0, 0);
}
uint get_ticks()
{
	return syscall(SYSCALL_GET_TICKS, 0, 0, 0, 0);
}
