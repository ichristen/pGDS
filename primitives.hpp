#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdio.h>

#include "polyline.hpp"
#include "vector.hpp"
#include "math.hpp"
#include "device.hpp"

#define RF_WID  10
#define RF_GAP  5
#define RF_GND  20
#define RF_LAY  4

// CLOSED PRIMITIVES ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

POLYLINE rect(VECTOR u, VECTOR v);
POLYLINE rect(VECTOR u, VECTOR v, GLdouble p);
//POLYLINE rect(GLdouble x, GLdouble y, GLdouble w, GLdouble h);
POLYLINE rect(VECTOR c, GLdouble w, GLdouble h=0, int anchorx=0, int anchory=0);
//POLYLINE rect(VECTOR c, VECTOR s, int anchorx=0, int anchory=0);

POLYLINE circle(GLdouble r, VECTOR c=VECTOR(), int steps=0);

POLYLINE ellipse(GLdouble a, GLdouble b, VECTOR c=VECTOR(), VECTOR semimajorunit=VECTOR(1,0));
POLYLINE ellipse(VECTOR focus1, VECTOR focus2, GLdouble L);

// PATHS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parametrics
POLYLINE parametric(            std::function<VECTOR(GLdouble t)> lambda, size_t steps=0);
POLYLINE parametricCartesian(   std::function<GLdouble(GLdouble t)> lambdaX,
                                std::function<GLdouble(GLdouble t)> lambdaY,
                                size_t steps=0);
POLYLINE parametricCylindrical( std::function<GLdouble(GLdouble t)> lambdaR,
                                std::function<GLdouble(GLdouble t)> lambdaT,
                                size_t steps=0);

// Conic Sections
POLYLINE arc(GLdouble r, GLdouble t1_, GLdouble t2_, bool CCW=true, int steps=0, GLdouble stepMutliplier=1);
GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest=true);
POLYLINE arc(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest=true, int steps=0, GLdouble stepMutliplier=1);

POLYLINE parabola(GLdouble x0, GLdouble x1, VECTOR focus, VECTOR vertex=VECTOR(), int steps=0);
POLYLINE parabola(GLdouble x0, GLdouble x1, GLdouble a, VECTOR center=VECTOR(), VECTOR direction=VECTOR(0,1), int steps=0);
POLYLINE parabola(GLdouble x0, GLdouble x1, GLdouble a, GLdouble b, GLdouble c, int steps=0);

//POLYLINE hyperbola(GLdouble x0, GLdouble x1, VECTOR focus, VECTOR vertex, VECTOR c=VECTOR(), int steps=0);
POLYLINE hyperbola(GLdouble x0, GLdouble x1, GLdouble a, GLdouble b, VECTOR c=VECTOR(), VECTOR direction=VECTOR(0,1), int steps=0);


POLYLINE linear( VECTOR p0, VECTOR p1, size_t steps=5);
POLYLINE qBezier(VECTOR p0, VECTOR p1, VECTOR p2, size_t steps=5);
POLYLINE cBezier(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, size_t steps=5);

POLYLINE qBezier(std::vector<VECTOR> pts);
//POLYLINE longBezier(std::vector<VECTOR> pts, bool isQuadratic=true);


enum CONNECTIONTYPE { LINEAR=0, QBEZIER=1, CBEZIER=2, CIRCULAR=3, MONOCIRCULAR=4 };

POLYLINE connect(CONNECTION b, CONNECTION e, CONNECTIONTYPE type=CIRCULAR, int numPointsDuringLinear=2, GLdouble stepMutliplier=1);

// THICKENED CONNECTIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep=1);
void connectThickenAndAdd(POLYLINES* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep=1);

POLYLINES connectThickenShortestDistance(CONNECTION b, CONNECTION e, GLdouble r, GLdouble adiabat=10, GLdouble mult=3);

POLYLINE thicken(POLYLINE open, std::function<GLdouble(GLdouble t)> lambda, GLdouble side=0, GLdouble minstep=1);
POLYLINE thicken(POLYLINE open, GLdouble width, GLdouble side=0, GLdouble minstep=1);

void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambda, GLdouble side, GLdouble minstep, int i, GLdouble currentLength); // Change? Recursion might be inefficient...

enum RFTYPE { UPPER=0, UPPERwithGROUND=1 };

POLYLINES rfThicken(POLYLINE open, GLdouble width, GLdouble gap, GLdouble groundWidth, uint16_t layer, uint16_t groundlayer=0);
POLYLINES rfThicken(POLYLINE open, RFTYPE type);
POLYLINES rfConnectThicken(CONNECTION b, CONNECTION e, RFTYPE type);

#endif




