#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <cstdint>

#include <string>
#include <vector>
#include <iterator>

#include <OpenGL/OpenGL.h>

#define EPSILON     .01
//#define EPSILON     .000000001
#define ERROR       .000000001

#define MINRADIUS   20
#define PADDING     2
//#define PADDING     1

#define TAU         6.283185307179586476925286766559005768394338798750211641949
#define PI          3.141592653589793238462643383279502884197169399375105820974
#define DEG2RAD     0.017453292519943295769236907684886127134428718885417254560

enum BOOLOPERATION { AND=0, OR=1, XOR=2 };                                      // The three possible boolean operations. Boolean Difference can be acomplished with AND and negative area.

inline int sign(GLdouble x) { return (x > 0)?( 1 ):( (x < 0)?( -1 ):( 0 ) ); }  // Returns the sign of a number (e.g. -1 for -2, +1 for 3). Zero returns zero.

inline GLdouble max(GLdouble x, GLdouble y) { return (x > y)?(x):(y); }         // Maximum of two `GLdouble`s.
inline GLdouble min(GLdouble x, GLdouble y) { return (x < y)?(x):(y); }         // Minimum of two `GLdouble`s.

bool getRoot(GLdouble* root, GLdouble a, GLdouble b, GLdouble c, GLdouble s=1); // Sets the value at the pointer `root` to be the right root (`s > 0`), left root (`s < 0`), or center (`s == 0`) of a parabola `y = ax^2 + bx = c`.

#endif
