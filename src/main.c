#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "graphics.h"
#include "geometry.h"

#define ROTATION_CONST 0.0005f
#define TRANSLATION_CONST 0.3f

int main(int argc, char *argv[])
{
    Data3d *obj_data = read_obj(argv[1]);
    TriMesh3d *mesh = populate_trimesh(obj_data);

    setup_screen();
    int W = screen_width();
    int H = screen_height();

    float x_theta, y_theta, z_theta;
    x_theta = y_theta = z_theta = 0;
    int tri_count = mesh -> tri_count;
    int temp, key;
    while(1)
    {
        clear_screen();
        temp = getch();
        if (temp != -1)
        {
            key = temp;
        }

        switch (key)
        {
            case 'w':
                x_theta -= ROTATION_CONST;
                break;
            case 's':
                x_theta += ROTATION_CONST;
                break;
            case 'a':
                y_theta -= ROTATION_CONST;
                break;
            case 'd':
                y_theta += ROTATION_CONST;
                break;
            case 'q':
                z_theta -= ROTATION_CONST;
                break;
            case 'e':
                z_theta += ROTATION_CONST;
                break;
            case 'r':
                x_theta = y_theta = z_theta = 0;
                break;
            default:
                break;
        }

        for (int i = 0; i < tri_count; i++)
        {
            Tri3d tri = mesh -> tris[i];
            roll(&tri, x_theta);
            pitch(&tri, y_theta);
            yaw(&tri, z_theta);

            translate(&tri, TRANSLATION_CONST);
            project(&tri, W, H);

            draw_tri(tri.v[0].x, tri.v[0].y,
                tri.v[1].x, tri.v[1].y,
                tri.v[2].x, tri.v[2].y, '*');
        }
        show_screen();
    } 

    // endwin();
}

// ------------------------------------------------------------------
// CODE CEMETERY
// ------------------------------------------------------------------

// // Cube Verts
// Vert3d v1 = { -1.0f, -1.0f, -1.0f };
// Vert3d v2 = { -1.0f, 1.0f, -1.0f };
// Vert3d v3 = { 1.0f, 1.0f, -1.0f };
// Vert3d v4 = { 1.0f, -1.0f, -1.0f };
// Vert3d v5 = { -1.0f, -1.0f, 1.0f };
// Vert3d v6 = { -1.0f, 1.0f, 1.0f };
// Vert3d v7 = { 1.0f, 1.0f, 1.0f };
// Vert3d v8 = { 1.0f, -1.0f, 1.0f };

// // SOUTH
// Tri3d t1 = { { v1, v2, v3 } };
// Tri3d t2 = { { v1, v3, v4 } };

// // EAST
// Tri3d t3 = { { v4, v3, v7 } };
// Tri3d t4 = { { v4, v7, v8 } };

// // NORTH
// Tri3d t5 = { { v8, v7, v6 } };
// Tri3d t6 = { { v8, v6, v5 } };

// // WEST
// Tri3d t7 = { { v5, v6, v2 } };
// Tri3d t8 = { { v5, v2, v1 } };

// // TOP
// Tri3d t9 = { { v2, v6, v7 } };
// Tri3d t10 = { { v2, v7, v3 } };

// // BOTTOM
// Tri3d t11 = { { v8, v5, v1 } };
// Tri3d t12 = { { v8, v1, v4 } };

// TriMesh3d *mesh = calloc(1, sizeof(TriMesh3d));
// mesh -> tris = calloc(12, sizeof(Tri3d));

// mesh -> tris[0] = t1; mesh -> tris[1] = t2;  mesh -> tris[2] = t3;
// mesh -> tris[3] = t4; mesh -> tris[4] = t5; mesh -> tris[5] = t6;
// mesh -> tris[6] = t7; mesh -> tris[7] = t8; mesh -> tris[8] = t9;
// mesh -> tris[9] = t10; mesh -> tris[10] = t11; mesh -> tris[11] = t12;
