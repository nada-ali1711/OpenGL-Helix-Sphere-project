#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h>
// Globals for sphere.
static float R_sphere = 5.0; // Radius of sphere.
static int p_sphere = 6; // Number of longitudinal slices.
static int q_sphere = 4; // Number of latitudinal slices.
static bool wireframe_sphere = false; // Flag to draw sphere in wireframe or filled mode.
// Globals for helix.
static float Xc = 0.0, Yc = 0.0; // Center of helix.
static float R_helix = 2.0; // Radius of helix.
static float P = 5.0; // Pitch of helix (height of one complete turn).
static int n_helix = 20; // Number of vertices used to draw helix.
static float turns_helix = 5.0; // Number of turns to draw.
static float offset = -20.0; // Offset for z-axis.
static float spinSpeed = 5.0; // Speed for spinning.
static float prev_time = 0.0; // Previous time for spinning.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}
int p = p_sphere - 1; // Number of longitudinal slices.
int q = q_sphere;
// Drawing routine.
void drawScene(void)
{
    int i, j;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Command to push the hemisphere, which is drawn centered at the origin,
    // into the viewing frustum.
    glTranslatef(0.0, 0.0, offset);

    // Commands to turn the hemisphere.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    // Hemisphere properties.
    if (wireframe_sphere) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glColor3f(0.0, 0.0, 0.0);

    // Array of latitudinal triangle strips, each parallel to the equator, stacked one
    // above the other from the equator to the north pole.
    for (j = 0; j < q_sphere; j++) {
        // One latitudinal triangle strip.
        glBegin(GL_TRIANGLE_STRIP);
        for (i = 0; i <= p; i++)
        {
            glVertex3f(R_sphere * cos((float)(j + 1) / q_sphere * M_PI / 2.0) * cos(2.0 * (float)i / p_sphere * M_PI),
                       R_sphere * sin((float)(j + 1) / q_sphere * M_PI / 2.0),
                       -R_sphere * cos((float)(j + 1) / q_sphere * M_PI / 2.0) * sin(2.0 * (float)i / p_sphere * M_PI));
            glVertex3f(R_sphere * cos((float)j / q_sphere * M_PI / 2.0) * cos(2.0 * (float)i / p_sphere * M_PI),
                       R_sphere * sin((float)j / q_sphere * M_PI / 2.0),
                       -R_sphere * cos((float)j / q_sphere * M_PI / 2.0) * sin(2.0 * (float)i / p_sphere * M_PI));
        }
        glEnd();
    }
    glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void spinDisplay() {
    Yangle += spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
    prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
    if (Yangle > 360.0) Yangle -= 360.0;
    glutPostRedisplay();
}
void spinDisplayReverse() {
    Yangle -= spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
    prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
    std::cout << Yangle << std::endl;
    if (Yangle < 0.0) Yangle += 360.0; // Fix the condition for reverse spin direction
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(spinDisplay);
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(spinDisplayReverse);
            break;
        default:
            break;
    }
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'P':
            p += 1;
            glutPostRedisplay();
            break;
        case 'p':
            if (p > 3) p -= 1;
            glutPostRedisplay();
            break;
        case 'Q':
            q += 1;
            glutPostRedisplay();
            break;
        case 'q':
            if (q > 3) q -= 1;
            glutPostRedisplay();
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        case 'O':
            offset += 1;
            glutPostRedisplay();
            break;
        case 'o':
            offset -= 1;
            glutPostRedisplay();
            break;
        case ' ':
            glutIdleFunc(NULL);
            break;
        default:
            break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press P/p to increase/decrease the number of longitudinal slices." << std::endl;
    std::cout << "Press Q/q to increase/decrease the number of latitudinal slices." << std::endl;
    std::cout << "Press x, X, y, Y, z, Z to turn the hemisphere." << std::endl;
}
// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("hemisphere.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouse);
    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
