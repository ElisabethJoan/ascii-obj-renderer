#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

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

Data3d *obj_data;
TriMesh3d *mesh;
Vert3d *camera;
Mat4x4 *mat_proj;

float x_theta = 0;
float y_theta = 0;
float z_theta = 0;

int tri_count = 0;
int W = 0;
int H = 0;

void init()
{
    W = grid_width();
    H = grid_height();

    obj_data = read_obj("assets/cube.obj");
    mesh = populate_trimesh(obj_data);
    camera = calloc(1, sizeof(Vert3d));

    setup_screen();

    tri_count = mesh -> tri_count;

    mat_proj = make_projection_matrix(H, W);
}

void main_loop(void)
{
    handle_events();
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
            float len = sqrtf(light_dir.x * light_dir.x + light_dir.y * light_dir.y + light_dir.z * light_dir.z);
            light_dir.x /= len; light_dir.y /= len; light_dir.z /= len;

            float rel_dist = fmax(0.1f, tri.n.x * light_dir.x + tri.n.y * light_dir.y + tri.n.z * light_dir.z);

            // Transform

            // View

            // Clip
            
            // Project
            project(&tri, mat_proj, W, H);
            
            // Sort

            // Clip?

            fill_tri(tri.v[0].x, tri.v[0].y,
                tri.v[1].x, tri.v[1].y,
                tri.v[2].x, tri.v[2].y, '*', get_colour(rel_dist), &bgm);
            // draw_tri(tri.v[0].x, tri.v[0].y,
            //     tri.v[1].x, tri.v[1].y,
            //     tri.v[2].x, tri.v[2].y, '*', get_colour(rel_dist), &bgm); 
        }
    }
    show_screen();
}

int main(int argc, char *argv[])
{
    init();
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #endif

    #ifndef __EMSCRIPTEN__
        while (!done)
        {
            main_loop();
        }
    #endif

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
