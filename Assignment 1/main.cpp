//
//  main.cpp
//  Assignment 1
//
//  Created by Charles Plater on 10/13/18.
//  Copyright © 2018 Charles Plater. All rights reserved.
//


//1. Read a obj mesh file containing a geometric model represented as triangle meshes and display it centered in the display window. The obj mesh file format is described here. A sample obj file can be downloaded here. A simple cube.obj could be used for testing and debugging first. More obj mesh files may be found on the internet, e.g. www.3dcafe.com.
//2. Define a virtual camera in a 3D virtual scene, specifying its position, orientation and field of view.
//3. Render an object using points, wireframe and surface representations.
//4. Using your user interface (Preferably GUI), together with the mouse and keyboard, interactively perform the following tasks:
//• Translate the model / camera in X, Y and Z directions.
//• Rotate the model / camera around X, Y, and Z axes.
//• Rotate the model /camera according to the moving direction and distance of the mouse.
//• Zoom in and zoom out view of the model.
//• Change the field of view of the camera – both horizontal and vertical.
//• Change the values of the near and far clipping plane.
//• BONUS: Rotate the model around non-major axes as intuitively as possible!

#define GL_SILENCE_DEPRECATION

#include <OpenGL/OpenGL.h>
#include <glut/glut.h>
#include <stdlib.h>
#include <tuple>
#include <vector>
#include <math.h>
#include "jpeglib.h"
#include "jerror.h"
#include <iostream>

GLuint loadImage(const char *);

GLuint pointsModel, faceModel, surfaceModel;

std::vector<std::tuple<GLfloat, GLfloat, GLfloat>> vertices;
std::vector<std::tuple<GLfloat, GLfloat, GLfloat>> facets;

int width = 1024, height = 768, rotatex = 0, rotatey = 0, rotatez = 0, mode = 0,
light = 0;
float xrot, yrot, zrot,
x = -.50, y = -.50, z = -128.0f, zNear = 10.0f, zFar = 128.0f,
hratio = (float)width / height, vratio = (float)width / height, fov = 60.0f,
eyex = 0.0f, eyey = 0.0f, eyez = -100.0f, focusx = 0.0f, focusy = 0.0f,
focusz = 0.0f, minx = 0.0f, miny = 0.0f, minz = 0.0f, maxx = 0.0f,
maxy = 0.0f, maxz = 0.0f, red = 1.0f, blue =1.0f, green = 1.0f, alpha = 1.0f;

static int menu_id, submenu_id, submenu2_id, submenu3_id, submenu4_id,
submenu5_id, submenu6_id;
GLuint check, cow, texture;


void init(void) {
    
    FILE *objFile;
    int read;
    GLfloat x, y, z;
    char ch;
    const char *filename =
    "/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment 1/cow.obj";
    
    objFile = fopen(filename, "r");
    if (!objFile) {
        printf("%s not found!\n", filename);
        exit(1);
    }
    {
        while (!(feof(objFile))) {
            read = fscanf(objFile, "%c %f %f %f", &ch, &x, &y, &z);
            if (read == 4 && ch == 'v') {
                // store the vertices for use when adding facets
                std::tuple<GLfloat, GLfloat, GLfloat> vertex(x, y, z);
                vertices.push_back(vertex);
                
                if (minx == 0 || x < minx) {
                    minx = x;
                }
                if (miny == 0 || y < miny) {
                    miny = y;
                }
                if (minz == 0 || z < minz) {
                    minz = z;
                }
                if (maxx == 0 || x > maxx) {
                    maxx = x;
                }
                if (maxy == 0 || y > maxy) {
                    maxy = y;
                }
                if (maxz == 0 || z > maxz) {
                    maxz = z;
                }
            }
            if (read == 4 && ch == 'f') {
                std::tuple<int, int, int> facet(x, y, z);
                facets.push_back(facet);
            }
        }
    }
    fclose(objFile);
    
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    
    const char *filename2 = "/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment "
    "1/checker_256x256.jpg";
    check = loadImage(filename2);
    const char *filename3 = "/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment "
    "1/cow-tex-fin.jpg";
    cow = loadImage(filename3);
    
    texture = cow;
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Create light components.
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { red, green, blue, alpha };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    // Assign created components to GL_LIGHT0.
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMatrixMode(GL_PROJECTION);
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
    
    // Plot points
    
    pointsModel = glGenLists(1);
    glPointSize(1.0);
    glNewList(pointsModel, GL_COMPILE);
    glBegin(GL_POINTS);
    for (unsigned i = 0; i < vertices.size(); i++) {
        GLfloat x1, y1, z1;
        std::tie(x1, y1, z1) = vertices.at(i);
        glVertex3f(x1, y1, z1);
    }
    glEnd();
    glEndList();
    
    // Plot facets
    
    faceModel = glGenLists(2);
    glNewList(faceModel, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for (unsigned i = 0; i < facets.size(); i++) {
        GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3;
        
        int a, b, c;
        
        std::tie(a, b, c) = facets.at(i);
        
        std::tie(x1, y1, z1) = vertices.at(a - 1);
        std::tie(x2, y2, z2) = vertices.at(b - 1);
        std::tie(x3, y3, z3) = vertices.at(c - 1);
        
        glTexCoord2f(0, 0);
        glVertex3f(x1, y1, z1);
        glTexCoord2f(1, 1);
        glVertex3f(x2, y2, z2);
        if (i%2 == 0) {
            glTexCoord2f(0,1);
        } else {
            glTexCoord2f(1, 0);
        }
        glVertex3f(x3, y3, z3);
        
        // 2 vectors
        // a->b
        // a->c
        float v1x = x2 - x1;
        float v1y = y2 - y1;
        float v1z = z2 - z1;
        
        float v2x = x3 - x1;
        float v2y = y3 - y1;
        float v2z = z3 - z1;
        
        float xn = v1y * v2z - v1z * v2y;
        float yn = v1z * v2x - v1x * v2z;
        float zn = v1x * v2y - v1y * v2x;
        
        float len = sqrt((xn * xn) + (yn * yn) + (zn * zn));
        xn /= len;
        yn /= len;
        zn /= len;
        
        glNormal3f(xn, yn, zn);
    }
    glEnd();
    glEndList();
}

void display(void) {
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glLoadIdentity();
    
    if (light) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLfloat diffuseLight[] = { red, green, blue, alpha };
        
        // Assign created components to GL_LIGHT0.
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    } else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(1.5, 1.5, 1.5);
    switch (mode) {
        case 0:
            glDisable(GL_TEXTURE_2D);
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_TEXTURE_2D);
            break;
        case 2:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindTexture(GL_TEXTURE_2D, texture);
            glEnable(GL_TEXTURE_2D);
            break;
    }
    glTranslatef(x, y, -(z / 10));
    glRotated(xrot, 1, 0, 0);
    glRotated(yrot, 0, 1, 0);
    glRotated(zrot, 0, 0, 1);
    glTranslatef(0, 0, -6.4);
    switch (mode) {
        case 0:
            glCallList(pointsModel);
            break;
        case 1:
            glCallList(faceModel);
            break;
        case 2:
            glCallList(faceModel);
            break;
    }
    
    if (rotatex) {
        xrot += 1;
    }
    if (rotatey) {
        yrot += 1;
    }
    if (rotatez) {
        zrot += 1;
    }
    if (xrot > 360) {
        xrot -= 360;
    }
    if (yrot > 360) {
        yrot -= 360;
    }
    if (zrot > 360) {
        zrot -= 360;
    }
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    hratio = (float)w / h;
    vratio = (float)w / h;
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

void loadModel(const char *filename) {
    
}

static void xrotate() {
    if (rotatex) {
        rotatex = 0;
        //        rotate = 0;
    } else
        rotatex = 1;
}

static void yrotate() {
    if (rotatey) {
        rotatey = 0;
    } else
        rotatey = 1;
}

static void zrotate() {
    if (rotatez) {
        rotatez = 0;
    } else
        rotatez = 1;
}

static void zoomIn() { z += .5; }

static void zoomOut() { z -= .5; }

static void modelUp() { ::y += .1; }

static void modelLeft() { ::x -= .1; }

static void modelDown() { ::y -= .1; }

static void modelRight() { ::x += .1; }

static void moveFarFarther() {
    ::zFar += .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void moveFarNearer() {
    ::zFar -= .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void moveNearFarther() {
    ::zNear += .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void moveNearNearer() {
    ::zNear -= .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void changeFarplane() {
    ::zFar -= .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void changeNearplane() {
    ::zNear += .01;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}
static void camrotright() {
    eyez += .5;
    //    focusy += 10;
    glMatrixMode(GL_PROJECTION);
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
}

static void camrotleft() {
    eyez -= .5;
    //    focusy -= 10;
    glMatrixMode(GL_PROJECTION);
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
}
static void camrotdown() {
    eyex += 10;
    focusx += 10;
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
}

static void camrotup() {
    eyex -= 10;
    focusx -= 10;
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
}

static void camzoomin() {
    eyez -= 10;
    focusz -= 10;
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
}

static void camzoomout() {
    eyez -= 10;
    focusz -= 10;
    gluLookAt(eyex, eyey, eyez, focusx, focusy, focusz, 0, 1, 0);
}

static void incVertFov() {
    vratio += .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void decVertFov() {
    vratio -= .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void incHorFov() {
    hratio += .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

static void decHorFov() {
    hratio -= .5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-hratio, hratio, -vratio, vratio, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

void menu(int num) {
    
    switch (num) {
        case 0:
            // quit
            glutDestroyWindow(0);
            exit(0);
            break;
        case 1:
            char *c;
            glColor3d(1.0, 0.0, 0.0);
            glRasterPos2f(200, 200);
            for (c = "Assignment 1 by Charles Plater (000032329"; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
            }
            break;
        case 3:
            // Rotate Camera X
            camrotup();
            break;
        case 4:
            // Rotate Camera Y
            camrotleft();
            break;
        case 5:
            // Rotate Camera Z
            camzoomin();
            break;
        case 6:
            // Stop Rotating Camera
            break;
        case 7:
            // Rotate Model X
            xrotate();
            break;
        case 8:
            // Rotate Model Y
            yrotate();
            break;
        case 9:
            // Rotate Model Z
            zrotate();
            break;
        case 10:
            // Stop Rotating Model
            rotatex = 0;
            rotatey = 0;
            rotatez = 0;
            xrot = 0;
            yrot = 0;
            zrot = 0;
            //            xrotate = 0;
            break;
        case 11:
            // Translate Camera X
            eyex += 10;
            break;
        case 12:
            // Translate Camera Y
            eyey += 10;
            break;
        case 13:
            // Translate Camera Z
            eyez += 10;
            break;
        case 14:
            // Stop Translating Camera
            eyex = 0;
            eyey = 0;
            eyez = 0;
            break;
        case 15:
            // View as Points
            mode = 0;
            break;
        case 16:
            // View as Wireframe
            mode = 1;
            break;
        case 17:
            // View as Solid
            mode = 2;
            break;
        case 19:
            // Zoom in
            zoomIn();
            break;
        case 20:
            // Zoom out
            zoomOut();
            break;
        case 21:
            // Change FOV horizontal
            fov -= 10;
            break;
        case 22:
            // Change FOV vertical
            fov -= .010;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            break;
        case 23:
            // Change Near Plane
            moveNearFarther();
            break;
        case 24:
            // Change Far Plane
            moveNearNearer();
            break;
        case 25:
            // Move Model Left
            modelLeft();
            break;
        case 26:
            // Move Model Right
            modelRight();
            break;
        case 27:
            // Move Model Up
            modelUp();
            break;
        case 28:
            // Move Model Down
            modelDown();
            break;
        case 29:
            moveFarFarther();
            break;
        case 30:
            moveFarNearer();
        default:
            break;
    }
    glutPostRedisplay();
}

void createMenu(void) {
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("X", 7);
    glutAddMenuEntry("Y", 8);
    glutAddMenuEntry("Z", 9);
    glutAddMenuEntry("Reset", 10);
    submenu5_id = glutCreateMenu(menu);
    glutAddMenuEntry("Points", 15);
    glutAddMenuEntry("Wireframe", 16);
    glutAddMenuEntry("Surface", 17);
    submenu2_id = glutCreateMenu(menu);
    glutAddMenuEntry("In", 19);
    glutAddMenuEntry("Out", 20);
    submenu3_id = glutCreateMenu(menu);
    glutAddMenuEntry("Horizontal", 21);
    glutAddMenuEntry("Vertical", 22);
    submenu4_id = glutCreateMenu(menu);
    glutAddMenuEntry("Increase Near", 23);
    glutAddMenuEntry("Decrease Nar", 24);
    glutAddMenuEntry("Increase Far", 29);
    glutAddMenuEntry("Decrease Far", 30);
    submenu6_id = glutCreateMenu(menu);
    glutAddMenuEntry("Left", 25);
    glutAddMenuEntry("Right", 26);
    glutAddMenuEntry("Up", 27);
    glutAddMenuEntry("Down", 28);
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("About", 1);
    glutAddSubMenu("Draw", submenu5_id);
    glutAddSubMenu("Rotate Model", submenu_id);
    glutAddSubMenu("Translate Model", submenu6_id);
    glutAddSubMenu("Zoom", submenu2_id);
    glutAddSubMenu("Change FOV", submenu3_id);
    glutAddSubMenu("Change Plane", submenu4_id);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y) {
    
    switch (key) {
        case 'x':
            xrotate();
            break;
        case 'y':
            yrotate();
            break;
        case 'z':
            zrotate();
            break;
        case 'w':
            zoomIn();
            break;
        case 's':
            zoomOut();
            break;
        case 'a':
            camrotleft();
            break;
        case 'd':
            camrotright();
            break;
        case 'i':
            modelUp();
            break;
        case 'j':
            modelLeft();
            break;
        case 'k':
            modelDown();
            break;
        case 'l':
            modelRight();
            break;
        case '1':
            mode = 0;
            break;
        case '2':
            mode = 1;
            break;
        case '3':
            mode = 2;
            break;
        case '4':
            light = 1;
            break;
        case '5':
            light = 0;
            break;
        case '6':
            texture = check;
            break;
        case '7':
            texture = cow;
            break;
        case 'r':
            moveFarFarther();
            break;
        case 'f':
            moveFarNearer();
            break;
        case 't':
            moveNearFarther();
            break;
        case 'g':
            moveNearNearer();
            break;
        case 'c':
            decHorFov();
            break;
        case 'v':
            incHorFov();
            break;
        case 'b':
            decVertFov();
            break;
        case 'n':
            incVertFov();
            break;
        case '8':
            red += .1;
            if (red > 1) {
                red = 1;
            }
            break;
        case '9':
            green += .1;
            if (green > 1) {
                green = 1;
            }
            break;
        case '0':
            blue += .1;
            if (blue > 1) {
                blue = 1;
            }
            break;
        case '-':
            alpha += .1;
            if (alpha > 1) {
                alpha = 1;
            }
            break;
        case '*':
            red -= .1;
            if (red < 0) {
                red = 0;
            }
            break;
        case '(':
            green -= .1;
            if (green < 0) {
                green = 0;
            }
            break;
        case ')':
            blue -= .1;
            if (blue < 0) {
                blue = 0;
            }
            break;
        case '_':
            alpha -= .1;
            if (alpha < 0) {
                alpha = 0;
            }
            break;
            
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            break;
        case GLUT_MIDDLE_BUTTON:
        case GLUT_RIGHT_BUTTON:
            break;
        default:
            break;
    }
}

void mouseMotion(int x, int y) {
    
    while (x > 360) {
        x -= 360;
    }
    while (y > 360) {
        y -= 360;
    }
    
    rotatex = x;
    rotatey = y;
}

GLuint loadImage(const char *FileName) {
    unsigned long x, y;
    unsigned int texture_id;
    unsigned long data_size; // Size of the image
    int channels;            // 3 =>RGB   4 =>RGBA
    unsigned int type;
    unsigned char *rowptr[1];           // pointer to an array
    unsigned char *jdata;               // data for the image
    struct jpeg_decompress_struct info; // for our jpeg info
    struct jpeg_error_mgr err;          // the error handler
    
    FILE *file = fopen(FileName, "rb"); // open the file
    
    info.err = jpeg_std_error(&err);
    jpeg_create_decompress(&info); // fills info structure
    
    // if the jpeg file doesn't load
    if (!file) {
        fprintf(stderr, "Error reading JPEG file %s!", FileName);
        return 0;
    }
    
    jpeg_stdio_src(&info, file);
    jpeg_read_header(&info, TRUE); // read jpeg file header
    
    jpeg_start_decompress(&info); // decompress the file
    
    // set width and height
    x = info.output_width;
    y = info.output_height;
    channels = info.num_components;
    type = GL_RGB;
    if (channels == 4)
        type = GL_RGBA;
    
    data_size = x * y * 3;
    
    // Read the image lines in one at a time and
    // store the date in an array.
    jdata = (unsigned char *)malloc(data_size);
    while (info.output_scanline < info.output_height) // loop
    {
        rowptr[0] =
        (unsigned char *)jdata + 3 * info.output_width * info.output_scanline;
        
        jpeg_read_scanlines(&info, rowptr, 1);
    }
    jpeg_finish_decompress(&info);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_RGB, GL_UNSIGNED_BYTE, jdata);
    
    jpeg_destroy_decompress(&info);
    fclose(file); // close the file
    free(jdata);
    
    return texture_id; // return the OpenGL textureid
}

int main(int argc, char **argv) {
    // Environment initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CSC 5870 Fall 2018 - Assignment 1");
    init();
    //    filename = "/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment "
    //    "1/checker_256x256.jpg";
    //    check = loadImage(filename);
    //    filename = "/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment "
    //    "1/cow-tex-fin.jpg";
    //    cow = loadImage(filename);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    createMenu();
    glutMainLoop();
    return 0;
}
