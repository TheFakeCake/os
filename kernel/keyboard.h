//////////////////////////////////////////////////////////////////////////////////////////
/// \file keyboard.h
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 24 november 2015
/// \brief Declaration of the keyboard  functions.
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void keyboard_init()
/// \brief Initializes the keyboard.
///
/// This function doesn't do anything.
//////////////////////////////////////////////////////////////////////////////////////////
extern void keyboard_init();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void keyboard_handler()
/// \brief Keyboard interruption routine.
///
/// This function is called when a keyboard interruption occurs. It fills the keyboard
/// buffer with typed characters.
//////////////////////////////////////////////////////////////////////////////////////////
extern void keyboard_handler();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn char getc()
/// \brief Returns a typed character.
///
/// Waits until a character is typed then returns it.
//////////////////////////////////////////////////////////////////////////////////////////
extern char getc();

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void keyboard_flush_buffer()
/// \brief Clears the keaboard buffer.
//////////////////////////////////////////////////////////////////////////////////////////
extern void keyboard_flush_buffer();

#endif
