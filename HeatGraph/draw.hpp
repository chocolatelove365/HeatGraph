//
//  object.hpp
//  SfM
//
//  Created by tomiya on 2017/10/06.
//  Copyright © 2017年 tomiya. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include <stdio.h>
#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <Eigen/Core>

void draw_string(double x, double y, char *string, void *font);
void draw_point(GLfloat *vtx);
void draw_points(GLdouble *vtx, int num);
void draw_points(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx);
void draw_lines(GLdouble *vtx, int num);
void draw_lines(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx);
void draw_line_loop(GLdouble *vtx, int num, double line_width);
void draw_line_loop(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx, double line_width);
void draw_circle(double cx, double cy, double cz, double radius, int sides, double line_width);
void draw_coordinate(Eigen::Matrix4d mat);
void draw_cross(double cx, double cy, double cz, double size);
void draw_rect(double x, double y, double z, double w, double h, double line_width=1.0);
void draw_rect(double x, double y, double z, double width, double height, int sides_w, int sides_h, double var, double max, double r, double g, double b);
void draw_circle(double cx, double cy, double cz, double radius, int sides1, int sides2, double var, double max, double r, double g, double b);

#endif /* object_hpp */
