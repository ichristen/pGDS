#include "polyline.hpp"
#include "material.hpp"

BOUNDINGBOX::BOUNDINGBOX() {
    initialized = false;
}
BOUNDINGBOX::BOUNDINGBOX(VECTOR a) {
    ur = a;
    ll = a;
    initialized = true;     // Initialize with one point...
}
BOUNDINGBOX::BOUNDINGBOX(VECTOR a, VECTOR b) {
    ur = VECTOR(max(a.x, b.x), max(a.y, b.y));
    ll = VECTOR(min(a.x, b.x), min(a.y, b.y));
    initialized = true;
}
GLdouble BOUNDINGBOX::width() {
    if (initialized) {  return ur.x - ll.x; }
    else {              return 0; }
}
GLdouble BOUNDINGBOX::height() {
    if (initialized) {  return ur.y - ll.y; }
    else {              return 0; }
}
BOUNDINGBOX BOUNDINGBOX::copy() const {
    return BOUNDINGBOX(ur, ll);
}
void BOUNDINGBOX::clear() {
    ur.zero();
    ll.zero();
    initialized = false;
}
bool BOUNDINGBOX::isEmpty() {
    return !initialized;
}
bool BOUNDINGBOX::doesContain(VECTOR v) {
    return initialized && (v.x <= ur.x && v.x >= ll.x) && (v.y <= ur.y && v.y >= ll.y);
}
bool BOUNDINGBOX::doesContainX(GLdouble x) {
    return initialized && (x <= ur.x && x >= ll.x);
}
bool BOUNDINGBOX::doesContainY(GLdouble y) {
    return initialized && (y <= ur.y && y >= ll.y);
}
bool BOUNDINGBOX::doesContain(BOUNDINGBOX bb) {
    return initialized && doesContain(bb.ur) && doesContain(bb.ll);
}

bool BOUNDINGBOX::enlarge(VECTOR v) {
    if (!initialized) {
        ur = v;
        ll = v;
        initialized = true;
        
        return true;
    } else {
        bool toReturn = false;
        
        if (ur.x < v.x) {
            ur.x = v.x; toReturn = true; } else
        if (ll.x > v.x) {
            ll.x = v.x; toReturn = true; }
        
        if (ur.y < v.y) {
            ur.y = v.y; toReturn = true; } else
        if (ll.y > v.y) {
            ll.y = v.y; toReturn = true; }
    
        return toReturn;
    }
}
bool BOUNDINGBOX::enlarge(BOUNDINGBOX bb) {
    if (initialized) {
        return enlarge(bb.ur) || enlarge(bb.ll);
    } else {
        *this = bb;
        
        return true;
    }
}
bool BOUNDINGBOX::doesIntersect(BOUNDINGBOX bb) {
    return (ur.x >= bb.ll.x && bb.ur.x >= ll.x) && (ur.y >= bb.ll.y && bb.ur.y >= ll.y);
}

BOUNDINGBOX BOUNDINGBOX::operator&(BOUNDINGBOX bb) {    return copy() &= bb; }
BOUNDINGBOX BOUNDINGBOX::operator&=(BOUNDINGBOX bb) {
    if (!initialized) {
        return BOUNDINGBOX();
    } else {
        if (doesIntersect(bb)) {
            ur.x = min(ur.x, bb.ur.x);
            ur.y = min(ur.y, bb.ur.y);
            ll.x = max(ll.x, bb.ll.x);
            ll.y = max(ll.y, bb.ll.y);
        } else {
            clear();
        }
        
        return *this;
    }
}

BOUNDINGBOX BOUNDINGBOX::operator|(BOUNDINGBOX bb) {    return copy() |= bb; }
BOUNDINGBOX BOUNDINGBOX::operator|=(BOUNDINGBOX bb) {
    if (!initialized) {
        return operator=(bb);
    } else {
        ur.x = max(ur.x, bb.ur.x);
        ur.y = max(ur.y, bb.ur.y);
        ll.x = min(ll.x, bb.ll.x);
        ll.y = min(ll.y, bb.ll.y);
        
        return *this;
    }
}

BOUNDINGBOX BOUNDINGBOX::operator|(VECTOR v) {          return copy() |= v; }
BOUNDINGBOX BOUNDINGBOX::operator|=(VECTOR v) {
    if (!initialized) {
        ur = v;
        ll = v;
        initialized = true;
        
        return *this;
    } else {
        ur.x = max(ur.x, v.x);
        ur.y = max(ur.y, v.y);
        ll.x = min(ll.x, v.x);
        ll.y = min(ll.y, v.y);
    
        return *this;
    }
}
BOUNDINGBOX BOUNDINGBOX::operator=(BOUNDINGBOX bb) {
    ll = bb.ll;
    ur = bb.ur;
    initialized = bb.initialized;
    
    return *this;
}

BOUNDINGBOX BOUNDINGBOX::operator/ (GLdouble s)   const { return copy() /= s; }
BOUNDINGBOX BOUNDINGBOX::operator* (GLdouble s)   const { return copy() *= s; }
BOUNDINGBOX BOUNDINGBOX::operator/=(GLdouble s) {
    ur /= s;
    ll /= s;
    return *this;
}
BOUNDINGBOX BOUNDINGBOX::operator*=(GLdouble s) {
    ur /= s;
    ll /= s;
    return *this;
}

GLdouble BOUNDINGBOX::area()                 const {
    if (initialized) {
        return (ur.x - ll.x) * (ur.y - ll.y);
    } else {
        return 0;
    }
}
bool BOUNDINGBOX::operator==(BOUNDINGBOX bb) const {
    return bb.initialized == initialized && bb.ll == ll && bb.ur == ur;
}
void BOUNDINGBOX::print()                    const {
#ifdef MATLAB_MEX_FILE
    mexPrintf("Bounding box with\n");
    mexPrintf("  ll = ");   ll.printNL();
    mexPrintf("  ur = ");   ur.printNL();
#else
    printf("Bounding box with\n");
    printf("  ll = ");   ll.printNL();
    printf("  ur = ");   ur.printNL();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

POLYLINE::POLYLINE(size_t size_) {
    points.reserve(size_);
    
    layer = MATERIAL::currentLayer;
}
POLYLINE::POLYLINE(POLYLINE p, int b, int e) {
    printf("WARNING: POLYLINE(POLYLINE p, int b, int e) is currently slightly bugged (with begin/end calculation).\n");
    
    printf("b=%i\n", b);
    printf("e=%i\n", e);
    
    b %= p.size();
    e %= p.size();
    
    isClosed = false;   // p.isClosed;
    isReversed = false;
    
    if (b == e) {
        points.push_back(p[b]);
        
        print();
    } else if (b < e) {
        points.reserve(e-b+1);
        
        if (p.isReversed) { std::copy(p.points.rbegin() + b, p.points.rbegin() + e + 1, std::back_inserter(points)); }  // Check the + 1...
        else {              std::copy(p.points.begin()  + b, p.points.begin()  + e + 1, std::back_inserter(points)); }
        
//        printf("SIZE: %i", points.size());
        
        if (b == 0 && !p.isClosed) {
            if (p.isReversed) { begin = p.end; }
            else {              begin = p.begin; }
        } else {
            begin = ( (p[b] - p[b-1]).unit() + (p[b+1] - p[b]).unit() ).unit() * ( (p.isReversed)?(-1):(1) ); // Check if b±1 is the same point?
        }
        
        if (e == p.size()-1 && !p.isClosed) {
            if (p.isReversed) { end = p.begin; }
            else {              end = p.end; }
        } else {
            end = ( (p[e] - p[e-1]).unit() + (p[e+1] - p[e]).unit() ).unit() * ( (p.isReversed)?(-1):(1) ); // Check if e±1 is the same point?
        }
        
        print();
    } else if (b > e) {
        if (p.isClosed) {
            points.reserve(p.size()-e+b+1);
            
            if (p.isReversed) {
                std::copy(p.points.rbegin() + b,    p.points.rend(),            std::back_inserter(points));
                std::copy(p.points.rbegin(),        p.points.rbegin() + e + 1,  std::back_inserter(points));
            }
            else {
                std::copy(p.points.begin() + b,     p.points.end(),             std::back_inserter(points));
                std::copy(p.points.begin(),         p.points.begin() + e + 1,   std::back_inserter(points));
            }
            
            begin = ( (p[b] - p[b-1]).unit() + (p[b+1] - p[b]).unit() ).unit() * ( (p.isReversed)?(-1):(1) ); // Check if b±1 is the same point?
            end =   ( (p[e] - p[e-1]).unit() + (p[e+1] - p[e]).unit() ).unit() * ( (p.isReversed)?(-1):(1) ); // Check if e±1 is the same point?
            
            print();
        } else {
            // Don't know what to do...
            throw std::runtime_error("POLYLINE(POLYLINE p, int b, int e): Unable to clip an open polyline with e < b.");
        }
    }
    
    layer = p.layer;
}

void POLYLINE::add(VECTOR v){
    operator+=(v);
}
void POLYLINE::add(POLYLINE p){
    operator+=(p);
}

void POLYLINE::recomputeBoundingBox() {
    bb.clear();
    
    for (int i = 0; i < points.size(); i++){
        bb.enlarge(points[i]);
    }
}

POLYLINE POLYLINE::clip(int b, int e) {
    return POLYLINE(*this, b, e);
}


VECTOR POLYLINE::operator[](int i) const {
    //    if (i < 0 || i >= points.size()){   return VECTOR(); }  // Or throw error?
//    if (i < 0 || i >= points.size()){   printf("POLYLINE[int i]: i out of range. Setting i = i %% size().\n"); }
    if (points.size()) {
        i = i % points.size();
        
        if (isReversed) {                   return points[points.size() - i - 1]; }
        else {                              return points[i]; }
    } else {
        throw std::runtime_error("This POLYLINE has no points to reference[]...");
        //        return VECTOR();
    }
}
bool POLYLINE::insert(int i, VECTOR v) {
    if (i < 0 || i > points.size()) {
        return false;
    } else {
        if (isReversed) {   points.insert(points.begin() + points.size() - i, v); }
        else {              points.insert(points.begin() + i, v); }
        return true;
    }
}

//void POLYLINE::operator=(POLYLINE other) {
//    
//}

POLYLINE POLYLINE::operator-() {        // Revist whether area should be calculated.
    POLYLINE toReturn = *this;
    toReturn.area_ = -area();
    toReturn.isReversed = !isReversed;
    
    return toReturn;
}
POLYLINE POLYLINE::reverse() {
    area_ = -area();
    isReversed = !isReversed;
    
//    VECTOR temp = begin;
//    
//    begin = end;
//    end = temp;
    
    return *this;
}

void POLYLINE::makeforward() {
    if (isReversed) {
        reverse();
        points = std::vector<VECTOR>(points.rbegin(), points.rend());
        
        VECTOR temp = begin;
        begin = end;
        end = temp;
    }
}
VECTOR POLYLINE::getBeginPoint() {
    if (size()) {
        if (isReversed) {   return points[points.size()-1]; }
        else {              return points[0]; }
    }
    
    return VECTOR();
}
VECTOR POLYLINE::getEndPoint() {
    if (size()) {
        if (isReversed) {   return points[0]; }
        else {              return points[points.size()-1]; }
    }

    return VECTOR();
}
VECTOR POLYLINE::getBeginDirection() {
    if (isClosed) {
        return VECTOR();
    } else {
        if (isReversed) {   return end; }
        else {              return begin; }
    }
}
VECTOR POLYLINE::getEndDirection() {
    if (isClosed) {
        return VECTOR();
    } else {
        if (isReversed) {   return begin; }
        else {              return end; }
    }
}

POLYLINE POLYLINE::operator/(double s) const { return copy() /= s; }
POLYLINE POLYLINE::operator*(double s) const { return copy() *= s; }
POLYLINE POLYLINE::operator*(AFFINE m) const { return copy() *= m; }

POLYLINE POLYLINE::operator/=(double s) {
    std::transform(points.begin(), points.end(), points.begin(),
                   [s](VECTOR v) -> VECTOR { return v/s; });
    bb /= s;
    return *this;
}
POLYLINE POLYLINE::operator*=(double s) {
    std::transform(points.begin(), points.end(), points.begin(),
                   [s](VECTOR v) -> VECTOR { return v*s; });
    bb *= s;
    return *this;
}
POLYLINE POLYLINE::operator*=(AFFINE m) {
    std::transform(points.begin(), points.end(), points.begin(),
                   [m](VECTOR v) -> VECTOR { return v*m; });
    recomputeBoundingBox();
    return *this;
}

POLYLINE POLYLINE::operator+(VECTOR v) const {  return copy() += v; }
POLYLINE POLYLINE::operator-(VECTOR v) const {  return copy() -= v; }

//POLYLINE POLYLINE::operator+=(VECTOR v) {                         // Use a AFFINE for a shift like this.
//    std::transform(points.begin(), points.end(), points.begin(),
//                   [v](VECTOR v0) -> VECTOR { return v0 + v; });
//    return *this;
//}
//POLYLINE POLYLINE::operator-=(VECTOR v) {
//    std::transform(points.begin(), points.end(), points.begin(),
//                   [v](VECTOR v0) -> VECTOR { return v0 - v; });
//    return *this;
//}

POLYLINE POLYLINE::operator+=(VECTOR v) {
    area_ = 0;
    length_ = 0;
    
#ifdef USE_GL_RENDER
    if (fillList) {     glDeleteLists(fillList, 1); }
    if (outlineList) {  glDeleteLists(outlineList, 1); }
#endif
    
    bb.enlarge(v);
    
//    if (v != *(points.end())) {
        points.push_back(v);
//    }

    return *this;
}
POLYLINE POLYLINE::operator-=(VECTOR v) {
    return operator+=(-v);
}

POLYLINE POLYLINE::operator+(POLYLINE p) const {  return copy() += p; }
POLYLINE POLYLINE::operator-(POLYLINE p) const {  return copy() -= p; }

POLYLINE POLYLINE::operator+=(POLYLINE p) {
    if (!p.size()) {
        // Do nothing...
    } else if (layer == p.layer && p.size() == 1) {
        operator+=(p[0]);
    } else if (layer == p.layer && p.size() > 1) {
        if (isClosed && p.isClosed) {           // Boolean union of two closed shapes.
            throw std::runtime_error("POLYLINE::operator+=(POLYLINE): Use &, |, or ^ for boolean operations.");
//            return boolean(p, AND);
        } else if (!isClosed && p.isClosed) {   // Addition of a line segment to a closed shape.
            throw std::runtime_error("A closed polyline cannot be added to an open polyline");
        } else {
            area_ = 0;
            length_ = 0;
            
#ifdef USE_GL_RENDER
            if (fillList) {     glDeleteLists(fillList, 1); }
            if (outlineList) {  glDeleteLists(outlineList, 1); }
#endif
            bb.enlarge(p.bb);
            
            makeforward();
            
            int add = 0;
            
            if (size()) {
                add = (p[0] == operator[](0));
            }
            
            if (p.isReversed) {
//                std::copy(p.points.begin()  + add, p.points.end(), std::back_inserter(points));
                std::copy(p.points.rbegin() + add, p.points.rend(), std::back_inserter(points));
                end = p.begin;
            } else {
//                std::copy(p.points.rbegin() + add, p.points.rend(), std::back_inserter(points));
                std::copy(p.points.begin()  + add, p.points.end(), std::back_inserter(points));
                end = p.end;
            }
        }
    } else {
//        throw std::runtime_error("Polylines of different layers cannot be combined.");
    }
    
    return *this;
}
POLYLINE POLYLINE::operator-=(POLYLINE p) {
    return operator+=(-p);
}


POLYLINES POLYLINE::boolean(POLYLINE p, BOOLOPERATION op) const {
    return POLYLINES(booleanOp(*this, p, op));
    
//    throw std::runtime_error("Boolean arithmatic has not been implemented (yet)...");
}

POLYLINES POLYLINE::operator&(POLYLINE p) const { return boolean(p, AND); }
POLYLINES POLYLINE::operator|(POLYLINE p) const { return boolean(p, OR); }
POLYLINES POLYLINE::operator^(POLYLINE p) const { return boolean(p, XOR); }

GLdouble POLYLINE::area() {
    if (isClosed && points.size() >= 3) {
        if (area_ == 0){
            area_ = points[points.size()-1].perpCCWdot(points[0]);
            
            for (int i = 0; i < points.size()-1; i++) {
                area_ += points[i].perpCCWdot(points[i+1]);
            }
            
            area_ /= (isReversed)?( 2 ):( -2 );
        }
        
        return area_;
    } else {
        return 0;
    }
}

size_t POLYLINE::size() const {
    return points.size();
}

POLYLINE POLYLINE::close() {
    isClosed = true;
    return *this;
}
POLYLINE POLYLINE::open() {
    isClosed = false;
    return *this;
}

bool POLYLINE::isCCW() {
    return area() > 0;      // What about unclosed?
}
bool POLYLINE::isEmpty() const {
//    printf("SIZE = %i\n", points.size());
    return points.size() == 0;
}

std::vector<GLdouble> POLYLINE::getIntersections(GLdouble x) const {
    std::vector<GLdouble> toReturn;
    
    for (int i = 0; i < points.size()-1; i++) {
        if ( (points[i].x <= x && points[i+1].x > x) || (points[i].x > x && points[i+1].x <= x)) {
            VECTOR dpoints = points[i+1] - points[i];
            
            toReturn.push_back( (dpoints.y / dpoints.x) * (x - points[i].x) + points[i].y );
        }
    }
    
    if ( (points[points.size()-1].x <= x && points[0].x > x) || (points[points.size()-1].x > x && points[0].x <= x)) {
        VECTOR dpoints = points[0] - points[points.size()-1];
        
        toReturn.push_back( (dpoints.y / dpoints.x) * (x - points[points.size()-1].x) + points[points.size()-1].y );
    }
    
    std::sort(toReturn.begin(), toReturn.end());
    
    return toReturn;
}
std::vector<int> POLYLINE::getIntersectionsIndex(GLdouble x) const {
    std::vector<GLdouble> ylist;
    std::vector<int> toReturn;
    
    for (int i = 0; i < points.size()-1; i++) {
        if ( (points[i].x <= x && points[i+1].x > x) || (points[i].x > x && points[i+1].x <= x)) {
            VECTOR dpoints = points[i+1] - points[i];
            
            ylist.push_back( (dpoints.y / dpoints.x) * (x - points[i].x) + points[i].y );
            if (isReversed) { toReturn.push_back((int)points.size()-i-1); }
            else            { toReturn.push_back(i); }
        }
    }
    
    if ( (points[points.size()-1].x <= x && points[0].x > x) || (points[points.size()-1].x > x && points[0].x <= x)) {
        VECTOR dpoints = points[0] - points[points.size()-1];
        
        ylist.push_back( (dpoints.y / dpoints.x) * (x - points[points.size()-1].x) + points[points.size()-1].y );
        if (isReversed) { toReturn.push_back(0); }
        else            { toReturn.push_back((int)points.size()-1); }
    }
    
//    std::sort(toReturn.begin(), toReturn.end());
    
    std::vector<std::size_t> p(toReturn.size());
    std::iota(p.begin(), p.end(), 0);               // Fill p with [ 0, 1, 2, ... ]
    std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j){ return ylist[i] < ylist[j]; });
    
    std::vector<int> sortedReturn(toReturn.size());
    std::transform(p.begin(), p.end(), sortedReturn.begin(),
                   [&](std::size_t i){ return toReturn[i]; });
    
    return sortedReturn;
}
bool POLYLINE::contains(VECTOR v) const {
    if (isClosed) {
        std::vector<GLdouble> y = getIntersections(v.x);
        
        int i = 0;
        
        while (i < y.size() && v.y > y[i]) { i++; }
        
        if (i == y.size())  { return false; }
        
        if (v.y == y[i])    { return true; }
        
        return i % 2;
    }
    return false;
}

GLdouble POLYLINE::length() {
    if (length_ == 0) {
        for (int i = 0; i < points.size()-1; i++) {
            length_ += (points[i] - points[i+1]).magn();
        }
    }
    return length_;
}

POLYLINE POLYLINE::setLayer(uint16_t l) {
    layer = l;
    return *this;
}

POLYLINE POLYLINE::copy() const {
    POLYLINE toReturn(points.size());
    
    toReturn.points.resize(points.size());
    
    std::copy(points.begin(), points.end(), toReturn.points.begin());
    
    toReturn.isReversed =   isReversed;
    toReturn.isClosed =     isClosed;
    
    toReturn.area_ =        area_;
    toReturn.length_ =      length_;
    
    toReturn.layer =        layer;
    
    toReturn.begin =        begin;
    toReturn.end =          end;
    
    return toReturn;
}
/*
//void POLYLINE::fill() {
//    if (fillList == 0) {
//        fillList = glGenLists(1);
//        
//        glNewList(fillList, GL_COMPILE);
//        
//        // create tessellator
//        GLUtesselator *tess = gluNewTess();
//        
//        // describe non-convex polygon
//        gluTessBeginPolygon(tess, nullptr);
//        // first contour
//        gluTessBeginContour(tess);
//        
//        for (int i = 0; i < points.size(); i++){
//            gluTessVertex(tess, (GLdouble*)&points[i], (GLdouble*)&points[i]);
//        }
//        
//        gluTessEndContour(tess);
//        
//        gluTessEndPolygon(tess);
//        
//        // delete tessellator after processing
//        gluDeleteTess(tess);
//        
//        glEndList();
//    }
//    
//    glPushAFFINE();
//    //    glScaled(i,i,i);
//    glCallList(fillList);
//    glPopAFFINE();
//}
//void POLYLINE::outline() {
//    if (outlineList == 0) {
//        outlineList = glGenLists(1);
//        glNewList(outlineList, GL_COMPILE);
//        
//        glBegin(GL_LINE_STRIP);
//        
////        for (int i = 0; i < points.size(); i++) {
////            glVertex2dv((GLdouble*)&points[i]);
////        }
////        
////        glVertex2dv((GLdouble*)&points[0]);
//        
//        for (int i = 0; i < points.size(); i++) {
//            glVertex2d(points[i].x, points[i].y);
//        }
//        
//        if (isClosed) {
//            glVertex2d(points[0].x, points[0].y);
//        }
//        
//        glEnd();
//        
//        glEndList();
//    }
//    
////    transformation.print();
//    
//    glPushAFFINE();
////    transformation.glTransform();
//    glCallList(outlineList);
//    glPopAFFINE();
//}*/
void POLYLINE::print() const {
    if (isEmpty()) {
#ifdef MATLAB_MEX_FILE
        mexPrintf("Empty POLYLINE\n");
#else
        printf("Empty POLYLINE\n");
#endif
    } else {
        
#ifdef MATLAB_MEX_FILE
        if (isClosed) { mexPrintf("Closed"); } else { mexPrintf("Open"); }
        
        mexPrintf(" POLYLINE with points: \n");
#else
        if (isClosed) { printf("Closed"); } else { printf("Open"); }
        
        printf(" POLYLINE with points: \n");
#endif
        
        if (isReversed) {
            for (int i = (int)points.size()-1; i >= 0; i--) {   printf("[%i] - ", (int)points.size()-1-i);  points[i].printNL(); }
        } else {
            for (int i = 0; i < points.size(); i++) {           printf("[%i] - ", i);                       points[i].printNL(); }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

POLYLINES::POLYLINES() {
    // Nothing to do...
}
POLYLINES::POLYLINES(POLYLINE polyline) {
    add(polyline);
}
POLYLINES::POLYLINES(std::vector<POLYLINE> polylines_) {
    polylines.reserve(polylines.size());
    std::copy(polylines_.begin(), polylines_.end(), std::back_inserter(polylines)); // Or use std::insert?
    
    for (int i = 0; i < polylines.size(); i++) { bb.enlarge(polylines[i].bb); }
}

POLYLINE    POLYLINES::operator[](int i)        const {
    if (isEmpty()) {
        return POLYLINE();
    } else {
        return polylines[i % polylines.size()];
    }
}
bool        POLYLINES::insert(int i, POLYLINE p) {
    if (i < 0 || i > polylines.size()) {
        return false;
    } else {
        polylines.insert(polylines.begin() + i, p);
        return true;
    }
}
void        POLYLINES::clear() {
    polylines.clear();
    bb.clear();
}

void        POLYLINES::add(POLYLINE p) {
    if (!p.isEmpty()) {
        polylines.push_back(p);
        
        bb.enlarge(p.bb);
    }
}
void        POLYLINES::add(POLYLINES p) {
    polylines.insert(polylines.end(), p.polylines.begin(), p.polylines.end());
    
    for (int i = 0; i < polylines.size(); i++) { bb.enlarge(polylines[i].bb); }
}

void POLYLINES::recomputeBoundingBox() {
    bb.clear();
    
    for (int i = 0; i < polylines.size(); i++){
        polylines[i].recomputeBoundingBox();
        bb.enlarge(polylines[i].bb);
    }
}

size_t      POLYLINES::size()                   const {
    return polylines.size();
}
bool        POLYLINES::isEmpty()                const {
    return polylines.size() == 0;
}

POLYLINES   POLYLINES::operator/ (double s)     const { return copy() /= s; }
POLYLINES   POLYLINES::operator* (double s)     const { return copy() *= s; }
POLYLINES   POLYLINES::operator* (AFFINE m)     const { return copy() *= m; }
POLYLINES   POLYLINES::operator/=(double s) {
    for (int i = 0; i < polylines.size(); i++) { polylines[i] /= s; }
    bb /= s;
    return *this;
}
POLYLINES   POLYLINES::operator*=(double s) {
    for (int i = 0; i < polylines.size(); i++) { polylines[i] *= s; }
    bb *= s;
    return *this;
}
POLYLINES   POLYLINES::operator*=(AFFINE m) {
    for (int i = 0; i < polylines.size(); i++) { polylines[i] *= m; }
    recomputeBoundingBox();
    return *this;
}


POLYLINES   POLYLINES::operator-() { return copy().reverse(); }
POLYLINES   POLYLINES::reverse() {
    for (int i = 0; i < polylines.size(); i++) { polylines[i].reverse(); }
    return *this;
}


POLYLINES   POLYLINES::boolean(POLYLINE p, BOOLOPERATION op) const { return copy().booleanEquals(p, op); }
POLYLINES   POLYLINES::booleanEquals(POLYLINE p, BOOLOPERATION op) {
//    for (int i = 0; i < polylines.size(); i++) { polylines[i].boolean(p, op); }
    
    if (op == OR) {
        POLYLINES pUnique(p);
        
        for (int i = 0; i < polylines.size(); i++) {
            pUnique.booleanEquals(-polylines[i], AND);  // Cut away any intersections.
        }
        
        add(pUnique);   // And add this new polyline(s).
    } else {
        std::vector<POLYLINE> temp(polylines);  // Inefficient; consider improving.
        
        clear();
        
        switch (op) {
            default:
                throw std::runtime_error("Expected OR to have been caught previously...");
            case AND:
                for (int i = 0; i < temp.size(); i++) {
                    add(temp[i].boolean(p, AND));
                }
                
                break;
            case XOR:
                for (int i = 0; i < temp.size(); i++) {
                    add(temp[i].boolean(-p, AND));  // Add all of the area outside of the new polyline.
                }
                
                POLYLINES pUnique(p);   // Note that no case can be after this...
                
                for (int i = 0; i < temp.size(); i++) {
                    pUnique.booleanEquals(-temp[i], AND); // Cut away any part of the new polyline that is inside our previous polylines.
                }
                
                add(pUnique);
                
                break;
        }
    }
    
    return *this;
}
POLYLINES   POLYLINES::boolean(POLYLINES p, BOOLOPERATION op) const { return copy().booleanEquals(p, op); }
POLYLINES   POLYLINES::booleanEquals(POLYLINES p, BOOLOPERATION op) {
    if (size()*p.size() > 100) {
        printf("POLYLINES::booleanEquals(POLYLINES, BOOLOPERATION): There are this.size() * other.size() = %lu * %lu = %lu comparisons to make.\nMaybe it's time to implement Bentley–Ottmann\n", size(), p.size(), size()*p.size());
    }
    
    if (op == OR) {
        POLYLINES pUnique(p);
        
        for (int i = 0; i < polylines.size(); i++) {
            pUnique.booleanEquals(-polylines[i], AND);  // Cut away any intersections.
        }
        
        add(pUnique);   // And add this new polyline(s).
    } else {
        std::vector<POLYLINE> temp(polylines);  // Inefficient; consider improving.
        
        clear();
        
        switch (op) {
            default:
                throw std::runtime_error("Expected OR to have been caught previously...");
            case AND:
                for (int i = 0; i < temp.size(); i++) {
                    for (int j = 0; j < p.polylines.size(); j++) {
                        add(temp[i].boolean(p.polylines[j], AND));
                    }
                }
                
                break;
            case XOR:
                for (int i = 0; i < temp.size(); i++) {
                    for (int j = 0; j < p.polylines.size(); j++) {
                        add(temp[i].boolean(-p.polylines[j], AND));  // Add all of the area outside of the new polyline.
                    }
                }
                
                POLYLINES pUnique = p;              // Note that no case can be after this...
                
                for (int i = 0; i < temp.size(); i++) {
                    pUnique.booleanEquals(-temp[i], AND); // Cut away any part of the new polyline that is inside our previous polylines.
                }
                
                add(pUnique);
                
                break;
        }
    }
    
    return *this;
}


POLYLINES   POLYLINES::operator&(POLYLINE p)    const { return boolean(p, AND); }
POLYLINES   POLYLINES::operator|(POLYLINE p)    const { return boolean(p, OR); }
POLYLINES   POLYLINES::operator^(POLYLINE p)    const { return boolean(p, XOR); }

POLYLINES   POLYLINES::operator&=(POLYLINE p) { return booleanEquals(p, AND); }
POLYLINES   POLYLINES::operator|=(POLYLINE p) { return booleanEquals(p, OR); }
POLYLINES   POLYLINES::operator^=(POLYLINE p) { return booleanEquals(p, XOR); }


POLYLINES   POLYLINES::operator&(POLYLINES p)   const { return boolean(p, AND); }
POLYLINES   POLYLINES::operator|(POLYLINES p)   const { return boolean(p, OR); }
POLYLINES   POLYLINES::operator^(POLYLINES p)   const { return boolean(p, XOR); }

POLYLINES   POLYLINES::operator&=(POLYLINES p) { return booleanEquals(p, AND); }
POLYLINES   POLYLINES::operator|=(POLYLINES p) { return booleanEquals(p, OR); }
POLYLINES   POLYLINES::operator^=(POLYLINES p) { return booleanEquals(p, XOR); }

GLdouble    POLYLINES::area() {
    GLdouble toReturn = 0;
    
    for (int i = 0; i < polylines.size(); i++) { toReturn += polylines[i].area(); }
    
    return toReturn;
}

bool        POLYLINES::contains(VECTOR v)       const {
    bool toReturn = false;
    
    for (int i = 0; i < polylines.size() && !toReturn; i++) { toReturn &= polylines[i].contains(v); }
    
    return toReturn;
}

POLYLINES   POLYLINES::copy()                   const { return POLYLINES(polylines); }

void        POLYLINES::print()                  const {
    if (isEmpty()) {
#ifdef MATLAB_MEX_FILE
        mexPrintf("Empty POLYLINES\n");
#else
        printf("Empty POLYLINES\n");
#endif
    } else {
        for (int i = 0; i < polylines.size(); i++) { polylines[i].print(); }
    }
}




