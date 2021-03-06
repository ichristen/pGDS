#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <functional>

//#include <OpenGL/OpenGL.h>

//#include <GL/glew.h>
#ifdef pGDSGLFW
    #include <GLFW/glfw3.h>
#endif

#include "vector.hpp"
#include "math.hpp"

//#define POLYLINE_DEBUG 1

class POLYLINE;

class BOUNDINGBOX {
private:
    bool initialized;
public:
    VECTOR ur;
    VECTOR ll;
    
    BOUNDINGBOX();
    BOUNDINGBOX(VECTOR a);
    BOUNDINGBOX(VECTOR a, VECTOR b);                // Makes the smallest bounding box that contains both a and b (a != ur, b != ll, generally).
    BOUNDINGBOX(VECTOR c, GLdouble w);              // Makes a box with width `w` and height `w` centered at `c`.
    BOUNDINGBOX(VECTOR c, GLdouble w, GLdouble h);  // Makes a box with width `w` and height `h` centered at `c`.
    BOUNDINGBOX(GLdouble left, GLdouble right, GLdouble top, GLdouble bottom);
    
    bool isInitialized() const;
    
    GLdouble width() const;
    GLdouble height() const;
    
    VECTOR center() const;
    
    VECTOR north() const;
    VECTOR south() const;
    VECTOR east() const;
    VECTOR west() const;
    
    VECTOR n() const;
    VECTOR s() const;
    VECTOR e() const;
    VECTOR w() const;
    
    VECTOR nw() const;
    VECTOR ne() const;
    VECTOR sw() const;
    VECTOR se() const;
    
//    VECTOR north() const;
//    VECTOR south() const;
//    VECTOR east() const;
//    VECTOR west() const;
    
    BOUNDINGBOX copy() const;
    void clear();
    
    bool isEmpty() const;
    
    bool doesContain(VECTOR v) const;         // Checks whether the bounding box contains v.
    bool doesContainX(GLdouble x) const;
    bool doesContainY(GLdouble y) const;
    bool doesContain(BOUNDINGBOX bb) const;
    
    bool enlarge(VECTOR v);             // Insist that the boundary box contain v. Returns whether enlargement was neccessary.
    bool enlarge(BOUNDINGBOX bb);       // Insist that the boundary box contain b. Returns whether enlargement was neccessary.
    
    bool doesIntersect(BOUNDINGBOX bb);     // Checks whether this bounding box intersects with another.
    
    BOUNDINGBOX  operator&(BOUNDINGBOX bb); // Boolean and.
    BOUNDINGBOX& operator&=(BOUNDINGBOX bb);
    
    BOUNDINGBOX  operator|(BOUNDINGBOX bb); // Boolean or.
    BOUNDINGBOX& operator|=(BOUNDINGBOX bb);
    
    BOUNDINGBOX  operator|(VECTOR v);       // Same as enlarge.
    BOUNDINGBOX& operator|=(VECTOR v);
    
    BOUNDINGBOX& operator=(BOUNDINGBOX bb);
    
    BOUNDINGBOX  operator* (GLdouble s)  const;
    BOUNDINGBOX  operator/ (GLdouble s)  const;
    
    BOUNDINGBOX& operator*=(GLdouble s);
    BOUNDINGBOX& operator/=(GLdouble s);
    
    GLdouble area()                 const;
    
    bool operator==(BOUNDINGBOX bb) const;
    
    void print()                    const;
    
    POLYLINE draw() const;
};

//bool intersect(VECTOR a1, VECTOR a2, VECTOR b1, VECTOR b2, VECTOR** i);
//
//enum BOOLOPERATION { AND=0, OR=1, XOR=2 };

//class POLYLINE;
class POLYLINES;

//std::vector<POLYLINE> booleanOp(POLYLINE a, POLYLINE b, BOOLOPERATION op);

class POLYLINE {
private:
    
    VECTOR begin;                       // For unclosed polylines, this defines the *direction* of the beginning and end.
    VECTOR end;
    
    double area_            = 0;
    double length_          = 0;
    
    GLuint fillBuffer       = 0;
    GLuint outlineBuffer    = 0;
    
public:
    bool isReversed         = false;
    
    int returnValidIndex(int i) const;
    
    std::vector<VECTOR> points;         // Make private eventually?
    
    BOUNDINGBOX bb;
    
    uint16_t layer          = 0;
    
public:
    bool isClosed           = false;
                       // When a polyline is reversed, begin still corresponds to the internal first index.
    
    POLYLINE(size_t size_=200);
//    POLYLINE(size_t size_, GLdouble* x, GLdouble* y);
    POLYLINE(size_t size_, GLdouble* x, GLdouble* y, bool isCylindrical=false);
    POLYLINE(std::vector<GLdouble> x, std::vector<GLdouble> y, bool isCylindrical=false);
    
    POLYLINE(POLYLINE p, int b, int e);
    
    VECTOR  operator[](int i)       const;
    VECTOR  at(int i)       const;
    bool    insert(int i, VECTOR v);
    bool    erase(int i);
    void    clear();
//    void        erase();
//    void        erase(int i);
//    void        erase(int i, int j);
    
    void add(VECTOR v);
    void addToPrev(VECTOR v);
    void add(POLYLINE p);
    
    void recomputeBoundingBox();
    
    POLYLINE clip(int b, int e);
    
//    void operator=(POLYLINE p);
    
    POLYLINE  operator-();
    POLYLINE& reverse();                    // Changes the flag `isReversed` such that the enclosed data is treated as if it was reversed. This is useful for quickly changing the orientation of a closed contour.
    void makeforward();                     // If `isReversed` is flagged, changes the order of data in memory such the the current aparent order is maintained, while `isReversed` is no longer needed.
    
    VECTOR getBeginPoint();
    VECTOR getEndPoint();
    VECTOR getBeginDirection();
    VECTOR getEndDirection();
    
    bool setBeginDirection(VECTOR dir);
    bool setEndDirection(VECTOR dir);
    
    bool setBeginDirection();
    bool setEndDirection();
    
    POLYLINE  operator+ (VECTOR v)      const;
    POLYLINE  operator- (VECTOR v)      const;
    POLYLINE& operator+=(VECTOR v);
    POLYLINE& operator-=(VECTOR v);
    
    POLYLINE  operator/ (double s)   const;
    POLYLINE  operator* (double s)   const;
    POLYLINE  operator* (AFFINE m)   const;
    POLYLINE& operator/=(double s);
    POLYLINE& operator*=(double s);
    POLYLINE& operator*=(AFFINE m);
    
    POLYLINE  operator+ (POLYLINE p) const;
    POLYLINE  operator- (POLYLINE p) const;
    POLYLINE& operator+=(POLYLINE p);
    POLYLINE& operator-=(POLYLINE p);
    
    POLYLINES boolean(POLYLINE p, BOOLOPERATION op) const;
    
    POLYLINES operator&(POLYLINE p) const;  // Boolean AND
    POLYLINES operator|(POLYLINE p) const;  // Boolean OR
    POLYLINES operator^(POLYLINE p) const;  // Boolean XOR
    
    GLdouble area();
    
    size_t size() const;
    
    POLYLINE& close();
    POLYLINE& open();
    
    bool isCCW();
    bool isEmpty() const;
    
    bool contains(VECTOR v) const;  // Note that this imperfectly considers the boundary...
    bool boundaryContains(VECTOR v) const;
    std::vector<GLdouble>   getIntersections(GLdouble x)        const;  // Returns y values of intersections.
    std::vector<int>        getIntersectionsIndex(GLdouble x)   const;  // Returns index values of intersections. For intersections that are not the ends of a line segment, the lowest index of the segment is returned).
    
    GLdouble length();
    
    POLYLINE setLayer(uint16_t l);
    
    POLYLINE copy()         const;
    
    void generateFillBuffer();
    void generateOutlineBuffer();
    
    void fill();
    void outline();
    void fillOutline();
    
//    POLYLINE resolve();
    
    void print()            const;
    long hash()             const;
};

class POLYLINES {
public:
    std::vector<POLYLINE> polylines;
    
    BOUNDINGBOX bb;
    
    POLYLINES();
    POLYLINES(POLYLINE polyline);
    POLYLINES(std::vector<POLYLINE> polylines_);
    
    POLYLINE    operator[](int i)       const;
    POLYLINE&   operator[](int i);
    POLYLINE    at(int i)       const;
//    POLYLINE&   at(int i);
    bool        insert(int i, POLYLINE p);
    void        clear();
    
    void add(POLYLINE p);
    void add(POLYLINES p);
    
    void recomputeBoundingBox();
    
    size_t size()                       const;
    bool isEmpty()                      const;
    
    POLYLINES getLayer(uint16_t l) const;
    bool removeLayer(uint16_t l);
    bool setLayer(uint16_t to);
    bool setLayer(uint16_t from, uint16_t to);
    bool exchangeLayers(uint16_t l1, uint16_t l2);
    
    POLYLINES   operator/ (double s)    const;
    POLYLINES   operator* (double s)    const;
    POLYLINES   operator* (AFFINE m)    const;
    POLYLINES&  operator/=(double s);
    POLYLINES&  operator*=(double s);
    POLYLINES&  operator*=(AFFINE m);
    
    
    POLYLINES   operator-();
    POLYLINES&  reverse();
    
//    POLYLINES&  makePositiveArea();
    
    POLYLINES   boolean(POLYLINE p, BOOLOPERATION op) const;
    
    POLYLINES   operator&(POLYLINE p)   const;  // Boolean AND
    POLYLINES   operator|(POLYLINE p)   const;  // Boolean OR
    POLYLINES   operator^(POLYLINE p)   const;  // Boolean XOR
    
    POLYLINES&  booleanEquals(POLYLINE p, BOOLOPERATION op);
    
    POLYLINES&  operator&=(POLYLINE p);         // Boolean AND equals
    POLYLINES&  operator|=(POLYLINE p);         // Boolean OR  equals
    POLYLINES&  operator^=(POLYLINE p);         // Boolean XOR equals
    
    
    POLYLINES   boolean(POLYLINES p, BOOLOPERATION op) const;
    
    POLYLINES   operator&(POLYLINES p)  const;  // Boolean AND
    POLYLINES   operator|(POLYLINES p)  const;  // Boolean OR
    POLYLINES   operator^(POLYLINES p)  const;  // Boolean XOR
    
    POLYLINES&  booleanEquals(POLYLINES p, BOOLOPERATION op);
    
    POLYLINES&  operator&=(POLYLINES p);        // Boolean AND equals
    POLYLINES&  operator|=(POLYLINES p);        // Boolean OR equals
    POLYLINES&  operator^=(POLYLINES p);        // Boolean XOR equals
    
    double area();
    
    bool contains(VECTOR v) const;
    
    POLYLINES copy()        const;
    
    void print()            const;
    
    void render(AFFINE m, bool fill=true, bool outline=true);
};

#include "boolean.hpp"
#include "tesselation.hpp"

#endif // POLYLINE_H




