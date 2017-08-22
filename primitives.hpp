#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdio.h>

#include "polyline.hpp"
#include "vector.hpp"
#include "math.hpp"
#include "device.hpp"

POLYLINE rect(VECTOR u, VECTOR v);
POLYLINE rect(GLdouble x, GLdouble y, GLdouble w, GLdouble h);

//std::function<VECTOR> linearLambda();

POLYLINE parametric(            std::function<VECTOR(GLdouble t)> lambda, size_t steps=0);
POLYLINE parametricCartesian(   std::function<GLdouble(GLdouble t)> lambdaX,
                                std::function<GLdouble(GLdouble t)> lambdaY,
                                size_t steps=0);
POLYLINE parametricCylindrical( std::function<GLdouble(GLdouble t)> lambdaR,
                                std::function<GLdouble(GLdouble t)> lambdaT,
                                size_t steps=0);

POLYLINE arc(GLdouble r, GLdouble t1_, GLdouble t2_, bool CCW=true);
GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest=true);
POLYLINE arc(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest=true);

POLYLINE linear(VECTOR p0, VECTOR p1, size_t steps=5);
POLYLINE qBezier(VECTOR p0, VECTOR p1, VECTOR p2, size_t steps=5);
POLYLINE cBezier(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, size_t steps=5);

enum CONNECTIONTYPE { LINEAR=0, QBEZIER=1, CBEZIER=2, CIRCULAR=3, MONOCIRCULAR=4 };

void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep=1);

POLYLINE connect(CONNECTION b, CONNECTION e, CONNECTIONTYPE type, bool pointsDuringLinear=false);

POLYLINE thicken(POLYLINE open, std::function<GLdouble(GLdouble t)> lambda, int side=0, GLdouble minstep=1);
POLYLINE thicken(POLYLINE open, GLdouble width, int side=0, GLdouble minstep=1);

void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambda, int side, GLdouble minstep, int i, GLdouble currentLength);

#endif
