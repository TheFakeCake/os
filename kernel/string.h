//////////////////////////////////////////////////////////////////////////////////////////
/// \file string.h
/// \author Benjamin Ganty
/// \date 16 november 2015
/// \brief Definition of string and memory manipulation functions.
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _STRING_H_
#define _STRING_H_

#include "../common/types.h"

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void *memset(void *dst, int value, uint count)
/// \brief Fills a block of memory.
///
/// Fills the first count bytes of the block of memory pointed by dst with the value
/// (which is interpreted as an unsigned char).
///
/// \param dst : Pointer to the block of memory to fill.
/// \param value : Value to be set in the block of memory. The value is passed as an int
///                but it is casted as an unsigned char by the function.
/// \param count : Number of bytes to fill with the value.
///
/// \return dst is returned.
//////////////////////////////////////////////////////////////////////////////////////////
extern void *memset(void *dst, int value, uint32_t count);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void *memcpy(void *dst, void *src, uint count)
/// \brief Copies a block of memory.
///
/// Copies count bytes from the block of memory pointed by src to the block of memory
/// pointed by dst.
///
/// \param dst : Pointer to the destination block of memory.
/// \param src : Pointer to the source block of memory.
/// \param count : Number of bytes to copy.
///
/// \return dst is returned.
//////////////////////////////////////////////////////////////////////////////////////////
extern void *memcpy(void *dst, void *src, uint32_t count);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn int strncmp(const char *p, const char *q, uint n)
/// \brief Compares characters of two strings.
///
/// Compares up to n characters from the C strings p and q. The comparison ends when two
/// characters don't match, when terminating null-character is found or if n characters
/// have been compared.
///
/// \param p : C string to be compared.
/// \param q : C string to be compared.
/// \param n : Maximum number of characters to compare.
///
/// \return -1 is returned if the first character that differs has a lower value in p.
///          0 is returned if the strings are the same
///          1 is returned if the first character that differs has a higher value in p.
//////////////////////////////////////////////////////////////////////////////////////////
extern int strncmp(const char *p, const char *q, uint32_t n);

#endif

