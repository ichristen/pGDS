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

#define FONT_THICK  1   // um
#define FONT_SPACE  1   // um
#define FONT_WIDTH  5   // um
#define FONT_HEIGHT 7   // um

class FONT {
    
#ifdef USE_FT_FONT
    FT_Library    library;
    FT_Face       face;
    
    std::map<char, VECTOR> advance; // Kerning.
//    FT_GlyphSlot  slot;
#else
    uint16_t    layer;
    
    GLdouble thick =    FONT_THICK;
    GLdouble space =    FONT_SPACE;
    
    GLdouble width =    FONT_WIDTH;
    GLdouble height =   FONT_HEIGHT;
#endif
    
    std::string fontprefix;
    
    std::map<char, DEVICE*> chars;
    
public:
    
    
#ifdef USE_FT_FONT
//    FONT(std::string fname);
#else
    
    FONT();
    FONT(uint16_t l);
    FONT(uint16_t l, GLdouble scale);
    FONT(uint16_t l, GLdouble t, GLdouble h);   // Initialize the font
    
private:
    void setFontprefix();
    
public:
    void setThick(GLdouble t);                  // Set the line thickness of the default font.
    void setSpace(GLdouble s);                  // Set the space between two characters.
    void setHeight(GLdouble h);                 // Set the height of the font.
    void setWidth(GLdouble w);                  // Set the width of the font.
    
    void recalculate();                         // NotImplemented; to be used if certain variables (`thick`, `space`, etc) are changed after characters have been generated.
#endif
private:
    DEVICE* getChar(char c);
    DEVICE* getTextPrivate(std::string text);
    
public:
    DEVICEPTR getText(std::string text, int anchorx=0, int anchory=0);  // Returns device containing text. Anchor determines location of origin (e.g. `anchorx == -1 ==>` origin is on the leftmost part of resulting device; `anchorx == 0 ==>` origin is at center; etc).
};

#endif




