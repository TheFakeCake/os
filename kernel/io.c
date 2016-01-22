//////////////////////////////////////////////////////////////////////////////////////////
/// \file io.c
/// \author Benjamin Ganty
/// \author Thierry Mourao
/// \author Gérôme Pasquier
/// \date 6 november 2015
/// \brief Implementation of the input / output functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "io.h"
#include "periph.h"
#include "../common/common_io.h"

//////////////////////////////////// STATIC GLOBALS //////////////////////////////////////

static uint16_t *display = (uint16_t*)TEXT_DISPLAY_ADDR; ///< Pointer to the display memory
static uint8_t text_color;          ///< Color for the printed text
static uint8_t background_color;    ///< Background color for the printed text
static uint16_t cursor_offset;      ///< Cursor offset

/////////////////////////////////// STATIC FUNCTIONS /////////////////////////////////////

// Scrolls the display content n lines upward.
static void scroll_up(int n)
{
    // Content scrolling
    for (int i = 0; i < TEXT_DISPLAY_SIZE - n * TEXT_DISPLAY_COLUMNS; i++)
    {
        display[i] = display[i + n * TEXT_DISPLAY_COLUMNS];
    }
    // Clears the screen where new lines appeared
    for (int i = TEXT_DISPLAY_SIZE - n * TEXT_DISPLAY_COLUMNS; i < TEXT_DISPLAY_SIZE; i++)
    {
        display[i] = (text_color << 8) | (background_color << 12);
    }
}

/////////////////////////////////// DISPLAY FUNCTIONS ////////////////////////////////////

void init_display()
{
    // Default colors
    set_colors(WHITE, BLACK);

    // Clearing the screen
	clear_display();

	// Initializing the cursor
	uint8_t data;

	outb(CURSOR_COMMAND, CURSOR_START);
	data = inb(CURSOR_DATA);
	data &= ~(0b111111);
	outb(CURSOR_DATA, data);

	outb(CURSOR_COMMAND, CURSOR_END);
	data = inb(CURSOR_DATA);
	data |= 0b11111;
	outb(CURSOR_DATA, data);

	set_cursor_offset(0);
}

void clear_display()
{
    for (uint16_t i = 0; i < TEXT_DISPLAY_SIZE * 2; i++)
    {
        display[i] = (text_color << 8) | (background_color << 12);
    }
}

//////////////////////////////// COLORS HANDLING FUNCTIONS ///////////////////////////////

void set_text_color(uint8_t color)
{
    if (color < NB_COLORS)
    {
        text_color = color;
    }
}

void set_background_color(uint8_t color)
{
    if (color < NB_COLORS)
    {
        background_color = color;
    }
}

void set_colors(uint8_t text, uint8_t background)
{
    set_text_color(text);
    set_background_color(background);
}

uint8_t get_text_color()
{
    return text_color;
}

uint8_t get_background_color()
{
    return background_color;
}

///////////////////////////////// CURSOR HANDLING FUNCTIONS //////////////////////////////

void set_cursor_offset(uint16_t offset)
{
    // If the offset is outside the display range => scrolling
    if (offset >= TEXT_DISPLAY_SIZE)
    {
        int nb_new_lines = (offset - TEXT_DISPLAY_SIZE + 1) / TEXT_DISPLAY_COLUMNS + 1;
        scroll_up(nb_new_lines);
        cursor_offset = TEXT_DISPLAY_SIZE - TEXT_DISPLAY_COLUMNS + offset % TEXT_DISPLAY_COLUMNS;
    }
    else
    {
        cursor_offset = offset;
    }

    // Sets the cursor to the new position
    outb(CURSOR_COMMAND, CURSOR_POSITION_MSB);
    outb(CURSOR_DATA, (uint8_t)(cursor_offset >> 8));

    outb(CURSOR_COMMAND, CURSOR_POSITION_LSB);
    outb(CURSOR_DATA, (uint8_t)(cursor_offset & 0xFF));
}

uint16_t get_cursor_offset()
{
    return cursor_offset;
}

void set_cursor_position(uint8_t line, uint8_t column)
{
    set_cursor_offset(line * TEXT_DISPLAY_COLUMNS + column);
}

void get_cursor_position(uint8_t *line, uint8_t *column)
{
    *line = cursor_offset / TEXT_DISPLAY_COLUMNS;
    *column = cursor_offset % TEXT_DISPLAY_COLUMNS;
}

/////////////////////////////////// TEXT OUTPUT FUNCTIONS ////////////////////////////////

void print_char(char c)
{
    uint16_t new_cursor_offset;

    // Checks for special characters
    switch (c)
    {
    case '\n':  // Line feed
        new_cursor_offset = cursor_offset + (TEXT_DISPLAY_COLUMNS - cursor_offset % TEXT_DISPLAY_COLUMNS);
        break;

    case '\r':  // Carriage return
        new_cursor_offset = cursor_offset - (cursor_offset % TEXT_DISPLAY_COLUMNS);
        break;

    case '\t':  // Tabulation
        new_cursor_offset = cursor_offset + 8 - (cursor_offset + 8) % 8;
        break;

    case '\b':  // Backspace
        if (cursor_offset != 0)
        {
            new_cursor_offset = cursor_offset - 1;
            display[new_cursor_offset] = (text_color << 8) | (background_color << 12);
        }
        break;

    default:    // Standard character
        display[cursor_offset] = (uint16_t)c | (text_color << 8) | (background_color << 12);
	    new_cursor_offset = cursor_offset + 1;
	    break;
    }
	set_cursor_offset(new_cursor_offset);
}

void print_str(char *str)
{
    // Prints each character of the string
    while (*str != '\0')
    {
        print_char(*str);
        str++;
    }
}

void print_int(int32_t n)
{
    if (n < 0)
    {
        print_char('-');
        print_int(-n);
    }
    else
    {
        int upperRank = n / 10;
        if (upperRank)
        {
            print_int(upperRank);
        }
        print_char((char)(n % 10 + 48));
    }
}

void print_hex(uint32_t n)
{
    int upperRank = n / 16;
    int unit = n % 16;

    if (upperRank)
    {
        print_hex(upperRank);
    }
    else
    {
        print_str("0x");
    }

    // Prints the right character (digit or letter)
    if (unit >= 0 && unit <= 9)
    {
        print_char((char)(unit + 48));
    }
    else
    {
        print_char((char)(unit + 55));
    }
}

void printf(char *frmt, ...)
{
    __genericPrintFormat(print_char, print_str, &frmt);
}
