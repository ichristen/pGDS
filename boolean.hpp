#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include <vector>
#include <map>

#include "vector.hpp"
#include "polyline.hpp"

//#define BOOLEAN_DEBUG 1
//#define BOOLEAN_DEBUG_INTERSECTION 1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SEGMENT {    // Helper struct for boolean...
    int b;
    int e;
    
    bool forward;
    
    BOUNDINGBOX bb;
    
    bool operator<(const SEGMENT& s) const;
    bool operator>(const SEGMENT& s) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool intersect(VECTOR a1, VECTOR a2, VECTOR b1, VECTOR b2, VECTOR** i);

std::vector<POLYLINE> booleanOp(POLYLINE a, POLYLINE b, BOOLOPERATION op);

std::map<VECTOR, std::vector<POLYLINE*>> splitPolylines(POLYLINE a, POLYLINE b, BOOLOPERATION op);  // Splits a and b into segments, separated by points of intersection.
void cutPolyline(POLYLINE& a, POLYLINE& b, std::vector<int> acuts, BOOLOPERATION op, std::map<VECTOR, std::vector<POLYLINE*>>& sortedSplitPolylines);

void noIntersectionNoContainmentLogic(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op);
void noIntersectionPossibleContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op);
void resolveContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op);

std::vector<VECTOR> intersections(POLYLINE a, POLYLINE b);                                          // Gets keys from the above. Use the below if the above will be called.
std::vector<VECTOR> intersections(std::map<VECTOR, std::vector<POLYLINE*>> m);

std::vector<SEGMENT> getMonotonicInsideSegmentsFromPolyline(POLYLINE p, BOUNDINGBOX interesting);

#endif // BOOLEAN_HPP




