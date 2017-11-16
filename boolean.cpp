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
    
//    printf("\nIntersecting the segment ");  a1.print(); printf(" -> "); a2.print();
//    printf(" with the segment ");           b1.print(); printf(" -> "); b2.print(); printf(" ...\n");
    
//    printf("1 ... ");
    
    if ( ( max(a1.x, a2.x) < min(b1.x, b2.x) || max(b1.x, b2.x) < min(a1.x, a2.x) ) ||
        ( max(a1.y, a2.y) < min(b1.y, b2.y) || max(b1.y, b2.y) < min(a1.y, a2.y) ) ) {   // If the bounding boxes do not intersect...
        return false;
    }
    
//    printf("2 ... ");
    
    if ((a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1)) { // If the lines are equal,
        *i = nullptr;   // Then, there are infintely many points of intersection.
        return true;    // But we still report that they intersect.
    }
    
//    printf("3 ... ");
    
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
    
//    printf("4 ... ");
    
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
    
//    printf("5 ... ");
    
    // Lastly, calculate intersection of y = aslope*(x-a1.x) + a1.y and y = bslope*(x-b1.x) + b1.y.
    GLdouble x = (a1.x*aslope - b1.x*bslope - a1.y + b1.y)/(aslope - bslope);   // Check this;
    
    if ( (x >= min(a1.x, a2.x) && x <= max(a1.x, a2.x)) && (x >= min(b1.x, b2.x) && x <= max(b1.x, b2.x)) ) {
        *i = new VECTOR(x, aslope*(x-a1.x) + a1.y);
        return true;
    }
    
//    printf("6 ... ");
    
    return false;
}

std::vector<POLYLINE> booleanOp(POLYLINE a, POLYLINE b, BOOLOPERATION op) {
    std::vector<POLYLINE> finalClosedPolylines;
    std::map<VECTOR, std::vector<POLYLINE*>> ssp;
    
    if (std::abs(a.area()) < EPSILON || std::abs(b.area()) < EPSILON) {               // If one of the shapes is empty...
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
    
    a.bb.print();
    b.bb.print();
    interesting.print();
    
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
                        
//                        printf("AREA!: %f", finalPath.area());
                        
                        if (std::abs(finalPath.area()) > EPSILON) {
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
            
            //            free(*it);      // Free all of the temporary polylines.
        }
    }
    
//    printf("\nThere are %i resulting polylines:\n\n", (int)finalClosedPolylines.size());
//    for (int i = 0; i < finalClosedPolylines.size(); i++) {
//        finalClosedPolylines[i].print();
//    }
//    
//    printf("Finished printing...\n\n");
    
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
                    
//                    printf("\n\nIntersecting a[%i] -> a[%i], b[%i] -> b[%i] ... ", i, i+1, j, j+1);
                    
                    if ( intersect(a[i], a[i+1], b[j], b[j+1], &intersection) ) {   // If they intersect...
                        if (intersection) {                                         // ...and there are not infintely-many intersections (if there are infintely-many intersections, null is returned).
                            intersection->printNL();
                            
                            if      (*intersection == a[i]) {   acuts.push_back(i); }
                            else if (*intersection == a[i+1]) { acuts.push_back(i+1); }
                            else {
//                                a[i].printNL();
//                                a[i+1].printNL();
                                
                                a.insert(i+1, *intersection);
                                for (int k = 0; k < acuts.size(); k++) { if (acuts[k] > i) { acuts[k]++; } }
                                acuts.push_back(i+1);
                            }
                            
                            if      (*intersection == b[j]) {   bcuts.push_back(j); }
                            else if (*intersection == b[j+1]) { bcuts.push_back(j+1); }
                            else {
//                                b[j].printNL();
//                                b[j+1].printNL();
                                
                                b.insert(j+1, *intersection);
                                for (int k = 0; k < bcuts.size(); k++) { if (bcuts[k] > j) { bcuts[k]++; } }
                                bcuts.push_back(j+1);
                            }
                            
//                            a.print();
//                            b.print();
                            
                            delete intersection;
                            intersection = nullptr;
                        }
                    }
                }
            }
        }
    } else {        // New, probably O(n) algorithm
                    // Find the segments in a...
        a += a[0];  // Duplicate the first point for simplicity
        b += b[0];
        
        std::vector<SEGMENT> asegments = getMonotonicInsideSegmentsFromPolyline(a, interesting);
        std::vector<SEGMENT> bsegments = getMonotonicInsideSegmentsFromPolyline(b, interesting);
        
        printf("There are %lu asegments...\n", asegments.size());
        printf("There are %lu bsegments...\n", bsegments.size());
        
        for (int i = 0; i < asegments.size(); i++) {
            printf("asegments from %i to %i with bounding box\n", asegments[i].b, asegments[i].e);
//            asegments[i].bb.print();
        }
        
        for (int i = 0; i < bsegments.size(); i++) {
            printf("bsegments from %i to %i with bounding box\n", bsegments[i].b, bsegments[i].e);
//            bsegments[i].bb.print();
        }
        
        for (int i = 0; i < asegments.size(); i++) {
            for (int j = 0; j < bsegments.size(); j++) {
                if (asegments[i].bb.doesIntersect(bsegments[j].bb)) {   // If the bounding boxes intersect...
                    BOUNDINGBOX superInteresting = asegments[i].bb & bsegments[j].bb;
                    
//                    printf("doesIntersect!\n");
//                    superInteresting.print();
                    
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
                    
//                    printf("ai: %i\n", ai);
//                    printf("bi: %i\n", bi);
//                    
//                    printf("ae: %i\n", aend);
//                    printf("be: %i\n", bend);
//                    
//                    printf("ax: %f\n", a[ai-adir].x);
//                    printf("bx: %f\n", b[bi-bdir].x);
//                    
//                    printf("ax2: %f\n", a[ai].x);
//                    printf("bx2: %f\n", b[bi].x);
//                    
//                    printf("adir: %i\n", adir);
//                    printf("bdir: %i\n", bdir);
//                    
//                    printf("sx: %f\n", superInteresting.ur.x);
                    
                    // (a[ai].x <= superInteresting.ur.x || b[bi].x <= superInteresting.ur.x)
                    while ( (a[ai-adir].x <= superInteresting.ur.x && b[bi-bdir].x <= superInteresting.ur.x) && !(ai == aend && bi == bend)) {
                        //                    while ( (a[ai-adir].x <= superInteresting.ur.x && b[bi-bdir].x <= superInteresting.ur.x) && !(ai == aend && bi == bend)) {
                        
//                        printf("Checking intersection between the asegment from "); a[ai-adir].print();
//                        printf(" to "); a[ai].print();
//                        printf(" and the bsegment from "); b[bi-bdir].print();
//                        printf(" to "); b[bi].printNL();
                        
                        if ( intersect(a[ai-adir], a[ai], b[bi-bdir], b[bi], &intersection) ) {   // If they intersect... (note this line can be improved)
//                            printf("intersection: 0x%i\n", intersection);
                            
                            if (intersection) {                                         // ...and there are not infintely-many intersections (if there are infintely-many intersections, null is returned).
//                                printf("Line intersection at ");
//                                intersection->printNL();
                                
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
    
//    printf("\n\n");
//    
//    for (int i = 0; i < acuts.size(); i++) {
//        printf("acuts [%i] = %i\n", i, acuts[i]);
//    }
    
//    a.print();
    
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
            l += a.size() + 1;  // Not sure about the +1...
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
                        sortedSplitPolylines[toAdd1->getEndPoint()  ].push_back(toAdd1);
                    } //else { printf("Not inside...\n"); }
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




