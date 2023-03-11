#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "graphics.h"

#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)

Screen *m_screen = NULL;

// ------------------------------------------------------------------
// DRAW FUNCTIONS
// ------------------------------------------------------------------

void draw_char(int x, int y, char value, int colour)
{
    if (x >= 0 && x < m_screen -> width && y >= 0 && y < m_screen -> height)
    {
        m_screen -> pixels[y][x] = value;
        m_screen -> colours[y][x] = colour;
    }
}

void draw_line(int x1, int y1, int x2, int y2, char value, int colour)
{
    if (x1 == x2)
    {
        // Draw vertical line
        int y_min = MIN(y1, y2);
        int y_max = MAX(y1, y2);

        for (int i = y_min; i <= y_max; i++)
        {
            draw_char(x1, i, value, colour);
        }
    }
    else if (y1 == y2)
    {
        // Draw horizontal line
        int x_min = MIN(x1, x2);
        int x_max = MAX(x1, x2);

        for (int i = x_min; i <= x_max; i++)
        {
            draw_char(i, y1, value, colour);
        }
    }
    else
    {
        // Draw Diagonal Lines
        if (x1 > x2) 
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        float dx = x2 - x1;
        float dy = y2 - y1;
        float err = 0.0f;
        float derr = ABS(dy / dx);

        for (int x = x1, y = y1; (dx > 0) ? x <= x2 : x >= x2; (dx > 0) ? x++ : x--)
        {
            draw_char(x, y, value, colour);
            err += derr;
            while (err >= 0.5f && ((dy > 0) ? y <= y2 : y >= y2))
            {
                draw_char(x, y, value, colour);
                y += (dy > 0) - (dy < 0);

                err -= 1.0f;
            }
        }
    }
}

void draw_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, int colour)
{
    draw_line(x1, y1, x2, y2, value, colour);
    draw_line(x2, y2, x3, y3, value, colour);
    draw_line(x3, y3, x1, y1, value, colour);
}

void fill_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, int colour)
{
    int a, b, y, last;
    // Sort coordinates by in desc order of y (y3 >= y2 >= y1)
    if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y2 > y3) { SWAP(y3, y2); SWAP(x3, x2); }
    if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }

     // All y coordinates on same line
    if (y1 == y3) {
        a = b = x1;
        if (x2 < a)
        {
            a = x2;
        }
        else if (x2 > b)
        {
            b = x2;
        }

        if (x3 < a)
        {
            a = x3;
        }
        else if (x3 > b) 
        {
            b = x3;
        }
        draw_line(a, y1, b, y1, value, colour);
        return;
    }

    // Find scanline crossings for segment 0-1 and 0-2
    // If y1 = y2 the scanline y is included here otherwise 
    // scanline y1 is skipped and handled
    if(y2 == y3)
    {
        last = y2;
    }
    else 
    {
        last = y2 - 1;
    }

    for(y = y1; y <= last; y++) {
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        draw_line(a, y, b, y, value, colour);
    }

    // For lower part of triangle, find scanline crossings for segment
    // 0-2 and 1-2.  This loop is skipped if y1=y2
    for(; y <= y3; y++) {
        a = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
        b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        draw_line(a, y, b, y, value, colour);
    }
}

// ------------------------------------------------------------------
// SCREEN FUNCTIONS
// ------------------------------------------------------------------

static void create_buffer(Screen **old_screen, int width, int height, char value, int colour)
{
    Screen *new_screen = calloc(1, sizeof(Screen));

    // Set Screen Dimensions
    new_screen -> width = width;
    new_screen -> height = height;

    // Allocate memory to screen buffer
    void **pixel_buffer = calloc(height, sizeof(void *));
    void **colour_buffer = calloc(height, sizeof(void *));
    pixel_buffer[0] = calloc(width * height, sizeof(char));
    colour_buffer[0] = calloc(width * height, sizeof(int));

    for (int y = 1; y < height; y++)
    {
        pixel_buffer[y] = (char *) pixel_buffer[y - 1] + width * sizeof(char);
        colour_buffer[y] = (char *) colour_buffer[y - 1] + width * sizeof(int);
    }

    // Fill allocated memory with space/blank character
    memset(pixel_buffer[0], value, width * height * sizeof(char));
    new_screen -> pixels = (char **) pixel_buffer;
    memset(colour_buffer[0], colour, width * height * sizeof(int));
    new_screen -> colours = (int **) colour_buffer;

    *old_screen = new_screen;
}

void setup_screen(void) 
{
    // Initialize curses mode
    initscr();

    // Turn on coloured
    start_color();

    // Do not echo keypresses
    noecho();

    // Turn off cursor
    curs_set(0);

    // Make typed chars immediately available
    cbreak();

    // Causes getch to be non-blocking
    nodelay(stdscr, TRUE);

    // Clear potentially lingering state of screen
    clear();

    // Create screen buffer
    create_buffer(&m_screen, getmaxx(stdscr), getmaxy(stdscr), ' ', 0);
}

void show_screen(void)
{
    char **new_pixels = m_screen -> pixels;
    int **new_colour = m_screen -> colours;
    int width = m_screen -> width;
    int height = m_screen -> height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            attron(new_colour[y][x]);
            mvaddch(y, x, new_pixels[y][x]);
            attroff(new_colour[y][x]);
        }
    }
    refresh();
}

void clear_screen(void)
{
    if (m_screen != NULL) {
        int W = screen_width();
        int H = screen_height();

        char *screen_pixels = m_screen -> pixels[0];
        int *colours = m_screen -> colours[0];

        memset(screen_pixels, ' ', W * H);

        for (int i = 0; i < W * H; i++)
        {
            colours[i] = 0;
        }
    }
}

int screen_width(void)
{
    return m_screen -> width;
}

int screen_height(void)
{
    return m_screen -> height;
}
