#ifndef VECTOR_H
#define VECTOR_H

#ifdef MATLAB_MEX_FILE
    #include <mex.h>
#endif

#include "math.hpp"

#include <stdio.h>
#include <cmath>

#ifdef USE_FT_FONT
    #include <ft2build.h>
    #include FT_FREETYPE_H
#endif

//#include "/usr/local/include/freetype2/ft2build.h"
//#include "/usr/local/include/freetype2/freetype/freetype.h"

#include <OpenGL/gl3.h>         //OpenGL
//#include <OpenGL/glu.h>         //GLU
//#include <OpenGL/glui.h>

class AFFINE;

class VECTOR{
public:
    GLdouble x;
    GLdouble y;

#ifdef USE_FT_FONT
    VECTOR(FT_Vector v);
#endif

    VECTOR(GLdouble x_=0, GLdouble y_=0, bool isCylindrical=false);

    bool isZero()                       const;
    bool operator!()                    const;

    bool operator==(VECTOR v)           const;
    bool operator!=(VECTOR v)           const;
    bool operator< (VECTOR v)           const;
    bool operator<=(VECTOR v)           const;
    bool operator> (VECTOR v)           const;
    bool operator>=(VECTOR v)           const;

    bool inLine(VECTOR v)               const;

    VECTOR operator=(VECTOR v);
    VECTOR operator-()                  const;

    VECTOR operator+(VECTOR v)          const;
    VECTOR operator-(VECTOR v)          const;
    VECTOR operator*(GLdouble factor)   const;
    VECTOR operator/(GLdouble factor)   const;

    VECTOR operator+=(VECTOR v);
    VECTOR operator-=(VECTOR v);
    VECTOR operator*=(GLdouble factor);
    VECTOR operator/=(GLdouble factor);

    VECTOR operator*(AFFINE m)          const;     // Equivalent to m*v
    VECTOR operator*=(AFFINE m);

    GLdouble magn()                     const;
    GLdouble magn2()                    const;

    void unitEquals();
    VECTOR unit()                       const;
    bool isUnit()                       const;
    
    void zero();

    VECTOR perpCCW()                    const;
    VECTOR perpCW()                     const;

    GLdouble operator*(VECTOR v)        const;
    GLdouble dot(VECTOR v)              const;
    GLdouble perpCCWdot(VECTOR v)       const;
    GLdouble cross(VECTOR v)            const;

    VECTOR rotate(GLdouble radians)     const;
    void rotateEquals(GLdouble radians);

    void print()                        const;
    void printNL()                      const;

    void render()                           const;
    void render(GLdouble x_, GLdouble y_)   const;
    void render(VECTOR v)                   const;

    void glVertex()                         const;
};

class LABEL : public VECTOR {
    std::string label;

    LABEL(VECTOR v, std::string label_);
    LABEL(GLdouble x_, GLdouble y_, std::string label_);

    void render(GLdouble x_, GLdouble y_)  const;  // Make sure that render() and render(VECTOR v) call this.
};

class CONNECTION {
public:
    VECTOR v;       // Position of the connection,
    VECTOR dv;      // Direction of the connection,
    GLdouble w;     // Width of connection.

    short l;        // Layer.

    std::string name;

    CONNECTION();
    CONNECTION(VECTOR v_, VECTOR dv_, GLdouble w_=1, std::string name_="Connection");

    CONNECTION operator=(CONNECTION c);
    CONNECTION operator-()              const;

    CONNECTION operator+ (VECTOR v_)    const;
    CONNECTION operator+=(VECTOR v_);
    
    CONNECTION operator- (VECTOR v_)    const;
    CONNECTION operator-=(VECTOR v_);
    
    CONNECTION operator* (GLdouble s)   const;
    CONNECTION operator*=(GLdouble s);
    
    CONNECTION operator/ (GLdouble s)   const;
    CONNECTION operator/=(GLdouble s);
    
    CONNECTION operator* (AFFINE m)     const;     // Equivalent to m*c
    CONNECTION operator*=(AFFINE m);

    bool operator==(CONNECTION c)       const;

    CONNECTION setName(std::string name_);
    
    void print()                        const;
    
    CONNECTION copy()                   const;

    void render()                       const;
};

class BOUNDINGBOX;

class AFFINE {              // Affine transformation:
public:
    GLdouble a, b, c, d;    // Linear 2x2 AFFINE part,
    GLdouble e, f;          // Vec2 offset.

    AFFINE();

    AFFINE(double radians);
    AFFINE(double radians, VECTOR v);
    AFFINE(double radians, GLdouble e_, GLdouble f_);

    AFFINE(VECTOR v);
    AFFINE(GLdouble e_, GLdouble f_);
    AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, VECTOR v);
    AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, GLdouble e_=0, GLdouble f_=0);

    AFFINE(CONNECTION from, CONNECTION to);
    
    AFFINE(BOUNDINGBOX from, BOUNDINGBOX to);

    bool isIdentity()                       const;
    bool isZero()                           const;

    VECTOR operator*(VECTOR v)              const;
    VECTOR linearTimes(VECTOR v)            const;
    
    CONNECTION operator*(CONNECTION c)      const;
    CONNECTION linearTimes(CONNECTION c)    const;
    
    BOUNDINGBOX operator*(BOUNDINGBOX bb)   const;
//    BOUNDINGBOX linearTimes(BOUNDINGBOX bb) const;

    AFFINE operator*(AFFINE m)              const;
    AFFINE operator*=(AFFINE m);

    AFFINE operator+(VECTOR v)              const;
    AFFINE operator-(VECTOR v)              const;
    AFFINE operator*(GLdouble s)            const;
    AFFINE operator/(GLdouble s)            const;

    AFFINE operator+=(VECTOR v);
    AFFINE operator-=(VECTOR v);
    AFFINE operator*=(GLdouble s);
    AFFINE operator/=(GLdouble s);

    VECTOR translation()                    const;

    void print()                            const;

    GLdouble det()                          const;
    AFFINE inv()                            const;
    bool islinear()                         const;
    void glTransform()                      const;
};

AFFINE mirrorX();
AFFINE mirrorY();
AFFINE mirrorYeX();
AFFINE mirrorYemX();
AFFINE zeroAFFINE();

#include "polyline.hpp"

#endif
