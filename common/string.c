//////////////////////////////////////////////////////////////////////////////////////////
/// \file string.c
/// \author Benjamin Ganty
/// \date 16 november 2015
/// \brief Implementation of string and memory manipulation functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "string.h"

//////////////////////////////////////////////////////////////////////////////////////////
void *memset(void *dst, int value, uint32_t count)
{
    while (count--)
    {
        ((unsigned char*)dst)[count] = (unsigned char)value;
    }
    return dst;
}

//////////////////////////////////////////////////////////////////////////////////////////
void *memcpy(void *dst, void *src, uint32_t count)
{
    while (count--)
    {
        ((unsigned char*)dst)[count] = ((unsigned char*)src)[count];
    }
    return dst;
}

//////////////////////////////////////////////////////////////////////////////////////////
int strncmp(const char *p, const char *q, uint32_t n)
{
    for (uint i = 0; i < n; i++)
    {
        if (p[i] != q[i])
        {
            return (p[i] < q[i] ? -1 : 1);
        }
        else if (p[i] == '\0')
        {
            return 0;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
extern int strcmp(const char *str1, const char *str2)
{
    int len_str1 = strlen(str1);
    int len_str2 = strlen(str2);

    if(len_str1 == len_str2)
    {
        for (int i = 0; i < len_str2; ++i)
        {
            if(str1[i] != str2[i])
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
uint32_t strlen(const char *str)
{
	uint32_t i;
	for (i = 0; str[i] != '\0'; i++);
    return i;
}

//////////////////////////////////////////////////////////////////////////////////////////
void split(char* str, char c, char* tab_str, int nb_str, int size_str)
{
    int j = 0;
    for (int i = 0; i < nb_str; ++i)
    {
        int k = 0;
        while (str[j] != 0)
        {
        	if (str[j] == c)
        	{
        		j++;
        		break;
        	}
            tab_str[i * size_str + k] = str[j];
            j++;
            k++;
        }
        tab_str[i * size_str + k] = 0;
    }
}
