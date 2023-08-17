#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include "graphics.h"
#include "geometry.h"

#define ROTATION_CONST 0.0005f
#define TRANSLATION_CONST 3.0f

#define DEBUG true

int W;
int H;

int main(int argc, char *argv[])
{
    #if DEBUG
    printf("test\n"); 
    

    #else
    Data3d *obj_data = read_obj(argv[1]);
    TriMesh3d *mesh = populate_trimesh(obj_data);
    Vert3d *camera = calloc(1, sizeof(Vert3d));

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
        clear_screen();
        for (int i = 0; i < tri_count; i++)
        {
            Tri3d tri = mesh -> tris[i];
            roll(&tri, x_theta);
            pitch(&tri, y_theta);
            yaw(&tri, z_theta);

            // World Matrix Transform
            translate(&tri, TRANSLATION_CONST);
            // Calculate triangle Normal
            calculate_normals(&tri);

            // Get Ray from triangle to camera
            Vert3d ray = { tri.v[0].x - camera -> x, tri.v[0].y - camera -> y, tri.v[0].z - camera -> z };
            float ray_dist = tri.n.x * ray.x + tri.n.y * ray.y + tri.n.z * ray.z;

            if (ray_dist < 0.0f)
            {
                Vert3d light_dir = { 0.0f, 1.0f, -1.0f };
                float len = sqrtf(light_dir.x * light_dir.x + light_dir.y * light_dir.y 
                                    + light_dir.z * light_dir.z);
                light_dir.x /= len; light_dir.y /= len; light_dir.z /= len;

                float rel_dist = fmax(0.1f, tri.n.x * light_dir.x + tri.n.y * light_dir.y
                                    + tri.n.z * light_dir.z);

                if (!DEBUG)
                {
                    // Transform

                    // View

                    // Clip

                    // Project
                    project(&tri, W, H);

                    // Sort

                    // Clip?

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


