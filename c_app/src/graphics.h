#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#define WHITE_P       1
#define GREY_10_P     2
#define GREY_20_P     3
#define GREY_30_P     4
#define GREY_40_P     5
#define GREY_50_P     6
#define GREY_60_P     7
#define GREY_70_P     8
#define GREY_80_P     9
#define GREY_90_P     10
#define BLACK_P       11

#define WHITE       255
#define GREY_10     252
#define GREY_20     249
#define GREY_30     246
#define GREY_40     243
#define GREY_50     241
#define GREY_60     239
#define GREY_70     237
#define GREY_80     235
#define GREY_90     232
#define BLACK       0

typedef struct Screen
{
    int width;
    int height;
    char **pixels;
    int **colours;
} Screen;

void draw_char(int x, int y, char value, int colour);
void draw_line(int x1, int y1, int x2, int y2, char value, int colour);
void draw_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, int colour);
void fill_tri(int x1, int y1, int x2, int y2, int x3, int y3, char value, int colour);
int get_colour(float lum);

void setup_screen(void);
void setup_colours(void);
void show_screen(void);
void clear_screen(void);
int screen_width(void);
int screen_height(void);

#endif /* GRAPHICS_H_ */