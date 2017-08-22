#undef __WIN32__

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

//#include "include.h"
//#include "window.h"
//#include "game.h"

//#include <SDL2/SDL.h>        //SDL

#include <OpenGL/OpenGL.h>      //OpenGL
#include <OpenGL/gl3.h>      //OpenGL
//    #include <OpenGL/gl.h>
#include <OpenGL/glu.h>         //GLU

#include "vector.hpp"
#include "polyline.hpp"
#include "primitives.hpp"

//#include <vector>
//#include <string>
//#include <iterator>

//class string4 {
//    char str[4];
//};

int main(int argc, const char * argv[]) {
    //    WINDOW window([[[NSBundle mainBundle] resourcePath] UTF8String]);
    //    if (window.initiate() < 1){ return 0; }
    //
    //    GAME game(&window);
    
    VECTOR v = VECTOR(1,2);
    VECTOR w = VECTOR(3,4);
    
    v.printNL();
    w.printNL();
    
    printf("%.2f\n", v * w);
    printf("%.2f\n", v.unit() * w.unit());
    
//    std::string str = "fish";
//    printf("%s", str.c_str());
//    
//    std::string str2 = str;
//    printf("%s", str2.c_str());
    
    printf("sizeof(GLdouble) = %lu\n", sizeof(GLdouble));
    printf("sizeof(VECTOR)   = %lu\n", sizeof(VECTOR));
    
    POLYLINE rectangle = rect(v, w);
    rectangle.print();
    printf("Area = %.2f\n", rectangle.area());
    
//    long l = 101231231242342342;
//    printf("LONG: %li\n", l);
//    
//    char* str = new char(sizeof(long)/sizeof(char));
//    *str = l;
    
//    printf("LONG: %s\n", ((string4)l).str );
//    printf("LONG: %s\n", str);
    
    
    return NSApplicationMain(argc,  (const char **) argv);
//
//    for (int i = 0; i < sizeof(unsigned int)*8; i++){
//        
//    }
    
    //    window.
    
    //    printf("HERE");
    
    //glVertex2d(1, 1);
    
    //    wxEntry(argc, (char**)argv);
    
    //    while (true){
    //        game.tick();
    //    }
    
//    return 0;
}
