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
#include <math.h>
#include "draw.hpp"

using namespace std;

struct Point{
    int day;
    double x;
    double y;
    double z;
    int result;
};

struct RGB{
    double r;
    double g;
    double b;
};

const int win_width = 600;
const int win_height = 500;
const double aspect = (double)win_width/win_height;
const double height = 6.0;
const double width = height * aspect;
vector<Point> points;
const double size = 0.2;
const double size2 = 0.01;

GLfloat* data;
const unsigned int channels = 3;

const double rect_width = 0.5;
const double scale = 3.0;
const double rect_height = 1.5 * scale;

const double center_x = 3.0;
const double center_y = 3.0;
const double rect_x = 6.0;
const double rect_y = 3.0 - rect_height / 2;

void init(){
#if 1
    ifstream ifs("../../../../../HeatGraph/Harumafuji.csv");
#else
    ifstream ifs("../../../../../HeatGraph/Goeido.csv");
#endif
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
        points.push_back(pt);
    }
}

RGB hsv2rgb(double h, double s, double v){
    double max = v;
    double min = max - ((s / 255) * max);
    RGB rgb;
    rgb.r = 0.0;
    rgb.g = 0.0;
    rgb.b = 0.0;
    
    if (h == 360){
        h = 0;
    }
    
    s = s / 100;
    v = v / 100;
    
    if (s == 0){
        rgb.r = v * 255;
        rgb.g = v * 255;
        rgb.b = v * 255;
        return rgb;
    }
    
    int dh = floor(h / 60);
    double p = v * (1 - s);
    double q = v * (1 - s * (h / 60 - dh));
    double t = v * (1 - s * (1 - (h / 60 - dh)));
    
    switch (dh){
        case 0 : rgb.r = v; rgb.g = t; rgb.b = p;  break;
        case 1 : rgb.r = q; rgb.g = v; rgb.b = p;  break;
        case 2 : rgb.r = p; rgb.g = v; rgb.b = t;  break;
        case 3 : rgb.r = p; rgb.g = q; rgb.b = v;  break;
        case 4 : rgb.r = t; rgb.g = p; rgb.b = v;  break;
        case 5 : rgb.r = v; rgb.g = p; rgb.b = q;  break;
    }
    
    rgb.r =  round(rgb.r * 255);
    rgb.g =  round(rgb.g * 255);
    rgb.b =  round(rgb.b * 255);
    return rgb;
}

void draw_heatmap(GLfloat *data, int channels){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, win_width, 0, win_height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for(int y = 0; y < win_height; y++){
        for(int x = 0; x < win_width; x++){
            if(channels == 3){
                GLfloat r = data[(y*win_width+x)*channels+0];
                GLfloat g = data[(y*win_width+x)*channels+1];
                GLfloat b = data[(y*win_width+x)*channels+2];
                double h = (1.0-b) * 220.0;
                RGB rgb = hsv2rgb(h, 100.0, 100.0);
                glColor3f(rgb.r/255.0, rgb.g/255.0, rgb.b/255.0);
                glBegin(GL_POINTS);
#if 1
                double rx = x - center_x / width * win_width;
                double ry = y - center_y / height * win_height;
//                double recx = rect_x / width * win_width;
//                double recy = rect_y / height * win_height;
                double recw = rect_width / width * win_width;
                double rech = rect_height / height * win_height;
                double recx = rect_x / width * win_width - recw/2;
                double recy = rect_y / height * win_height;
//                double 
                if(sqrt(rx*rx+ry*ry) > 4.55*0.5 / width * win_width && (x < recx || x > recx+recw || y < recy || y > recy+rech))
                    glColor3f(0.25, 0.25, 1.0);
                glVertex3d(x, y, 1.0);
#else
                glVertex3d(x, y, 1.0);
#endif
                glEnd();
            }
            else{
                cout << "the number of channels is incorrect.";
            }
        }
    }
}

void disp_density(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(center_x, center_y, 0.0);
    for(int i = 0; i < points.size(); i++){
        double x = points[i].x;
        double y = points[i].y;
//        if(x*x+y*y < 4.55*4.55/4)
        draw_circle(x, y, 0.5, 0.5, 16, 10, 0.005, 0.05, 1.0, 1.0, 1.0);
    }
    
    glLoadIdentity();
    glTranslated(rect_x, rect_y, 0.0);
    glColor4d(1.0, 1.0, 1.0, 0.015);
    for(int i = 0; i < points.size(); i++){
        double z = points[i].z * scale;
//        draw_rect(-rect_width*0.5, z-size2*0.5, 0.0, rect_width, size2, -1);
//        draw_rect(-rect_width*0.5, z-size2*0.5, 0.0, rect_width, size2, 10, 5, 0.05, 0.05, 1.0, 1.0, 1.0);
        draw_rect(0.0, z-size2*0.5, 0.0, rect_width, 0.01, 10, 20, 0.01, 0.02, 1.0, 1.0, 1.0);
    }
    
    GLfloat *dataBuffer = (GLfloat*)malloc(win_width * win_height * channels * sizeof(GLfloat));
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, win_width, win_height, GL_RGB, GL_FLOAT, dataBuffer);
    data = static_cast<GLfloat*>( dataBuffer );
    
    glutSwapBuffers();
}

void disp_heatgraph(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Heat Map
    if(data != NULL){
        draw_heatmap(data, channels);
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(center_x, center_y, 0.0);
    
    
    // 試合開始終了の位置
    for(int i = 0; i < points.size(); i++){
        double x = points[i].x;
        double y = points[i].y;
        if(sqrt(x*x+y*y) > 4.55 * 0.5){
            double s = sqrt(x*x+y*y) / (4.55*0.5);
            x /= s;
            y /= s;
        }
        if(points[i].result == 1){
            glColor4d(1.0, 1.0, 1.0, 1.0);
            draw_circle(x, y, -0.1, 0.08, 32, -1);
            glColor4d(0.0, 0.0, 0.0, 1.0);
            draw_circle(x, y, -0.1, 0.08, 32, 1);
        }
        else if(points[i].result == -1){
            glColor4d(0.0, 0.0, 0.0, 1.0);
            draw_circle(x, y, -0.1, 0.08, 32, -1);
        }
    }
    
    glLineWidth(1.0f);
    // 勝負俵
    glColor4d(1.0, 1.0, 1.0, 1.0);
    draw_circle(0.0, 0.0, 0.0, 4.55 * 0.5, 64, 1.0);
    
    // 仕切線
    double h = 0.8;
    glColor4d(1.0, 1.0, 1.0, 1.0);
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
    
    // Z-coordinate
    glLoadIdentity();
    glTranslated(rect_x, rect_y, 0.0);
    draw_rect(-rect_width * 0.5, 0.0, 0.0, rect_width, rect_height);
#if 0
    glColor4d(1.0, 1.0, 1.0, 1.0);
    draw_string(rect_width * 0.5, 0.0, "0 m", GLUT_BITMAP_TIMES_ROMAN_24);
    draw_string(rect_width * 0.5, rect_height, "1.5 cm", GLUT_BITMAP_TIMES_ROMAN_24);
#endif
    
    glutSwapBuffers();
}

int main(int argc, char * argv[]) {
    init();
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(0, 100);
    glutCreateWindow("density");
    glutDisplayFunc(disp_density);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(0, 200);
    glutCreateWindow("heatgraph");
    glutDisplayFunc(disp_heatgraph);

    glutMainLoop();
    
    return 0;
}
