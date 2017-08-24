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

#include <OpenGL/OpenGL.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AFFINE;       // Forward declaraction of `AFFINE` for use in `VECTOR`.

class VECTOR {      // 2D Coordinate Class.
public:
    GLdouble x;
    GLdouble y;

#ifdef USE_FT_FONT
    VECTOR(FT_Vector v);    // Convert Freetype vector (for truetype font) to `VECTOR`
#endif

    VECTOR(GLdouble x_=0, GLdouble y_=0, bool isCylindrical=false);     // Initializes `[x_, y_]`; initializes `[y_*cos(x_),y_*sin(x_)]`, if 3rd coeficient equals `true`.

    bool isZero()                       const;  // Checks if both coordinates are within `E` of zero.
    bool operator!()                    const;  // Calls `isZero()`.

    bool operator==(VECTOR v)           const;  // Checks if each set of coordinates are within `E` of each other.
    bool operator!=(VECTOR v)           const;  // Inverts `operator==(VECTOR)`.
    bool operator< (VECTOR v)           const;  // First compares `x`, compares `y` if `x` are equal.
    bool operator<=(VECTOR v)           const;  // See `operator<(VECTOR)`.
    bool operator> (VECTOR v)           const;  // See `operator<(VECTOR)`.
    bool operator>=(VECTOR v)           const;  // See `operator<(VECTOR)`.

    bool inLine(VECTOR v)               const;  // Returns `true` if either the `x` or `y` coordinates are equal, or both.

    VECTOR operator=(VECTOR v);                 // Sets to other vector.
    VECTOR operator-()                  const;  // Returns copy with negated coordinates.

    VECTOR operator+(VECTOR v)          const;  // Returns sum of two vectors.
    VECTOR operator-(VECTOR v)          const;  // Returns difference of two vectors.
    VECTOR operator*(GLdouble factor)   const;  // Returns copy scaled by `factor`.
    VECTOR operator/(GLdouble factor)   const;  // Returns copy scaled by `1/factor`.

    VECTOR operator+=(VECTOR v);                // Sets to sum of two vectors.
    VECTOR operator-=(VECTOR v);                // Sets to difference of two vectors.
    VECTOR operator*=(GLdouble factor);         // Scales by `factor`.
    VECTOR operator/=(GLdouble factor);         // Scales by `1/factor`.

    VECTOR operator*(AFFINE m)          const;  // Blasphemously equivalent to `m*v`.
    VECTOR operator*=(AFFINE m);                // Sets to `m*v`.
    
    GLdouble magn()                     const;  // Returns the vector norm: `sqrt(x^2 + y^2)`.
    GLdouble magn2()                    const;  // Returns the square of the vector norm, `x^2 + y^2`. Use this for comparisons.
    
    GLdouble norm()                     const;  // Copy of `magn()`.
    GLdouble norm2()                    const;  // Copy of `magn2()`;
    
    void zero();                                // Sets the vector to zero.

    void unitEquals();                          // Scales by `1/magn()`; does not change zero vector.
    VECTOR unit()                       const;  // Returns copy scaled by `1/magn()`; returns zero vector if `magn() == 0`.
    bool isUnit()                       const;  // Checks if `magn() == 1`.

    VECTOR perpCCW()                    const;  // Returns the perpendicular vector in the CounterClockWise direction.
    VECTOR perpCW()                     const;  // Returns the perpendicular vector in the ClockWise direction.

    GLdouble operator*(VECTOR v)        const;  // Returns the dot product of two vectors.
    GLdouble dot(VECTOR v)              const;  // Returns the dot product of two vectors.
    GLdouble perpCCWdot(VECTOR v)       const;  // Returns the dot product of this vector and the `perpCCW()` of the second vector.
    GLdouble cross(VECTOR v)            const;  // Returns the magnitude of the cross product of two vectors.

    VECTOR rotate(GLdouble radians)     const;  // Returns a copy of the vector rotated by specified radians.
    void rotateEquals(GLdouble radians);        // Rotates the vector by specified radians.

    void print()                        const;  // Prints the vector in `[ x, y ]` form.
    void printNL()                      const;  // Prints the vector in `[ x, y ]` form, with a newline.

    void render()                           const;  // Draws a line from the origin to the vector using OpenGL2 syntax.
    void render(GLdouble x_, GLdouble y_)   const;  // Draws the vector, based from `[ x_, y_ ]` instead of the origin.
    void render(VECTOR v)                   const;  // Draws the vector, based from the second vector instead of the origin.

    void glVertex()                         const;  // Using OpenGL2 syntax, pushes the vertex.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LABEL : public VECTOR {                               // Non-physical labels. Implementation of GDSII's TEXT structure.
    std::string label;

    LABEL(VECTOR v, std::string label_);                    // Initializes label at vector with text.
    LABEL(GLdouble x_, GLdouble y_, std::string label_);    // Initializes label at `[ x_, y_ ]` with text.

    void render(GLdouble x_, GLdouble y_)  const;           // Make sure that `render()` and `render(VECTOR v)` call this.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CONNECTION {
public:
    VECTOR v;           // Position of the connection,
    VECTOR dv;          // Direction of the connection,
    GLdouble w;         // Width of connection. If negative, +/- `PADDING` is drawn around connection.

    int16_t l;          // Layer.

    std::string name;   // Name of connection.

    CONNECTION();
    CONNECTION(VECTOR v_, VECTOR dv_, GLdouble w_=1, std::string name_="Connection");

    CONNECTION operator=(CONNECTION c);         // Changes the values of this connection to the second connection.
    CONNECTION operator-()              const;  // Returns connection with `dv = -dv`. No other changes.

    CONNECTION operator+ (VECTOR v_)    const;  // Returns connection with `v = v + v_`.
    CONNECTION operator+=(VECTOR v_);           // Sets `v = v + v_`.
    
    CONNECTION operator- (VECTOR v_)    const;  // Returns connection with `v = v - v_`.
    CONNECTION operator-=(VECTOR v_);           // Sets `v = v - v_`.
    
    CONNECTION operator* (GLdouble s)   const;  // Returns scaled connection such that `v *= s`, `dv *= sign(s)` and `w *= |s|`. Does not check for `s = 0`.
    CONNECTION operator*=(GLdouble s);          // Scales connection such that `v *= s`, `dv *= sign(s)` and `w *= |s|`. Does not check for `s = 0`.
    
    CONNECTION operator/ (GLdouble s)   const;  // Returns scaled connection such that `v /= s`, `dv /= sign(s)` and `w /= |s|`. Does not check for `s = 0`.
    CONNECTION operator/=(GLdouble s);          // Scales connection such that `v /= s`, `dv /= sign(s)` and `w /= |s|`. Does not check for `s = 0`.
    
    CONNECTION operator* (AFFINE m)     const;  // Blasphemously equivalent to `m*c`, where the transformation is applied perpendicularly to retain the appropraite connection angle.
    CONNECTION operator*=(AFFINE m);            // Sets to `m*c`

    bool operator==(CONNECTION c)       const;  // Checks equality in `v`, `dv`, and `w`. Does not check `l` or `name`.

    CONNECTION setName(std::string name_);      // Changes the name `name` of the connection.
    CONNECTION setLayer(int16_t l_);            // Changes the layer `l` of the connection.
    
    void print()                        const;  // Prints the name `name`, location `v`, direction `dv`, and width `w` of the connection.
    
    CONNECTION copy()                   const;  // Returns a copy of the connection.

    void render()                       const;  // Draws an arrow with appropriate location, width and direction using OpenGL2.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BOUNDINGBOX;          // Forward declaraction of `BOUNDINGBOX` for use in `AFFINE`.

class AFFINE {              // Affine transformation:
public:
    GLdouble a, b, c, d;    // Linear `2x2` matrix part (applied first)
    GLdouble e, f;          // 2D vector translation (applied second)

    AFFINE();                                           // Identity matrix.

    AFFINE(GLdouble radians);                           // Matrix that rotates by `radians` with no translation.
    AFFINE(GLdouble radians, VECTOR v);                 // Matrix that rotates by `radians` with translation `v`.
    AFFINE(GLdouble radians, GLdouble e_, GLdouble f_); // Matrix that rotates by `radians` with translation `[ e_, f_ ]`.

    AFFINE(VECTOR v);                                                                           // Returns a translation by `v`.
    AFFINE(GLdouble e_, GLdouble f_);                                                           // Returns a translation by `[ e_, f_ ]`.
    AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, VECTOR v);                       // Returns the matrix `[ a, b; c, d ]` with an additional translation by `v`.
    AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, GLdouble e_=0, GLdouble f_=0);   // Returns the matrix `[ a, b; c, d ]` with an additional translation by `[ e_, f_ ]`.

    AFFINE(CONNECTION from, CONNECTION to);         // Returns the (unique) affine transformtion that shifts from one `CONNECTION` frame to another.
    AFFINE(BOUNDINGBOX from, BOUNDINGBOX to);       // Returns the (unique) affine transformtion that shifts from one `BOUNDINGBOX` frame to another.

    bool isIdentity()                       const;  // Returns whether the matrix component is the identity and the vector offset is zero.
    bool isZero()                           const;  // Returns whether all coeficients are zero.

    VECTOR operator*(VECTOR v)              const;  // Standard vector product `matrix*v + offset`
    VECTOR linearTimes(VECTOR v)            const;  // Only applies the linear matrix part, does not apply the translation.
    
    CONNECTION operator*(CONNECTION c)      const;  // Same as `c*m`; see above.
    CONNECTION linearTimes(CONNECTION c)    const;  // Only applies the linear matrix part, does not apply the translation.
    
    BOUNDINGBOX operator*(BOUNDINGBOX bb)   const;  // Returns the `BOUNDINGBOX` that contains all four corners of the transformed `BOUNDINGBOX`.
    BOUNDINGBOX linearTimes(BOUNDINGBOX bb) const;  // Returns the `BOUNDINGBOX` that contains all four corners of the transformed `BOUNDINGBOX`, wihtout applying the translation.

    AFFINE operator*(AFFINE m)              const;  // Returns a transformation that applies the multiplied affine first and the original affine second.
    AFFINE operator*=(AFFINE m);                    // Sets the transformation to be the product of the two transformations.

    AFFINE operator+(VECTOR v)              const;  // Returns a transformation with additional translation of `v`.
    AFFINE operator-(VECTOR v)              const;  // Returns a transformation with additional translation of `-v`.
    AFFINE operator*(GLdouble s)            const;  // Returns the product of this transformation with the linear transformation that scales by `s`.
    AFFINE operator/(GLdouble s)            const;  // Returns the product of this transformation with the linear transformation that scales by `1/s`.

    AFFINE operator+=(VECTOR v);                    // Increases translation by `v`.
    AFFINE operator-=(VECTOR v);                    // Increases translation by `-v`.
    AFFINE operator*=(GLdouble s);                  // Sets the transformation to `m*s`.
    AFFINE operator/=(GLdouble s);                  // Sets the transformation to `m/s`.
    
    AFFINE copy()                           const;  // Returns a copy of this transformation.
    VECTOR translation()                    const;  // Returns the translation vector part of the affine transformation.
    AFFINE linear()                         const;  // Returns the linear matrix part of the affine transformation.

    void print()                            const;  // Prints the matrix in `[ a, b; c, d ] + [ e; f ]` form.

    GLdouble det()                          const;  // Returns the determinant of the linear matrix part of the affine transformation.
    AFFINE inv()                            const;  // Returns the affine transformation `m2` such that `m*m2 = I`.
    
    bool islinear()                         const;  // Returns whether there is no translation component (i.e. completely linear).
    void glTransform()                      const;  // Pushes this transformation as an OpenGL matrix. Useful for transforming already-loaded geometry.
};

AFFINE mirrorX();       // Transformation that mirrors across the `x`-axis.
AFFINE mirrorY();       // Transformation that mirrors across the `y`-axis.
AFFINE mirrorYeX();     // Transformation that mirrors across the line `y = x`.
AFFINE mirrorYemX();    // Transformation that mirrors across the line `y = -x`.
AFFINE zeroAFFINE();    // Zero matrix with no translation.

#include "polyline.hpp"

#endif
