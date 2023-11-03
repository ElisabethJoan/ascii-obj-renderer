#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include "graphics.h"
#include "geometry.h"

#define ROTATION_CONST 0.05f
#define TRANSLATION_CONST 2.0f

SDL_Color bgm = { 22, 22, 22, 255 };

int main(int argc, char *argv[])
{
    Data3d *obj_data = read_obj(argv[1]);
    TriMesh3d *mesh = populate_trimesh(obj_data);
    Vert3d *camera = calloc(1, sizeof(Vert3d));
   
    setup_screen();
    int W = grid_width();
    int H = grid_height();

    float x_theta, y_theta, z_theta;
    x_theta = y_theta = z_theta = 0;
    int tri_count = mesh -> tri_count;
    SDL_bool done = SDL_FALSE;
    while (!done)
    {
        SDL_Event event;
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

                float rel_dist = fmax(0.1f, tri.n.x * light_dir.x + tri.n.y * light_dir.y + tri.n.z * light_dir.z);

                // Transform

                // View

                // Clip

                // Project
                project(&tri, W, H);

                // Sort

                // Clip?

                fill_tri(tri.v[0].x, tri.v[0].y,
                    tri.v[1].x, tri.v[1].y,
                    tri.v[2].x, tri.v[2].y, '*', get_colour(rel_dist), &bgm);
                // draw_tri(tri.v[0].x, tri.v[0].y,
                //     tri.v[1].x, tri.v[1].y,
                //     tri.v[2].x, tri.v[2].y, '*', &fg, &bg);
            }
        }
        show_screen();
    }
    exit_app();
    return 0;
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
