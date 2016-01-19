//////////////////////////////////////////////////////////////////////////////////////////
/// \file keyboard.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 24 november 2015
/// \brief Implementation of the keyboard functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "keyboard.h"

#include "../common/types.h"
#include "x86.h"
#include "io.h"
#include "periph.h"

#define SHIFT_CODE  0x2A
#define BUFFER_SIZE 2048

//////////////////////////////////// STATIC GLOBALS //////////////////////////////////////

static char buffer[BUFFER_SIZE];
static uint16_t read_pointer = 0;
static uint16_t write_pointer = 0;

static char swissKeyboardShift[] = "--+\"*c%&/()=?`\b\tQWERTZUIOPu!\n-ASDFGHJKLoa?-?YXCVBNM;:_--- -------------------";
static char swissKeyboard[] =       "--1234567890'^\b\tqwertzuiope?\n-asdfghjklea--$yxcvbnm,.---- -------------------";

/////////////////////////////////// STATIC FUNCTIONS /////////////////////////////////////

// Translates a key code to a character using the swiss keyboard layout.
static inline char keyboardToChar(uint8_t key_code, bool shift)
{
    if (!shift)
        return swissKeyboard[key_code];
    else
        return swissKeyboardShift[key_code];
}

// Adds a character to the keyboard buffer.
static void putCharInBuffer(char c)
{
    // If the buffer is full, prints an error message
    if (read_pointer == (write_pointer + 1) % BUFFER_SIZE)
    {
        uint8_t text_color = get_text_color();
        uint8_t background_color = get_background_color();
        set_colors(RED, BLACK);
        printf("\r\n***Keyboard buffer is full***");
        set_colors(text_color, background_color);
        printf("\r\n");
    }
    else
    {
        buffer[write_pointer] = c;
        write_pointer = (write_pointer + 1) % BUFFER_SIZE;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void keyboard_init() { }

//////////////////////////////////////////////////////////////////////////////////////////
void keyboard_flush_buffer()
{
    read_pointer = write_pointer = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void keyboard_handler()
{
    static bool shift = false;
    uint8_t key_code = inb(0x60);

    // If key is released (break code)
    if (key_code & (1 << 7))
    {
        // If "shift" is released
        if (key_code == (SHIFT_CODE + (1 << 7)))
        {
            shift = false;
        }
    }
    else // Else if key is pressed
    {
        // If "shift" is pressed
        if (key_code == SHIFT_CODE)
        {
            shift = true;
        }
        else // Else another key is pressed
        {
            char c = keyboardToChar(key_code, shift);

            // Prints only the authorized characters
            if (c != '-' || key_code == 0x35)
            {
                putCharInBuffer(c);

                #ifdef DEBUG
                printf("%c",c);
                #endif
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
char getc()
{
    // Wait while the buffer is empty
    while (read_pointer == write_pointer);

    char c = buffer[read_pointer];
    read_pointer = (read_pointer + 1) % BUFFER_SIZE;

    return c;
}
