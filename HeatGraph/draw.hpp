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
void draw_line_loop(GLdouble *vtx, int num);
void draw_line_loop(Eigen::Matrix<double, 3, Eigen::Dynamic> vtx);
void draw_circle(double cx, double cy, double cz, double radius, int sides);
void draw_coordinate(Eigen::Matrix4d mat);
void draw_cross(double cx, double cy, double cz, double size);

#endif /* object_hpp */
