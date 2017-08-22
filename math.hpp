#ifndef MATH_H
#define MATH_H

//#include <cstdlib>         //Console stuff
//#include <cstdio>

#include <cmath>            //Math stuff
#include <cstdint>

#include <string>
#include <vector>
#include <iterator>

//#include <SDL2/SDL.h>        //SDL

#include <OpenGL/gl3.h>      //OpenGL

//    #include <OpenGL/gl.h>
//#include <OpenGL/glu.h>         //GLU

//#define EPSILON     .000000001
#define EPSILON     .01
//#define E           10*std::nextafter(0.0,1.0)
#define E           .000000001

#define MINRADIUS   20
#define PADDING     2

#define TAU         6.283185307179586476925286766559005768394338798750211641949
#define PI          3.141592653589793238462643383279502884197169399375105820974
#define DEG2RAD     0.017453292519943295769236907684886127134428718885417254560

inline int sign(double x) { return (x > 0)?( 1 ):( (x < 0)?( -1 ):( 0 ) ); }

inline GLdouble max(GLdouble x, GLdouble y) { return (x > y)?(x):(y); }
inline GLdouble min(GLdouble x, GLdouble y) { return (x < y)?(x):(y); }

bool getRoot(GLdouble* root, GLdouble a, GLdouble b, GLdouble c, GLdouble s=1);

class VECTOR;

#endif
