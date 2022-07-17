#include <stdlib.h>
#include <curses.h>
#include <math.h>
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

typedef struct Mat4x4
{
    float m[4][4];
} Mat4x4;


Vert3d multiple_matrix_vector(Mat4x4 *m, Vert3d v) 
{
    Vert3d o;

    o.x = v.x * m -> m[0][0] + v.y * m -> m[1][0] + v.z * m -> m[2][0] + m -> m[3][0];
    o.y = v.x * m -> m[0][1] + v.y * m -> m[1][1] + v.z * m -> m[2][1] + m -> m[3][1];
    o.z = v.x * m -> m[0][2] + v.y * m -> m[1][2] + v.z * m -> m[2][2] + m -> m[3][2];
    float w = v.x * m -> m[0][3] + v.y * m -> m[1][3] + v.z * m -> m[2][3] + m -> m[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }

    return o;
}

void roll(Tri3d *tri, float f_theta)
{
    Mat4x4 *mat_rot_x = calloc(1, sizeof(Mat4x4));
    mat_rot_x -> m[0][0] = 1;
    mat_rot_x -> m[1][1] = cosf(f_theta);
    mat_rot_x -> m[1][2] = sinf(f_theta);
    mat_rot_x -> m[2][1] = -sinf(f_theta);
    mat_rot_x -> m[2][2] = cosf(f_theta);
    mat_rot_x -> m[3][3] = 1;

    Tri3d tri_rot_x;
    tri_rot_x.v[0] = multiple_matrix_vector(mat_rot_x, tri -> v[0]);
    tri_rot_x.v[1] = multiple_matrix_vector(mat_rot_x, tri -> v[1]);
    tri_rot_x.v[2] = multiple_matrix_vector(mat_rot_x, tri -> v[2]);
    *tri = tri_rot_x;

    free(mat_rot_x);
}

void pitch(Tri3d *tri, float f_theta) 
{
    Mat4x4 *mat_rot_y = calloc(1, sizeof(Mat4x4));
    mat_rot_y -> m[0][0] = cosf(f_theta);
    mat_rot_y -> m[0][2] = sinf(f_theta);
    mat_rot_y -> m[1][1] = 1;
    mat_rot_y -> m[2][0] = -sinf(f_theta);
    mat_rot_y -> m[2][2] = cosf(f_theta);
    mat_rot_y -> m[3][3] = 1;

    Tri3d tri_rot_y;
    tri_rot_y.v[0] = multiple_matrix_vector(mat_rot_y, tri -> v[0]);
    tri_rot_y.v[1] = multiple_matrix_vector(mat_rot_y, tri -> v[1]);
    tri_rot_y.v[2] = multiple_matrix_vector(mat_rot_y, tri -> v[2]);
    *tri = tri_rot_y;

    free(mat_rot_y);
}

void yaw(Tri3d *tri, float f_theta)
{
    Mat4x4 *mat_rot_z = calloc(1, sizeof(Mat4x4));
    mat_rot_z -> m[0][0] = cosf(f_theta);
    mat_rot_z -> m[0][1] = sinf(f_theta);
    mat_rot_z -> m[1][0] = -sinf(f_theta);
    mat_rot_z -> m[1][1] = cosf(f_theta);
    mat_rot_z -> m[2][2] = 1;
    mat_rot_z -> m[3][3] = 1;

    Tri3d tri_rot_z;
    tri_rot_z.v[0] = multiple_matrix_vector(mat_rot_z, tri -> v[0]);
    tri_rot_z.v[1] = multiple_matrix_vector(mat_rot_z, tri -> v[1]);
    tri_rot_z.v[2] = multiple_matrix_vector(mat_rot_z, tri -> v[2]);
    *tri = tri_rot_z;

    free(mat_rot_z);
}

void translate(Tri3d *tri, float offset)
{
    Tri3d tri_trans;
    tri_trans.v[0].z = tri -> v[0].z + offset;
    tri_trans.v[1].z = tri -> v[1].z + offset;
    tri_trans.v[2].z = tri -> v[2].z + offset;

    *tri = tri_trans;
}

void project(Tri3d *tri)
{
    int W = screen_width();
    int H = screen_height();

    float f_near = 0.1f;
    float f_far = 1000.0f;
    float f_fov = 90.0f;
    float f_aspect_ratio = (float) H / (float) W;
    float f_fov_rad = 1.0f / tanf(f_fov * 0.5f / 180.0f * 3.14159f);

    Mat4x4 *mat_proj = calloc(1, sizeof(Mat4x4));

    mat_proj -> m[0][0] = f_aspect_ratio * f_fov_rad;
    mat_proj -> m[1][1] = f_fov_rad;
    mat_proj -> m[2][2] = f_far / (f_far - f_near);
    mat_proj -> m[3][2] = (-f_far * f_near) / (f_far - f_near);
    mat_proj -> m[2][3] = 1.0f;
    mat_proj -> m[3][3] = 0.0f;

    Tri3d tri_proj;
    tri_proj.v[0] = multiple_matrix_vector(mat_proj, tri -> v[0]);
    tri_proj.v[1] = multiple_matrix_vector(mat_proj, tri -> v[1]);
    tri_proj.v[2] = multiple_matrix_vector(mat_proj, tri -> v[2]);

    tri_proj.v[0].x += 1.0f; tri_proj.v[0].y += 1.0f;
    tri_proj.v[1].x += 1.0f; tri_proj.v[1].y += 1.0f;
    tri_proj.v[2].x += 1.0f; tri_proj.v[2].y += 1.0f;

    tri_proj.v[0].x *= 0.5f * W;
    tri_proj.v[0].y *= 0.5f * H;
    tri_proj.v[1].x *= 0.5f * W;
    tri_proj.v[1].y *= 0.5f * H;
    tri_proj.v[2].x *= 0.5f * W;
    tri_proj.v[2].y *= 0.5f * H;
    *tri = tri_proj;

    free(mat_proj);
}

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

    float f_theta = 0;
    while(true)
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
            project(&tri);

            draw_tri(tri.v[0].x, tri.v[0].y,
                tri.v[1].x, tri.v[1].y,
                tri.v[2].x, tri.v[2].y, '*');
        }
        show_screen();
    }

    // endwin();
}