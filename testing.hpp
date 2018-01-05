#ifndef TESTING_HPP
#define TESTING_HPP

#include <stdio.h>
#include "vector.hpp"
#include "device.hpp"
#include "font.hpp"
#include "math.hpp"
#include "polyline.hpp"
#include "primitives.hpp"
#include "material.hpp"
#include "testdevices.hpp"
#include <OpenGL/gl3.h>         //OpenGL
                                //#include <OpenGL/glu.h>         //GLU

void testAll();

void testPrimitives();
void testBooleans();

#endif  // TESTING_H
