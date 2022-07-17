#ifndef GEOMETRY_H_
#define GEOMETRY_H_

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

static Vert3d multiple_matrix_vector(Mat4x4 *m, Vert3d v);
void roll(Tri3d *tri, float f_theta);
void pitch(Tri3d *tri, float f_theta);
void yaw(Tri3d *tri, float f_theta);
void translate(Tri3d *tri, float offset);
void project(Tri3d *tri, int W, int H);

#endif /* GEOMETRY_H_ */