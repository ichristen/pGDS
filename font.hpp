#ifndef FONT_H
#define FONT_H

#include <stdio.h>

//#define USE_FT_FONT 1

#ifdef USE_FT_FONT
    #include <ft2build.h>
    #include FT_FREETYPE_H

    #define FONT_SCALAR 5568
#else
//    #include "material.hpp"
#endif

#include "device.hpp"
#include "primitives.hpp"
#include "polyline.hpp"

#include <map>
#include <vector>
#include <string>
#include <algorithm>

class FONT {
    
#ifdef USE_FT_FONT
    FT_Library    library;
    FT_Face       face;
    
    std::map<char, VECTOR> advance; // Kerning.
//    FT_GlyphSlot  slot;
#else
    uint16_t    layer;
    
    GLdouble thick =    1;  // um
    GLdouble space =    1;  // um
    
    GLdouble height =   7;  // um
    GLdouble width =    5;  // um
#endif
    
    std::string fontprefix;
    
    std::map<char, DEVICE*> chars;
    
public:
    
    
#ifdef USE_FT_FONT
//    FONT(std::string fname);
#else
    
    FONT();
    FONT(uint16_t l);
    FONT(uint16_t l, GLdouble s);
    FONT(uint16_t l, GLdouble t, GLdouble h);
    
    void setThick(GLdouble t);
    void setSpace(GLdouble s);
    void setHeight(GLdouble h);
    void setWidth(GLdouble w);
    
    void recalculate();
#endif
    
    DEVICE* getChar(char c);
    DEVICE* getText(std::string text, std::string devname="");
};

#endif




