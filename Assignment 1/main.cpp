//
//  main.cpp
//  Assignment 1
//
//  Created by Charles Plater on 10/13/18.
//  Copyright © 2018 Charles Plater. All rights reserved.
//

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <glut/glut.h>
#include <stdio.h>
#include <tuple>
#include <vector>

GLuint model;
float rotate;
//std::tuple<int,int,int> vertex;
std::vector< std::tuple<GLfloat,GLfloat,GLfloat> > vertices;
//std::tuple<int,int,int> facet;
std::vector< std::tuple<GLfloat,GLfloat,GLfloat> > facets;

void init(void)
{

}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

}

void mouse(int button, int state, int x, int y) {

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
                std::tuple<int,int,int> vertex (x,y,z);
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

void plotPoints() {
    
    model=glGenLists(1);
    glPointSize(2.0);
    glNewList(model, GL_COMPILE);
    glPushMatrix();
//    GL_POINTS == points;
//    GL_TRIANGLE== facets;
    glBegin(GL_POINTS);
    for (unsigned i=0; i < vertices.size(); i++) {
        int x1,y1,z1;
        std::tie (x1,y1,z1) = vertices.at(i);
        glVertex3f(x1,y1,z1);
    }
    glEnd();
    glPopMatrix();
    glEndList();
}

void plotFacets() {
    model=glGenLists(1);
    glPointSize(2.0);
    glNewList(model, GL_COMPILE);
    glPushMatrix();
            glBegin(GL_TRIANGLES);
            for (unsigned i=0; i < facets.size(); i++) {
                int x1,y1,z1,a,b,c;
                
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
void drawModel()
{
    glPushMatrix();
    glTranslatef(0, 0.00, -256);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(50.0, 50.0, 50.0);
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glRotatef(rotate, 0, 1, 0);
    glCallList(model);
    glPopMatrix();
    rotate += 1;
    if(rotate>360) {
        rotate -= 360;
    }
}

void display(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawModel();
    glutSwapBuffers();

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1024, 768);
    glutInitWindowPosition (20, 20);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    gluLookAt(.1, .1, .5, 10.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    char *filename="/Users/cplater/Developer/CSC 5870/Assignment 1/Assignment 1/cow.obj";
    loadModel(filename);
    plotPoints();
//    plotFacets();
    glutMainLoop();
    return 0;
}
