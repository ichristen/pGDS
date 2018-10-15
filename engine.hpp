#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <stdio.h>
#include <string>
#include <array>
#include <vector>
#include <map>

#ifdef __APPLE__
//    #include <OpenGL/OpenGL.h>
//    #include <Cocoa/Cocoa.h>
#endif
#ifdef __MINGW32__

#endif
#ifdef __linux__

#endif

#include "window.hpp"
#include "vector.hpp"
#include "device.hpp"

//#include <extern/include/engine.h>  // MATLAB engine; it is slightly confusing that this has the same name...

//class WINDOW {
//#ifdef __APPLE__
//    NSView* view;
//#endif
//};

class ENGINE {
    WINDOW* window =    nullptr;    // The window that we render to.
//    Engine* engine =    nullptr;    // Our encapsulated MATLAB context.
    DEVICE* focus =     nullptr;    // The device we are looking at.
    
    std::string         path;       // The current MATLAB path.
    
    ENGINE();
    
    void update();
};

#endif
