#include <curses.h>
#include "graphics.h"

int main(void) 
{
    setup_screen();

    draw_line(20, 20, 10, 10, '*');

    show_screen();
    getch();

    endwin();
}