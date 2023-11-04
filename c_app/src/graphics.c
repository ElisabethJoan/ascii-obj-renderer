#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.h"

#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)

GridCell *grid;
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;
SDL_Color bg = { 22, 22, 22, 255 };
SDL_Color fg = { 255, 255, 255, 255 };

int window_w;
int window_h;
int grid_w = 83;
int grid_h = 28;
int grid_cell_w = 0;
int grid_cell_h = 0;    

// ------------------------------------------------------------------
// DRAW FUNCTIONS
// ------------------------------------------------------------------

void draw_char(int x, int y, char value, SDL_Color *fg, SDL_Color *bg)
{
    if (x >= 0 && x < window_w && y >= 0 && y < window_h)
    {
        GridCell *cell = &grid[x + grid_w * y];
        cell -> c = '*';
        cell -> fg = fg;
        cell -> bg = bg;
    }
}

void draw_line(int x1, int y1, int x2, int y2, char value, SDL_Color *fg, SDL_Color *bg)
{
    if (x1 == x2)
    {
        // Draw vertical line
        int y_min = MIN(y1, y2);
        int y_max = MAX(y1, y2);

        for (int i = y_min; i <= y_max; i++)
        {
            draw_char(x1, i, value, fg, bg);
        }
    }
    else if (y1 == y2)
    {
        // Draw horizontal line
        int x_min = MIN(x1, x2);
        int x_max = MAX(x1, x2);

        for (int i = x_min; i <= x_max; i++)
        {
            draw_char(i, y1, value, fg, bg);
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
            draw_char(x, y, value, fg, bg);
            err += derr;
            while (err >= 0.5f && ((dy > 0) ? y <= y2 : y >= y2))
            {
                draw_char(x, y, value, fg, bg);
                y += (dy > 0) - (dy < 0);

                err -= 1.0f;
            }
        }
    }
}

void draw_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, SDL_Color *fg, SDL_Color *bg)
{
    draw_line(x1, y1, x2, y2, value, fg, bg);
    draw_line(x2, y2, x3, y3, value, fg, bg);
    draw_line(x3, y3, x1, y1, value, fg, bg);
}

void fill_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, SDL_Color *fg, SDL_Color *bg)
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
        draw_line(a, y1, b, y1, value, fg, bg);
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

    for(y = y1; y <= last; y++) 
    {
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        draw_line(a, y, b, y, value, fg, bg);
    }

    // For lower part of triangle, find scanline crossings for segment
    // 0-2 and 1-2.  This loop is skipped if y1=y2
    for(; y <= y3; y++) 
    {
        a = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
        b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        draw_line(a, y, b, y, value, fg, bg);
    }
}

SDL_Color* get_colour(float lum)
{
    int light_index = (int) (7.0f * lum);
    SDL_Color *colour;
    colour = calloc(1, sizeof(SDL_Color));
    switch (light_index)
    {
        case 0:
        case 1:
            colour -> r = 60;
            colour -> g = 60;
            colour -> b = 60;
            colour -> a = 255;
            return colour;
        case 2:
            colour -> r = 100;
            colour -> g = 100;
            colour -> b = 100;
            colour -> a = 255;
            return colour;
        case 3:
            colour -> r = 150;
            colour -> g = 150;
            colour -> b = 150;
            colour -> a = 255;
            return colour;
        case 4:
            colour -> r = 200;
            colour -> g = 200;
            colour -> b = 200;
            colour -> a = 255;
            return colour;
        case 5:
            colour -> r = 230;
            colour -> g = 230;
            colour -> b = 230;
            colour -> a = 255;
            return colour;
        case 6:
            colour -> r = 240;
            colour -> g = 240;
            colour -> b = 240;
            colour -> a = 255;
            return colour;
        default:
            colour -> r = 60;
            colour -> g = 60;
            colour -> b = 60;
            colour -> a = 255;
            return colour;
    }
}

// ------------------------------------------------------------------
// SCREEN FUNCTIONS
// ------------------------------------------------------------------

void setup_screen()
{   
    int grid_size = grid_w * grid_h;
    grid = calloc((size_t)grid_size, sizeof(GridCell));
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s", SDL_GetError());
      return;
    }
    if (TTF_Init() < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL_ttf: %s", TTF_GetError());
      return;
    }

    font = TTF_OpenFont("assets/FiraMono-Regular.ttf", 14);
    if (font == NULL)
    { 
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Open Font: %s", TTF_GetError());
      return;
    }
    if (TTF_SizeText(font, " ", &grid_cell_w, &grid_cell_h) < 0)
    { 
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Get text metrics: %s", TTF_GetError());
      return;
    }
    
    SDL_Cursor *cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    if (cursor == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Get the system hand cursor: %s", SDL_GetError());
      return;
    }
    SDL_SetCursor(cursor);

    window_w = grid_w * grid_cell_w;
    window_h = grid_h * grid_cell_h;
    if (SDL_CreateWindowAndRenderer(window_w, window_h, 0, &window, &renderer) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create window and renderer: %s", SDL_GetError());
      return;
    }
    SDL_SetWindowTitle(window, "3D ASCII Renderer");
}

void show_screen(void)
{
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderClear(renderer);
    SDL_Rect dest = {
        .x = 0,
        .y = 0,
        .w = grid_cell_w,
        .h = grid_cell_h,
    };

    for (int x = 0; x < grid_w; x++)
    {
        for (int y = 0; y < grid_h; y++)
        {
            dest.x = grid_cell_w * x;
            dest.y = grid_cell_h * y;

            GridCell *cell = &grid[x + grid_w * y];
            
            SDL_Surface *surface = TTF_RenderGlyph_Shaded(font, (Uint16) cell -> c, *cell -> fg, *cell -> bg);

            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            
            SDL_SetRenderDrawColor(renderer, cell -> bg -> r, cell -> bg -> g, cell -> bg -> b, cell -> bg -> a);

            SDL_RenderFillRect(renderer, &dest);

            SDL_RenderCopy(renderer, texture, NULL, &dest);

            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }

    SDL_RenderPresent(renderer);
}

void clear_screen(void)
{
    for (int x = 0; x < grid_w; x++)
    {
        for (int y = 0; y < grid_h; y++)
        {
            GridCell *cell = &grid[x + grid_w * y];
            cell -> c = ' ';
            cell -> fg = &fg;
            cell -> bg = &bg;
        }
    }
}

void exit_app(void)
{
    free(grid);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int grid_width(void)
{
  return grid_w;
}

int grid_height(void)
{
  return grid_h;
}
