#include "common_io.h"

#include "string.h"

void __genericPrintFormat(void (*printChar)(char), void (*printString)(char*), char **frmtAddr)
{
    // Pointer to the current argument
    uint32_t *args = (uint32_t*)(frmtAddr) + 1;

    // Format string
    char *frmt = *frmtAddr;

	// Buffer for itoa
	char buffer[33];

    // Prints the format string
    while (*frmt != '\0')
    {
        // If a format code is found
        if (*frmt == '%')
        {
            frmt++;
            switch (*frmt)
            {
            case '%': // Escapes the % character
                printChar('%');
                break;

            case 'c': // Inserts a character
                printChar(*((char*)args));
                args++;
                break;

            case 's': // Inserts a string
                printString(*((char**)args));
                args++;
                break;

			case 'o': // Inserts an unsigned octal integer
                printString(itoa(*((int*)args), buffer, 8));
                args++;
                break;

            case 'd': // Inserts a signed integer
                printString(itoa(*((int*)args), buffer, 10));
                args++;
                break;

            case 'x': // Inserts an unsigned hexadecimal integer
				printString(itoa(*((int*)args), buffer, 16));
                args++;
                break;

            default:
                break;
            }
        }
        else
        {
            printChar(*frmt);
        }
        frmt++;
    }
}
