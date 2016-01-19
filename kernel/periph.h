//////////////////////////////////////////////////////////////////////////////////////////
/// \file periph.h
/// \author Benjamin Ganty
/// \author Thierry Mourao
/// \date 4 november 2015
/// \brief Definition of functions to write and read into registers of peripherals.
///
/// Implementation of these functions is made in assembly.
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PERIPH_H_
#define _PERIPH_H_

#include "../common/types.h"

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void outb(uint16_t port, uint8_t data)
/// \brief Writes a byte into a peripheral register.
///
/// Writes the 8-bits value of data at the address contained in port.
///
/// \param port : Address of the register.
/// \param data : Data to write (8-bits).
//////////////////////////////////////////////////////////////////////////////////////////
extern void outb(uint16_t port, uint8_t data);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn void outw(uint16_t port, uint16_t data)
/// \brief Writes a 2-bytes value into a peripheral register.
///
/// Writes the 16-bits value of data at the address contained in port.
///
/// \param port : Address of the register.
/// \param data : Data to write (16-bits).
//////////////////////////////////////////////////////////////////////////////////////////
extern void outw(uint16_t port, uint16_t data);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint8_t inb(uint16_t port)
/// \brief Reads a byte from a peripheral register.
///
/// Reads the 8-bits value of the register at the port address and returns it.
///
/// \param port : Address of the register.
///
/// \return 8-bits value of the register.
//////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t inb(uint16_t port);

//////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint16_t inw(uint16_t port)
/// \brief Reads a 2-bytes value from a peripheral register.
///
/// Reads the 16-bits value of the register at the port address and returns it.
///
/// \param port : Address of the register.
///
/// \return 8-bits value of the register.
//////////////////////////////////////////////////////////////////////////////////////////
extern uint16_t inw(uint16_t port);

#endif

