#include <stdlib.h>
// #include <curses.h>
// #include <math.h>
#include "graphics.h"
#include "geometry.h"

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

    setup_screen();
    int W = screen_width();
    int H = screen_height();

    float f_theta = 0;
    while(1)
    {
        clear_screen();
        f_theta += 0.0003f;
        for (int i = 0; i < 12; i++)
        {
            Tri3d tri = mesh -> tris[i];
            roll(&tri, f_theta);
            // pitch(&tri, f_theta);
            // yaw(&tri, f_theta);

            translate(&tri, 3.0f);
            project(&tri, W, H);

            draw_tri(tri.v[0].x, tri.v[0].y,
                tri.v[1].x, tri.v[1].y,
                tri.v[2].x, tri.v[2].y, '*');
        }
        show_screen();
    }

    // endwin();
}