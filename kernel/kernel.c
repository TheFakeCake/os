//////////////////////////////////////////////////////////////////////////////////////////
/// \file kernel.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 24 november 2015
/// \brief Implementation of the kernel main function.
//////////////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"

#include "gdt.h"
#include "idt.h"
#include "periph.h"
#include "io.h"
#include "pic.h"
#include "x86.h"
#include "keyboard.h"
#include "timer.h"
#include "pfs.h"

#ifdef TEST
#include "test.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////
void runKernel()
{
    // Initializing the GDT
    gdt_init();

    // Initializing the screen
    init_display();

    // Initializing the interruption controler (PIC)
    pic_init();

    // Initializing the IDT
    idt_init();

    // Initializing timer @ 100Hz
    timer_init(100);

    // Initializing the keyboard);
    keyboard_init();

    // Init the file system superblock
    superblock_init();

    // Enables hardware interruptions
    sti();

    #ifdef TEST

    // Runs the test procedure if test mode is enabled
    runFileSystemTests();

    #else

    // Executing the Shell
    exec_task("shell");

    #endif

    halt();
}
