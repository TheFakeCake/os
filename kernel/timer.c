//////////////////////////////////////////////////////////////////////////////////////////
/// \file timer.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 24 november 2015
/// \brief Implementation of the timer functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "timer.h"

#include "x86.h"
#include "periph.h"

#define FREQ_MIN 19
#define FREQ_MAX 1193180

//////////////////////////////////// STATIC GLOBALS //////////////////////////////////////

static uint32_t freq = 0;    // Frequence in [Hz]
static uint32_t ticks = 0;   // Ticks counter

//////////////////////////////////////////////////////////////////////////////////////////
void timer_init(uint32_t freq_hz)
{
	// Checks if FREQ_MIN < freq_hz < FREQ_MAX
    if (freq_hz < FREQ_MIN)
    {
		freq_hz = FREQ_MIN;
	}
	else if (freq_hz > FREQ_MAX) 
    {
        freq_hz = FREQ_MAX;
    }
    
	// Sets the divider in the timer peripheral
	uint16_t divider = FREQ_MAX / freq_hz;
	
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divider & 0xFF));
    outb(0x40, (uint8_t)(divider >> 8));

    freq = freq_hz;
    ticks = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void timer_handler()
{
	ticks++;
}

//////////////////////////////////////////////////////////////////////////////////////////
uint32_t get_ticks()
{
	return ticks;
}

//////////////////////////////////////////////////////////////////////////////////////////
void sleep(uint32_t ms)
{
    uint32_t stop = ticks + (ms * freq / 1000);
    while (stop > ticks);
}

