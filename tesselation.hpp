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

void tessCTL(POLYLINE& p, FILE* f);
void tessCTL(POLYLINE& p, std::string output);
void tessGL(POLYLINE& p);
void tess(POLYLINE p, FILE* f);
void outputTriangle(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f);
//void fprintCTL(FILE* f, VECTOR v, std::string name="vector3");

#endif
