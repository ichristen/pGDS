#include "math.hpp"

bool getRoot(GLdouble* root, GLdouble a, GLdouble b, GLdouble c, GLdouble s) {
    if (a == 0 && b!= 0) {
        // bx + c = 0
        *root = -c/b;
        return true;    // Or return half-true?
    } else if (a != 0 && b*b - 4*a*c > 0) {
        // ax^2 + bx + c = 0
        *root = ( -b + sign(s)*sqrt(b*b - 4*a*c) ) / (2*a);
        return true;
    } else {
        return false;
    }
}




