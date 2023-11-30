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

void roll(Tri3d *tri, float f_theta);
void pitch(Tri3d *tri, float f_theta);
void yaw(Tri3d *tri, float f_theta);
void translate(Tri3d *tri, float offset);
void calculate_normals(Tri3d *tri);
void project(Tri3d *tri, Mat4x4 *mat_proj, int W, int H);
Mat4x4* make_projection_matrix(int height, int width);

#endif /* GEOMETRY_H_ */
