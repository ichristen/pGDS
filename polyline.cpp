#include "polyline.hpp"
#include "material.hpp"

//void CURRENTLAYER::setCurrent(uint16_t newlayer) {
//    layer = newlayer;
//}


////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////

bool SEGMENT::operator<(const SEGMENT& s) const {
    return bb.ll.x < s.bb.ll.x;
}
bool SEGMENT::operator>(const SEGMENT& s) const {
    return bb.ll.x > s.bb.ll.x;
}

bool intersect(VECTOR a1, VECTOR a2, VECTOR b1, VECTOR b2, VECTOR** i) {
    if (a1 == a2 || b1 == b2) {     // Don't consider points...
        return false;
    }
    
//    if ( ( max(a1.x, a2.x) >= min(b1.x, b2.x) && max(b1.x, b2.x) >= min(a1.x, a2.x) ) &&
//         ( max(a1.y, a2.y) >= min(b1.y, b2.y) && max(b1.y, b2.y) >= min(a1.y, a2.y) ) ) {   // If the bounding boxes do not intersect...
//        return false;
//    }
    
    printf("\nIntersecting the segment ");  a1.print(); printf(" -> "); a2.print();
    printf(" with the segment ");           b1.print(); printf(" -> "); b2.print(); printf(" ...\n");
    
    printf("1 ... ");
    
    if ( ( max(a1.x, a2.x) < min(b1.x, b2.x) || max(b1.x, b2.x) < min(a1.x, a2.x) ) ||
         ( max(a1.y, a2.y) < min(b1.y, b2.y) || max(b1.y, b2.y) < min(a1.y, a2.y) ) ) {   // If the bounding boxes do not intersect...
        return false;
    }
    
    printf("2 ... ");
    
    if ((a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1)) { // If the lines are equal,
        *i = nullptr;   // Then, there are infintely many points of intersection.
        return true;    // But we still report that they intersect.
    }
    
    printf("3 ... ");
    
    if (a1.x == a2.x && b1.x == b2.x) { // If a and b are vertical
                                        //        if (a1.x == b1.x) {
        GLdouble mina = min(a1.y, a2.y);
        GLdouble maxa = max(a1.y, a2.y);
        
        GLdouble minb = min(b1.y, b2.y);
        GLdouble maxb = max(b1.y, b2.y);
        
        if (mina == maxb) {                 // If a is below b and they touch...
            *i = new VECTOR(a1.x, mina);    // Then, return this intersection point
        } else if (maxa == minb) {          // If b is below a and they touch...
            *i = new VECTOR(a1.x, maxa);    // Then, return this intersection point
        } else {                            // If they overlap...
            *i = nullptr;                   // Then, there are infintely many points of intersection.
        }
        
        return true;
        //        } else {
        //            throw std::runtime_error("intersect(VECTOR^4, VECTOR*): a and b are vertical, but not on the same line. Bounding box check should have caught this.");
        //        }
    } else if (a1.x == a2.x) {
        if (a1.x <= max(b1.x, b2.x) && a1.x >= min(b1.x, b2.x)) {
//            GLdouble bslope = (b1.y - b2.y)/(b1.x - b2.x);
            
            GLdouble by = (a1.x-b1.x)*(b1.y - b2.y)/(b1.x - b2.x) + b1.y;
            
            if (by <= max(a1.y, a2.y) && by >= min(a1.y, a2.y)) {
                *i = new VECTOR(a1.x, by);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if (b1.x == b2.x) {
        if (b1.x <= max(a1.x, a2.x) && b1.x >= min(a1.x, a2.x)) {
//            GLdouble aslope = (a1.y - a2.y)/(a1.x - a2.x);
            
            GLdouble ay = (b1.x-a1.x)*(a1.y - a2.y)/(a1.x - a2.x) + a1.y;
            
            if (ay <= max(b1.y, b2.y) && ay >= min(b1.y, b2.y)) {
                *i = new VECTOR(b1.x, ay);
                return true;
            } else {
                return false;
            }
            
//            *i = new VECTOR(b1.x, aslope*(b1.x-a1.x) + a1.y);
//            return true;
        } else {
            return false;
        }
    }
    
    printf("4 ... ");
    
    GLdouble aslope = (a1.y - a2.y)/(a1.x - a2.x);
    GLdouble bslope = (b1.y - b2.y)/(b1.x - b2.x);
    
    if (aslope == bslope) {             // If a and b have the same slope
        if (a1 == b1 || a1 == b2) {     // Check to see if endpoints match...
            if ((a2.x > b1.x && a2.x > b2.x) || (a2.x < b1.x && a2.x < b2.x)) {     // If a2 is not on b.
                *i = new VECTOR(a1.x, a1.y);
                return true;
            }
        } else if (a2 == b1 || a2 == b2) {
            if ((a1.x > b1.x && a1.x > b2.x) || (a1.x < b1.x && a1.x < b2.x)) {     // If a1 is not on b.
                *i = new VECTOR(a2.x, a2.y);
                return true;
            }
        }
        
        *i = nullptr;
        return true;                            // Otherwise, there are infintely many points of intersection, because the bounding boxes connect.
    }
    
    printf("5 ... ");
    
    // Lastly, calculate intersection of y = aslope*(x-a1.x) + a1.y and y = bslope*(x-b1.x) + b1.y.
    GLdouble x = (a1.x*aslope - b1.x*bslope - a1.y + b1.y)/(aslope - bslope);   // Check this;
    
    if ( (x >= min(a1.x, a2.x) && x <= max(a1.x, a2.x)) && (x >= min(b1.x, b2.x) && x <= max(b1.x, b2.x)) ) {
        *i = new VECTOR(x, aslope*(x-a1.x) + a1.y);
        return true;
    }
    
    printf("6 ... ");
    
    return false;
}

std::vector<POLYLINE> booleanOp(POLYLINE a, POLYLINE b, BOOLOPERATION op) {
    std::vector<POLYLINE> finalClosedPolylines;
    std::map<VECTOR, std::vector<POLYLINE*>> ssp;
    
    if (!a.area() || !b.area()) {               // If one of the shapes is empty...
        switch (op) {
            case OR:
            case XOR:
                if (a.area()) {                         // Add the shape with area...
                    finalClosedPolylines.push_back(a);
                } else if (b.area()) {
                    finalClosedPolylines.push_back(b);
                }
                
            case AND:
                return finalClosedPolylines;            // Return finalClosedPolylines, empty or with the shape with area (if there is one), depending upon op.
        }
    }
    
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
    BOUNDINGBOX interesting = a.bb & b.bb;
    
    if (interesting.isEmpty()) {                // If there is no intersection whatsoever...
        noIntersectionNoContainmentLogic(a, b, finalClosedPolylines, op);
        
        return finalClosedPolylines;
    }
    
    bool reverseAtEnd = false;
    
    if (aPos && bPos) {                 // If both positive: ++
        ssp = splitPolylines(a, b, op);
    } else if (aPos || bPos) {          // If mixed: +-
        switch (op) {
            case AND:       ssp = splitPolylines(a, b, AND);                            break;
            case OR:        ssp = splitPolylines(-a, -b, AND);  reverseAtEnd = true;    break;
            case XOR:
                if (aPos) { ssp = splitPolylines(a, -b, XOR); }
                if (bPos) { ssp = splitPolylines(-a, b, XOR); }
                
                reverseAtEnd = true;
                
                break;
        }
    } else {                            // If both negative: --
        switch (op) {
            case AND:   ssp = splitPolylines(-a, -b, OR);   reverseAtEnd = true;    break;
            case OR:    ssp = splitPolylines(-a, -b, AND);  reverseAtEnd = true;    break;
            case XOR:   ssp = splitPolylines(-a, -b, XOR);  reverseAtEnd = true;    break;
        }
    }
    
    if (ssp.empty()) {  // If there are no intersections but the bounding boxes overlap...
        bool aSmaller = std::abs(a.area()) < std::abs(b.area());
        
        if          (aSmaller && b.bb.doesContain(a.bb)) {  // If a might be inside b
            noIntersectionPossibleContainment(a, b, finalClosedPolylines, op);
        } else if  (!aSmaller && a.bb.doesContain(b.bb)) {  // If b might be inside a
            noIntersectionPossibleContainment(b, a, finalClosedPolylines, op);
        } else {                                            // No possible intersection
            // Don't fill finalClosedPolylines
        }
        
        return finalClosedPolylines;
    } else {
        // First, make a set of all of the values, so that we can free the memory after we are done.
        std::set<POLYLINE*> values;
        
        for (std::map<VECTOR, std::vector<POLYLINE*>>::iterator it = ssp.begin(); it != ssp.end(); ++it) {
            for (int i = 0; i < it->second.size(); i++) {
                values.insert(it->second[i]);
            }
        }
        
        std::vector<VECTOR> keys = intersections(ssp);
        
        printf("\n\n");
        
        for (int i = 0; i < keys.size(); i++) {
            VECTOR firstIntersection = keys[i];
            VECTOR currentIntersection;
            
            printf("{ %i } - ", i);    firstIntersection.printNL(); printf("\n");
            
            // Check if intersecting.size() = 4?
            
            for (int j = 0; j < ssp[firstIntersection].size(); j++) {
                
                printf("{ %i, %i } - 0x%i - ", i, j, ssp[firstIntersection][j]);
                firstIntersection.printNL();
                
                if (ssp[firstIntersection][j]) {  // If this POLYLINE* is not null, then follow the path...
                    POLYLINE* firstPath = ssp[firstIntersection][j];
                    
                    if (firstPath->getBeginPoint() == firstIntersection) { // If the first point in our path is this intersection...
                        POLYLINE finalPath;                     // ...Then let's follow this path.
                        
                        currentIntersection = firstPath->getEndPoint();
                        
                        finalPath += *firstPath;
                        
                        ssp[firstIntersection][j] = nullptr;    // firstPath has been 'used' and is removed.
                        
                        POLYLINE* currentPath = firstPath;
                        
                        while (currentIntersection != firstIntersection) {
                            std::vector<POLYLINE*> nextPaths = ssp[currentIntersection];
                            
//                            VECTOR in = currentPath->getEndDirection();
                            VECTOR in = (currentPath->operator[]((int)currentPath->size()-1) - currentPath->operator[]((int)currentPath->size()-2)).unit();
                            
                            int mostLeftk = -1;
                            GLdouble mostLeftAngle = TAU;
                            
                            for (int k = 0; k < nextPaths.size(); k++) {
                                printf("nextPaths[%i] = 0x%i\n", k, nextPaths[k]);
                                
                                if (nextPaths[k]) {
                                    nextPaths[k]->getBeginPoint().printNL();
                                }
                                
                                if (nextPaths[k] && nextPaths[k]->getBeginPoint() == currentIntersection) {
//                                    VECTOR out = nextPaths[k]->getBeginDirection();
                                    
                                    VECTOR out = (nextPaths[k]->operator[](1) - nextPaths[k]->operator[](0)).unit();
                                    
                                    printf("nextPaths[k][0]  = "); nextPaths[k]->operator[](0).printNL();
                                    printf("nextPaths[k][1]  = "); nextPaths[k]->operator[](1).printNL();
                                    printf("nextPaths[k][2]  = "); nextPaths[k]->operator[](2).printNL();
                                    
                                    printf("IN   = "); in.printNL();
                                    printf("OUT  = "); out.printNL();
//                                    printf("OUT2 = "); (nextPaths[k]->operator[](1) - nextPaths[k]->operator[](0)).unit().printNL();
                                    
                                    GLdouble angle = acos( -(in * out) );
                                    
                                    if (angle != PI) {    // If the angle doesn't go back on itself...
//                                        if (in.perpCCW() * out < 0) {   // If the vector is to the right...
                                        if (in.perpCCW() * out < 0) {   // If the vector is to the right...
                                                angle = TAU - angle;
                                        }
                                        
                                        printf("ang = %f\n", angle);
                                        
                                        if (angle < mostLeftAngle) {
                                            mostLeftAngle = angle;
                                            mostLeftk = k;
                                        }
                                    }
                                }
                            }
                            
                            if (mostLeftk >= 0) {
                                // Choose the leftmost path...
                                
                                currentPath = nextPaths[mostLeftk];
                                
                                finalPath += *currentPath;
                                
                                printf("mostLeftk=%i\n\n", mostLeftk);
                                
                                ssp[currentIntersection][mostLeftk] = nullptr;    // This path has been 'used' and is removed.
                                
                                currentIntersection = currentPath->getEndPoint();
                            } else {
                                printf("This should not have happened...");
                                currentIntersection = firstIntersection;
                            }
                        }
                        
                        finalPath.close();
                        
                        if (reverseAtEnd) { finalClosedPolylines.push_back(-finalPath); }
                        else {              finalClosedPolylines.push_back(finalPath); }
                    } else {    // Only traverse forward for now. We might want to eventually consider the intersection of open polylines.
                                //                    nextIntersection = currentPath->getBegin();
                    }
                }
            }
        }
        
        for (std::set<POLYLINE*>::iterator it = values.begin(); it != values.end(); ++it) {
            delete (*it);
            
//            free(*it);      // Free all of the temporary polylines.
        }
    }
    
    printf("\nThere are %i resulting polylines:\n\n", (int)finalClosedPolylines.size());
    for (int i = 0; i < finalClosedPolylines.size(); i++) {
        finalClosedPolylines[i].print();
    }
    
    printf("Finished printing...\n\n");
    
    return finalClosedPolylines;
}

std::map<VECTOR, std::vector<POLYLINE*>> splitPolylines(POLYLINE a, POLYLINE b, BOOLOPERATION op) {
    std::vector<POLYLINE*> polylines;
    std::map<VECTOR, std::vector<POLYLINE*>> sortedSplitPolylines;
    
    if ( !a.area() || !b.area() ) {
        throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Previous checks should have caught this.");
    }
    
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
//    printf("a.area() = %f\n", a.area());
//    printf("b.area() = %f\n", b.area());
//    
//    switch (op) {
//        case AND:   printf("OP = AND\n");   break;
//        case OR:    printf("OP = OR\n");   break;
//        case XOR:   printf("OP = XOR\n");   break;
//    }
    
    if ( !((aPos && bPos) || ((aPos || bPos) && op == AND)) ) {
        throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Only ++ AND, OR, XOR, or -+ AND are acceptable for splitPolylines");
    }
    
    // First, find all intersections...
    //
    std::vector<int> acuts;
    std::vector<int> bcuts;
    
    VECTOR* intersection = nullptr;
    
    BOUNDINGBOX interesting = a.bb & b.bb;
    
    if (true) {    // Old, naive O(n^2) algorithm
        if (!interesting.isEmpty()) {
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < b.size(); j++) {
                    
                    printf("\n\nIntersecting a[%i] -> a[%i], b[%i] -> b[%i] ... ", i, i+1, j, j+1);
                    
                    if ( intersect(a[i], a[i+1], b[j], b[j+1], &intersection) ) {   // If they intersect...
                        if (intersection) {                                         // ...and there are not infintely-many intersections (if there are infintely-many intersections, null is returned).
                            intersection->printNL();
                            
                            if      (*intersection == a[i]) {   acuts.push_back(i); }
                            else if (*intersection == a[i+1]) { acuts.push_back(i+1); }
                            else {
                                a[i].printNL();
                                a[i+1].printNL();
                                
                                a.insert(i+1, *intersection);
                                for (int k = 0; k < acuts.size(); k++) { if (acuts[k] > i) { acuts[k]++; } }
                                acuts.push_back(i+1);
                            }
                            
                            if      (*intersection == b[j]) {   bcuts.push_back(j); }
                            else if (*intersection == b[j+1]) { bcuts.push_back(j+1); }
                            else {
                                b[j].printNL();
                                b[j+1].printNL();
                                
                                b.insert(j+1, *intersection);
                                for (int k = 0; k < bcuts.size(); k++) { if (bcuts[k] > j) { bcuts[k]++; } }
                                bcuts.push_back(j+1);
                            }
                            
                            a.print();
                            b.print();
                            
                            delete intersection;
                            intersection = nullptr;
                        }
                    }
                }
            }
        }
    } else {        // New, probably O(n) algorithm
//        BOUNDINGBOX interesting = a.bb & b.bb;
        
        /*
         //        a += a[0];  // Duplicate the first point for simplicity
         //
         //        std::vector<int>            asegmentsstart;
         //        std::vector<int>            asegmentsend;
         //        std::vector<BOUNDINGBOX>    asegmentsbb;
         //        std::vector<bool>           asegmentsdir;
         //
         //        bool inside = interesting.doesContain(a[0]);
         //        bool forward = a[1].x - a[0].x > 0;
         //
         //        BOUNDINGBOX current;
         //
         //        if (inside) {
         //            asegmentsstart.push_back(0);
         //            current = BOUNDINGBOX(a[0]);
         //            asegmentsdir.push_back(forward);
         //        }
         //
         //        for (int i = 1; i < a.size(); i++) {
         //            if (inside) { current.enlarge(a[i]); }
         //
         //            if (inside != interesting.doesContain(a[i])) {  // If we cross the boundary of the interesting bounding box...
         //                if (inside) {                   // If we are going outside...
         //                    asegmentsend.push_back(i);
         //                    asegmentsbb.push_back(current);
         //                } else {                        // If we are coming inside...
         //                    asegmentsstart.push_back(i-1);
         //                    current = BOUNDINGBOX(a[i-1], a[i]);
         //                    forward = a[i].x - a[i-1].x;
         //                    asegmentsdir.push_back(forward);
         //                }
         //
         //                inside = !inside;
         //            } else if (inside) {
         //                if (forward != (a[i].x - a[i-1].x > 0)) {   // If we switch direction...
         //                    asegmentsend.push_back(i-1);
         //                    asegmentsstart.push_back(i-1);
         //                    asegmentsbb.push_back(current);
         //                    current = BOUNDINGBOX(a[i-1], a[i]);
         //                    forward = !forward;
         //                    asegmentsdir.push_back(forward);
         //                }
         //            }
         //        }
         */
        
        // Find the segments in a...
        a += a[0];  // Duplicate the first point for simplicity
        b += b[0];
        
        std::vector<SEGMENT> asegments = getMonotonicInsideSegmentsFromPolyline(a, interesting);
        std::vector<SEGMENT> bsegments = getMonotonicInsideSegmentsFromPolyline(b, interesting);
        
        printf("There are %lu asegments...\n", asegments.size());
        printf("There are %lu bsegments...\n", bsegments.size());
        
        for (int i = 0; i < asegments.size(); i++) {
            printf("asegments from %i to %i with bounding box\n", asegments[i].b, asegments[i].e);
            asegments[i].bb.print();
        }
        
        for (int i = 0; i < bsegments.size(); i++) {
            printf("bsegments from %i to %i with bounding box\n", bsegments[i].b, bsegments[i].e);
            bsegments[i].bb.print();
        }
        
        for (int i = 0; i < asegments.size(); i++) {
            for (int j = 0; j < bsegments.size(); j++) {
                if (asegments[i].bb.doesIntersect(bsegments[j].bb)) {   // If the bounding boxes intersect...
                    BOUNDINGBOX superInteresting = asegments[i].bb & bsegments[j].bb;
                    
                    printf("doesIntersect!\n");
                    superInteresting.print();
                    
                    int adir =  (asegments[i].forward)?(1):(-1);
                    int abeg =  (asegments[i].forward)?(asegments[i].b):(asegments[i].e);
                    int aend =  (asegments[i].forward)?(asegments[i].e):(asegments[i].b);
                    int ai =    abeg;
                    
                    while (ai != aend && a[ai+adir].x < superInteresting.ll.x) { ai += adir; }
                    
                    if (ai == abeg) { ai += adir; }
                    
                    
                    int bdir =  (bsegments[j].forward)?(1):(-1);
                    int bbeg =  (bsegments[j].forward)?(bsegments[i].b):(bsegments[j].e);
                    int bend =  (bsegments[j].forward)?(bsegments[i].e):(bsegments[j].b);
                    int bi =    bbeg;
                    
                    while (bi != bend && b[bi+bdir].x < superInteresting.ll.x) { bi += bdir; }
                    
                    if (bi == bbeg) { bi += bdir; }
                    
                    printf("ai: %i\n", ai);
                    printf("bi: %i\n", bi);
                    
                    printf("ae: %i\n", aend);
                    printf("be: %i\n", bend);
                    
                    printf("ax: %f\n", a[ai-adir].x);
                    printf("bx: %f\n", b[bi-bdir].x);
                    
                    printf("ax2: %f\n", a[ai].x);
                    printf("bx2: %f\n", b[bi].x);
                    
                    printf("adir: %i\n", adir);
                    printf("bdir: %i\n", bdir);
                    
                    printf("sx: %f\n", superInteresting.ur.x);
                    
                    // (a[ai].x <= superInteresting.ur.x || b[bi].x <= superInteresting.ur.x)
                    while ( (a[ai-adir].x <= superInteresting.ur.x && b[bi-bdir].x <= superInteresting.ur.x) && !(ai == aend && bi == bend)) {
//                    while ( (a[ai-adir].x <= superInteresting.ur.x && b[bi-bdir].x <= superInteresting.ur.x) && !(ai == aend && bi == bend)) {
                        
                        printf("Checking intersection between the asegment from "); a[ai-adir].print();
                        printf(" to "); a[ai].print();
                        printf(" and the bsegment from "); b[bi-bdir].print();
                        printf(" to "); b[bi].printNL();
                        
                        if ( intersect(a[ai-adir], a[ai], b[bi-bdir], b[bi], &intersection) ) {   // If they intersect... (note this line can be improved)
                            printf("intersection: 0x%i\n", intersection);
                            
                            if (intersection) {                                         // ...and there are not infintely-many intersections (if there are infintely-many intersections, null is returned).
                                printf("Line intersection at ");
                                intersection->printNL();
                                
                                if      (*intersection == a[ai-adir]) { acuts.push_back(ai-adir); }
                                else if (*intersection == a[ai]) {      acuts.push_back(ai); }
                                else {
                                    int aindex = max(ai, ai-adir);
                                    
                                    // Add the intersection to a if it isn't already there...
                                    a.insert(aindex, *intersection);
                                    
                                    // ...and incriment the indices that will be shifted by its presence.
                                    for (int k = 0; k < acuts.size(); k++) { if (acuts[k] >= aindex) { acuts[k]++; } }
                                    
                                    for (int k = 0; k < asegments.size(); k++) {
                                        if (asegments[k].b >= aindex) { asegments[k].b++; }
                                        if (asegments[k].e >= aindex) { asegments[k].e++; }
                                    }
                                    
                                    if (asegments[i].forward) { aend++; ai++; printf("ai++;\n"); }
                                    
                                    // Then add the index to the list of cuts.
                                    acuts.push_back(aindex);
                                }
                                
                                if      (*intersection == b[i]) {   bcuts.push_back(i); }
                                else if (*intersection == b[i+1]) { bcuts.push_back(i+1); }
                                else {
                                    int bindex = max(bi, bi-bdir);
                                    
                                    // Add the intersection to a if it isn't already there...
                                    b.insert(bindex, *intersection);
                                    
                                    // ...and incriment the indices that will be shifted by its presence.
                                    for (int k = 0; k < bcuts.size(); k++) { if (bcuts[k] >= bindex) { bcuts[k]++; } }
                                    
                                    for (int k = 0; k < bsegments.size(); k++) {
                                        if (bsegments[k].b >= bindex) { bsegments[k].b++; }
                                        if (bsegments[k].e >= bindex) { bsegments[k].e++; }
                                    }
                                    
                                    if (bsegments[j].forward) { bend++; bi++; printf("bi++;\n"); }
                                    
//                                    bi += bdir;
                                    
                                    // Then add the index to the list of cuts.
                                    bcuts.push_back(bindex);
                                }
                                
//                                free(intersection);     // And free the memory...
                                delete intersection;     // And free the memory...
                            }
                        }
                        
                        if      (a[ai-adir].x < b[bi-bdir].x) { ai += adir; }   // If a is lagging...
                        else if (a[ai-adir].x > b[bi-bdir].x) { bi += bdir; }   // If b is lagging...
                        else {                                                  // If a and b are tied..
                            if      (ai == aend) { bi += bdir; }  // If a has ended, incriment b...
                            else if (bi == bend) { ai += adir; }  // If b has ended, incriment a...
                            else {
                                ai += adir;
                                bi += bdir; // Incriment them both...
                            }
                        }
                        
                        printf("ai: %i\n", ai);
                        printf("bi: %i\n", bi);
                        
                        printf("ae: %i\n", aend);
                        printf("be: %i\n", bend);
                        
                        printf("ax: %f\n", a[ai-adir].x);
                        printf("bx: %f\n", b[bi-bdir].x);
                        
                        printf("ax2: %f\n", a[ai].x);
                        printf("bx2: %f\n", b[bi].x);
                        
                        printf("sx: %f\n", superInteresting.ur.x);
                    }
                }
            }
        }
        /*
        //        GLdouble L = interesting.ll.x;
        
        //        std::vector<int> aactive(asegments.size(), -1);     // Initialize to -1...
        //        std::vector<int> bactive(bsegments.size(), -1);     // ...when one is active, it will contain the current index of the corresponding segment.
        //
        //        std::vector<int> order;
        //        std::vector<GLdouble> y;
        //
        //        int i = 0;
        //
        //        for (i = 0; i < asegments.size(); i++) {
        //            if (asegments[i].bb.ll.x < L) {     // If the bounding box is in range, activate the segment
        //                aactive[i] = (asegments[i].forward)?(asegments[i].b):(asegments[i].e);
        //
        //                int j = 0;
        //
        //                while (j < order.size() && a[aactive[i]].y < y[j]) { j++; }
        //
        //                order.insert(   order.begin()   + j, i);                // Yes, .insert() is intensive, but there should be very few active lines...
        //                y.insert(       y.begin()       + j, a[aactive[i]].y);
        //            }
        //        }
        //
        //        int nexta = i;
        //
        //        for (i = 0; i < bsegments.size(); i++) {
        //            if (bsegments[i].bb.ll.x < L) {     // If the bounding box is in range, activate the segment
        //                bactive[i] = (bsegments[i].forward)?(bsegments[i].b):(bsegments[i].e);
        //
        //                int j = 0;
        //
        //                while (j < order.size() && b[bactive[i]].y < y[j]) { j++; }
        //
        //                order.insert(   order.begin()   + j, -i);   // Notice the minus sign...
        //                y.insert(       y.begin()       + j, b[bactive[i]].y);
        //            }
        //        }
        //
        //        int nextb = i;
        //
        //        while (L <= interesting.ur.x) {
        //
        //
        //
        //        }
         */
    }
    
    /*
    std::sort(acuts.begin(), acuts.end());
    acuts.erase(std::unique(acuts.begin(), acuts.end()), acuts.end());
    
    std::sort(bcuts.begin(), bcuts.end());
    bcuts.erase(std::unique(bcuts.begin(), bcuts.end()), bcuts.end());
    
    for (int i = 0; i < acuts.size(); i++) {
        printf("acuts [%i] = %i\n", i, acuts[i]);
    }
    
    for (int i = 0; i < bcuts.size(); i++) {
        printf("bcuts[%i] = %i\n", i, bcuts[i]);
    }
    
    // Second, cleave the polylines into peices, based on the points of intersection.
    
    for (int i = 0; i < acuts.size(); i++) {
        
        printf("i1 = %i\n", i);
        printf("i2 = %lu\n", (i+1) % acuts.size());
        
        int x1 = acuts[i];
        int x2 = acuts[ (i+1) % acuts.size() ];
        
        printf("x1 = %i\n", x1);
        printf("x2 = %i\n", x2);
        
        int l = x2-x1+1;
        
        if (l < 0) {
            l += a.size();
        }
        
        VECTOR testPoint;
        
        printf("l = %i\n", l);
        
        if (l > 2) {
            testPoint = a[x1+1]; // (*toAdd)[1];
        } else if (l == 2) {
            testPoint = (a[x1] + a[x2]) / 2;
        } else {
            throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Did not expect a polyline with less than two points...");
        }
        
        bool isInside = b.contains(testPoint);
        
        POLYLINE* toAdd1;
        POLYLINE* toAdd2;
        
        if (aPos && bPos) {
            
            switch (op) {
                case AND:   // Adds reversed segment only if inside other.
                    if (isInside) {
                        toAdd1 = new POLYLINE(a, x1, x2);
                        
                        toAdd1->reverse();
                        toAdd1->print();
                        
                        sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                        sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    }
                    break;
                    
                case OR:    // Adds all segments, adds an additional reversed segment if inside other.
                    toAdd1 = new POLYLINE(a, x1, x2);
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    
                    toAdd1->print();
                    
                    if (isInside) {
                        toAdd2 = new POLYLINE(a, x1, x2);
                        
                        toAdd2->reverse();
                        toAdd2->print();
                        
                        sortedSplitPolylines[toAdd2->getBeginPoint()].push_back(toAdd2);
                        sortedSplitPolylines[toAdd2->getEndPoint()  ].push_back(toAdd2);
                    }
                    
                    break;
                    
                case XOR:   // Adds all segments, reverses if inside other.
                    toAdd1 = new POLYLINE(a, x1, x2);
                    
                    if (isInside) { toAdd1->reverse(); }
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    
                    toAdd1->print();
                    
                    break;
            }
            
        } else if (aPos) {  // Adds segment if not inside other.
            if (!isInside) {
                toAdd1 = new POLYLINE(a, x1, x2);
                
                sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                
                toAdd1->print();
            }
        } else if (bPos) {  // Adds reversed segment if inside other.
            if (isInside) {
                toAdd1 = new POLYLINE(a, x1, x2);
                
                toAdd1->reverse();
                
                sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                
                toAdd1->print();
            }
            
        } else {
            throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Previous check should have accounded for all negative case...");
        }
    }
    
    for (int i = 0; i < bcuts.size(); i++) {
        int x1 = bcuts[i];
        int x2 = bcuts[ (i+1) % bcuts.size() ];
        
        int l = x2-x1+1;
        
        if (l < 0) {
            l += b.size();
        }
        
        VECTOR testPoint;
        
        if (l > 2) {
            testPoint = b[x1+1]; // (*toAdd)[1];
        } else if (l == 2) {
            testPoint = (b[x1] + b[x2]) / 2;
        } else {
            throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Did not expect a polyline with less than two points...");
        }
        
        bool isInside = a.contains(testPoint);
        
        POLYLINE* toAdd1;
        POLYLINE* toAdd2;
        
        if (aPos && bPos) {
            
            switch (op) {
                case AND:   // Adds reversed segment only if inside other.
                    if (isInside) {
                        toAdd1 = new POLYLINE(b, x1, x2);
                        
                        toAdd1->reverse();
                        
                        sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                        sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    }
                    break;
                    
                case OR:    // Adds all segments, adds an additional reversed segment if inside other.
                    toAdd1 = new POLYLINE(b, x1, x2);
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    
                    if (isInside) {
                        toAdd2 = new POLYLINE(b, x1, x2);
                        
                        toAdd2->reverse();
                        
                        sortedSplitPolylines[toAdd2->getBeginPoint()].push_back(toAdd2);
                        sortedSplitPolylines[toAdd2->getEndPoint()  ].push_back(toAdd2);
                    }
                    
                    break;
                    
                case XOR:   // Adds all segments, reverses if inside other.
                    toAdd1 = new POLYLINE(b, x1, x2);
                    
                    if (isInside) { toAdd1->reverse(); }
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    
                    break;
            }
            
        } else if (bPos) {  // Adds segment if not inside other.
            if (!isInside) {
                toAdd1 = new POLYLINE(b, x1, x2);
                
                sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
            }
        } else if (aPos) {  // Adds reversed segment if inside other.
            if (isInside) {
                toAdd1 = new POLYLINE(b, x1, x2);
                
                toAdd1->reverse();
                
                sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
            }
            
        } else {
            throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Previous check should have accounded for all negative case...");
        }
    }
    */
    
    cutPolyline(a, b, acuts, op, sortedSplitPolylines);
    cutPolyline(b, a, bcuts, op, sortedSplitPolylines);
    
    return sortedSplitPolylines;
}
void cutPolyline(POLYLINE& a, POLYLINE& b, std::vector<int> acuts, BOOLOPERATION op, std::map<VECTOR, std::vector<POLYLINE*>>& sortedSplitPolylines) {
    std::sort(acuts.begin(), acuts.end());
    acuts.erase(std::unique(acuts.begin(), acuts.end()), acuts.end());
    
//    printf("CUTS: ");
//    
//    for (int i = 0; i < acuts.size(); i++) {
//        printf("%i ", acuts[i]);
//    }
//    
//    printf("\n");
    
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
    printf("\n\n");
    
    for (int i = 0; i < acuts.size(); i++) {
        printf("acuts [%i] = %i\n", i, acuts[i]);
    }
    
    a.print();
    
    // Second, cleave the polylines into peices, based on the points of intersection.
    
    for (int i = 0; i < acuts.size(); i++) {
        
        printf("i1 = %i\n", i);
        printf("i2 = %lu\n", (i+1) % acuts.size());
        
        int x1 = acuts[i];
        int x2 = acuts[ (i+1) % acuts.size() ];
        
        printf("x1 = %i\n", x1);
        printf("x2 = %i\n", x2);
        
        int l = x2-x1+1;
        
        if (l <= 0) {
            l += a.size();
        }
        
        VECTOR testPoint;
        
        printf("l = %i\n", l);
        
        if (l > 2) {
            testPoint = a[x1+1]; // (*toAdd)[1];
        } else if (l == 2) {
            testPoint = (a[x1] + a[x2]) / 2;
        } else {
            throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Did not expect a polyline with less than two points...");
        }
        
        bool isInside = b.contains(testPoint);
        
        POLYLINE* toAdd1;
        POLYLINE* toAdd2;
        
        a.print();
        
        if (aPos && bPos) {
            
            switch (op) {
                case AND:   // Adds reversed segment only if inside other.
                    if (isInside) {
                        printf("Inside...\n");
                        
                        toAdd1 = new POLYLINE(a, x1, x2);
                        
                        printf("0x%i\n", toAdd1);
                        
//                        toAdd1->reverse();
                        
                        printf("BEGIN - "); toAdd1->getBeginPoint().printNL();
                        printf("END   - "); toAdd1->getEndPoint().printNL();
                        
                        sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                        sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    } else { printf("Not inside...\n"); }
                    break;
                    
                case OR:    // Adds all segments, adds an additional reversed segment if inside other.
                    toAdd1 = new POLYLINE(a, x1, x2);
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    
                    if (isInside) {
                        toAdd2 = new POLYLINE(a, x1, x2);
                        
                        toAdd2->reverse();
                        
                        sortedSplitPolylines[toAdd2->getBeginPoint()].push_back(toAdd2);
                        sortedSplitPolylines[toAdd2->getEndPoint()  ].push_back(toAdd2);
                    }
                    
                    break;
                    
                case XOR:   // Adds all segments, reverses if inside other.
                    toAdd1 = new POLYLINE(a, x1, x2);
                    
                    if (isInside) { toAdd1->reverse(); }
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    
                    break;
            }
            
        } else if (aPos) {  // Adds segment if not inside other.
            if (!isInside) {
                toAdd1 = new POLYLINE(a, x1, x2);
                
                sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
            }
        } else if (bPos) {  // Adds segment if inside other.
            if (isInside) {
                toAdd1 = new POLYLINE(a, x1, x2);
                
                sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
                sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
            }
        } else {
            throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Previous check should have accounded for all negative case...");
        }
    }
}

void noIntersectionNoContainmentLogic(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op) {
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
    if (aPos && bPos) {
        if (op == OR || op == XOR) {
            finalClosedPolylines.push_back(a);
            finalClosedPolylines.push_back(b);
        }
    } else if (aPos) {
        if (op == AND) {
            finalClosedPolylines.push_back(a);
        } else if (op == OR) {
            finalClosedPolylines.push_back(b);
        } else if (op == XOR) {
            finalClosedPolylines.push_back(a);
            finalClosedPolylines.push_back(b);
        }
    } else if (bPos) {
        if (op == AND) {
            finalClosedPolylines.push_back(b);
        } else if (op == OR) {
            finalClosedPolylines.push_back(a);
        } else if (op == XOR) {
            finalClosedPolylines.push_back(a);
            finalClosedPolylines.push_back(b);
        }
    } else {
        if (op == AND) {
            finalClosedPolylines.push_back(a);
            finalClosedPolylines.push_back(b);
        } else if (op == XOR) {
            finalClosedPolylines.push_back(-a);
            finalClosedPolylines.push_back(-b);
        }
    }
}

void resolveContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op) {
    // Expects negative a contained in positive b.
    
    GLdouble cutx = a[0].x;
    
    std::vector<int> acuts = a.getIntersectionsIndex(cutx);
    std::vector<int> bcuts = b.getIntersectionsIndex(cutx);
    
    if (acuts.empty() || bcuts.empty()) {
        throw std::runtime_error("resolveContainment(POLYLINE^2, std::vector<POLYLINE>, op): Expected at least two intersections thorugh a because x = a[0]x. Also expected two through b because a is inside b.");
    }
    
    if (acuts.size() % 2 || bcuts.size() % 2) {
        throw std::runtime_error("resolveContainment(POLYLINE^2, std::vector<POLYLINE>, op): Expected an even number of intersections.");
    }
    
    int i = 0;
    
    while (i < bcuts.size() && b[bcuts[i]].y < a[acuts[0]].y) { i++; }
    
    if (i % 2) {
        throw std::runtime_error("resolveContainment(POLYLINE^2, std::vector<POLYLINE>, op): Expected a to be inside b.");
    }
}

void noIntersectionPossibleContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op) {
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
    if (b.contains(a[0])) { // If a is inside b,
        if (aPos && bPos) {
            if          (op == OR) {
                finalClosedPolylines.push_back(b);                      // Return the larger, containing shape
            } else if   (op == AND) {
                finalClosedPolylines.push_back(a);                      // Return the inner shape
            } else if   (op == XOR) {
                a.reverse();
                resolveContainment(a, b, finalClosedPolylines, AND);    // Remove the inner shape
            }
        } else if (aPos) {
            if (op == AND) {
                a.reverse();
                b.reverse();
                resolveContainment(a, b, finalClosedPolylines, AND);    // Remove the inner shape from the negative outer shape.
                finalClosedPolylines[0].reverse();
            } else if (op == OR || op == XOR) {
                a.reverse();
                b.reverse();
                resolveContainment(a, b, finalClosedPolylines, AND);    // Remove the positive inner shape from the negative outer shape.
                finalClosedPolylines[0].reverse();
            }
        } else if (bPos) {
            if (op == AND) {
                resolveContainment(a, b, finalClosedPolylines, AND);    // Remove the inner shape.
            } else if (op == OR) {
                // This boolean evaluates to all space.
            } else if (op == XOR) {
                resolveContainment(a, b, finalClosedPolylines, AND);    // Remove the inner shape from the outer shape and negate.
                finalClosedPolylines[0].reverse();
            }
        } else {
            if (op == AND) {
                finalClosedPolylines.push_back(b);                      // Return the larger negative.
            } else if (op == OR) {
                finalClosedPolylines.push_back(b);                      // Return the smaller negative.
            } else if (op == XOR) {
                b.reverse();
                resolveContainment(a, b, finalClosedPolylines, AND);    // Return the area between larger and smaller.
            }
        }
    } else {                // Otherwise,
        noIntersectionNoContainmentLogic(a, b, finalClosedPolylines, op);
    }
}

std::vector<VECTOR> intersections(POLYLINE a, POLYLINE b) {
    return intersections(splitPolylines(a, b, OR));
}
std::vector<VECTOR> intersections(std::map<VECTOR, std::vector<POLYLINE*>> m) {
    std::vector<VECTOR> toReturn;
    
    for (std::map<VECTOR, std::vector<POLYLINE*>>::iterator it = m.begin(); it != m.end(); ++it) {
        toReturn.push_back(it->first);
        printf("key! "); it->first.printNL();
    }
    
    return toReturn;
}

std::vector<SEGMENT> getMonotonicInsideSegmentsFromPolyline(POLYLINE p, BOUNDINGBOX interesting) {
    std::vector<SEGMENT> segments;
    
    if (interesting.initialized) {
        bool inside = interesting.doesContain(p[0]);
        bool forward = p[1].x - p[0].x > 0;
        
        SEGMENT current;
        
        if (inside) {
            current.b = 0;
            current.bb = BOUNDINGBOX(p[0]);
            current.forward = forward;
        }
        
        for (int i = 1; i < p.size(); i++) {
            if (inside) { current.bb.enlarge(p[i]); }
            
            if (inside != interesting.doesContain(p[i])) {  // If we cross the boundary of the interesting bounding box...
                if (inside) {                   // If we are going outside...
                    current.e = i;
                    segments.push_back(current);
                } else {                        // If we are coming inside...
                    current.b = i-1;
                    current.bb = BOUNDINGBOX(p[i-1], p[i]);
                    forward = p[i].x - p[i-1].x;
                    current.forward = forward;
                }
                
                inside = !inside;
            } else if (inside) {
                if (forward != (p[i].x - p[i-1].x > 0)) {   // If we switch direction...
                    current.e = i-1;
                    segments.push_back(current);
                    
                    current.b = i-1;
                    current.bb = BOUNDINGBOX(p[i-1], p[i]);
                    forward = !forward;
                    current.forward = forward;
                }
            }
        }
        
        if (inside) {
            current.e = (int)p.size()-1;
            segments.push_back(current);
        }
        
        std::sort(segments.begin(), segments.end());    // Insert the segments into the correct positions initially?
    }
//    else {
//        printf("Not interesting!");
//    }
    
    return segments;
}




