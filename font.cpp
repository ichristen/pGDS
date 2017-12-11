#include "font.hpp"

#ifndef USE_FT_FONT


FONT::FONT() {
    layer = 0;
    
    setFontprefix();
}
FONT::FONT(uint16_t l) {
    layer = l;
    
    setFontprefix();
}
FONT::FONT(uint16_t l, GLdouble scale) {
    layer = l;
    
    thick   *= scale;
    space   *= scale;
    
    height  *= scale;
    width   *= scale;
    
    setFontprefix();
}
FONT::FONT(uint16_t l, GLdouble t, GLdouble h) {
    layer = l;
    
    thick   = t;
    space   = t;
    
    width  =  h*width/height;
    height  = h;
    
    setFontprefix();
}

void FONT::setFontprefix() {
    if (layer) {    fontprefix = "FONT layer=" + std::to_string(layer) + ", char="; }
    else       {    fontprefix = ""; }
}

void FONT::setThick(GLdouble t) {
    if (thick != t) {   thick = t;  recalculate(); }
}
void FONT::setSpace(GLdouble s) {
    if (space != s) {   space = s;  recalculate(); }
}
void FONT::setHeight(GLdouble h) {
    if (height != h) {  height = h; recalculate(); }
}
void FONT::setWidth(GLdouble w) {
    if (width != w) {   width = w;  recalculate(); }
}

void FONT::recalculate() {
    throw std::runtime_error("FONT::recalculate(): Not written yet...");
}

DEVICE* FONT::getChar(char c) {
    if (chars.find(c) == chars.end()) {
        if (c == 10) {          // Handle newline.
            return nullptr;
        } else if (c < 32) {    // Ignore special characters.
            return nullptr;
        }
        
        DEVICE* toReturn = new DEVICE(fontprefix + std::string(1,c));
        
        GLdouble bot = thick/2;
        GLdouble mid = height/2;
        GLdouble top = height - thick/2;
        GLdouble left = thick/2;
        GLdouble right = width - thick/2;
        
        GLdouble chamfer = width/8;
        
        uint16_t cl = MATERIAL::currentLayer;
        
        MATERIAL::currentLayer = layer;
        
        if (c == 'a' || c == 'A') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, 0);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(right, top);
            p += VECTOR(right, 0);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick, mid+bot)));
        } else if (c == 'b' || c == 'B') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, 0);
            p += VECTOR(left, top);
//            p += VECTOR(right-chamfer, top);
//            p += VECTOR(right, top-chamfer);
            p += VECTOR(right-1.5*chamfer, top);
            p += VECTOR(right-.5*chamfer, top-chamfer);
            p += VECTOR(right-.5*chamfer, mid);
            p += VECTOR(right, mid);
            p += VECTOR(right, bot);
            p += VECTOR(thick, bot);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick-.5*chamfer, mid+bot)));
        } else if (c == 'c' || c == 'C') {
            POLYLINE p = POLYLINE();
            p += VECTOR(width, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(width, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'd' || c == 'D') {
            POLYLINE p = POLYLINE();
//            p += VECTOR(0, bot);
            p += VECTOR(thick, bot);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(left, top);
            p += VECTOR(left, 0);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'e' || c == 'E') {
            POLYLINE p = POLYLINE();
            p += VECTOR(width, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(width, top);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick, mid+bot)));
        } else if (c == 'f' || c == 'F') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, 0);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(width, top);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick, mid+bot)));
        } else if (c == 'g' || c == 'G') {
            POLYLINE p = POLYLINE();
            p += VECTOR(right-1.5*thick, mid);
            p += VECTOR(right, mid);
            p += VECTOR(right, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(width, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'h' || c == 'H') {
            toReturn->add(rect(VECTOR(0, 0), VECTOR(thick, height)));
            toReturn->add(rect(VECTOR(width-thick, 0), VECTOR(width, height)));
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick, mid+bot)));
        } else if (c == 'i' || c == 'I') {
            toReturn->add(rect(VECTOR(0, height-thick), VECTOR(width, height)));
            toReturn->add(rect(VECTOR(0, 0), VECTOR(width, thick)));
            toReturn->add(rect(VECTOR(width/2 - thick/2, thick), VECTOR(width/2 + thick/2, height-thick)));
        } else if (c == 'j' || c == 'J') {
            POLYLINE p = POLYLINE();
            p += VECTOR(2*thick, top);
//            p += VECTOR(width/2, top);
            p += VECTOR(right, top);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(left, bot);
            p += VECTOR(left, 2*thick);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'k' || c == 'K') {
            toReturn->add(rect(VECTOR(0, 0), VECTOR(thick, height)));
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick, mid+bot)));
            toReturn->add(rect(VECTOR(width-thick, 0), VECTOR(width, mid+thick/2)));
            toReturn->add(rect(VECTOR(width-2*thick, mid+thick/2), VECTOR(width-thick, height)));
        } else if (c == 'l' || c == 'L') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, height);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(width, bot);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'm' || c == 'M') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, 0);
            p += VECTOR(left, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, 0);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(width/2 - thick/2, thick), VECTOR(width/2 + thick/2, height-thick)));
            
//            POLYLINE p = POLYLINE();
//            p += VECTOR(left, 0);
//            p += VECTOR(left, top);
//            p += VECTOR(2*thick+right-chamfer, top);
//            p += VECTOR(2*thick+right, top-chamfer);
//            p += VECTOR(2*thick+right, 0);
//            
//            toReturn->add(thicken(p, thick));
//            
//            toReturn->add(rect(VECTOR(thick+width/2 - thick/2, thick), VECTOR(thick+width/2 + thick/2, height-thick)));
        } else if (c == 'n' || c == 'N') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, 0);
            p += VECTOR(left, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, 0);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'o' || c == 'O') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, mid);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, mid);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'p' || c == 'P') {
            POLYLINE p = POLYLINE();
//            p += VECTOR(thick, mid-thick/2);
//            p += VECTOR(right-chamfer, mid-thick/2);
//            p += VECTOR(right, mid-thick/2+chamfer);
            p += VECTOR(thick, mid);
            p += VECTOR(right-chamfer, mid);
            p += VECTOR(right, mid+chamfer);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(left, top);
            p += VECTOR(left, 0);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'q' || c == 'Q') {
            POLYLINE p = POLYLINE();
//            p += VECTOR(width-1.5*thick, 0);
//            p += VECTOR(width-1.5*thick, 3*thick/2);
            p += VECTOR(width-thick, 3*thick/2);
            p += VECTOR(left+chamfer, 3*thick/2);
            p += VECTOR(left, 3*thick/2+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, thick);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(width-2*thick, 0), VECTOR(width-thick, thick)));
        } else if (c == 'r' || c == 'R') {
            POLYLINE p = POLYLINE();
//            p += VECTOR(thick, mid-thick/2);
//            p += VECTOR(right, mid-thick/2);
            p += VECTOR(thick, mid);
            p += VECTOR(right, mid);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(left, top);
            p += VECTOR(left, 0);
            
            toReturn->add(thicken(p, thick));
            
//            toReturn->add(rect(VECTOR(width-2*thick, 0), VECTOR(width-thick, mid-thick)));
            
            toReturn->add(rect(VECTOR(width-2*thick, 0), VECTOR(width-thick, mid-thick/2)));
        } else if (c == 's' || c == 'S') {
            POLYLINE p = POLYLINE();
            p += VECTOR(0, bot);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right, mid-chamfer);
            p += VECTOR(right-chamfer, mid);
            p += VECTOR(left+chamfer, mid);
            p += VECTOR(left, mid+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(width, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 't' || c == 'T') {
            toReturn->add(rect(VECTOR(width/2-thick/2, 0), VECTOR(width/2+thick/2, height-thick)));
            toReturn->add(rect(VECTOR(0, height-thick), VECTOR(width, height)));
        } else if (c == 'u' || c == 'U') {
            POLYLINE p = POLYLINE();
            p += VECTOR(right, height);
            p += VECTOR(right, bot + chamfer);
            p += VECTOR(right-chamfer, bot);
//            p += VECTOR(left+chamfer, bot);
//            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, bot);
            p += VECTOR(left, height);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'v' || c == 'V') {
            POLYLINE p = POLYLINE();
//            p += VECTOR(left, height);
//            p += VECTOR(left, bot + width-2*thick);
//            p += VECTOR(right-thick, bot);
//            p += VECTOR(right, bot);
//            p += VECTOR(right, height);
            p += VECTOR(right, height);
            p += VECTOR(right, bot + width-2*thick);
            p += VECTOR(left+thick, bot);
            p += VECTOR(left, bot);
            p += VECTOR(left, height);
            
            toReturn->add(thicken(p, thick));
        } else if (c == 'w' || c == 'W') {
            POLYLINE p = POLYLINE();
            p += VECTOR(right, height);
            p += VECTOR(right, bot + chamfer);
            p += VECTOR(right-chamfer, bot);
//            p += VECTOR(left+chamfer, bot);
//            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, bot);
            p += VECTOR(left, height);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(width/2-thick/2, thick), VECTOR(width/2+thick/2, height-thick)));
        } else if (c == 'x' || c == 'X') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, 0);
            p += VECTOR(left,   mid-width/2+thick/2);
            p += VECTOR(right,  mid+width/2-thick/2);
            p += VECTOR(right, height);
            
//            toReturn->add(thicken(p, thick));
            
            POLYLINE p2 = POLYLINE();
            
            p2 += VECTOR(right, 0);
            p2 += VECTOR(right,   mid-width/2+thick/2);
            p2 += VECTOR(left,  mid+width/2-thick/2);
            p2 += VECTOR(left, height);
            
//            toReturn->add(thicken(p2, thick));
            
            toReturn->add(thicken(p, thick) | thicken(p2, thick));
        } else if (c == 'y' || c == 'Y') {
            POLYLINE p = POLYLINE();
            p += VECTOR(right, height);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(thick, bot);
            
            toReturn->add(thicken(p, thick));
            
            POLYLINE p2 = POLYLINE();
            p2 += VECTOR(left, height);
            p2 += VECTOR(left, mid+chamfer);
            p2 += VECTOR(left+chamfer, mid);
            p2 += VECTOR(width-thick, mid);
            
            toReturn->add(thicken(p2, thick));
        } else if (c == 'z' || c == 'Z') {
            POLYLINE p = POLYLINE();
            p += VECTOR(width, bot);
            p += VECTOR(left, bot);
            p += VECTOR(left,   mid-width/2+thick/2);
            p += VECTOR(right,  mid+width/2-thick/2);
            p += VECTOR(right, top);
            p += VECTOR(0, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '0') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, mid);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, mid);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(width/2-thick/2, mid-thick/2), VECTOR(width/2+thick/2, mid+thick/2)));
        } else if (c == '1') {
//            toReturn->add(rect(VECTOR(thick, thick), VECTOR(2*thick, height)));
//            toReturn->add(rect(VECTOR(0, height-2*thick), VECTOR(thick, height-thick)));
//            toReturn->add(rect(VECTOR(0, 0), VECTOR(3*thick, thick)));
            
            toReturn->add(rect(VECTOR(width/2-thick/2, thick), VECTOR(width/2+thick/2, height)));
            toReturn->add(rect(VECTOR(.5*thick, height-2*thick), VECTOR(width/2-thick/2, height-thick)));
            toReturn->add(rect(VECTOR(0, 0), VECTOR(width, thick)));
        } else if (c == '2') {
            POLYLINE p = POLYLINE();
            p += VECTOR(width, bot);
            p += VECTOR(left, bot);
//            p += VECTOR(left, mid);
//            p += VECTOR(left+chamfer, bot);
//            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, mid-chamfer);
            p += VECTOR(left+chamfer, mid);
            p += VECTOR(right-chamfer, mid);
            p += VECTOR(right, mid+chamfer);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(0, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '3') {
            POLYLINE p = POLYLINE();
            p += VECTOR(0, bot);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(0, top);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(thick, mid-bot), VECTOR(width-thick, mid+bot)));
        } else if (c == '4') {
            toReturn->add(rect(VECTOR(width-thick, 0), VECTOR(width, height)));
            
            POLYLINE p2 = POLYLINE();
            p2 += VECTOR(left, height);
            p2 += VECTOR(left, mid+chamfer);
            p2 += VECTOR(left+chamfer, mid);
            p2 += VECTOR(width-thick, mid);
            
            toReturn->add(thicken(p2, thick));
        } else if (c == '5') {
            POLYLINE p = POLYLINE();
            p += VECTOR(0, bot);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(right, bot+chamfer);
            
            p += VECTOR(right, mid-chamfer);
            p += VECTOR(right-chamfer, mid);
//            p += VECTOR(right, mid);
            
//            p += VECTOR(left+chamfer, mid);
//            p += VECTOR(left, mid+chamfer);
            
            p += VECTOR(left, mid);
            
            p += VECTOR(left, top);
            p += VECTOR(width, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '6') {
            POLYLINE p = POLYLINE();
            p += VECTOR(thick, mid);
            p += VECTOR(right-chamfer, mid);
            p += VECTOR(right, mid-chamfer);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(width-thick, top);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '7') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left+thick, 0);
            p += VECTOR(left+thick,   mid-width/2+thick/2+thick);
            p += VECTOR(right,  mid+width/2-thick/2);
            p += VECTOR(right, top);
            p += VECTOR(0, top);
            
            toReturn->add(thicken(p, thick));
            
            //            toReturn->add(thicken(p, thick) | thicken(p2, thick));
        } else if (c == '8') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left, mid);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(left+chamfer, bot);
            p += VECTOR(left, bot+chamfer);
            p += VECTOR(left, mid);
            
            toReturn->add(thicken(p, thick));
            
            toReturn->add(rect(VECTOR(thick, mid-thick/2), VECTOR(width-thick, mid+thick/2)));
        } else if (c == '9') {
            POLYLINE p = POLYLINE();
            p += VECTOR(width-thick, mid);
            p += VECTOR(left+chamfer, mid);
            p += VECTOR(left, mid+chamfer);
            p += VECTOR(left, top-chamfer);
            p += VECTOR(left+chamfer, top);
            p += VECTOR(right-chamfer, top);
            p += VECTOR(right, top-chamfer);
            p += VECTOR(right, bot+chamfer);
            p += VECTOR(right-chamfer, bot);
            p += VECTOR(thick, bot);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '+') {
            toReturn->add(rect(VECTOR(0, mid-thick/2), VECTOR(width, mid+thick/2)));
            toReturn->add(rect(VECTOR(width/2-thick/2, mid-thick/2-2*thick), VECTOR(width/2+thick/2, mid-thick/2)));
            toReturn->add(rect(VECTOR(width/2-thick/2, mid+thick/2), VECTOR(width/2+thick/2, mid+thick/2+2*thick)));
        } else if (c == '-') {
            toReturn->add(rect(VECTOR(0, mid-thick/2), VECTOR(width, mid+thick/2)));
        }  else if (c == '=') {
            toReturn->add(rect(VECTOR(0, mid-3*thick/2), VECTOR(width, mid-thick/2)));
            toReturn->add(rect(VECTOR(0, mid+thick/2), VECTOR(width, mid+3*thick/2)));
        } else if (c == '*') {
            AFFINE m = AFFINE(width/2, mid) * AFFINE(TAU/8) * AFFINE(-width/2, -mid);
            
            toReturn->add(rect(VECTOR(0, mid-thick/2), VECTOR(width, mid+thick/2)) * m);
            toReturn->add(rect(VECTOR(width/2-thick/2, mid-thick/2-2*thick), VECTOR(width/2+thick/2, mid-thick/2)) * m);
            toReturn->add(rect(VECTOR(width/2-thick/2, mid+thick/2), VECTOR(width/2+thick/2, mid+thick/2+2*thick)) * m);
        } else if (c == '/') {
            AFFINE m = AFFINE(width/2, mid) * AFFINE(TAU/6) * AFFINE(-width/2, -mid);
            
            //            toReturn->add(rect(VECTOR(-2*thick, mid-thick/2), VECTOR(width+2*thick, mid+thick/2)) * m);
            
            toReturn->add(rect(VECTOR(-thick, mid-thick/2), VECTOR(width+thick, mid+thick/2)) * m);
        } else if (c == '^') {
            POLYLINE p = POLYLINE();
            p += VECTOR(width/2-1.5*thick, height-2.5*thick);
            p += VECTOR(width/2, height-thick);
            p += VECTOR(width/2+1.5*thick, height-2.5*thick);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '.') {
            toReturn->add(rect(VECTOR(width/2 - thick/2, 0), VECTOR(width/2 + thick/2, thick)));
        } else if (c == ',') {
            POLYLINE p = POLYLINE();
            
            p += VECTOR(width/2-thick/2, thick);
            p += VECTOR(width/2+thick/2, thick);
            p += VECTOR(width/2+thick/2, 0);
            p += VECTOR(width/2-thick/2, -thick);
            p.reverse();
            p.close();
            
            toReturn->add(p);
        } else if (c == '[') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left+4*chamfer, bot-thick/2);
            p += VECTOR(left+1*chamfer, bot-thick/2);
            p += VECTOR(left+1*chamfer, top+thick/2);
            p += VECTOR(left+4*chamfer, top+thick/2);
            
            toReturn->add(thicken(p, thick));
        } else if (c == ']') {
            POLYLINE p = POLYLINE();
            p += VECTOR(right-5*chamfer, bot-thick/2);
            p += VECTOR(right-1*chamfer, bot-thick/2);
            p += VECTOR(right-1*chamfer, top+thick/2);
            p += VECTOR(right-5*chamfer, top+thick/2);
            
            toReturn->add(thicken(p, thick));
        } else if (c == '(') {
            POLYLINE p = POLYLINE();
            p += VECTOR(left+4*chamfer, bot-thick/2);
            p += VECTOR(left+2*chamfer, bot-thick/2);
            p += VECTOR(left+1*chamfer, bot+chamfer-thick/2);
            p += VECTOR(left+1*chamfer, top-chamfer+thick/2);
            p += VECTOR(left+2*chamfer, top+thick/2);
            p += VECTOR(left+4*chamfer, top+thick/2);
            
            toReturn->add(thicken(p, thick));
        } else if (c == ')') {
            POLYLINE p = POLYLINE();
            p += VECTOR(right-5*chamfer, bot-thick/2);
            p += VECTOR(right-2*chamfer, bot-thick/2);
            p += VECTOR(right-1*chamfer, bot+chamfer-thick/2);
            p += VECTOR(right-1*chamfer, top-chamfer+thick/2);
            p += VECTOR(right-2*chamfer, top+thick/2);
            p += VECTOR(right-5*chamfer, top+thick/2);
            
            toReturn->add(thicken(p, thick));
        }
        
        
        MATERIAL::currentLayer = cl;
        
        return toReturn;
    }
    
    return nullptr;
}

DEVICE* FONT::getTextPrivate(std::string text) {
    DEVICE* toReturn = new DEVICE(text);
    
    VECTOR pen;
    
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '_' && i < text.size()-1) {
            i++;
            DEVICE* character = getChar(text[i]);
            
            if (character) {
                character->setLayer(layer);
                toReturn->add( DEVICEPTR(character, AFFINE(pen)*.5) );
                pen += VECTOR(width/2 + thick, 0);    // Change this...
            }
        } else {
            DEVICE* character = getChar(text[i]);
            if (character) {
                character->setLayer(layer);
                toReturn->add( DEVICEPTR(character, AFFINE(pen)) );
                pen += VECTOR(width + thick, 0);    // Change this...
            }
        }
    }
    
    return toReturn;
}
DEVICEPTR FONT::getText(std::string text, int anchorx, int anchory) {
    DEVICE* toReturn = getTextPrivate(text);
    
    VECTOR shift;
    
    if      (anchorx >  0) { shift.x = -toReturn->bb.width(); }      // Right,
    else if (anchorx == 0) { shift.x = -toReturn->bb.width()/2; }    // Center,
                                                                    // Otherwise left.
    
    if      (anchory >  0) { shift.y = -toReturn->bb.height(); }     // Top,
    else if (anchory == 0) { shift.y = -toReturn->bb.height()/2; }   // Mid,
                                                                    // Otherwise bottom.
    
    return DEVICEPTR(toReturn, AFFINE(shift));
}

#else

VECTOR::VECTOR(FT_Vector v) { x = ((GLdouble)v.x)/FONT_SCALAR; y = ((GLdouble)v.y)/FONT_SCALAR; }

FONT::FONT(std::string fname) {
    FT_Error error;
    
    fontprefix = fname.substr(0, 61);
    
    if ((error = FT_Init_FreeType(&library))) {                     printf("FT_Init_FreeType fail. %i", error); }
    if ((error = FT_New_Face(library, fname.c_str(), 0, &face))) {  printf("FT_New_Face fail. %i", error); }
    if ((error = FT_Set_Char_Size(face, 0, 1000*64, 30, 0))) {      printf("FT_Set_Char_Size fail. %i", error); }    /* use 50pt at 100dpi */
}

DEVICE* FONT::getChar(char c) {
    if (chars.find(c) == chars.end()) {
        DEVICE* toReturn = new DEVICE(fontprefix + " " + std::string(1,c));
        
        if (toReturn->initialized()) {
            printf("FONT::getChar('%c'): Use of the device name %s is reserved for the font %s.", c, toReturn->description.c_str(), fontprefix.c_str());
        }
        
        FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        
        if (error) { throw std::runtime_error("FONT::getChar(char): FT_Load_Char error with code."); }
        
        FT_Outline outline = face->glyph->outline;
        
//        If bit 0 is unset, the point is ‘off’ the curve, i.e., a Bézier control point, while it is ‘on’ if set.
//        Bit 1 is meaningful for ‘off’ points only. If set, it indicates a third-order Bézier arc control point;
//        and a second-order control point if unset.
        
        VECTOR p0(outline.points[0]);
        VECTOR p1;
        VECTOR p2;
        VECTOR p3;
        
        int k = 0;
        
//        printf("%c\n", c);
        
//        for (int i = 0; i < outline.n_points; i++) {
//            printf("%i%i - ", (outline.tags[i] & 0x02)/2, outline.tags[i] & 0x01);
//            VECTOR(outline.points[i]).printNL();
//        }
        
//        bool newcontour = true;
        
        for (int i = 0; i < outline.n_contours; i++) {
            POLYLINE contour = POLYLINE();
            
            short length = outline.contours[i];
            
            if (i != 0) {
                length -= outline.contours[i-1] + 1;
                
//                printf("c[0]: %i, c[1]: %i\n", outline.contours[0], outline.contours[1]);
            }
            
            short tlength = length;
            
            p0 = VECTOR(outline.points[k]);
            VECTOR po = p0;
            VECTOR po1 = VECTOR(outline.points[k+1]);
            char to = outline.tags[k];
            
//            printf("LENGTH: %i\n", length);
            
//            printf("k: %i\n", k);
//            p0.printNL();
            
//            newcontour = true;
            
            
            // Check outline.n_points?
            while (length > 0) {
//                printf("length: %i, k: %i\n", length, k);
                
                if (outline.tags[k] & 0x02) {   // If the second bit is flagged...   ...construct a cubic spline.
                    p1 = VECTOR(outline.points[k+1]);
                    p2 = VECTOR(outline.points[k+2]);
                    
//                    if (outline.tags[k] & 0x01) {         // If the first bit is flagged...
//                        p0 = VECTOR(outline.points[k]);
//                    }
                    
                    if (outline.tags[k+1] & 0x01) {         // If the 2nd point is 'on'
                        // throw std::runtime_error("FONT::getChar(char): Expected cubic, got linear.");
                        
                        
                        if (contour.isEmpty()) {
                            contour += p0;   // Add the first point if it hasn't already been added.
//                            p0.printNL();
//                            newcontour = false;
                        }
                        
                        contour += (p0 = VECTOR(outline.points[k+1]));     // Add a linear segment.
                        
                        length--;
                        k++;
                    } else if (outline.tags[k+2] & 0x01) {  // If the 3rd point is 'on'
                        throw std::runtime_error("FONT::getChar(char): Expected cubic, got quadratic.");
                    } else {
                        if (outline.tags[k+3] & 0x01) {     // If the 4th point is 'on', then our control point is the 4th point
                            p3 = VECTOR(outline.points[k+3]);
                            
                            length  -= 3;
                            k       += 3;
                        } else {                            // If the 4th point is 'off', then our control point is between the 3rd and 4th points.
                            p3 = (VECTOR(outline.points[k+2]) + VECTOR(outline.points[k+3]))/2;
                            
                            length  -= 2;
                            k       += 2;
                        }
                        
                        contour += cBezier(p0, p1, p2, p3);
                        
                        p0 = p3;    // Set the first point of the last curve to the first of the previous.
                    }
                    
                } else {                        // Otherwise...   ...construct a quadratic spline.
                    if (outline.tags[k+1] & 0x01) {         // If the 2nd point is 'on'
                        
//                        printf("LINEAR\n");
                        
                        if (contour.isEmpty()) {
                            contour += p0;      // Add the first point if it hasn't already been added.
                        }
                        
                        contour += (p0 = VECTOR(outline.points[k+1]));     // Add a linear segment.
                        
                        length--;
                        k++;
                    } else {
                        
                        //                        VECTOR p2;
                        //                        VECTOR p2;
//                        char tk1;
                        char tk2;
                        
                        
                        
                        if (length <= 1) {
                            tk2 = to;
                            p2 = po;
                            p1 = VECTOR(outline.points[k+1]);
//                            p1 = po1;
                        } else {
                            tk2 = outline.tags[k+2];
                            p2 = VECTOR(outline.points[k+2]);
                            p1 = VECTOR(outline.points[k+1]);
                        }
                        
                        if (tk2 & 0x01) {     // If the 3rd point is 'on', then our control point is the 3rd point
//                            p2 = VECTOR(outline.points[k+2]);
                            
                            length  -= 2;
                            k       += 2;
//                            printf("QUAD1\n");
                        } else {                            // If the 3rd point is 'off', then our control point is between the 2nd and 3rd points.
                            p2 = (p1 + p2)/2;
                            
                            length  -= 1;
                            k       += 1;
//                            printf("QUAD2\n");
                        }
                        
                        contour += qBezier(p0, p1, p2);
                        
                        p0 = p2;
                    }
                    
                }
                
//                contour.print();
            }
            
//            if (length < 0) {
//                throw std::runtime_error("FONT::getChar(char): Length does not make sense.");
//            }
            
            contour.close();
            
//            printf("%f\n", contour.area());
            
//            if (!contour.isCCW()) {
////                toReturn->polylines[0] += contour;
////                contour.reverse();
//            } // else {
                toReturn->add(contour);
//            }
            
            k++;
        }
        
//        if (toReturn->polylines.size() > 1){
//            int largest = 0;
//            GLdouble areaL = 0;
////            int smallest = 0;
////            GLdouble areaS = 0;
//            
//            for (int i = 0; i < toReturn->polylines.size(); i++) {
//                if (toReturn->polylines[i].area() > areaL) {
//                    largest = i;
//                    areaL = toReturn->polylines[i].area();
//                }
////                if (toReturn->polylines[i].area() < areaS) {
////                    smallest = i;
////                    areaS = toReturn->polylines[i].area();
////                }
//            }
//            
//            for (int i = 0; i < toReturn->polylines.size(); i++) {
//                if (toReturn->polylines[i].area() < 0) {
//                    toReturn->polylines[i].isClosed = false;
//                    toReturn->polylines[largest] += toReturn->polylines[i];
//                }
//            }
//            
//        }
        
        chars[c] = toReturn;
        advance[c] = VECTOR(face->glyph->advance) + VECTOR(.025,0);
        
        return toReturn;
    } else {
        return chars[c];
    }
}
DEVICE* FONT::getText(std::string text) {
    DEVICE* toReturn = new DEVICE(text);
    
    if (toReturn->initialized()) {
        printf("FONT::getText(\"%s\") was already created.", text.c_str());
        return toReturn;
    }
    
    VECTOR pen;
    
    bool use_kerning = FT_HAS_KERNING(face);
    
    FT_UInt previous = 0;
    FT_UInt current = 0;
    FT_Vector kerning;
    
    for (int i = 0; i < text.size(); i++) {
        if (use_kerning) {
            current = FT_Get_Char_Index(face, text[i]);
            
            if (!FT_Get_Kerning(face, previous, current, FT_KERNING_DEFAULT, &kerning)) { // If no error...
                pen += VECTOR(kerning);
            }
        
            previous = current;
        }
        
        toReturn->add( TDEVICE(getChar(text[i]), AFFINE(pen)) );
        pen += advance[text[i]];
    }
    
    return toReturn;
}
#endif



