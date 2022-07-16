#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "graphics.h"

#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

Screen *m_screen = NULL;

// ------------------------------------------------------------------
// DRAW FUNCTIONS
// ------------------------------------------------------------------

void draw_char(int x, int y, char value)
{
    if (x >= 0 && x < m_screen -> width && y >= 0 && y < m_screen -> height)
    {
        m_screen -> pixels[y][x] = value;
    }
}

void draw_line(int x1, int y1, int x2, int y2, char value)
{
    if (x1 == x2)
    {
        // Draw vertical line
        int y_min = MIN(y1, y2);
        int y_max = MAX(y1, y2);

        for (int i = y_min; i <= y_max; i++)
        {
            draw_char(x1, i, value);
        }
    }
    else if (y1 == y2)
    {
        // Draw horizontal line
        int x_min = MIN(x1, x2);
        int x_max = MAX(x1, x2);

        for (int i = x_min; i <= x_max; i++)
        {
            draw_char(i, y1, value);
        }
    }
    else
    {
        // Draw Diagonal Lines
        float dx = x2 - x1;
        float dy = y2 - y1;
        float err = 0.0;
        float derr = ABS(dy / dx);

        for (int x = x1, y = y1; (dx > 0) ? x <= x2 : x >= x2; (dx > 0) ? x++ : x--)
        {
            draw_char(x, y, value);
            err += derr;
            while (err >= 0.5 && ((dy > 0) ? y <= y2 : y >= y2))
            {
                draw_char(x, y, value);
                y += (dy > 0) - (dy < 0);

                err -= 1.0;
            }
        }
    }
}

// ------------------------------------------------------------------
// SCREEN FUNCTIONS
// ------------------------------------------------------------------

static void create_buffer(Screen **old_screen, int width, int height, char value)
{
    Screen *new_screen = calloc(1, sizeof(Screen));

    // Set Screen Dimensions
    new_screen -> width = width;
    new_screen -> height = height;

    // Allocate memory to screen buffer
    void **buffer = calloc(height, sizeof(void *));
    buffer[0] = calloc(width * height, sizeof(char));

    for (int y = 1; y < height; y++)
    {
        buffer[y] = (char *) buffer[y - 1] + width * sizeof(char);
    }

    // Fill allocated memory with space/blank character
    memset(buffer[0], value, width * height * sizeof(char));
    new_screen -> pixels = (char **) buffer;

    *old_screen = new_screen;
}

void setup_screen(void) 
{
    // Enter curses mode
    initscr();

    // Do not echo keypresses
    noecho();

    // Turn off cursor
    curs_set(0);

    // Clear potentially lingering state of screen
    clear();

    // Create screen buffers
    create_buffer(&m_screen, getmaxx(stdscr), getmaxy(stdscr), ' ');
}

void show_screen(void)
{
    char **new_pixels = m_screen -> pixels;
    int width = m_screen -> width;
    int height = m_screen -> height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            mvaddch(y, x, new_pixels[y][x]);
        }
    }

    refresh();
}