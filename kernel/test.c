//////////////////////////////////////////////////////////////////////////////////////////
/// \file test.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 10 december 2015
/// \brief Implementation of test functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "test.h"

#include "io.h"
#include "keyboard.h"
#include "pfs.h"
#include "timer.h"

//////////////////////////////////////////////////////////////////////////////////////////
void runTests()
{
    printf("The testing procedure begins ...");
    sleep(300);
    printf(" NOW !\r\n\n");
    sleep(200);

    printf("Testing colors :\r\n");
    printf("----------------\r\n\n");
    sleep(200);

    printf("Function : set_text_color\r\n");
    printf("You should see all the available colors for characters :\r\n");
    for (int i = 0; i < NB_COLORS; i++)
    {
        set_text_color(i);
        printf("%d ", i);
    }
    set_text_color(WHITE);
    sleep(800);

    printf("\r\n\nFunction : set_background_color\r\n");
    printf("You should see all the available colors for the background :\r\n");
    for (int i = 0; i < NB_COLORS; i++)
    {
        set_background_color(i);
        printf(" ");
    }
    set_background_color(BLACK);
    sleep(800);

    printf("\r\n\nFunction : set_color, get_text_color, get_background_color\r\n");
    set_colors(GREEN, DARK_GREY);
    printf("The colors of this text are : %d (text=GREEN) and %d (background=DARK_GREY)\r\n",
           get_text_color(), get_background_color());
    set_colors(WHITE, BLACK);
    sleep(800);

    printf("\r\nTesting cursor :\r\n");
    printf("----------------\r\n\n");
    sleep(200);

    printf("Function : get_cursor_offset, set_cursor_position\r\n");
    printf("The line numbers are now displayed at the end of each line\r\n");
    uint8_t l, c;
    int oldOffset = get_cursor_offset();
    for (l = 0; l < TEXT_DISPLAY_LINES; l++)
    {
        set_cursor_position(l, TEXT_DISPLAY_COLUMNS - 3);
        print_int(l);
    }
    sleep(800);

    set_cursor_offset(oldOffset);
    printf("\r\nFunction : set_cursor_offset, get_cursor_position\r\n");
    get_cursor_position(&l, &c);
    printf("The first character of this line was on line %d at column %d when printed\r\n", l, c);
    sleep(800);

    printf("\r\nTesting print functions :\r\n");
    printf("-----------------------------\r\n\n");
    sleep(200);

    printf("Function : print_char, print_str, print_int, print_hex, printf\r\n");
    printf("This is a character (=i) : %c\r\nThis is a string (=toto) : %s\r\n", 'i', "toto");
    printf("This is an integer (=1337) : %d\r\nThis is an hex integer (=0xAB42) : %x\r\n", 1337, 0xAB42);
    printf("This is a percentage : 10%%\r\n");
    sleep(800);

    printf("\r\nTests are over. Because of the scrolling, you should now see some lines without their numbers on the right.");
}

//////////////////////////////////////////////////////////////////////////////////////////
void runFileSystemTests()
{
    printf("The testing procedure for the file system begins ...\n\n");

    char name[32];

    printf("Function : file_iterator, file_next, file_stat\n\n");
    printf("Listing all the files in the file system :\n");
    sleep(2000);

    file_iterator_t it = file_iterator();
    stat_t stat;
    while (file_next(name, &it))
    {
        file_stat(name, &stat);
        printf("%s\t%d [Bytes]\n", name, stat.size);
    }
    sleep(1000);

    printf("\r\nFunction : file_exists\n\n");
    printf("Testing if the file 'Nope.txt' exists. It should NOT ! :\n");
    sleep(2000);

    if (file_exists("Nope.txt"))
    {
        printf("The file exists !\n");
    }
    else
    {
        printf("File not found.\n");
    }
    sleep(1000);

    printf("\nFunction : file_read\n\n");
    printf("Reading the file 'image.txt'. The word KERNEL should appear as ASCII art.\n\n");
    sleep(2000);

    uint8_t buffer[3000] = {0};
    file_read("image.txt", buffer);
    printf("%s\n", buffer);
    sleep(1000);

    printf("Function : file_remove\n\n");
    printf("Removing the file 'image.txt' from the file system. New list :\n");
    sleep(2000);

    file_remove("image.txt");
    it = file_iterator();
    while (file_next(name, &it))
    {
        file_stat(name, &stat);
        printf("%s\t%d [Bytes]\n", name, stat.size);
    }

    printf("\nEnd of testing procedure.");
}
