#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdio.h>

#include "polyline.hpp"
#include "vector.hpp"
#include "math.hpp"
#include "wavelengths.hpp"
#include "device.hpp"

#define RF_WID  16
#define RF_GAP  4
#define RF_GND  16
#define RF_LAY  4

//#define RF_WID  12
//#define RF_GAP  4
//#define RF_GND  24
//#define RF_LAY  4

//#define CONNECTTHICKEN_DEBUG 1

// CLOSED PRIMITIVES ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

POLYLINE rect(VECTOR u, VECTOR v);
POLYLINE rect(VECTOR c, GLdouble w, GLdouble h=0, int anchorx=0, int anchory=0);
POLYLINE rect(GLdouble w, GLdouble h=0, int anchorx=0, int anchory=0);

POLYLINE roundRect(VECTOR u, VECTOR v, GLdouble r);
POLYLINE roundRect(VECTOR c, GLdouble r, GLdouble w, GLdouble h=0, int anchorx=0, int anchory=0);

POLYLINE path(VECTOR u, VECTOR v, GLdouble p);

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
GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest);
GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, VECTOR db);
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

POLYLINE euler90(GLdouble R, int steps=100);


enum CONNECTIONTYPE { LINEAR=0, QBEZIER=1, CBEZIER=2, CIRCULAR=3, MONOCIRCULAR=4 };

POLYLINE connect(CONNECTION b, CONNECTION e, CONNECTIONTYPE type=CIRCULAR, int numPointsDuringLinear=2, GLdouble stepMutliplier=1);

// THICKENED CONNECTIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WAVELENGTH;

POLYLINES connectThicken(CONNECTION b, CONNECTION e, CONNECTIONTYPE type=CIRCULAR, GLdouble minstep=1, GLdouble padding=PADDING);
POLYLINES connectThicken(CONNECTION b, CONNECTION e, GLdouble pad);
void connectThickenSMTaper(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble a0, GLdouble adiabat);
void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble a0,      GLdouble tb, GLdouble te, GLdouble lb=0, GLdouble le=0, GLdouble rad=0);
void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, WAVELENGTH wl,    GLdouble tb, GLdouble te, GLdouble lb=0, GLdouble le=0, GLdouble rad=0);
void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep=1, GLdouble padding=PADDING);
void connectThickenAndAdd(POLYLINES* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep=1, GLdouble padding=PADDING);
void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, GLdouble stepmultiplier, CONNECTIONTYPE type);

POLYLINES connectThickenShortestDistance(CONNECTION b, CONNECTION e, GLdouble r, GLdouble adiabat=15, GLdouble mult=1, GLdouble padding=PADDING, GLdouble a=0, GLdouble stepMutliplier=1);

POLYLINE thicken(POLYLINE open, std::function<GLdouble(GLdouble t)> lambda, GLdouble side=0, GLdouble minstep=1);
POLYLINE thicken(POLYLINE open, GLdouble width, GLdouble side=0, GLdouble minstep=1);

//POLYLINE thicken(POLYLINE open, std::function<GLdouble(GLdouble t)> lambda, GLdouble side=0, GLdouble minstep=1);
POLYLINES thicken(POLYLINE open, std::vector<GLdouble> widths, GLdouble minstep=1);

POLYLINES thicken(POLYLINES open, std::function<GLdouble(GLdouble t)> lambda, GLdouble side=0, GLdouble minstep=1);
POLYLINES thicken(POLYLINES open, GLdouble width, GLdouble side=0, GLdouble minstep=1);

void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambda, GLdouble side, GLdouble minstep, int i, GLdouble currentLength); // Change? Recursion might be inefficient...
void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambdal, std::function<GLdouble(GLdouble t)>* lambdar, GLdouble minstep, int i, GLdouble currentLength);

enum RFTYPE { UPPER=0, UPPERwithGROUND=1 };

POLYLINES rfThicken(POLYLINE open, GLdouble width, GLdouble gap, GLdouble groundWidth, uint16_t layer, uint16_t groundlayer=0);
POLYLINES rfThicken(POLYLINE open, RFTYPE type);
POLYLINES rfConnectThicken(CONNECTION b, CONNECTION e, RFTYPE type);

//POLYLINE offset;

#endif




