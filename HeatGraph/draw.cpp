//
//  object.cpp
//  SfM
//
//  Created by tomiya on 2017/10/06.
//  Copyright © 2017年 tomiya. All rights reserved.
//

#include "draw.hpp"
#include <vector>
using namespace std;

void draw_string(double x, double y, char *string, void *font)
{
    int len, i;
    
    glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
    
}

void draw_point(GLdouble *vtx){
    glVertexPointer(3, GL_DOUBLE, 0, vtx);
    glPointSize(4.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_points(GLdouble *vtx, int num){
    glVertexPointer(3, GL_DOUBLE, 0, vtx);
    glPointSize(4.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, num);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    for(int i = 0; i < num; i++){
        glPushMatrix();
        glTranslated(vtx[i*3], vtx[i*3+1], vtx[i*3+2]);
        glutSolidSphere(0.000003,16,16);
        char text[50];
        sprintf(text, "%d", i);
        draw_string(0, 0, text, GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();
    }
}

void draw_points(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx){
    int num = (int)vtx.cols();
    Eigen::Map<Eigen::RowVectorXd> vec(vtx.data(), vtx.size());
    draw_points((GLdouble*)vec.data(), num);
}

void draw_lines(GLdouble *vtx, int num){
    glVertexPointer(3, GL_DOUBLE, 0, vtx);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES, 0, num);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_lines(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx){
    int num = (int)vtx.cols();
    Eigen::Map<Eigen::RowVectorXd> vec(vtx.data(), vtx.size());
    draw_lines((GLdouble*)vec.data(), num);
}

void draw_line_loop(GLdouble *vtx, int num, double line_width){
    glLineWidth(line_width);
    glVertexPointer(3, GL_DOUBLE, 0, vtx);
    glEnableClientState(GL_VERTEX_ARRAY);
    if(line_width > 0) glDrawArrays(GL_LINE_LOOP, 0, num);
    else glDrawArrays(GL_POLYGON, 0, num);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_line_loop(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx, double line_width){
    int num = (int)vtx.cols();
    Eigen::Map<Eigen::RowVectorXd> vec(vtx.data(), vtx.size());
    draw_line_loop((GLdouble*)vec.data(), num, line_width);
}

void draw_circle(double cx, double cy, double cz, double radius, int sides, double line_width){
    Eigen::MatrixXd vtx(3, sides);
    for(int i = 0; i < sides; i++){
        double x = cos(i * 2 * M_PI / sides) * radius;
        double y = sin(i * 2 * M_PI / sides) * radius;
        vtx.col(i) = Eigen::Vector3d(x+cx, y+cy, cz);
    }
    draw_line_loop(vtx, line_width);
}

void draw_coordinate(Eigen::Matrix4d mat){
    Eigen::Vector3d x_axis = mat.block(0, 0, 3, 1);
    Eigen::Vector3d y_axis = mat.block(0, 1, 3, 1);
    Eigen::Vector3d z_axis = mat.block(0, 2, 3, 1);
    Eigen::Vector3d origin = mat.block(0, 3, 3, 1);
    draw_lines((Eigen::MatrixXd(3, 6) << origin, origin+x_axis, origin, origin+y_axis, origin, origin+z_axis).finished());
}

void draw_cross(double cx, double cy, double cz, double size){
    glBegin(GL_LINES);
    glVertex3d(cx+size*0.5, cy+size*0.5, cz);
    glVertex3d(cx-size*0.5, cy-size*0.5, cz);
    glVertex3d(cx+size*0.5, cy-size*0.5, cz);
    glVertex3d(cx-size*0.5, cy+size*0.5, cz);
    glEnd();
}

void draw_rect(double x, double y, double z, double w, double h, double line_width){
    if(line_width == -1){
        glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x+w, y, z);
        glVertex3d(x+w, y+h, z);
        glVertex3d(x, y+h, z);
        glEnd();
    }
    else{
        glLineWidth(line_width);
        glBegin(GL_LINE_LOOP);
        glVertex3d(x, y, z);
        glVertex3d(x+w, y, z);
        glVertex3d(x+w, y+h, z);
        glVertex3d(x, y+h, z);
        glEnd();
    }
}

void draw_rect(double x, double y, double z, double width, double height, int sides_w, int sides_h, double var, double max, double r, double g, double b){
    GLdouble vtx[sides_w * sides_h * 4 * 3];
    GLdouble color[sides_w * sides_h * 4 * 4];
    for(int j = 0; j < sides_h; j++){
        double y1 = j * height / sides_h - height / 2;
        double y2 = (j+1) * height / sides_h - height / 2;
        GLdouble value1 = exp(-y1*y1/(2*var)) * max;
        GLdouble value2 = exp(-y2*y2/(2*var)) * max;
        for(int i = 0; i < sides_w; i++){
            double x1 = i * width / sides_w - width / 2;
            double x2 = (i+1) * width / sides_w - width / 2;
            vtx[j*sides_w*12+i*12+0] = x1 + x;
            vtx[j*sides_w*12+i*12+1] = y1 + y;
            vtx[j*sides_w*12+i*12+2] = z;
            vtx[j*sides_w*12+i*12+3] = x2 + x;
            vtx[j*sides_w*12+i*12+4] = y1 + y;
            vtx[j*sides_w*12+i*12+5] = z;
            vtx[j*sides_w*12+i*12+6] = x2 + x;
            vtx[j*sides_w*12+i*12+7] = y2 + y;
            vtx[j*sides_w*12+i*12+8] = z;
            vtx[j*sides_w*12+i*12+9] = x1 + x;
            vtx[j*sides_w*12+i*12+10] = y2 + y;
            vtx[j*sides_w*12+i*12+11] = z;
            double r1 = r;
            double g1 = g;
            double b1 = b;
            double a1 = value1;
            double r2 = r;
            double g2 = g;
            double b2 = b;
            double a2 = value1;
            double r3 = r;
            double g3 = g;
            double b3 = b;
            double a3 = value2;
            double r4 = r;
            double g4 = g;
            double b4 = b;
            double a4 = value2;
            color[j*sides_w*16+i*16+0] = r1;
            color[j*sides_w*16+i*16+1] = g1;
            color[j*sides_w*16+i*16+2] = b1;
            color[j*sides_w*16+i*16+3] = a1;
            color[j*sides_w*16+i*16+4] = r2;
            color[j*sides_w*16+i*16+5] = g2;
            color[j*sides_w*16+i*16+6] = b2;
            color[j*sides_w*16+i*16+7] = a2;
            color[j*sides_w*16+i*16+8] = r4;
            color[j*sides_w*16+i*16+9] = g4;
            color[j*sides_w*16+i*16+10] = b4;
            color[j*sides_w*16+i*16+11] = a4;
            color[j*sides_w*16+i*16+12] = r3;
            color[j*sides_w*16+i*16+13] = g3;
            color[j*sides_w*16+i*16+14] = b3;
            color[j*sides_w*16+i*16+15] = a3;
        }
    }
    glColorPointer(4, GL_DOUBLE, 0, color);
    glVertexPointer(3, GL_DOUBLE, 0, vtx);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_QUADS, 0, sides_w * sides_h * 4);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_circle(double cx, double cy, double cz, double radius, int sides1, int sides2, double var, double max, double r, double g, double b){
    GLdouble vtx[sides1*sides2*4*3];
    GLdouble color[sides1*sides2*4*4];
    
    for(int j = 0; j < sides2; j++){
        double r1 = j * radius / sides2;
        double r2 = (j+1) * radius / sides2;
        GLdouble value1 = exp(-r1*r1/(2*var)) * max;
        GLdouble value2 = exp(-r2*r2/(2*var)) * max;
        for(int i = 0; i < sides1; i++){
            double x1 = cos(i * 2 * M_PI / sides1) * j * radius / sides2 + cx;
            double y1 = sin(i * 2 * M_PI / sides1) * j * radius / sides2 + cy;
            double z1 = cz;
            double x2 = cos(i * 2 * M_PI / sides1) * (j+1) * radius / sides2 + cx;
            double y2 = sin(i * 2 * M_PI / sides1) * (j+1) * radius / sides2 + cy;
            double z2 = cz;
            double x3 = cos((i+1) * 2 * M_PI / sides1) * j * radius / sides2 + cx;
            double y3 = sin((i+1) * 2 * M_PI / sides1) * j * radius / sides2 + cy;
            double z3 = cz;
            double x4 = cos((i+1) * 2 * M_PI / sides1) * (j+1) * radius / sides2 + cx;
            double y4 = sin((i+1) * 2 * M_PI / sides1) * (j+1) * radius / sides2 + cy;
            double z4 = cz;
            vtx[j*sides1*12+i*12+0] = x1;
            vtx[j*sides1*12+i*12+1] = y1;
            vtx[j*sides1*12+i*12+2] = z1;
            vtx[j*sides1*12+i*12+3] = x2;
            vtx[j*sides1*12+i*12+4] = y2;
            vtx[j*sides1*12+i*12+5] = z2;
            vtx[j*sides1*12+i*12+6] = x4;
            vtx[j*sides1*12+i*12+7] = y4;
            vtx[j*sides1*12+i*12+8] = z4;
            vtx[j*sides1*12+i*12+9] = x3;
            vtx[j*sides1*12+i*12+10] = y3;
            vtx[j*sides1*12+i*12+11] = z3;
            double r1 = r;
            double g1 = g;
            double b1 = b;
            double a1 = value1;
            double r2 = r;
            double g2 = g;
            double b2 = b;
            double a2 = value2;
            double r3 = r;
            double g3 = g;
            double b3 = b;
            double a3 = value1;
            double r4 = r;
            double g4 = g;
            double b4 = b;
            double a4 = value2;
            color[j*sides1*16+i*16+0] = r1;
            color[j*sides1*16+i*16+1] = g1;
            color[j*sides1*16+i*16+2] = b1;
            color[j*sides1*16+i*16+3] = a1;
            color[j*sides1*16+i*16+4] = r2;
            color[j*sides1*16+i*16+5] = g2;
            color[j*sides1*16+i*16+6] = b2;
            color[j*sides1*16+i*16+7] = a2;
            color[j*sides1*16+i*16+8] = r4;
            color[j*sides1*16+i*16+9] = g4;
            color[j*sides1*16+i*16+10] = b4;
            color[j*sides1*16+i*16+11] = a4;
            color[j*sides1*16+i*16+12] = r3;
            color[j*sides1*16+i*16+13] = g3;
            color[j*sides1*16+i*16+14] = b3;
            color[j*sides1*16+i*16+15] = a3;
        }
    }
    glColorPointer(4, GL_DOUBLE, 0, color);
    glVertexPointer(3, GL_DOUBLE, 0, vtx);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_QUADS, 0, sides1*sides2*4);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


