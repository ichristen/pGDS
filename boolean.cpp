#include "boolean.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SEGMENT::operator<(const SEGMENT& s) const {
    return bb.ll.x < s.bb.ll.x;
}
bool SEGMENT::operator>(const SEGMENT& s) const {
    return bb.ll.x > s.bb.ll.x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool intersect(VECTOR a1, VECTOR a2, VECTOR b1, VECTOR b2, VECTOR** i) {
    if (a1 == a2 || b1 == b2) {     // Don't consider points...
        return false;
    }
    
    if ( ( max(a1.x, a2.x) < min(b1.x, b2.x) || max(b1.x, b2.x) < min(a1.x, a2.x) ) ||
         ( max(a1.y, a2.y) < min(b1.y, b2.y) || max(b1.y, b2.y) < min(a1.y, a2.y) ) ) {   // If the bounding boxes do not intersect...
        return false;
    }
    
    if ((a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1)) { // If the lines are equal,
        *i = nullptr;   // Then, there are infintely many points of intersection.
        return true;    // But we still report that they intersect.
    }
    
    bool avert = a1.x < a2.x + ERROR && a1.x > a2.x - ERROR;
    bool bvert = b1.x < b2.x + ERROR && b1.x > b2.x - ERROR;
    
    if (avert && bvert) {   // If the lines are both vertical,
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
    } else if (avert) {     // If only a is vertical
        if (a1.x <= max(b1.x, b2.x) && a1.x >= min(b1.x, b2.x)) {
            GLdouble y = (a1.x-b1.x)*(b2.y - b1.y)/(b2.x - b1.x) + b1.y;
            
            if (y <= max(a1.y, a2.y) && y >= min(a1.y, a2.y)) {
                *i = new VECTOR(a1.x, y);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if (bvert) {     // If only b is vertical
        if (b1.x <= max(a1.x, a2.x) && b1.x >= min(a1.x, a2.x)) {
            GLdouble y = (b1.x-a1.x)*(a2.y - a1.y)/(a2.x - a1.x) + a1.y;
            
            if (y <= max(b1.y, b2.y) && y >= min(b1.y, b2.y)) {
                *i = new VECTOR(b1.x, y);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    
    GLdouble aslope = (a2.y - a1.y)/(a2.x - a1.x);
    GLdouble bslope = (b2.y - b1.y)/(b2.x - b1.x);
    
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
        return true;    // Otherwise, there are infintely many points of intersection, because the bounding boxes connect.
    }
    
    // Lastly, calculate intersection of y = aslope*(x-a1.x) + a1.y and y = bslope*(x-b1.x) + b1.y.
    GLdouble x = (a1.x*aslope - b1.x*bslope - a1.y + b1.y)/(aslope - bslope);   // Check this;
    
    if ( (x >= min(a1.x, a2.x) - ERROR && x <= max(a1.x, a2.x) + ERROR) &&
         (x >= min(b1.x, b2.x) - ERROR && x <= max(b1.x, b2.x) + ERROR) ) {
        *i = new VECTOR(x, aslope*(x-a1.x) + a1.y);
        return true;
    }
    
    return false;
}

std::vector<POLYLINE> booleanOp(POLYLINE a, POLYLINE b, BOOLOPERATION op) {
    std::vector<POLYLINE> finalClosedPolylines;
    std::map<VECTOR, std::vector<POLYLINE*>> ssp;
    
    if (std::abs(a.area()) < ERROR || std::abs(b.area()) < ERROR) { // If one of the shapes is empty...
        switch (op) {
            case OR:
            case XOR:
                if          (std::abs(a.area()) < ERROR) {          // Add the shape with area...
                    finalClosedPolylines.push_back(b);
                } else if   (std::abs(b.area()) < ERROR) {
                    finalClosedPolylines.push_back(a);
                }
                
            case AND:
                return finalClosedPolylines;                        // Return finalClosedPolylines, empty or with the shape with area (if there is one), depending upon op.
        }
    }
    
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
    BOUNDINGBOX interesting = a.bb & b.bb;
    
#ifdef BOOLEAN_DEBUG
    a.bb.print();
    b.bb.print();
    interesting.print();
#endif
    
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
        
        if (false) {    // Disabled temporarily...
            if          (aSmaller && b.bb.doesContain(a.bb)) {  // If a might be inside b
                noIntersectionPossibleContainment(a, b, finalClosedPolylines, op);
            } else if  (!aSmaller && a.bb.doesContain(b.bb)) {  // If b might be inside a
                noIntersectionPossibleContainment(b, a, finalClosedPolylines, op);
            } else {                                            // No possible intersection
                noIntersectionNoContainmentLogic(a, b, finalClosedPolylines, op);
                                                                // Don't fill finalClosedPolylines
            }
        } else {
            if      (a.area() < 0) { finalClosedPolylines.push_back(b); }
            else if (b.area() < 0) { finalClosedPolylines.push_back(a); }
            else {
                finalClosedPolylines.push_back(a);
                finalClosedPolylines.push_back(b);
            }
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
        
#ifdef BOOLEAN_DEBUG
        printf("\n\n");
#endif
        
        for (int i = 0; i < keys.size(); i++) {
            VECTOR firstIntersection = keys[i];
            VECTOR currentIntersection;
            
#ifdef BOOLEAN_DEBUG
            printf("{ %i } - ", i);    firstIntersection.printNL(); printf("\n");
#endif
            
            for (int j = 0; j < ssp[firstIntersection].size(); j++) {
#ifdef BOOLEAN_DEBUG
                printf("{ %i, %i } - 0x%li - ", i, j, (long)ssp[firstIntersection][j]); firstIntersection.printNL();
#endif
                
                if (ssp[firstIntersection][j]) {                            // If this POLYLINE* is not null, then follow the path...
                    POLYLINE* firstPath = ssp[firstIntersection][j];
                    
                    if (firstPath->getBeginPoint() == firstIntersection) {  // If the first point in our path is this intersection...
                        POLYLINE finalPath;                                 // ...Then let's follow this path.
                        
                        finalPath.setLayer(firstPath->layer);
                        
                        currentIntersection = firstPath->getEndPoint();
                        
                        finalPath += *firstPath;
                        
                        ssp[firstIntersection][j] = nullptr;                // firstPath has been 'used' and is removed.
                        
                        POLYLINE* currentPath = firstPath;
                        
                        while (currentIntersection != firstIntersection) {
                            std::vector<POLYLINE*> nextPaths = ssp[currentIntersection];
                            VECTOR in = (currentPath->operator[]((int)currentPath->size()-1) - currentPath->operator[]((int)currentPath->size()-2)).unit();
                            
                            int mostLeftk = -1;
                            GLdouble mostLeftAngle = TAU;
                            
#ifdef BOOLEAN_DEBUG
                            printf("currentPath:\n");
                            currentPath->print();
#endif
                            
                            for (int k = 0; k < nextPaths.size(); k++) {
#ifdef BOOLEAN_DEBUG
                                printf("nextPaths[%i] = 0x%li:\n", k, (long)nextPaths[k]);
                                
                                if (nextPaths[k]) {
                                    nextPaths[k]->print();
                                    printf("nextPaths[k]->getBeginPoint(): "); nextPaths[k]->getBeginPoint().printNL();
                                    printf("currentIntersection: "); currentIntersection.printNL();
                                }
#endif
                                
                                if (nextPaths[k] && nextPaths[k]->getBeginPoint() == currentIntersection) {
                                    VECTOR out = (nextPaths[k]->operator[](1) - nextPaths[k]->operator[](0)).unit();
                                    
                                    GLdouble angle = acos( -(in * out) );
#ifdef BOOLEAN_DEBUG
                                    printf("nextPaths[k][0]  = "); nextPaths[k]->operator[](0).printNL();
                                    printf("nextPaths[k][1]  = "); nextPaths[k]->operator[](1).printNL();
//                                    printf("nextPaths[k][2]  = "); nextPaths[k]->operator[](2).printNL();
                                    
                                    printf("IN   = "); in.printNL();
                                    printf("OUT  = "); out.printNL();
                                    
                                    printf("ANG: %.20f ", angle);
#endif
                                    if (angle > 0.00001) {       // If the angle doesn't go back on itself...
                                        if (in.perpCCW() * out < 0) {   // If the vector is to the right...
                                            angle = TAU - angle;
                                        }
                                        
#ifdef BOOLEAN_DEBUG
                                        printf("ang = %f\n", angle);
#endif
                                        
                                        if (angle < mostLeftAngle) {
                                            mostLeftAngle = angle;
                                            mostLeftk = k;
                                        }
                                    }
                                }
//                                else {
//                                    printf("Something is wrong...\n");
//                                }
                            }
                            
                            if (mostLeftk >= 0) {   // Choose the leftmost path...
                                currentPath = nextPaths[mostLeftk];
                                
                                finalPath += *currentPath;
#ifdef BOOLEAN_DEBUG
                                currentPath->print();
                                printf("mostLeftk=%i\n\n", mostLeftk);
#endif
                                ssp[currentIntersection][mostLeftk] = nullptr;    // This path has been 'used' and is removed.
                                
                                currentIntersection = currentPath->getEndPoint();
                            } else {
//#ifdef BOOLEAN_DEBUG
                                printf("This should not have happened...");
//#endif
                                currentIntersection = firstIntersection;
                            }
#ifdef BOOLEAN_DEBUG
                            printf("FIRST:   "); firstIntersection.printNL();
                            printf("CURRENT: "); currentIntersection.printNL();
#endif
                        }
                        
                        finalPath.close();
                        finalPath.recomputeBoundingBox();
#ifdef BOOLEAN_DEBUG
                        finalPath.print();
                        
                        printf("AREA!: %f", finalPath.area());
#endif
                        if (std::abs(finalPath.area()) > sqrt(ERROR)) {
                            if (reverseAtEnd) { finalClosedPolylines.push_back(-finalPath); }
                            else {              finalClosedPolylines.push_back(finalPath); }
                        }
                    } else {    // Only traverse forward for now. We might want to eventually consider the intersection of open polylines.
                                //                    nextIntersection = currentPath->getBegin();
                    }
                }
            }
        }
        
        for (std::set<POLYLINE*>::iterator it = values.begin(); it != values.end(); ++it) {
            delete (*it);
        }
    }
    
#ifdef BOOLEAN_DEBUG
    printf("\nThere are %i resulting polylines:\n\n", (int)finalClosedPolylines.size());
    for (int i = 0; i < finalClosedPolylines.size(); i++) {
        finalClosedPolylines[i].print();
    }
    
    printf("Finished printing...\n\n");
#endif
    
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
    
#ifdef BOOLEAN_DEBUG
    printf("\n\na.area() = %f\n", a.area());
    printf("b.area() = %f\n", b.area());

    switch (op) {
        case AND:   printf("OP = AND\n");   break;
        case OR:    printf("OP = OR\n");    break;
        case XOR:   printf("OP = XOR\n");   break;
    }
    a.print();
    b.print();
#endif
    
    if ( !((aPos && bPos) || ((aPos || bPos) && op == AND)) ) {
        throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Only ++ AND, OR, XOR, or -+ AND are acceptable for splitPolylines");
    }
    
    // First, find all intersections...
    std::vector<int> acuts;
    std::vector<int> bcuts;
    
    VECTOR* intersection = nullptr;
    
    BOUNDINGBOX interesting = a.bb & b.bb;
    
    if (/* DISABLES CODE */ (false)) {    // Old, naive O(n^2) algorithm
        if (!interesting.isEmpty()) {
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < b.size(); j++) {
                    
                    if ( intersect(a[i], a[i+1], b[j], b[j+1], &intersection) ) {   // If they intersect...
                        if (intersection) {                                         // ...and there are not infintely-many intersections (if there are infintely-many intersections, null is returned).
                            if      (*intersection == a[i]) {   acuts.push_back(i); }
                            else if (*intersection == a[i+1]) { acuts.push_back(i+1); }
                            else {
                                a.insert(i+1, *intersection);
                                for (int k = 0; k < acuts.size(); k++) { if (acuts[k] > i) { acuts[k]++; } }
                                acuts.push_back(i+1);
                            }
                            
                            if      (*intersection == b[j]) {   bcuts.push_back(j); }
                            else if (*intersection == b[j+1]) { bcuts.push_back(j+1); }
                            else {
                                b.insert(j+1, *intersection);
                                for (int k = 0; k < bcuts.size(); k++) { if (bcuts[k] > j) { bcuts[k]++; } }
                                bcuts.push_back(j+1);
                            }
                            
                            delete intersection;
                            intersection = nullptr;
                        }
                    }
                }
            }
        }
    } else {        // New, probably O(n) algorithm
        
        a += a[0];  // Duplicate the first point for simplicity
        b += b[0];
        
        // Find the segments in a and b...
        std::vector<SEGMENT> asegments = getMonotonicInsideSegmentsFromPolyline(a, interesting);
        std::vector<SEGMENT> bsegments = getMonotonicInsideSegmentsFromPolyline(b, interesting);
        
#ifdef BOOLEAN_DEBUG_INTERSECTION
        printf("\nThere are %lu asegments...\n", asegments.size());
        
        for (int i = 0; i < asegments.size(); i++) {
            printf("asegments from %i to %i with bounding box\n", asegments[i].b, asegments[i].e);
            asegments[i].bb.print();
        }
        
        printf("\nThere are %lu bsegments...\n", bsegments.size());
        for (int i = 0; i < bsegments.size(); i++) {
            printf("bsegments from %i to %i with bounding box\n", bsegments[i].b, bsegments[i].e);
            bsegments[i].bb.print();
        }
#endif
        
        if (asegments.size()*bsegments.size() > 100) {
            printf("POLYLINES::booleanEquals(POLYLINES, BOOLOPERATION): There are asegments.size() * bsegments.size() = %lu * %lu = %lu comparisons to make.\nMaybe it's time to implement Bentley–Ottmann?\n", asegments.size(), bsegments.size(), asegments.size()*bsegments.size());
        }
        
        for (int i = 0; i < asegments.size(); i++) {
            for (int j = 0; j < bsegments.size(); j++) {
#ifdef BOOLEAN_DEBUG_INTERSECTION
                printf("\n\n*Checking intersection between\n**asegments[%i] = %i -> %i, %i:\n", i, asegments[i].b, asegments[i].e, asegments[i].forward);
                
//                a.print();
//                POLYLINE(a, asegments[i].b, asegments[i].e); //.print();
                
                printf("**and bsegments[%i] = %i -> %i, %i:\n", j, bsegments[j].b, bsegments[j].e, bsegments[j].forward);
                
//                b.print();
//                POLYLINE(b, bsegments[j].b, bsegments[j].e); //.print();
#endif
                
                if (asegments[i].bb.doesIntersect(bsegments[j].bb)) {   // If the bounding boxes intersect...
                    BOUNDINGBOX superInteresting = asegments[i].bb & bsegments[j].bb;
                    
#ifdef BOOLEAN_DEBUG_INTERSECTION
                    printf("\ndoesIntersect!\n");
                    superInteresting.print();
#endif
                    
                    int adir =  (asegments[i].forward)?(1):(-1);
                    int abeg =  (asegments[i].forward)?(asegments[i].b):(asegments[i].e);
                    int aend =  (asegments[i].forward)?(asegments[i].e):(asegments[i].b);
                    int ai =    abeg;
                    
                    while (ai != aend && a[ai+adir].x < superInteresting.ll.x) { ai += adir; }
                    
                    if (ai == abeg) { ai += adir; }
                    
                    
                    int bdir =  (bsegments[j].forward)?(1):(-1);
                    int bbeg =  (bsegments[j].forward)?(bsegments[j].b):(bsegments[j].e);
                    int bend =  (bsegments[j].forward)?(bsegments[j].e):(bsegments[j].b);
                    int bi =    bbeg;
                    
                    while (bi != bend && b[bi+bdir].x < superInteresting.ll.x) { bi += bdir; }
                    
                    if (bi == bbeg) { bi += bdir; }
                    
#ifdef BOOLEAN_DEBUG_INTERSECTION
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
#endif
                    
                    while ( (a[ai-adir].x <= superInteresting.ur.x && b[bi-bdir].x <= superInteresting.ur.x) ) {
#ifdef BOOLEAN_DEBUG_INTERSECTION
                        printf("Checking intersection between the aline from "); a[ai-adir].print();
                        printf(" to "); a[ai].print();
                        printf(" and the bline from "); b[bi-bdir].print();
                        printf(" to "); b[bi].printNL();
#endif
                        
                        if ( intersect(a[ai-adir], a[ai], b[bi-bdir], b[bi], &intersection) ) {   // If they intersect... (note this line can be improved)
#ifdef BOOLEAN_DEBUG_INTERSECTION
                            printf("intersection: 0x%li\n", (long)intersection);
#endif
                            
                            if (intersection) {                                         // ...and there are not infintely-many intersections (if there are infintely-many intersections, null is returned).
#ifdef BOOLEAN_DEBUG_INTERSECTION
                                printf("Line intersection at ");
                                intersection->printNL();
#endif
                                
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
                                    
                                    if (asegments[i].forward) {
                                        aend++; ai++;
#ifdef BOOLEAN_DEBUG_INTERSECTION
                                        printf("ai++;\n");
#endif
                                    }
                                    
                                    // Then add the index to the list of cuts.
                                    acuts.push_back(aindex);
                                }
                                
                                if      (*intersection == b[bi-bdir]) { bcuts.push_back(bi-bdir); }
                                else if (*intersection == b[bi]) {      bcuts.push_back(bi); }
                                else {
                                    int bindex = max(bi, bi-bdir);
                                    
                                    // Add the intersection to b if it isn't already there...
                                    b.insert(bindex, *intersection);
                                    
                                    // ...and incriment the indices that will be shifted by its presence.
                                    for (int k = 0; k < bcuts.size(); k++) { if (bcuts[k] >= bindex) { bcuts[k]++; } }
                                    
                                    for (int k = 0; k < bsegments.size(); k++) {
                                        if (bsegments[k].b >= bindex) { bsegments[k].b++; }
                                        if (bsegments[k].e >= bindex) { bsegments[k].e++; }
                                    }
                                    
                                    if (bsegments[j].forward) {
                                        bend++; bi++;
#ifdef BOOLEAN_DEBUG_INTERSECTION
                                        printf("bi++;\n");
#endif
                                    }
                                    
                                    // Then add the index to the list of cuts.
                                    bcuts.push_back(bindex);
                                }
                                
                                //                                free(intersection);     // And free the memory...
                                delete intersection;     // And free the memory...
                            }
                        }
                        
                        if (ai == aend && bi == bend) { break; }
                        
                        if      (a[ai].x < b[bi].x && ai != aend) { ai += adir; }   // If a is lagging...
                        else if (a[ai].x > b[bi].x && bi != bend) { bi += bdir; }   // If b is lagging...
                        else {                                                  // If a and b are tied..
                            if      (ai == aend) { bi += bdir; }  // If a has ended, incriment b...
                            else if (bi == bend) { ai += adir; }  // If b has ended, incriment a...
                            else {
                                ai += adir;
                                bi += bdir; // Incriment them both...
                            }
                        }
                        
#ifdef BOOLEAN_DEBUG_INTERSECTION
                        printf("ai: %i\n", ai);
                        printf("bi: %i\n", bi);
                        
                        printf("ae: %i\n", aend);
                        printf("be: %i\n", bend);
                        
                        printf("ax: %f\n", a[ai-adir].x);
                        printf("bx: %f\n", b[bi-bdir].x);
                        
                        printf("ax2: %f\n", a[ai].x);
                        printf("bx2: %f\n", b[bi].x);
                        
                        printf("sx: %f\n", superInteresting.ur.x);
#endif
                    }
                }
            }
        }
    }
    
    cutPolyline(a, b, acuts, op, sortedSplitPolylines);
    cutPolyline(b, a, bcuts, op, sortedSplitPolylines);
    
    return sortedSplitPolylines;
}
void cutPolyline(POLYLINE& a, POLYLINE& b, std::vector<int> acuts, BOOLOPERATION op, std::map<VECTOR, std::vector<POLYLINE*>>& sortedSplitPolylines) {
    std::sort(acuts.begin(), acuts.end());
    acuts.erase(std::unique(acuts.begin(), acuts.end()), acuts.end());
    
    bool aPos = a.area() > 0;
    bool bPos = b.area() > 0;
    
#ifdef BOOLEAN_DEBUG
    printf("\n\n");
    
    printf("a.area() = %f\n", a.area());
    printf("b.area() = %f\n", b.area());
    
    for (int i = 0; i < acuts.size(); i++) {
        printf("acuts [%i] = %i\n", i, acuts[i]);
    }
    
    a.print();
    b.print();
#endif
    
    // Second, cleave the polylines into peices, based on the points of intersection.
    
    for (int i = 0; i < acuts.size(); i++) {
        
        int x1 = acuts[i];
        int x2 = acuts[ (i+1) % acuts.size() ];
        
#ifdef BOOLEAN_DEBUG
        printf("i1 = %i\n", i);
        printf("i2 = %lu\n", (i+1) % acuts.size());
        
        printf("x1 = %i\n", x1);
        printf("x2 = %i\n", x2);
#endif
        
//        while (x1 < x2 && a[x1] == a[x1 + 1]) { x1++; }
//        while (x1 < x2 && a[x2] == a[x2 - 1]) { x2--; }
        
        int l = x2-x1+1;
        
        if (l <= 0) {
            l += a.size();  // Not sure about the +1...
        }
        
        if (l < 2 || (l == 2 && a[x1] == a[x2])) {
            // Empty polyline
        } else {
            VECTOR testPoint;
        
#ifdef BOOLEAN_DEBUG
            printf("l = %i\n", l);
#endif
        
            if (l > 2) {
                testPoint = a[x1+1]; // (*toAdd)[1];
            } else if (l == 2) {
                testPoint = (a[x1] + a[x2]) / 2;
            }
//            else {
//                throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Did not expect a polyline with less than two points...");
//            }
            
#ifdef BOOLEAN_DEBUG
            printf("\n");
            
            testPoint.printNL();
            
            printf("b.contains(testPoint) = %i\n",          b.contains(testPoint));
            printf("!b.boundaryContains(testPoint) = %i\n", !b.boundaryContains(testPoint));
#endif
        
            bool isInside =     b.contains(testPoint);
            bool isOnBoundary = b.boundaryContains(testPoint);
            
//            printf("INSIDE = %i\n\n", isInside);
        
            POLYLINE* toAdd1;
            POLYLINE* toAdd2;
        
    //        a.print();
        
            if (aPos && bPos) {
                
                switch (op) {
                    case AND:   // Adds reversed segment only if inside other.
                        if (isInside) {
    //                        printf("Inside...\n");
                            
                            toAdd1 = new POLYLINE(a, x1, x2);
                            
    //                        printf("0x%i\n", toAdd1);
    //
    //                        //                        toAdd1->reverse();
    //
    //                        printf("BEGIN - "); toAdd1->getBeginPoint().printNL();
    //                        printf("END   - "); toAdd1->getEndPoint().printNL();
                            
                            sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
//                            sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                        } //else { printf("Not inside...\n"); }
                        break;
                        
                    case OR:    // Adds all segments, adds an additional reversed segment if inside other.
                        toAdd1 = new POLYLINE(a, x1, x2);
                        
                        sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
//                        sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                        
                        if (isInside) {
                            toAdd2 = new POLYLINE(a, x1, x2);
                            
                            toAdd2->reverse();
                            
                            sortedSplitPolylines[toAdd2->getBeginPoint()].push_back(toAdd2);
//                            sortedSplitPolylines[toAdd2->getEndPoint()  ].push_back(toAdd2);
                        }
                        
                        break;
                        
                    case XOR:   // Adds all segments, reverses if inside other.
                        toAdd1 = new POLYLINE(a, x1, x2);
                        
                        if (isInside) { toAdd1->reverse(); }
                        
                        sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
//                        sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                        
                        break;
                }
                
            } else if (aPos) {  // Adds segment if not inside other.
                if (!isInside && !isOnBoundary) {
                    toAdd1 = new POLYLINE(a, x1, x2);
                    
#ifdef BOOLEAN_DEBUG
                    printf("aPos Key!: "); toAdd1->getBeginPoint().printNL();
#endif
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
//                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                }
            } else if (bPos) {  // Adds segment if inside other.
                if (isInside && !isOnBoundary) {
                    toAdd1 = new POLYLINE(a, x1, x2);
                    
#ifdef BOOLEAN_DEBUG
                    printf("bPos Key!: "); toAdd1->getBeginPoint().printNL();
#endif
                    
                    sortedSplitPolylines[toAdd1->getBeginPoint()].push_back(toAdd1);
//                    sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                }
            } else {
                throw std::runtime_error("splitPolylines(POLYLINE^2, OP): Previous check should have accounded for all negative case...");
            }
        }
    }
}

void noIntersectionNoContainmentLogic(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op) {
    printf("noIntersectionNoContainmentLogic\n");
    
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
    printf("resolveContainment\n");
    // Expects negative a contained in positive b.
    
    GLdouble cutx = a[0].x;
    
    std::vector<int> acuts = a.getIntersectionsIndex(cutx);
    std::vector<int> bcuts = b.getIntersectionsIndex(cutx);
    
    if (acuts.empty() || bcuts.empty()) {
        throw std::runtime_error("resolveContainment(POLYLINE^2, std::vector<POLYLINE>, op): Expected at least one intersection through a because x = a[0].x. Also expect at least two through b because a is inside b.");
    }
    
    if (acuts.size() % 2 || bcuts.size() % 2) {
        throw std::runtime_error("resolveContainment(POLYLINE^2, std::vector<POLYLINE>, op): Expected an even number of intersections.");
    }
    
    int i = 0;
    
    while (i < bcuts.size() && b[bcuts[i]].y < a[acuts[0]].y) { i++; }
    
    if (i % 2) {
        throw std::runtime_error("resolveContainment(POLYLINE^2, std::vector<POLYLINE>, op): Expected a to be inside b.");
    }
    
    throw std::runtime_error("Unfinished!");
}

void noIntersectionPossibleContainment(POLYLINE& a, POLYLINE& b, std::vector<POLYLINE>& finalClosedPolylines, BOOLOPERATION op) {
    printf("noIntersectionPossibleContainment\n");
    
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
#ifdef BOOLEAN_DEBUG
        printf("*key "); it->first.print(); printf(" with %i polylines:\n", m[it->first].size());
        for (int i = 0; i < m[it->first].size(); i++) {
            printf("**polyline[%i] = 0x%X:\n", i, m[it->first][i]);
            m[it->first][i]->print();
        }
#endif
    }
    
    return toReturn;
}

std::vector<SEGMENT> getMonotonicInsideSegmentsFromPolyline(POLYLINE p, BOUNDINGBOX interesting) {
    std::vector<SEGMENT> segments;
    
#ifdef BOOLEAN_DEBUG
    printf("\ngetMonotonicInsideSegmentsFromPolyline!\n");
    
    p.print();
    interesting.print();
#endif
    
    if (interesting.isInitialized()) {
#ifdef BOOLEAN_DEBUG
        p[0].printNL();
#endif
        
        bool inside = interesting.doesContain(p[0]);
        bool forward = p[1].x - p[0].x > 0;
        
        SEGMENT current;
        
        if (inside) {
#ifdef BOOLEAN_DEBUG
            printf("First point inside!\n");
#endif
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
                    forward = p[i].x - p[i-1].x > 0;
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
            } else if (!inside) {  // Lastly, check for grazing...
                current.bb = BOUNDINGBOX(p[i-1], p[i]);
                
                if (interesting.doesIntersect(current.bb)) {
                    current.b = i-1;
                    current.e = i;
                    current.forward = (p[i].x - p[i-1].x > 0);
                    segments.push_back(current);
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




