#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geometry.h"

int num_verts = 0;
int num_facets = 0;
int num_normals = 0;
Vert3d *verts;
Facet3d *facets;
Vert3d *normals;


// ------------------------------------------------------------------
// INGEST FUNCTIONS
// ------------------------------------------------------------------

static void count_labels(char *arg) 
{
    FILE *fp = fopen(arg, "r");
    char c;

    while (fscanf(fp, "%c", &c) == 1)
    {
        if (c == 'v')
        {
            fscanf(fp, "%c", &c);
            if (c == ' ') 
            {
                num_verts++;
            }
            else if (c == 'n')
            {
                num_normals++;
            }
            
        }
        else if (c == 'f')
        {
            fscanf(fp, "%c", &c);
            if (c == ' ') {
                num_facets++;
            }
        }
    }

    rewind(fp);
}

static void normalize_obj(Vert3d *verts, int v_size, Vert3d *norms, int n_size) {
    // There has to be a less stupid way to do this :^(
    float min = 100;
    float max = -100;
    for (int i = 0; i < v_size; i++) {
        if (verts[i].x > max) max = verts[i].x;
        if (verts[i].y > max) max = verts[i].y;
        if (verts[i].z > max) max = verts[i].z;
        if (verts[i].x < min) min = verts[i].x;
        if (verts[i].y < min) min = verts[i].y;
        if (verts[i].z < min) min = verts[i].z;
    }
    for (int i = 0; i < v_size; i++) {
        verts[i].x = 2 * (verts[i].x - min) / (max - min) - 1;
        verts[i].y = 2 * (verts[i].y - min) / (max - min) - 1;
        verts[i].z = 2 * (verts[i].z - min) / (max - min) - 1;
    }
    min = 100;
    max = -100;
    for (int i = 0; i < n_size; i++) {
        if (norms[i].x > max) max = norms[i].x;
        if (norms[i].y > max) max = norms[i].y;
        if (norms[i].z > max) max = norms[i].z;
        if (norms[i].x < min) min = norms[i].x;
        if (norms[i].y < min) min = norms[i].y;
        if (norms[i].z < min) min = norms[i].z;
    }
    for (int i = 0; i < n_size; i++) {
        norms[i].x = 2 * (norms[i].x - min) / (max - min) - 1;
        norms[i].y = 2 * (norms[i].y - min) / (max - min) - 1;
        norms[i].z = 2 * (norms[i].z - min) / (max - min) - 1;
    }
}


Data3d * read_obj(char *arg)
{
    count_labels(arg);
    FILE *fp = fopen(arg, "r");
    verts = calloc(num_verts, sizeof(Vert3d));
    facets = calloc(num_facets, sizeof(Facet3d));
    normals = calloc(num_normals, sizeof(Vert3d));
    int vert_count = 0;
    int facet_count = 0;
    int normal_count = 0;
    char c;
    
    while (fscanf(fp, "%c", &c) == 1)
    {
        if (c == 'v')
        {
            fscanf(fp, "%c", &c);
            if (c == ' ') 
            {
                fscanf(fp, " %lf %lf %lf\n", &verts[vert_count].x, 
                    &verts[vert_count].y, &verts[vert_count].z);
                vert_count++;
            }
            else if (c == 'n')
            {
                fscanf(fp, " %lf %lf %lf\n", &normals[normal_count].x, 
                    &normals[normal_count].y, &normals[normal_count].z);
                normal_count++;
            }
        }
        if (c == 'f')
        {
            fscanf(fp, "%c", &c);
            if (c == ' ') {
                long pos = ftell(fp);
                fscanf(fp, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%d\n", &facets[facet_count].a,
                    &facets[facet_count].b, &facets[facet_count].c, &facets[facet_count].n);
                if (facets[facet_count].a == 0 || facets[facet_count].b == 0 ||
                    facets[facet_count].c == 0 || facets[facet_count].n == 0) {
                    fseek(fp, pos, SEEK_SET);
                    fscanf(fp, "%d//%*d %d//%*d %d//%d\n", &facets[facet_count].a,
                        &facets[facet_count].b, &facets[facet_count].c, &facets[facet_count].n);
                }
                facet_count++;
            }
        }
    }

    normalize_obj(verts, vert_count, normals, normal_count);

    Data3d *obj_data = calloc(1, sizeof(Data3d));
    obj_data -> verts = verts;
    obj_data -> facets = facets;
    obj_data -> normals = normals;
    return obj_data;
}

static Tri3d populate_tri(Vert3d *verts, Vert3d *normals, int a, int b, int c, int n)
{
    Tri3d tri;
    Vert3d normal;
    Vert3d vert1 = verts[a - 1];
    Vert3d vert2 = verts[b - 1];
    Vert3d vert3 = verts[c - 1];
    normal.x = normals[n - 1].x;
    normal.y = normals[n - 1].y;
    normal.z = normals[n - 1].z;
    tri.v[0] = vert1;
    tri.v[1] = vert2;
    tri.v[2] = vert3;
    tri.n = normal;

    return tri;
}

TriMesh3d * populate_trimesh(Data3d *obj_data)
{
    TriMesh3d *mesh = calloc(1, sizeof(TriMesh3d));
    Tri3d *tris = calloc(num_facets, sizeof(Tri3d));
    int tri_count = 0;

    for (int i = 0; i < num_facets; i++)
    {
        Tri3d tri = populate_tri(obj_data -> verts, obj_data -> normals, obj_data -> facets[i].a, 
            obj_data -> facets[i].b, obj_data -> facets[i].c, obj_data -> facets[i].n);

        tris[i] = tri;
        tri_count++;
    }
    mesh -> tris = tris;
    mesh -> tri_count = tri_count;
    return mesh;
}

// ------------------------------------------------------------------
// MATH FUNCTIONS
// ------------------------------------------------------------------

Vert3d matrix_vector_product(Mat4x4 *m, Vert3d v) 
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

Mat4x4 * matrix_multiplication(Mat4x4 *m1, Mat4x4 *m2)
{
    Mat4x4 *matrix = calloc(1, sizeof(Mat4x4));
    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            matrix -> m[r][c] = m1 -> m[r][0] * m2 -> m[0][c] + m1 -> m[r][1] * m2 -> m[1][c] + m1 -> m[r][2] * m2 -> m[2][c] + m1 -> m[r][3] * m2 -> m[3][c];
        }
    }

    return matrix;
}

// static Mat4x4 * cofactor(Mat4x4 *m, int p, int q, int n)
// {
//     Mat4x4 *temp = calloc(1, sizeof(Mat4x4));
//     int i, j;
//     i = j = 0;

//     for (int row = 0; row < n; row++)
//     {
//         for (int col = 0; col < n; col++)
//         {
//             if (row != p && col != q)
//             {
//                 temp -> m[i][j++] = m -> m[row][col];

//                 if (j == n - 1)
//                 {
//                     j = 0;
//                     i++;
//                 }
//             }
//         }
//     }
//     return temp;
// }

// static float determinant(Mat4x4 *m, int n)
// {
//     float det = 0;
//     if (n == 1)
//     {
//         return m -> m[0][0];
//     }
    
//     Mat4x4 *cofactors = calloc(1, sizeof(Mat4x4));
//     int sign = 1;

//     for (int f = 0; f < n; f++)
//     {
//         cofactors = cofactor(m, 0, f, n);
//         det += sign * m -> m[0][f] * determinant(cofactors, n - 1);

//         sign = -sign;
//     }
//     return det;
// }

// static Mat4x4 * adjoint(Mat4x4 *m)
// {
//     Mat4x4 *adj = calloc(1, sizeof(Mat4x4));
//     if (4 == 1)
//     {
//         adj -> m[0][0] = 1;
//         return adj;
//     }

//     int sign = 1;
//     Mat4x4 *cofactors = calloc(1, sizeof(Mat4x4));

//     for (int i = 0; i < 4; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             cofactors = cofactor(m, i, j, 4);
//             sign = ((i + j) % 2 == 0) ? 1 : -1;
//             adj -> m[j][i] = (sign) * (determinant(cofactors, 3));
//         }
//     }
//     return adj;
// }

// static Mat4x4 * inverse(Mat4x4 *m)
// {
//     float det = determinant(m, 4);

//     Mat4x4 *adj = calloc(1, sizeof(Mat4x4));
//     Mat4x4 *inverse = calloc(1, sizeof(Mat4x4));
//     adj = adjoint(m);

//     for (int i = 0; i < 4; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             inverse -> m[i][j] = adj -> m[i][j] / (float) det;
//         }
//     }
//     return inverse;
// }

// static Mat4x4 * transpose(Mat4x4 *m)
// {
//     Mat4x4 *transposed = calloc(1, sizeof(Mat4x4));
//     for (int i = 0; i < 4; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             transposed -> m[j][i] = m -> m[i][j];
//         }
//     }
//     return transposed;
// }

Mat4x4 * make_x_rotation(float f_theta)
{
    Mat4x4 *mat_rot_x = calloc(1, sizeof(Mat4x4));
    mat_rot_x -> m[0][0] = 1;
    mat_rot_x -> m[1][1] = cosf(f_theta);
    mat_rot_x -> m[1][2] = sinf(f_theta);
    mat_rot_x -> m[2][1] = -sinf(f_theta);
    mat_rot_x -> m[2][2] = cosf(f_theta);
    mat_rot_x -> m[3][3] = 1;

    return mat_rot_x;
}

Mat4x4 * make_y_rotation(float f_theta) 
{ 
    Mat4x4 *mat_rot_y = calloc(1, sizeof(Mat4x4));
    mat_rot_y -> m[0][0] = cosf(f_theta);
    mat_rot_y -> m[0][2] = sinf(f_theta);
    mat_rot_y -> m[1][1] = 1;
    mat_rot_y -> m[2][0] = -sinf(f_theta);
    mat_rot_y -> m[2][2] = cosf(f_theta);
    mat_rot_y -> m[3][3] = 1;

    return mat_rot_y;
}

Mat4x4 * make_z_rotation(float f_theta)
{
    Mat4x4 *mat_rot_z = calloc(1, sizeof(Mat4x4));
    mat_rot_z -> m[0][0] = cosf(f_theta);
    mat_rot_z -> m[0][1] = sinf(f_theta);
    mat_rot_z -> m[1][0] = -sinf(f_theta);
    mat_rot_z -> m[1][1] = cosf(f_theta);
    mat_rot_z -> m[2][2] = 1;
    mat_rot_z -> m[3][3] = 1;

    return mat_rot_z;
}

Mat4x4 * make_projection_matrix(int height, int width)
{
    float f_near = 0.1f;
    float f_far = 1000.0f;
    float f_fov = 90.0f;
    float f_aspect_ratio = (float) height / (float) width;
    float f_fov_rad = 1.0f / tanf(f_fov * 0.5f / 180.0f * 3.14159f);

    Mat4x4 *mat_proj = calloc(1, sizeof(Mat4x4));

    mat_proj -> m[0][0] = f_aspect_ratio * f_fov_rad;
    mat_proj -> m[1][1] = f_fov_rad;
    mat_proj -> m[2][2] = f_far / (f_far - f_near);
    mat_proj -> m[3][2] = (-f_far * f_near) / (f_far - f_near);
    mat_proj -> m[2][3] = 1.0f;
    mat_proj -> m[3][3] = 0.0f;

    return mat_proj;
}

Mat4x4 * make_translation_matrix(float z)
{
    Mat4x4 *mat_trans = calloc(1, sizeof(Mat4x4));
    mat_trans -> m[0][0] = 1.0f;
	  mat_trans -> m[1][1] = 1.0f;
	  mat_trans -> m[2][2] = 1.0f;
	  mat_trans -> m[3][3] = 1.0f;
	  mat_trans -> m[3][0] = 0.0f;
	  mat_trans -> m[3][1] = 0.0f;
	  mat_trans -> m[3][2] = 3.0f;

    return mat_trans;
}

void roll(Mat4x4 *mat, float f_theta)
{
    mat -> m[1][1] = cosf(f_theta);
    mat -> m[1][2] = sinf(f_theta);
    mat -> m[2][1] = -sinf(f_theta);
    mat -> m[2][2] = cosf(f_theta);
}

void pitch(Mat4x4 *mat, float f_theta)
{
    mat -> m[0][0] = cosf(f_theta);
    mat -> m[0][2] = sinf(f_theta);
    mat -> m[2][0] = -sinf(f_theta);
    mat -> m[2][2] = cosf(f_theta);
}

void yaw(Mat4x4 *mat, float f_theta)
{
    mat -> m[0][0] = cosf(f_theta);
    mat -> m[0][1] = sinf(f_theta);
    mat -> m[1][0] = -sinf(f_theta);
    mat -> m[1][1] = cosf(f_theta);
}

void calculate_normals(Tri3d *tri)
{
    Vert3d line1, line2;
    line1.x = tri -> v[1].x - tri -> v[0].x;
    line1.y = tri -> v[1].y - tri -> v[0].y;
    line1.z = tri -> v[1].z - tri -> v[0].z;
    
    line2.x = tri -> v[2].x - tri -> v[0].x;
    line2.y = tri -> v[2].y - tri -> v[0].y;
    line2.z = tri -> v[2].z - tri -> v[0].z;

    tri -> n.x = line1.y * line2.z - line1.z * line2.y;
    tri -> n.y = line1.z * line2.x - line1.x * line2.z;
    tri -> n.z = line1.x * line2.y - line1.y * line2.x;

    float len = sqrtf(tri -> n.x * tri -> n.x + tri -> n.y * tri -> n.y + tri -> n.z * tri -> n.z);
    tri -> n.x /= len; tri -> n.y /= len; tri -> n.z /= len;
}

void project(Tri3d *tri, Mat4x4 *mat_proj, int W, int H)
{
    Tri3d tri_proj;
    tri_proj.v[0] = matrix_vector_product(mat_proj, tri -> v[0]);
    tri_proj.v[1] = matrix_vector_product(mat_proj, tri -> v[1]);
    tri_proj.v[2] = matrix_vector_product(mat_proj, tri -> v[2]);

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
}

