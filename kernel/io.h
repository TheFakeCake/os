//////////////////////////////////////////////////////////////////////////////////////////
/// \file io.h
/// \author Benjamin Ganty
/// \author Thierry Mourao
/// \author Gérôme Pasquier
/// \date 6 november 2015
/// \brief Declaration of the input / output functions.
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IO_H_
#define _IO_H_

#include "../common/types.h"

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHT_GREY      7
#define DARK_GREY       8
#define LIGHT_BLUE      9
#define LIGHT_GREEN     10
#define LIGHT_CYAN      11
#define LIGHT_RED       12
#define LIGHT_MAGENTA   13
#define YELLOW          14
#define WHITE           15
#define NB_COLORS       16

#define TEXT_DISPLAY_ADDR       0xB8000
#define TEXT_DISPLAY_LINES      25
#define TEXT_DISPLAY_COLUMNS    80
#define TEXT_DISPLAY_SIZE       (TEXT_DISPLAY_LINES * TEXT_DISPLAY_COLUMNS)

#define CURSOR_COMMAND          0x3D4
#define CURSOR_DATA             0x3D5
#define CURSOR_START            0xA
#define CURSOR_END              0xB
#define CURSOR_POSITION_MSB     0xE
#define CURSOR_POSITION_LSB     0xF

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void init_display()
/// \brief Initializes the display components.
///
/// Sets the default colors, clears the screen and initializes the cursor in the top left
/// corner.
//////////////////////////////////////////////////////////////////////////////////////////
extern void init_display();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void clear_display()
/// \brief Clears the display.
///
/// Clears all the characters from the screen and puts the background color everywhere.
/// The cursor position isn't modified.
//////////////////////////////////////////////////////////////////////////////////////////
extern void clear_display();

///////////////////// COLORS HANDLING FUNCTIONS /////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void set_text_color(uint8_t color)
/// \brief Sets the text color.
///
/// Sets the text color for the next calls to printing functions.
///
/// \param color : Text color
//////////////////////////////////////////////////////////////////////////////////////////
extern void set_text_color(uint8_t color);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void set_background_color(uint8_t color)
/// \brief Sets the background color.
///
/// Sets the background color for the next calls to printing functions.
///
/// \param color : Background color
//////////////////////////////////////////////////////////////////////////////////////////
extern void set_background_color(uint8_t color);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void set_colors(uint8_t text, uint8_t background)
/// \brief Sets the text and background color.
///
/// Sets the text background color for the next calls to printing functions.
///
/// \param text : Text color
/// \param background : Background color
//////////////////////////////////////////////////////////////////////////////////////////
extern void set_colors(uint8_t text, uint8_t background);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint8_t get_text_color()
/// \brief Returns the text color.
/// \return The text color.
//////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t get_text_color();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint8_t get_background_color()
/// \brief Returns the background color.
/// \return The background color.
//////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t get_background_color();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void set_cursor_offset(uint16_t offset)
/// \brief Puts the cursor to the given offset.
///
/// Sets the cursor to the given offset. The next printed character will be displayed at
/// the given offset.
/// If the offset is out of the display range, this function will do a scrolling of the
/// screen content.
///
/// \param offset : The offset
//////////////////////////////////////////////////////////////////////////////////////////
extern void set_cursor_offset(uint16_t offset);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint16_t get_cursor_offset()
/// \brief Returns the cursor offset.
///
/// The cursor offset is a value >= 0 and < TEXT_DISPLAY_SIZE .
///
/// \return The cursor offset.
//////////////////////////////////////////////////////////////////////////////////////////
extern uint16_t get_cursor_offset();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void set_cursor_position(uint8_t line, uint8_t column)
/// \brief Puts the cursor to the given position.
///
/// This function transforms the given line / column coordinate to an offset and then
/// calls the set_cursor_offset function.
///
/// \param line : The line
/// \param column : The column
//////////////////////////////////////////////////////////////////////////////////////////
extern void set_cursor_position(uint8_t line, uint8_t column);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void get_cursor_position(uint8_t *line, uint8_t *column)
/// \brief Gives the line / column position of the cursor.
/// \param line : Pointer to the line variable that will be filled.
/// \param column : Pointer to the column variable that will be filled.
//////////////////////////////////////////////////////////////////////////////////////////
extern void get_cursor_position(uint8_t *line, uint8_t *column);

////////////////////// TEXT OUTPUT FUNCTIONS ////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void print_char(char c)
/// \brief Prints a character on the screen.
///
/// Prints the given character on the screen where the cursor is and moves the cursor to
/// the right.
///
/// \param c : The character to be printed.
//////////////////////////////////////////////////////////////////////////////////////////
extern void print_char(char c);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void print_str(char *str)
/// \brief Prints a string on the screen.
///
/// Prints the given string on the screen where the cursor is. This function internally
/// calls print_char with each character of the string.
///
/// \param str : The string to be printed.
//////////////////////////////////////////////////////////////////////////////////////////
extern void print_str(char *str);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void print_int(int32_t n)
/// \brief Prints a signed integer on the screen.
///
/// Prints the given signed integer on the screen where the cursor is.
///
/// \param n : The signed integer to be printed.
//////////////////////////////////////////////////////////////////////////////////////////
extern void print_int(int32_t n);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void print_hex(uint32_t n)
/// \brief Prints an unsigned integer in its hexadecimal format.
///
/// Prints the given unsigned integer on the screen where the cursor is. The integer
/// is interpreted in base 16. The output format is 0x[n].
///
/// \param n : The unsigned integer to be printed.
//////////////////////////////////////////////////////////////////////////////////////////
extern void print_hex(uint32_t n);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void printf(char *frmt, ...)
/// \brief Prints a formated string.
///
/// Parses and prints the frmt string. The frmt string can contain several format codes
/// used to insert variable values into the string. Each format code must match an
/// argument given to the printf function. The order of the arguments must be the same as
/// the order of the format codes in the frmt string.
/// 
/// Here are the supported format codes :
/// 
///     %c : Character
///     %s : String
///     %d : Signed integer
///     %x : Hexadecimal unsigned integer
///     %% : Escape the % character
///
/// \param frmt : The format string.
/// \param ... : Values to be inserted into the string.
//////////////////////////////////////////////////////////////////////////////////////////
extern void printf(char *frmt, ...);

#endif

