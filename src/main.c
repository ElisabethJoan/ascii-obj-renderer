#include <stdlib.h>
#include <curses.h>
#include "graphics.h"

typedef struct Vert3d
{
    double x, y, z;
} Vert3d;

typedef struct Tri3d
{
    Vert3d v[3];
} Tri3d;

typedef struct TriMesh3d 
{
    Tri3d *tris;
} TriMesh3d;


int main(void) 
{
    // Cube Verts
    Vert3d v1 = { -1.0f, -1.0f, -1.0f };
    Vert3d v2 = { -1.0f, 1.0f, -1.0f };
    Vert3d v3 = { 1.0f, 1.0f, -1.0f };
    Vert3d v4 = { 1.0f, -1.0f, -1.0f };
    Vert3d v5 = { -1.0f, -1.0f, 1.0f };
    Vert3d v6 = { -1.0f, 1.0f, 1.0f };
    Vert3d v7 = { 1.0f, 1.0f, 1.0f };
    Vert3d v8 = { 1.0f, -1.0f, 1.0f };

    // SOUTH
    Tri3d t1 = { { v1, v2, v3 } };
    Tri3d t2 = { { v1, v3, v4 } };

    // EAST
    Tri3d t3 = { { v4, v3, v7 } };
    Tri3d t4 = { { v4, v7, v8 } };

    // NORTH
    Tri3d t5 = { { v8, v7, v6 } };
    Tri3d t6 = { { v8, v6, v5 } };

    // WEST
    Tri3d t7 = { { v5, v6, v2 } };
    Tri3d t8 = { { v5, v2, v1 } };

    // TOP
    Tri3d t9 = { { v2, v6, v7 } };
    Tri3d t10 = { { v2, v7, v3 } };

    // BOTTOM
    Tri3d t11 = { { v8, v5, v1 } };
    Tri3d t12 = { { v8, v1, v4 } };

    TriMesh3d *mesh = calloc(1, sizeof(TriMesh3d));
    mesh -> tris = calloc(12, sizeof(Tri3d));

    mesh -> tris[0] = t1; mesh -> tris[1] = t2;  mesh -> tris[2] = t3;
    mesh -> tris[3] = t4; mesh -> tris[4] = t5; mesh -> tris[5] = t6;
    mesh -> tris[6] = t7; mesh -> tris[7] = t8; mesh -> tris[8] = t9;
    mesh -> tris[9] = t10; mesh -> tris[10] = t11; mesh -> tris[11] = t12;

    // setup_screen();
    
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%f %f %f\n", mesh -> tris[i].v[j].x, mesh -> tris[i].v[j].y,
                mesh -> tris[i].v[j].z);
        }
        printf("\n");
    }

    // show_screen();
    getch();

    // endwin();
}