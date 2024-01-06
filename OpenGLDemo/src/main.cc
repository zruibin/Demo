/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2024/01/06.
 * Copyright (c) 2024年 Ruibin.Chow All rights reserved.
 */

 
#include <iostream>
#include <math.h>
#include <time.h>

#if __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#endif

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 800);

    glBegin(GL_LINES);
    glVertex2f(-1.0, 0);
    glVertex2f(1.0, 0);
    glVertex2f(0.0, -1.0);
    glVertex2f(0.0, 1.0);
    glEnd();

    //左下角
    glColor3f(0.0, 1.0, 0.0);
    glViewport(0, 0, 400, 400);
    glutSolidTeapot(0.5); //茶壶
    
    //左上
    glColor3f(1.0, 0.0, 0.0);
    glViewport(0, 400, 400, 400);
    glutSolidTorus(0.2, 0.4, 20, 30); //圆环

    //右下
    glColor3f(1.0, 1.0, 1.0);
    glViewport(400, 0, 400, 400);
    glutWireSphere(1.0, 40, 50); //经纬线
    
    //右上
    glColor3f(0.0, 0.0, 1.0);
    glViewport(400, 400, 400, 400);
    glutSolidSphere(1.0, 40, 50); //球

    glFlush();
}

void init () {
    glClearColor(0.0,0.0,0.0,0.0);//设置背景色
    glColor3f(1.0,1.0,0.0);//设置前景色
}

int main(int argc, char* argv[]) {
	glutInit(&argc,argv);//初始化

	glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL分割窗口并分别绘图");//创建窗
	
    init();//调用display函数之前，调用初始化函数 设置背景前景 
	
    glutDisplayFunc(display);
	
    glutMainLoop();
	
    return 0;
}

