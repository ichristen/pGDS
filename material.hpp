#ifndef MATERIAL_H
#define MATERIAL_H

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
    static uint16_t currentLayer;                   // 'global' variable that new polylines will take as thier layer.
    static std::map<uint16_t, MATERIAL> layers;     // 'global' variable consisting of all loaded layers.

    std::string name;

    uint8_t colorFill[3];
    uint8_t colorOutline[3];
    
    FONT font;

    
    MATERIAL();
    MATERIAL(std::string name_, int16_t layer_, std::array<int, 3> colorFill_={255,255,255}, std::array<int, 3> colorOutline_={255,255,255}, GLdouble fontThick=1, GLdouble fontHeight=7);

    MATERIAL getLayer(uint16_t l);
    
    void setFill(   std::array<int, 3> colorFill_);
    void setOutline(std::array<int, 3> colorOutline_);
};

#endif
