//
//  main.cpp
//  HeatGraph
//
//  Created by tomiya on 2017/10/27.
//  Copyright © 2017年 tomiya. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <GLUT/glut.h>
#include "draw.hpp"

using namespace std;

struct Point{
    int day;
    double x;
    double y;
    double z;
    int result;
};

const int win_width = 960;
const int win_height = 540;
const double aspect = (double)win_width/win_height;
const double height = 6.0;
const double width = height * aspect;
vector<Point> points;
const double size = 0.2;
const double size2 = 0.01;
void init(){
//    ifstream ifs("../../../../../HeatGraph/Harumafuji.csv");
    ifstream ifs("../../../../../HeatGraph/Goeido.csv");
    if(!ifs){
        cout << "ERROR: Could not read CSV files.";
    }
    string str;
    getline(ifs,str);
    while(getline(ifs,str)){
        string token;
        istringstream stream(str);
        Point pt;
        getline(stream,token,',');
        pt.day = stod(token);
        getline(stream,token,',');
        pt.x = stod(token);
        getline(stream,token,',');
        pt.y = stod(token);
        getline(stream,token,',');
        pt.z = stod(token);
        getline(stream,token,',');
        pt.result = stod(token);
//        cout << pt.day << ", " << pt.x << ", " << pt.y <<  ", " << pt.z << ", " << pt.result << "\n";
        points.push_back(pt);
    }
}

void disp(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // XY-coordinate
    glLoadIdentity();
    glTranslated(5.0, 3.0, 0.0);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    draw_circle(0.0, 0.0, 0.0, 4.55 * 0.5, 64);
//    glBegin(GL_POINTS);
//    for(int i = 0; i < points.size(); i++){
//        glPointSize(4);
//        cout << points[i].x << ", " << points[i].y << ", " << points[i].z << "\n";
//        glVertex2d(points[i].x, points[i].y);
//    }
//    glEnd();
    
    // 仕切線
    double h = -1.0;
    glColor4d(0.0, 0.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3d(-0.35f, -0.45f, h);
    glVertex3d(-0.35f, +0.45f, h);
    glVertex3d(-0.41f, +0.45f, h);
    glVertex3d(-0.41f, -0.45f, h);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3d(+0.35f, -0.45f, h);
    glVertex3d(+0.35f, +0.45f, h);
    glVertex3d(+0.41f, +0.45f, h);
    glVertex3d(+0.41f, -0.45f, h);
    glEnd();
    
    for(int i = 0; i < points.size(); i++){
        glColor4d(1.0, 0.0, 0.0, 0.1);
        glBegin(GL_QUADS);
        cout << points[i].x << ", " << points[i].y << ", " << points[i].z << "\n";
        glVertex3d(points[i].x+size/2, points[i].y+size/2, 0.0);
        glVertex3d(points[i].x-size/2, points[i].y+size/2, 0.0);
        glVertex3d(points[i].x-size/2, points[i].y-size/2, 0.0);
        glVertex3d(points[i].x+size/2, points[i].y-size/2, 0.0);
        glEnd();
        if(points[i].result == 1){
            glColor4d(0.0, 0.0, 1.0, 0.8);
            draw_circle(points[i].x, points[i].y, 0.1, 0.1, 64);
        }
        else if(points[i].result == -1){
            glColor4d(0.0, 1.0, 0.0, 0.8);
            draw_cross(points[i].x, points[i].y, 0.1, 0.2);
        }
    }
    
//    glColor4d(0.0, 0.0, 1.0, 0.1);
//    for(int i = 0; i < points.size(); i++){
//        if(points[i].result == 1){
//            glBegin(GL_QUADS);
//            glVertex3d(points[i].x+size, points[i].y+size, 0.0);
//            glVertex3d(points[i].x-size, points[i].y+size, 0.0);
//            glVertex3d(points[i].x-size, points[i].y-size, 0.0);
//            glVertex3d(points[i].x+size, points[i].y-size, 0.0);
//            glEnd();
//        }
//    }
    
    // Z-coordinate
    glLoadIdentity();
    glTranslated(9.0, 3.0, 0.0);
    glColor4d(1.0, 0.0, 0.0, 0.1);
//    glBegin(GL_POINTS);
    for(int i = 0; i < points.size(); i++){
        glBegin(GL_QUADS);
        glVertex2d(0.5, points[i].z+size2);
        glVertex2d(-0.5, points[i].z+size2);
        glVertex2d(-0.5, points[i].z-size2);
        glVertex2d(0.5, points[i].z-size2);
        glEnd();
    }
    
    glColor4d(0.0, 0.0, 0.0, 1.0);
    draw_string(0.5, 0.0, "0 cm", GLUT_BITMAP_TIMES_ROMAN_24);
    draw_string(0.5, 1.2, "120 cm", GLUT_BITMAP_TIMES_ROMAN_24);
    
    
    glutSwapBuffers();
}

int main(int argc, char * argv[]) {
    init();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(0, 100);
    glutCreateWindow("heatgraph");
    glutDisplayFunc(disp);

    glutMainLoop();
    
    return 0;
}
