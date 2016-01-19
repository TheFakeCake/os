// To avoid warnings if an argument is not used.
// Example:
// void f(int arg) {
//     UNUSED(arg);
// }

#include "io.h"
#include "keyboard.h"
#include "pfs.h"
#include "timer.h"
#include "gdt.h"
#include "../common/types.h"
#include "../common/syscall_nb.h"

#define UNUSED(x) ((void)(x))

int syscall_putc(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    UNUSED(task_addr);

    print_char((char) arg1);
    return 0;
}

int syscall_puts(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);

    print_str((char*)(task_addr + arg1));
    return 0;
}

int syscall_exec(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);

    return exec_task((char*)(task_addr + arg1));
}

int syscall_getc(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg1);
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    UNUSED(task_addr);

    return (int) getc();
}

int syscall_file_stat(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg3);
    UNUSED(arg4);

    return file_stat((char*)(task_addr + arg1), (stat_t*)(task_addr + arg2));
}

int syscall_file_read(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg3);
    UNUSED(arg4);

    return file_read((char*)(task_addr + arg1), (void*)(task_addr + arg2));
}

int syscall_file_remove(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr) {
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);

    return file_remove((char*)(task_addr + arg1));
}

int syscall_file_iterator(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);

    *((file_iterator_t*)(task_addr + arg1)) = file_iterator();
    return 0;
}

int syscall_file_next(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg3);
    UNUSED(arg4);

    return file_next((char*)(task_addr + arg1), (file_iterator_t*)(task_addr + arg2));
}

int syscall_get_ticks(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg1);
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    UNUSED(task_addr);

    return get_ticks();
}

int syscall_sleep(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    UNUSED(task_addr);

    sleep(arg1);
    return 0;
}

int syscall_clear_screen(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg1);
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    UNUSED(task_addr);

    clear_display();
    return 0;
}

int syscall_set_cursor(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t task_addr)
{
    UNUSED(arg3);
    UNUSED(arg4);
    UNUSED(task_addr);

    set_cursor_position((uint8_t) arg1, (uint8_t) arg2);
    return 0;
}

// Table containing pointers to all the syscall functions
int (*syscall_functions[__SYSCALL_END__])(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) = {
    syscall_putc,
    syscall_puts,
    syscall_exec,
    syscall_getc,
    syscall_file_stat,
    syscall_file_read,
    syscall_file_remove,
    syscall_file_iterator,
    syscall_file_next,
    syscall_get_ticks,
    syscall_sleep,
    syscall_clear_screen,
    syscall_set_cursor
};

// System call handler: call the appropriate system call according to the nb argument.
// Called by the assembly code _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t caller_tss_selector)
{
    if (nb >= __SYSCALL_END__)
    {
        return -1;
    }
    return syscall_functions[nb](arg1, arg2, arg3, arg4, get_task(caller_tss_selector)->addr);
}
