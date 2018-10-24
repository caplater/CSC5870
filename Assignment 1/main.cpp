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

//TODO: render using surface representation
//TODO: Rotate the camera around the X, Y, and Z axes.
//TODO: Rotate the model / camera according to the moving direction and distance of the mouse.
//TODO: Change the field of view of the camera - both horizontal and vertical.
//TODO: Change the values of the near and far clipping plane

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <glut/glut.h>
#include <stdio.h>
#include <tuple>
#include <vector>
#include <math.h>

//using namespace std;

std::vector< std::tuple<GLfloat,GLfloat,GLfloat> > vertices;
std::vector< std::tuple<GLfloat,GLfloat,GLfloat> > facets;
GLuint pointsModel,faceModel, surfaceModel;
float rotate;


int width = 1024, height = 768, eyex = 0, eyey = 0, eyez = 5, rotatex = 0, rotatey = 0, rotatez = 0, mode = 0;
float x = 0.0f, y = 0.0f, z = -64.0f, zNear = -5.0f, zFar = -128.0f;
float ratio = (float) width / height;


void plotPoints() {
    
    pointsModel=glGenLists(1);
    glPointSize(2.0);
    glNewList(pointsModel, GL_COMPILE);
    glPushMatrix();
    glBegin(GL_POINTS);
    for (unsigned i=0; i < vertices.size(); i++) {
        GLfloat x1,y1,z1;
        std::tie (x1,y1,z1) = vertices.at(i);
        glVertex3f(x1,y1,z1);
    }
    glEnd();
    glPopMatrix();
    glEndList();
}

void plotFacets() {
    faceModel=glGenLists(2);
    glPointSize(2.0);
    glNewList(faceModel, GL_COMPILE);
    glPushMatrix();
    
    glBegin(GL_TRIANGLES);
    
    glShadeModel(GL_FLAT);
    for (unsigned i=0; i < facets.size(); i++) {
        GLfloat x1,y1,z1;
        int a,b,c;
        
        std::tie (a,b,c) = facets.at(i);
        std::tie (x1,y1,z1) = vertices.at(a-1);
        glVertex3f(x1,y1,z1);
        std::tie (x1,y1,z1) = vertices.at(b-1);
        glVertex3f(x1,y1,z1);
        std::tie (x1,y1,z1) = vertices.at(c-1);
        glVertex3f(x1,y1,z1);
    }
    
    glEnd();
    glPopMatrix();
    glEndList();
    
}

//void plotSurface() {
//    surfaceModel=glGenLists(3);
//    glPointSize(2.0);
//    glNewList(faceModel, GL_COMPILE);
//    glPushMatrix();
//    
//    glBegin(GL_TRIANGLES);
//    
//    glShadeModel(GL_FLAT);
//    for (unsigned i=0; i < facets.size(); i++) {
//        GLfloat x1,y1,z1,x2,y2,z2,x3,y3,z3;
//        int a,b,c;
//        
//        std::tie (a,b,c) = facets.at(i);
//        std::tie (x1,y1,z1) = vertices.at(a-1);
//        std::tie (x2,y2,z2) = vertices.at(b-1);
//        std::tie (x3,y3,z3) = vertices.at(c-1);
//        
////        GLfloat Ax = x2-x1;
////        GLfloat Ay = y2-y1;
////        GLfloat Az = z2-z1;
////        GLfloat Bx = x3-x2;
////        GLfloat By = y3-y2;
////        GLfloat Bz = z3-z2;
//        
//        GLfloat Nx = (y2-y1)*(z3-z2)-(z2-z1)*(y3-y2);
//        GLfloat Ny = (z2-z1)*(x3-x2)-(x2-x1)*(z3-z2);
//        GLfloat Nz = (x2-x1)*(y3-y2)-(y2-y1)*(x3-x2);
//        float length=sqrt(Nx * Nx + Ny * Ny + Nz * Nz);
//        Nx /= length;
//        Ny /= length;
//        Nz /= length;
//        
//        glNormal3f(-Nx, -Ny,-Nz);
//        glVertex3f(x1,y1,z1);
//        glVertex3f(x2,y2,z2);
//        glVertex3f(x3,y3,z3);
//    }
//    
//    glEnd();
//    glPopMatrix();
//    glEndList();
//    
//}

void drawModel()
{
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(1.5, 1.5, 1.5);
    switch (mode) {
        case 1:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            break;
        case 2:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            break;
    }
//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glTranslatef(x, y, -(z/10));
    
    if (rotatex | rotatey | rotatez) {
    
    glRotatef(rotate, rotatex, rotatey, rotatez);
    }
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
    
    glPopMatrix();
//    glTranslatef(0,0,64*5);
//    glLoadIdentity();
//    glPushMatrix();
//    gluLookAt(eyex, eyey, eyez, 0, 0, -128, 0, 0, 1);
//    glRotatef(rotatex, 0, 1, 0);
//    eyex -= 10;
//    eyey -= 10;
//    eyez -= 10;
//    glPopMatrix();
//    glFlush();
    
//    gluPerspective(60, width/height/2, 1, 1000);
//    glFrustum(1.0, 1.0, 1.0, 1.0, zNear, zFar);
//    glMatrixMode(GL_MODELVIEW);
    
    rotate += 1;
    if(rotate>360) {
        rotate -= 360;
    }
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

void display(void)
{
    
    plotPoints();
    plotFacets();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawModel();
    
    
//    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.1, 1000.0);
//    glFrustum(1.0, 1.0, 1.0, 1.0, zNear, zFar);
    glFrustum(-ratio, ratio, -1.0, 1.0, zNear, zFar);

//    gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(200, 0, 1, 200, 0, 0, 1, 0, 1);

    glutSwapBuffers();
}

void reshape (int w, int h)
{
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // The next line swapped out for glfrustum cuases the image to not load.
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);
    glFrustum(-ratio, ratio, -1.0, 1.0, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);

}

void loadModel(const char *filename)
{
    FILE *objFile;
    int read;
    GLfloat x,y,z;
    char ch;
    objFile=fopen(filename, "r");
    if (!objFile) {
        printf("%s not found!\n", filename);
        exit(1);
    }
    {
        while(!(feof(objFile))) {
            read=fscanf(objFile, "%c %f %f %f", &ch,&x,&y,&z);
            if(read==4&&ch=='v') {
                // store the vertices for use when adding facets
                std::tuple<GLfloat,GLfloat,GLfloat> vertex (x,y,z);
                vertices.push_back(vertex);
            }
            if(read==4&&ch=='f') {
                std::tuple<int,int,int> facet (x,y,z);
                facets.push_back(facet);
            }
        }
    }
    fclose(objFile);
}

void keyboard(unsigned char key, int x, int y) {
    
    switch (key) {
        case 'x':
            if (rotatex) {
                rotatex = 0;
            }
            else {
                rotatex =1;
            }
            break;
        case 'y':
            if (rotatey) {
                rotatey = 0;
            }
            else {
                rotatey =1;
            }
            break;
        case 'z':
            if (rotatez) {
                rotatez = 0;
            }
            else {
                rotatez =1;
            }
            break;
        case 'w':
            z += .5;
            break;
        case 's':
            z -= .5;
            break;
        case 'i':
            ::y += .1;
            break;
        case 'j':
            ::x -= .1;
            break;
        case 'k':
            ::y -= .1;
            break;
        case 'l':
            ::x += .1;
            break;
        case 'n':
            ::zNear -= 1;
            if (zNear < z) {
                zNear = z + 1;
            }
            break;
        case 'f':
            ::zFar -= 1;
            if (zFar > z) {
                zFar = z - 1;
            }
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
    }
}

int main(int argc, char** argv)
{
    char *filename="/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment 1/cow.obj";
    loadModel(filename);
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("CSC 5870 Fall 2018 - Assignment 1");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
