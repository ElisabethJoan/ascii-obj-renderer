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

void setup_screen(void);
void show_screen(void);

#endif /* GRAPHICS_H_ */