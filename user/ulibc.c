//////////////////////////////////////////////////////////////////////////////////////////
/// \file ulibc.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 22 janvier 2015
/// \brief Implementation of the ulibc functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "ulibc.h"

#include "../common/syscall_nb.h"
#include "../common/common_io.h"

extern int syscall(uint32_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

//////////////////////////////////////////////////////////////////////////////////////////
int read_file(char *filename, uchar *buf)
{
	return syscall(SYSCALL_FILE_READ, (uint32_t) filename, (uint32_t) buf, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
int get_stat(char *filename, stat_t *stat)
{
	return syscall(SYSCALL_FILE_STAT, (uint32_t) filename, (uint32_t) stat, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
int remove_file(char *filename)
{
	return syscall(SYSCALL_FILE_REMOVE, (uint32_t) filename, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
file_iterator_t get_file_iterator()
{
	file_iterator_t result;
	syscall(SYSCALL_FILE_ITERATOR, (uint32_t) &result, 0, 0, 0);
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////
int get_next_file(char *filename, file_iterator_t *it)
{
	return syscall(SYSCALL_FILE_NEXT, (uint32_t) filename, (uint32_t) it, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
int exec(char *filename)
{
	return syscall(SYSCALL_EXEC, (uint32_t) filename, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
int getc()
{
	return syscall(SYSCALL_GETC, 0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int gets(char *buffer, unsigned int bufferSize)
{
	char input;
    unsigned int bufferCursor = 0;

	// Wait for a character input while the buffer isn't full and RETURN isn't pressed
    while (bufferCursor < bufferSize - 1 && (input = getc()) != '\n')
    {
		// Ignore tab character and
        if (input == '\t')
        {
            continue;
        }

		// If backspace is pressed
        if (input == '\b')
        {
			// Don't remove character if none was entered
			if (bufferCursor == 0)
			{
				continue;
			}
			else
			{
	            bufferCursor--;
			}
        }
        else
        {
        	buffer[bufferCursor++] = input;
        }

		putc(input);
    }

	// Print the new line
	putc('\n');

	// Add the end of string character
    buffer[bufferCursor] = 0;

	// Return the number of entered characters
	return bufferCursor;
}

//////////////////////////////////////////////////////////////////////////////////////////
void putc(char c)
{
	syscall(SYSCALL_PUTC, (uint32_t) c, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void puts(char *str)
{
	syscall(SYSCALL_PUTS, (uint32_t) str, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void printf(char *frmt, ...)
{
	__genericPrintFormat(putc, puts, &frmt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void clear_display()
{
    syscall(SYSCALL_CLEAR_SCREEN, 0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void set_cursor(int ligne, int colonne)
{
	syscall(SYSCALL_SET_CURSOR, (uint32_t) ligne, (uint32_t) colonne, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void sleep(uint ms)
{
	syscall(SYSCALL_SLEEP, (uint32_t) ms, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
uint get_ticks()
{
	return syscall(SYSCALL_GET_TICKS, 0, 0, 0, 0);
}
