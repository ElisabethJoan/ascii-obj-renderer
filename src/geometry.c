#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include "geometry.h"

static Vert3d multiple_matrix_vector(Mat4x4 *m, Vert3d v) 
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

void project(Tri3d *tri, int W, int H)
{
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