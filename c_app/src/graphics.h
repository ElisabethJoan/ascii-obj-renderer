#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct GridCell {
    char c;
    SDL_Color *fg;
    SDL_Color *bg;
} GridCell;

void draw_char(int x, int y, char value, SDL_Color *fg, SDL_Color *bg);
void draw_line(int x1, int y1, int x2, int y2, char value, SDL_Color *fg, SDL_Color *bg);
void draw_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, SDL_Color *fg, SDL_Color *bg);
void fill_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, SDL_Color *fg, SDL_Color *bg);
SDL_Color* get_colour(float lum);

void setup_screen(void);
void show_screen(void);
void clear_screen(void);
void exit_app(void);
int grid_width(void);
int grid_height(void);

#endif /* GRAPHICS_H_ */
