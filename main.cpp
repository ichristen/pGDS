//#define USE_FT_FONT 1
#define USE_GL_RENDER 1
//#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>

#include "window.hpp"

#include "testdevices.hpp"
//#include "diamonddevices.hpp"
//#include "eodevices.hpp"
//#include "testing.hpp"
//#include "harvarddevices.hpp"
#include "quantumdevices.hpp"
#include "vapordevices.hpp"
//#include "tesselation.hpp"

int main(int argc, const char * argv[]) {
    WINDOW w;
    
//    DEVICE* c = getDevice("Circle");
//    c->add(circle(5));
    
//    DEVICE* c = grating(637);
//    DEVICE* c = testY2();
//    DEVICEPTR c = DEVICEPTR(superDuperDevice2(637));
//    DEVICEPTR c = DEVICEPTR(loss(2));
    //testD2(637);
    
    w.focus = VAPORTEST();
    
//    w.loop();
}




