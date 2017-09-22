#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <OpenGL/OpenGL.h>

#include "vector.hpp"
#include "device.hpp"

class WINDOW {
public:
    VECTOR      center;
    GLdouble    scale;
    GLdouble    rotation;
    
    DEVICE*     focus;
    
    int         width;
    int         height;
    
    GLuint      lines;
    
    void drawLines();
    void render();
};

#endif
