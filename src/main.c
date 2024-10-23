#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "graphics.h"
#include "geometry.h"

#define ROTATION_CONST 0.05f
#define TRANSLATION_CONST 3.0f

void handle_events(void);

SDL_bool done = SDL_FALSE;
SDL_Event event;
SDL_Color bgm = { 22, 22, 22, 255 };

TriMesh3d *mesh;
Vert3d *camera;
Mat4x4 *mat_proj;
Mat4x4 *mat_trans;
Mat4x4 *mat_rot_x; 
Mat4x4 *mat_rot_y;
Mat4x4 *mat_rot_z;
Mat4x4 *world_mat;

float x_theta = 0;
float y_theta = 0;
float z_theta = 0;

int tri_count = 0;
int W = 0;
int H = 0;
int frames = 0;

void init(Data3d *obj_data)
{
    W = grid_width();
    H = grid_height();

    mesh = populate_trimesh(obj_data);
    camera = calloc(1, sizeof(Vert3d));

    setup_screen();

    tri_count = mesh -> tri_count;

    mat_proj = make_projection_matrix(H, W);
    mat_trans = make_translation_matrix(TRANSLATION_CONST);

    mat_rot_x = make_x_rotation(x_theta);
    mat_rot_y = make_y_rotation(y_theta);
    mat_rot_z = make_z_rotation(z_theta);
    
    world_mat = calloc(1, sizeof(Mat4x4));


    free(obj_data -> verts);
    free(obj_data -> facets);
    free(obj_data -> normals);
    free(obj_data);
}

void cleanup()
{
  free(mesh -> tris);
  free(mesh);
  free(camera);
  free(mat_proj);
  free(mat_trans);
  free(mat_rot_x);
  free(mat_rot_y);
  free(mat_rot_z);
  free(world_mat);
}

void main_loop(void)
{
    handle_events();
    clear_screen();

    // roll(mat_rot_x, x_theta);
    mat_rot_x -> m[1][1] = cosf(x_theta);
    mat_rot_x -> m[1][2] = sinf(x_theta);
    mat_rot_x -> m[2][1] = -sinf(x_theta);
    mat_rot_x -> m[2][2] = cosf(x_theta);
    // pitch(mat_rot_y, y_theta);
    mat_rot_y -> m[0][0] = cosf(y_theta);
    mat_rot_y -> m[0][2] = sinf(y_theta);
    mat_rot_y -> m[2][0] = -sinf(y_theta);
    mat_rot_y -> m[2][2] = cosf(y_theta);
    // yaw(mat_rot_z, z_theta);
    mat_rot_z -> m[0][0] = cosf(z_theta);
    mat_rot_z -> m[0][1] = sinf(z_theta);
    mat_rot_z -> m[1][0] = -sinf(z_theta);
    mat_rot_z -> m[1][1] = cosf(z_theta);

    *world_mat = matrix_multiplication(mat_rot_x, mat_rot_y);
    *world_mat = matrix_multiplication(world_mat, mat_rot_z);
    *world_mat = matrix_multiplication(world_mat, mat_trans);
    
    for (int i = 0; i < tri_count; i++)
    {
        Tri3d *tri = calloc(1, sizeof(Tri3d));

        // World Matrix Transformation
        tri -> v[0] = matrix_vector_product(world_mat, mesh -> tris[i].v[0]);
        tri -> v[1] = matrix_vector_product(world_mat, mesh -> tris[i].v[1]);
        tri -> v[2] = matrix_vector_product(world_mat, mesh -> tris[i].v[2]);
        
        // Calculate triangle Normal
        calculate_normals(tri);

        // Get Ray from triangle to camera
        Vert3d ray = { tri -> v[0].x - camera -> x, tri -> v[0].y
          - camera -> y, tri -> v[0].z - camera -> z };
        float ray_dist = tri -> n.x * ray.x + tri -> n.y * ray.y + tri -> n.z * ray.z;

        if (ray_dist < 0.0f)
        {
            Vert3d light_dir = { 0.0f, 1.0f, -1.0f };
            float len = sqrtf(light_dir.x * light_dir.x + light_dir.y * light_dir.y
                + light_dir.z * light_dir.z);
            light_dir.x /= len; light_dir.y /= len; light_dir.z /= len;

            float rel_dist = fmax(0.1f, tri -> n.x * light_dir.x + tri -> n.y * light_dir.y
                + tri -> n.z * light_dir.z);

            // Project
            project(tri, mat_proj, W, H);
            
            // Render
            fill_tri(tri -> v[0].x, tri -> v[0].y,
                tri -> v[1].x, tri -> v[1].y,
                tri -> v[2].x, tri -> v[2].y, '*', get_colour(rel_dist), &bgm);
            // draw_tri(tri.v[0].x, tri.v[0].y,
            //     tri.v[1].x, tri.v[1].y,
            //     tri.v[2].x, tri.v[2].y, '*', get_colour(rel_dist), &bgm); 
        }
        free(tri);
    }
    show_screen();
    // TODO free colour
    frames++;
}

int main(int argc, char *argv[])
{
    Data3d *obj_data = read_obj(argv[1]);
    init(obj_data);
    Uint32 start = SDL_GetTicks();

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #endif

    #ifndef __EMSCRIPTEN__
        while (!done)
        {
            clock_t begin = clock();
            main_loop();
            clock_t end = clock();
            double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
            time_spent = time_spent * 1000;
            time_spent = time_spent * 1000000;
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 33333333.3333L - time_spent; 
            nanosleep(&ts, NULL);
        }
    #endif

    double fps = frames / ((SDL_GetTicks() - start) / 1000.0);
    printf("FPS: %f\n", fps);
    cleanup();
    exit_app();
    return 0;
}


void handle_events(void)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_w:
                    case SDLK_UP:
                        x_theta -= ROTATION_CONST;
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        x_theta += ROTATION_CONST;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        y_theta -= ROTATION_CONST;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        y_theta += ROTATION_CONST;
                        break;
                    case SDLK_q:
                        z_theta -= ROTATION_CONST;
                        break;
                    case SDLK_e:
                        z_theta += ROTATION_CONST;
                        break;
                    case SDLK_r:
                        x_theta = y_theta = z_theta = 0;
                        break;
                }
                break;
            case SDL_QUIT:
                done = SDL_TRUE;
                break;
        }
    }
}




// ------------------------------------- OpenGL ----------------------------------------
 
// #include <GL/glut.h> 
// 
// GLuint height, width;
// int window;
// void* current_font;
// 
// int create_window(char* title, int width, int height, int x, int y) {
//   int id;
// 
//   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//   glutInitWindowSize(width, height);
//   glutInitWindowPosition(x, y);
//   id = glutCreateWindow(title);
// 
//   return id;
// }
// 
// void set_font(void* font) {
//   current_font = font;
// }
// 
// void draw_string(int x, int y, char *string) 
// {
//   glRasterPos2i(x, y);
//   for (char* c = string; *c != '\0'; c++) 
//   {
//     glutBitmapCharacter(current_font, *c);
//   }
// }
// 
// void init() 
// {
//   glClearColor(1.0, 1.0, 1.0, 1.0);
// 
//   // Setup the projection
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
// 
//   glOrtho(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);  // Orthographic
// 
//   set_font(GLUT_BITMAP_HELVETICA_18);
// }
// 
// void begin_text() 
// {
//   glMatrixMode(GL_PROJECTION);
// 
//   // Save the current projection matrix
//   glPushMatrix();
// 
//   // Make the current matrix the identity matrix
//   glLoadIdentity();
// 
//   // Set the projection (to 2D orthographic)
//   gluOrtho2D(0, width, 0, height);
// 
//   glMatrixMode(GL_MODELVIEW);
// }
// 
// void end_text() 
// {
//   glMatrixMode(GL_PROJECTION);
// 
//   // Restore the original projection matrix
//   glPopMatrix();
// 
//   glMatrixMode(GL_MODELVIEW);
// }
// 
// void display() 
// {
//   glClear(GL_COLOR_BUFFER_BIT);
//   glColor3f(0.0, 0.0, 0.0);
// 
//   // Set things up so that we can render text in
//   // window coordinates
//   begin_text();
// 
//   // Render the text
//   draw_string(0, 0, "MEOW");
// 
//   // Set things up for normal rendering
//   end_text();
// 
//   // Normal rendering of the scene goes here
// 
//   glFlush();
// }
// 
// void reshape(int w, int h) 
// {
//   GLfloat aspect;
// 
//   // Save the width and height for text processing
//   width = w;
//   height = h;
// 
//   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
// 
//   glMatrixMode(GL_PROJECTION);
// 
//   glLoadIdentity();
// 
//   if (w <= h) {
//     aspect = (GLfloat) h / (GLfloat) w;
//     glOrtho(-1.5, 1.5, -1.5 * aspect, 1.5 * aspect, -10.0, 10.0);
//   } else {
//     aspect = (GLfloat) w / (GLfloat) h;
//     glOrtho(-1.5 * aspect, 1.5 * aspect, -1.5, 1.5, -10.0, 10.0);
//   }
// }
// 
// int main(int argc, char** argv) 
// {
//   glutInit(&argc, argv);
// 
//   window = create_window("ASCII OBJ VIEWER", 640, 480, 0, 0);
// 
//   glutDisplayFunc(display);
//   glutReshapeFunc(reshape);
// 
//   init();
// 
//   glutMainLoop();
// }
//
// LDLIBS := -lGL -lGLU -lglut -lm
