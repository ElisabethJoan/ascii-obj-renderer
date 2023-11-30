#ifndef GEOMETRY_H_
#define GEOMETRY_H_

typedef struct Vert3d
{
    double x, y, z;
} Vert3d;

typedef struct Facet3d
{
    int a, b, c, n;
} Facet3d;

typedef struct Data3d 
{
    Vert3d *verts;
    Facet3d *facets;
    Vert3d *normals;
} Data3d;

typedef struct Tri3d
{
    Vert3d v[3];
    Vert3d n;
} Tri3d;

typedef struct TriMesh3d
{
    Tri3d *tris;
    int tri_count;
} TriMesh3d;

typedef struct Quad3d
{
    Vert3d v[4];
    Vert3d n;
} Quad3d;

typedef struct QuadMesh3d
{
    Quad3d *quads;
    int quad_count;
} QuadMesh3d;

typedef struct Mat4x4
{
    float m[4][4];
} Mat4x4;

Data3d * read_obj(char *arg);
TriMesh3d * populate_trimesh(Data3d *obj_data);

Vert3d matrix_vector_product(Mat4x4 *m, Vert3d v); 
Mat4x4 * matrix_multiplication(Mat4x4 *m1, Mat4x4 *m2);

Mat4x4 * make_x_rotation(float f_theta);
Mat4x4 * make_y_rotation(float f_theta);
Mat4x4 * make_z_rotation(float f_theta);
Mat4x4 * make_projection_matrix(int height, int width);
Mat4x4 * make_translation_matrix(float z);

void calculate_normals(Tri3d *tri);
void project(Tri3d *tri, Mat4x4 *mat_proj, int W, int H);

#endif /* GEOMETRY_H_ */
