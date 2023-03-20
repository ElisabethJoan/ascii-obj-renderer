#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include "graphics.h"
#include "geometry.h"

#define ROTATION_CONST 0.0005f
#define TRANSLATION_CONST 3.0f

#define DEBUG false

int W;
int H;

int main(int argc, char *argv[])
{
    Data3d *obj_data = read_obj(argv[1]);
    TriMesh3d *mesh = populate_trimesh(obj_data);

    #if DEBUG
    int count = 0;
    while(count < 1)
    {
        float x_theta = ROTATION_CONST;
        for (int i = 0; i < 1; i++)
        {
            Tri3d tri = mesh -> tris[i];
            roll(&tri, x_theta);

            translate(&tri, TRANSLATION_CONST);
            calculate_normals(&tri);
        }
        count++;
    }
    #else
    Vert3d *vCamera = calloc(1, sizeof(Vert3d));

    setup_screen();
    W = screen_width();
    H = screen_height();
    setup_colours();

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
            calculate_normals(&tri);

            if (tri.n.x * (tri.v[0].x - vCamera -> x) + 
                tri.n.y * (tri.v[0].y - vCamera -> y) +
                tri.n.z * (tri.v[0].z - vCamera -> z) < 0.0f)
            {
                Vert3d light_dir = { 0.0f, 0.0f, -1.0f };
                float len = sqrtf(light_dir.x * light_dir.x + light_dir.y * light_dir.y 
                                    + light_dir.z * light_dir.z);
                light_dir.x /= len; light_dir.y /= len; light_dir.z /= len;

                float rel_dist = tri.n.x * light_dir.x + tri.n.y * light_dir.y 
                                    + tri.n.z * light_dir.z;

                if (!DEBUG)
                {
                    project(&tri, W, H);

                    fill_tri(tri.v[0].x, tri.v[0].y,
                        tri.v[1].x, tri.v[1].y,
                        tri.v[2].x, tri.v[2].y, '*', COLOR_PAIR(get_colour(rel_dist)));
                }
            }
        }
        show_screen();
    }
    #endif
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
