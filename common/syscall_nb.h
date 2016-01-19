#ifndef _SYSCALL_NB_H_
#define _SYSCALL_NB_H_

typedef enum {
    SYSCALL_PUTC = 0,
    SYSCALL_PUTS,
    SYSCALL_EXEC,
    SYSCALL_GETC,
    SYSCALL_FILE_STAT,
    SYSCALL_FILE_READ,
    SYSCALL_FILE_REMOVE,
    SYSCALL_FILE_ITERATOR,
    SYSCALL_FILE_NEXT,
    SYSCALL_GET_TICKS,
    SYSCALL_SLEEP,
    SYSCALL_CLEAR_SCREEN,
    SYSCALL_SET_CURSOR,

    __SYSCALL_END__
} syscall_t;

#endif