#ifndef TESSELATION_HPP
#define TESSELATION_HPP

#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <functional>

#include <OpenGL/OpenGL.h>

#include "polyline.hpp"
#include "device.hpp"

#define TESSDEBUG 1

class POLYLINE;
class POLYLINES;

void tessCTL(POLYLINE& p,   FILE* f);
void tessCTL(POLYLINES& p,  FILE* f);
void tessCTL(POLYLINE& p,   std::string output);
void tessCTL(POLYLINES& p,  std::string output);

#ifdef MATLAB_MEX_FILE
void tessCTL(POLYLINE& p);
void tessCTL(POLYLINES& p);
#endif

void tessGL(POLYLINE& p);
//void tessGL(POLYLINES& p);

//void tess(POLYLINES plines, FILE* f);
void tess(POLYLINES plines, FILE* f);
void tess(POLYLINE p,       void* ptr, bool isCTL=true
#ifdef TESSDEBUG
          , DEVICE* tessTest=nullptr
#endif
          );

void outputTriangle(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f);
void outputQuadrilateral(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f);
//void fprintCTL(FILE* f, VECTOR v, std::string name="vector3");

#endif
