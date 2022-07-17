#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct Screen
{
    int width;
    int height;
    char **pixels;
} Screen;

void draw_char(int x, int y, char value);
void draw_line(int x1, int y1, int x2, int y2, char value);
void draw_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value);

void setup_screen(void);
void show_screen(void);
void clear_screen(void);
int screen_width(void);
int screen_height(void);

#endif /* GRAPHICS_H_ */