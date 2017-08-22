#ifndef POLYLINE_H
#define POLYLINE_H

//#include "include.h"
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include "vector.hpp"
#include "math.hpp"
//#include "boolean.hpp"
#include <functional>

//#include <SDL2/SDL.h>        //SDL

#include <OpenGL/gl3.h>         //OpenGL

//    #include <OpenGL/gl.h>
//#include <OpenGL/glu.h>         //GLU

//class CURRENTLAYER {
//public:
//    static uint16_t layer;      // Initializes to zero.
//    
//    void setCurrent(uint16_t newlayer);
//};
//
//namespace global {
//    CURRENTLAYER layer;
//}

class BOUNDINGBOX {
public:
    VECTOR ur;
    VECTOR ll;
    bool initialized;
    
    BOUNDINGBOX();
    BOUNDINGBOX(VECTOR a);
    BOUNDINGBOX(VECTOR a, VECTOR b);    // Makes the smallest bounding box that contains both a and b (a != ur, b != ll, generally).
    
    GLdouble width();
    GLdouble height();
    
    BOUNDINGBOX copy() const;
    void clear();
    
    bool isEmpty();
    
    bool doesContain(VECTOR v);         // Checks whether the bounding box contains v.
    bool doesContainX(GLdouble x);
    bool doesContainY(GLdouble y);
    bool doesContain(BOUNDINGBOX bb);
    
    bool enlarge(VECTOR v);             // Insist that the boundary box contain v. Returns whether enlargement was neccessary.
    bool enlarge(BOUNDINGBOX bb);       // Insist that the boundary box contain b. Returns whether enlargement was neccessary.
    
    bool doesIntersect(BOUNDINGBOX bb);     // Checks whether this bounding box intersects with another.
    
    BOUNDINGBOX operator&(BOUNDINGBOX bb);  // Boolean and.
    BOUNDINGBOX operator&=(BOUNDINGBOX bb);
    
    BOUNDINGBOX operator|(BOUNDINGBOX bb);  // Boolean or.
    BOUNDINGBOX operator|=(BOUNDINGBOX bb);
    
    BOUNDINGBOX operator|(VECTOR v);        // Same as enlarge.
    BOUNDINGBOX operator|=(VECTOR v);
    
    BOUNDINGBOX operator=(BOUNDINGBOX bb);
    
    BOUNDINGBOX operator/ (GLdouble s)   const;
    BOUNDINGBOX operator* (GLdouble s)   const;
    
    BOUNDINGBOX operator/=(GLdouble s);
    BOUNDINGBOX operator*=(GLdouble s);
    
    GLdouble area()                 const;
    
    bool operator==(BOUNDINGBOX bb) const;
    
    void print()                    const;
};

bool intersect(VECTOR a1, VECTOR a2, VECTOR b1, VECTOR b2, VECTOR** i);

enum BOOLOPERATION { AND=0, OR=1, XOR=2 };

class POLYLINES;

// Make methods const!
class POLYLINE {
public:
//private:
    std::vector<VECTOR> points;
    
    BOUNDINGBOX bb;
    
    bool isReversed         = false;
    
//    bool isInside           = false;    // Only used when booleaning...
    
    double area_            = 0;
    double length_          = 0;
    
    uint16_t layer          = 0;
    
    static uint16_t currentlayer;
    
    GLuint fillList         = 0;
    GLuint outlineList      = 0;
    
//    std::iterator<GLdouble, int> begin;
//    std::iterator<GLdouble, int> end;
    
//    std::vector<std::iterator<GLdouble, int>> intersect(POLYLINE);
    
public:
    bool isClosed           = false;
    
    VECTOR begin;                       // For unclosed polylines, this defines the *direction* of the beginning and end.
    VECTOR end;                         // When a polyline is reversed, begin still corresponds to the internal first index.
    
    POLYLINE(size_t size_=200);
    POLYLINE(POLYLINE p, int b, int e);
    
    VECTOR  operator[](int i)       const;
    bool    insert(int i, VECTOR v);
    
    void add(VECTOR v);
    void add(POLYLINE p);
    
    void recomputeBoundingBox();
    
    POLYLINE clip(int b, int e);
    
//    void operator=(POLYLINE p);
    
    POLYLINE operator-();
    POLYLINE reverse();             // Changes some flags such that the enclosed datapoints are treated as if they are reversed.
    void makeforward();             // Actually reverses the data points if reversed is flaged. Turns off reversed flags.
    
    VECTOR getBeginPoint();
    VECTOR getEndPoint();
    VECTOR getBeginDirection();
    VECTOR getEndDirection();
    
    POLYLINE operator+ (VECTOR v)   const;
    POLYLINE operator- (VECTOR v)   const;
    POLYLINE operator+=(VECTOR v);
    POLYLINE operator-=(VECTOR v);
    
    POLYLINE operator/ (double s)   const;
    POLYLINE operator* (double s)   const;
    POLYLINE operator* (AFFINE m)   const;
    POLYLINE operator/=(double s);
    POLYLINE operator*=(double s);
    POLYLINE operator*=(AFFINE m);
    
    POLYLINE operator+ (POLYLINE p) const;
    POLYLINE operator- (POLYLINE p) const;
    POLYLINE operator+=(POLYLINE p);
    POLYLINE operator-=(POLYLINE p);
    
    POLYLINES boolean(POLYLINE p, BOOLOPERATION op) const;
    
    POLYLINES operator&(POLYLINE p) const;  // Boolean AND
    POLYLINES operator|(POLYLINE p) const;  // Boolean OR
    POLYLINES operator^(POLYLINE p) const;  // Boolean XOR
    
    GLdouble area();
    
    size_t size() const;
    
    POLYLINE close();
    POLYLINE open();
    
    bool isCCW();
    bool isEmpty() const;
    
    bool contains(VECTOR v) const;  // Note that this imperfectly considers the boundary...
    std::vector<GLdouble>   getIntersections(GLdouble x)        const;  // Returns y values of intersections.
    std::vector<int>        getIntersectionsIndex(GLdouble x)   const;  // Returns index values of intersections. For intersections that are not the ends of a line segment, the lowest index of the segment is returned).
    
    GLdouble length();
    
    POLYLINE setLayer(uint16_t l);
    
    POLYLINE copy()         const;
    
//    void fill();
//    void outline();
//    POLYLINE resolve();
    
    void print()            const;
};

class POLYLINES {
public:
    std::vector<POLYLINE> polylines;
    
    BOUNDINGBOX bb;
    
    POLYLINES();
    POLYLINES(POLYLINE polyline);
    POLYLINES(std::vector<POLYLINE> polylines_);
    
    POLYLINE    operator[](int i)       const;
    bool        insert(int i, POLYLINE p);
    void        clear();
    
    void add(POLYLINE p);
    void add(POLYLINES p);
    
    void recomputeBoundingBox();
    
    size_t size()                       const;
    bool isEmpty()                      const;
    
    POLYLINES   operator/ (double s)    const;
    POLYLINES   operator* (double s)    const;
    POLYLINES   operator* (AFFINE m)    const;
    POLYLINES   operator/=(double s);
    POLYLINES   operator*=(double s);
    POLYLINES   operator*=(AFFINE m);
    
    
    POLYLINES   operator-();
    POLYLINES   reverse();
    
    
    POLYLINES   boolean(POLYLINE p, BOOLOPERATION op) const;
    
    POLYLINES   operator&(POLYLINE p)   const;  // Boolean AND
    POLYLINES   operator|(POLYLINE p)   const;  // Boolean OR
    POLYLINES   operator^(POLYLINE p)   const;  // Boolean XOR
    
    POLYLINES   booleanEquals(POLYLINE p, BOOLOPERATION op);
    
    POLYLINES   operator&=(POLYLINE p);         // Boolean AND equals
    POLYLINES   operator|=(POLYLINE p);         // Boolean OR equals
    POLYLINES   operator^=(POLYLINE p);         // Boolean XOR equals
    
    
    POLYLINES   boolean(POLYLINES p, BOOLOPERATION op) const;
    
    POLYLINES   operator&(POLYLINES p)  const;  // Boolean AND
    POLYLINES   operator|(POLYLINES p)  const;  // Boolean OR
    POLYLINES   operator^(POLYLINES p)  const;  // Boolean XOR
    
    POLYLINES   booleanEquals(POLYLINES p, BOOLOPERATION op);
    
    POLYLINES   operator&=(POLYLINES p);        // Boolean AND equals
    POLYLINES   operator|=(POLYLINES p);        // Boolean OR equals
    POLYLINES   operator^=(POLYLINES p);        // Boolean XOR equals
    
    
    double area();
    
    bool contains(VECTOR v) const;
    
    POLYLINES copy()        const;
    
    void print()            const;
};

struct SEGMENT {    // Helper struct for boolean...
    int b;
    int e;
    
    bool forward;
    
    BOUNDINGBOX bb;
    
    bool operator<(const SEGMENT& s) const;
    bool operator>(const SEGMENT& s) const;
};

std::vector<POLYLINE> booleanOp(POLYLINE a, POLYLINE b, BOOLOPERATION op);

std::map<VECTOR, std::vector<POLYLINE*>> splitPolylines(POLYLINE a, POLYLINE b, BOOLOPERATION op);  // Splits a and b into segments, separated by points of intersection.
void cutPolyline(POLYLINE& a, POLYLINE& b, std::vector<int> acuts, BOOLOPERATION op, std::map<VECTOR, std::vector<POLYLINE*>>& sortedSplitPolylines);

void noIntersectionNoContainmentLogic(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op);
void noIntersectionPossibleContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op);
void resolveContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op);

std::vector<VECTOR> intersections(POLYLINE a, POLYLINE b);                                          // Gets keys from the above. Use the below if the above will be called.
std::vector<VECTOR> intersections(std::map<VECTOR, std::vector<POLYLINE*>> m);

std::vector<SEGMENT> getMonotonicInsideSegmentsFromPolyline(POLYLINE p, BOUNDINGBOX interesting);

#endif // POLYLINE_H




