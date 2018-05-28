#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <GL/glew.h>

//#include <macOS/pgdsNSOpenGLView.mm>

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

//#include <OpenGL/OpenGL.h>


#define XRES 640
#define YRES 480

#include "vector.hpp"
#include "device.hpp"



GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

void scroll(GLFWwindow* window, double xoffset, double yoffset);
void frame(GLFWwindow* window, int width, int height);

class WINDOW {
public:
    BOUNDINGBOX screen;
    BOUNDINGBOX bb;
    
//    VECTOR      center;
//    GLdouble    scale;
    GLdouble    rotation;
    
    DEVICEPTR   focus;
    
    GLuint      lines;
    
    GLFWwindow* window;
    GLuint shaders;
    GLuint vertexbuffer;
    
//    WINDOW(POLYLINES target);
    WINDOW();
    
    int init();
    
    void getScreen();
//    void setCamera();
    
    void loop();
    
    
//    void scroll(GLFWwindow* window, double xoffset, double yoffset);
//    void mousebutton(GLFWwindow* window, int button, int action, int mods);
    
    void drawLines();
    void render();
};

#endif
