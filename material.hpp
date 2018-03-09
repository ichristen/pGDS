#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <stdio.h>
#include <string>
#include <array>
#include <vector>
#include <map>

#include <OpenGL/OpenGL.h>

#include "font.hpp"

class MATERIAL {
private:
    uint16_t layer;    // 1, 2, ...

public:
    static uint16_t currentLayer;                   // 'global' variable that new polylines will take as their layer.
    static std::map<uint16_t, MATERIAL> layers;     // 'global' variable consisting of all loaded layers.
    static GLuint shaders;
    
    std::string name;

    GLfloat colorFill[4];
    GLfloat colorOutline[4];
    
    GLdouble turnRadius = 10;
    
    FONT font;
    
    MATERIAL();
    MATERIAL(std::string name_, int16_t layer_, std::array<GLfloat, 4> colorFill_={255,255,255,255}, std::array<GLfloat, 4> colorOutline_={255,255,255,255}, GLdouble fontThick=1, GLdouble fontHeight=7);

    MATERIAL getLayer(uint16_t l);                          // Returns the material in the `static` map corresponding to layer `l`. Eventually make this a reference so changes to this are reflected in the map?
    void setLayer(MATERIAL m, uint16_t l);                  // Sets layer `l` in the `static` map to `m`.
    
    void print();
    
    // Add methods for setting font/etc...
    
    void setFill(   std::array<GLfloat, 4> colorFill_);     // Sets the fill    color (in rgba) for for this `MATERIAL`.
    void setOutline(std::array<GLfloat, 4> colorOutline_);  // Sets the outline color (in rgba) for for this `MATERIAL`.

    void glFillColor();
    void glOutlineColor();
};

#endif
