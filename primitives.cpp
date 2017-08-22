#include "primitives.hpp"

POLYLINE rect(VECTOR u, VECTOR v) {
    POLYLINE toReturn = POLYLINE(4);
    toReturn.isClosed = true;
    
//    u.printNL();
//    v.printNL();
    
    if (!u.inLine(v)) {
        toReturn.add(u);
        toReturn.add(VECTOR(v.x, u.y));
        toReturn.add(v);
        toReturn.add(VECTOR(u.x, v.y));
        
        if (toReturn.area() < 0) {
            toReturn.reverse();
        }
    } else {
        printf("primitives.hpp::rect(u,v): points defining rectangle are inline! Empty rectangle returned.\n");  // Make this more official.
    }
    
    return toReturn;
}
POLYLINE rect(GLdouble x, GLdouble y, GLdouble w, GLdouble h) {
    return rect(VECTOR(x,y), VECTOR(x+w, y+h));
}

POLYLINE arc(GLdouble r, GLdouble t1_, GLdouble t2_, bool CCW) {
    GLdouble t1 = fmod(t1_, TAU);   // What happens in t1 or t2 are negative?
    GLdouble t2 = fmod(t2_, TAU);
    
    GLdouble dt = fmod(t2 - t1, TAU);
    
    if (dt < 0) {
        dt += TAU;
    }
    
//    printf("t1: %f, t2: %f, dt: %f\n", t1, t2, dt);
    
    if (t1 == t2) {
        throw std::runtime_error("Use circle() instead of arc() for arcs returning to the same angle.");
    }
    
    int steps = ceil(dt/acos(1 - EPSILON/r));
    
    AFFINE m = AFFINE(dt/steps);
    
//    m.print();
    
    VECTOR s = VECTOR(r, t1, true);
    VECTOR e = VECTOR(r, t2, true);
    
    POLYLINE toReturn;
    toReturn.isClosed = false;
    
    toReturn.add(s);
    for (int i = 0; i < steps-1; i++) {
        s = m*s;
        toReturn.add(s);
    }
    toReturn.add(e);
    
//    toReturn.begin =    s.perpCCW().unit();
//    toReturn.end =      e.perpCCW().unit();
    
    return toReturn;
}

POLYLINE parametric(std::function<VECTOR(GLdouble t)> lambda, size_t steps) {
    if (steps < 2 || steps > 200) {
        steps = 200;    // Placeholder; this should estimate the
    }
    
//    printf("Steps: %zu", steps);
    
    POLYLINE toReturn(steps);
    
    toReturn.isClosed = false;
    
    for (int i = 0; i < steps-1; i++) {
        toReturn.add(lambda( ((GLdouble)i)/((GLdouble)(steps-1)) ));
    }
    toReturn.add(lambda(1));
    
    return toReturn;
}
POLYLINE parametricCartesian(   std::function<GLdouble(GLdouble t)> lambdaX,
                                std::function<GLdouble(GLdouble t)> lambdaY,
                                size_t steps) {
    return parametric([lambdaX, lambdaY] (GLdouble t) -> VECTOR { return VECTOR(lambdaX(t), lambdaY(t)); }	, steps);
}
POLYLINE parametricCylindrical( std::function<GLdouble(GLdouble t)> lambdaR,
                                std::function<GLdouble(GLdouble t)> lambdaT,
                                size_t steps) {
    return parametric([lambdaR, lambdaT] (GLdouble t) -> VECTOR { return VECTOR(lambdaR(t), lambdaT(t), true); }	, steps);
}

POLYLINE linear(VECTOR p0, VECTOR p1, size_t steps) {
    VECTOR dp = p1 - p0;
    GLdouble magn = dp.magn();
    
    if (steps == 0) {
        steps = std::ceil(magn);
    }
    if (steps < 2) {
        steps = 2;
    }
    
    dp /= (steps-1);
    
    POLYLINE p = POLYLINE();
    VECTOR x = p0;
    
    p += p0;
    
    for (int i = 1; i < steps-1; i++) {
        x += dp;
        p += x;
    }
    
    p += p1;
    
    return p;
}
POLYLINE qBezier(VECTOR p0, VECTOR p1, VECTOR p2, size_t steps) {
    if (steps == 0) {
        VECTOR dp1 = p1 - p0;
        VECTOR dp2 = p2 - p1;
        
        GLdouble dot = dp1.unit() * dp2.unit();
        
        if (dot == -1) { // Use epsilon?
            printf("qBezier(VECTOR^3): Control points should not be colinear.");
        }
            
        steps = ceil((dp1.magn() + dp2.magn()) / (dot + 1));
    }
//    printf("steps: %i", steps);
    
//    printf("qBezier: p0=");
//    p0.print();
//    printf(" p1=");
//    p1.print();
//    printf(" p2=");
//    p2.print();
//    printf("\n");
    
    return parametric([p0,p1,p2] (GLdouble t) -> VECTOR {
        return p0*((1.0-t)*(1.0-t)) + p1*(2.*(1.0-t)*t) + p2*(t*t);
    }, steps);
}
POLYLINE cBezier(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, size_t steps) {
    if (steps == 0) {
        VECTOR dp1 = p1 - p0;
        VECTOR dp2 = p2 - p1;
        VECTOR dp3 = p3 - p2;
        
        GLdouble dot1 = dp1.unit() * dp2.unit();
        GLdouble dot2 = dp2.unit() * dp3.unit();
        
        if (dot1 == -1 || dot2 == -1) { // Use epsilon?
            printf("cBezier(VECTOR^4): Control points should not be colinear.");
        }
        
        steps = ceil(2*(dp1.magn() + dp2.magn()) / (dot1 + dot2 + 2));
    }
    
    return parametric([p0,p1,p2,p3] (GLdouble t) -> VECTOR {
        return p0*((1.0-t)*(1.0-t)*(1.0-t)) + p1*(3*(1.0-t)*(1.0-t)*t) + p2*(3*(1.0-t)*t*t) + p3*(t*t*t);
    }, steps);
}

GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest) {
//    printf("\tr1=%f\n", (b - c).magn2());
//    printf("\tr2=%f\n", (e - c).magn2());
    
    if ((b - c).magn2() - (e - c).magn2() > 1e-9) { // 100*E) {
        printf("getArcAngle(VECTOR^3, bool): Can't make an arc with two radii...\n");
        return 1e22;  // Arbitrary!
//        throw std::runtime_error("getArcAngle(VECTOR^3, bool): Can't make an arc with two radii...");
    }
    
    if ((e - b).magn2() < 1e-9) {
        printf("getArcAngle(VECTOR^3, bool): Start and end are at the same position\n");
        return 0;
//        throw std::runtime_error("getArcAngle(VECTOR^3, bool): Start and end are at the same position");
    }
    
    GLdouble dir = (b - c).cross(e - c);
    GLdouble ang;
    
    if (dir == 0) {
//        dir = 1;
        ang = -PI;
        printf("Arcs are equidistant, choosing CCW as shortest...\n");
    } else {
        ang = dir*acos(((b - c).unit()) * ((e - c).unit()));
    }
    
    if (!chooseShortest) {
        if (ang > 0) {  ang -= TAU; }
        else {          ang += TAU; }
    }
    
    return ang;
}

POLYLINE arc(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest) {
    GLdouble ang = getArcAngle(c, b, e, chooseShortest);
    GLdouble r = (b - c).magn();
    
//    printf("ang: %f\n", ang);
    
    int steps = ceil(std::abs(ang)/acos(1 - EPSILON/r));
    
//    printf("steps: %i\n", steps);
    
    AFFINE m = AFFINE(ang/steps);
    
    //    m.print();
    
    POLYLINE toReturn;
    toReturn.isClosed = false;
    
    if (ang == 0) {
        return toReturn;
    }
    
    VECTOR v = b - c;
    
    toReturn.add(b);
    for (int i = 0; i < steps-1; i++) {
        v = m*v;
        toReturn.add(v + c);
    }
    toReturn.add(e);
    
    toReturn.begin =    (b-c).perpCCW().unit()*sign(ang);
    toReturn.end =      (e-c).perpCCW().unit()*sign(ang);
    
    return toReturn;
}

void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep) {
    std::function<GLdouble(GLdouble t)> lambda;
    
    if ( (b.w > 0 && e.w < 0) || (b.w < 0 && e.w > 0) ) {
        return;
    }
    
    bool outer = b.w + e.w < 0;
    
    if (b.w != e.w) {
        GLdouble w1 = std::abs(b.w);
        GLdouble w2 = std::abs(e.w);
        
        lambda = [w1,w2] (GLdouble t) -> GLdouble { return ((w2 - w1)*(3*t*t - 2*t*t*t) + w1); };
    } else {
        GLdouble w = std::abs(b.w);
        
        lambda = [w] (GLdouble t) -> GLdouble { return w; };
    }
    
    POLYLINE p = connect(b, e, type, b.w != e.w);
    
//    printf("p.begin="); p.begin.printNL();
//    printf("p.end=");   p.end.printNL();
    
    std::vector<POLYLINE> polylines;
    
    // Currently bugged...
//    if (p.size() > 100) { // GDS prefers polylines with points < 200...
//        int numpeices = ceil(p.size()/100)+1;
//        
//        int lengthpeice = ceil(p.size()/numpeices); // Check this...
//        
//        int x = 0;
//        
//        for (int i = 0; i < numpeices-1; i++) {
//            polylines.push_back(POLYLINE(p, x, x + lengthpeice));
//            
//            x += lengthpeice;
//        }
//        
//        polylines.push_back(POLYLINE(p, x, p.size()-1));
//    } else {
        polylines.push_back(p);
//    }
    
    for (int j = 0; j < polylines.size(); j++) {
        for (int i = (outer)?(-1):(0); i < 2; i += 2) {
            addto->add(thicken(polylines[j], lambda, i, minstep));
        }
    }
}

POLYLINE connect(CONNECTION i, CONNECTION f, CONNECTIONTYPE type, bool pointsDuringLinear) {
//    if (b.type != e.type) {
//        
//    }
    POLYLINE toReturn;
    
    toReturn.begin = i.dv;
    toReturn.end = -f.dv;
    
//    printf("toReturn.begin=");  toReturn.begin.printNL();
//    printf("toReturn.end=");    toReturn.end.printNL();
    
//    printf("FISH");
//    i.dv.print();
//    f.dv.print();
//    
//    i.print();
//    f.print();
    
//    printf("(i.dv + f.dv).magn2() = %f\n",      (i.dv + f.dv).magn2());
//    printf("(f.v - i.v).unit() * i.dv = %f\n",  ((f.v - i.v).unit() * i.dv));
    
    if ((i.dv + f.dv).magn2() < 1e-6 && 1 - (f.v - i.v).unit() * i.dv < 1e-6) {
//        printf("HERE2");
        
        // Connections are inline; return linear.
        if (pointsDuringLinear) {
            toReturn += linear(i.v, f.v, 0);
        } else {
            toReturn += i.v;
            toReturn += f.v;
        }
        
        toReturn.begin =    i.dv;
        toReturn.end =      -f.dv;
        
        return toReturn;
    }
    
    switch (type) {
        case QBEZIER:
        case CBEZIER:
        case LINEAR:
        case MONOCIRCULAR:
            throw std::runtime_error("CONNECTIONTYPE not currently supported...");
            
            break;
        case CIRCULAR:
            // First find the radius of the two circles (math that I won't explain):
            
            VECTOR D = f.v - i.v;
            VECTOR d = f.dv.perpCCW() - i.dv.perpCCW();
            
            GLdouble a = (d.magn2() - 4);
            GLdouble b = 2*(d*D);
            GLdouble c = D.magn2();
            
            GLdouble root[2];
            bool success[2];
            
            success[0] = getRoot(&(root[0]), a, b, c, +1);
            success[1] = getRoot(&(root[1]), a, b, c, -1);
            
            int j =         0;
            GLdouble min =  -1;
            int minj =      0;
            
            for (int r = 0; r < 2; r++) {
                if (success[r]) {
                    for (int s = -1; s < 2; s += 2) {
                        VECTOR ci = i.v + i.dv.perpCCW()*(s*root[r]);
                        VECTOR cf = f.v + f.dv.perpCCW()*(s*root[r]);
                        
                        VECTOR m = (ci + cf)/2;
                        
                        if (std::abs((cf - ci).magn2() - 4*root[r]*root[r]) < 1e-6) {
                            GLdouble x = std::abs(root[r])*(std::abs(getArcAngle(ci, i.v, m, (m-i.v)*i.dv > 0)) + std::abs(getArcAngle(cf, m, f.v, (m-f.v)*f.dv > 0)));
                            
//                            printf("a1: %f\n", getArcAngle(ci, i.v, m, (m-i.v)*i.dv > 0));
//                            printf("a2: %f\n", getArcAngle(cf, m, f.v, (m-f.v)*f.dv > 0));
//                            printf("R: %f\n", root[r]);
//                            printf("x: %f\n", x);
                            if (x < min || min == -1) {
//                                printf("min: %f\n", x);
                                min = x;
                                minj = j;
                            }
                        }
                        
                        j++;
                    }
                }
            }
            
            j = 0;
            for (int r = 0; r < 2; r++) {
                if (success[r]) {
                    for (int s = -1; s < 2; s += 2) {
//                        printf("j: %i\n", j);
//                        printf("r: %i\n", r);
//                        printf("s: %i\n", s);
                        if (j == minj) {
//                            printf("R: %f\n", root[r]);
//                            printf("minj: %i\n", minj);
                            
                            VECTOR ci = i.v + i.dv.perpCCW()*(s*root[r]);
                            VECTOR cf = f.v + f.dv.perpCCW()*(s*root[r]);
                            
                            VECTOR m = (ci + cf)/2;
                            
//                            i.v.printNL();
//                            ci.printNL();
//                            m.printNL();
//                            cf.printNL();
//                            f.v.printNL();
                            
                            toReturn.add(arc(ci, i.v, m, (m-i.v)*i.dv > 0));
                            toReturn.add(arc(cf, m, f.v, (m-f.v)*f.dv > 0));
                            
//                            toReturn += i.v;
//                            toReturn += ci;
//                            toReturn += m;
//                            toReturn += cf;
//                            toReturn += f.v;
                            
//                            return toReturn;
                        }
                        
                        j++;
                    }
                }
            }
            
            break;
            
//        default:
//            break;
    }
    
//    toReturn.print();
    
//    toReturn.close();
//    if (!toReturn.isCCW()) {
//        toReturn.reverse();
//    }
    
//    printf("toReturn.begin=");  toReturn.begin.printNL();
//    printf("toReturn.end=");    toReturn.end.printNL();
    
//    toReturn.print();
    
    //    printf("AREA: %f\n", toReturn.area());
    
    toReturn.begin = i.dv;
    toReturn.end = -f.dv;
    
    return toReturn;
}

POLYLINE thicken(POLYLINE open, GLdouble width, int side, GLdouble minstep) {
    return thicken(open, [width] (GLdouble t) -> GLdouble { return width; }, side, minstep);
}
POLYLINE thicken(POLYLINE open, std::function<GLdouble(GLdouble t)> lambda, int side, GLdouble minstep) {
    POLYLINE toReturn = POLYLINE(open.size()*2);
    
    if (open.size() >= 2) {
        thickenRecurse(&open, &toReturn, &lambda, side, minstep, 0, 0);
    }
    
//    toReturn.print();
    
    toReturn.close();
//    printf("recursing! - AREATHICKEN: %f\n", toReturn.area());
    
    return toReturn;
}
void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambda, int side, GLdouble minstep, int i, GLdouble currentLength) {
    VECTOR u;
    VECTOR v;
    
    GLdouble offset;
    GLdouble thisLength = minstep;
    
    if (i == open->size()-1) {
        if (!open->end.isZero()) {  u = open->end; }
        else {                      u = (open->points[open->size()-1] - open->points[open->size()-2]).unit(); }
        
//        printf("open->end.isZero()=%i; u=", open->end.isZero());
//        u.printNL();
        
        v = u;
        
        offset = (*lambda)(1)/2;
    } else if (i == 0) {
        if (!open->begin.isZero()) {    u = open->begin; }
        else {                          u = (open->points[1] - open->points[0]).unit(); }
        
//        printf("open->begin.isZero()=%i; u=", open->begin.isZero());
//        u.printNL();
        
        v = u;
        
        offset = (*lambda)(0)/2;
    } else {
        if (open->points[i] == open->points[i-1]) { u = open->points[i-1] - open->points[i-2]; }
        else {                                      u = open->points[i] - open->points[i-1]; }
        
        if (open->points[i+1] == open->points[i]) { v = (open->points[i+2] - open->points[i+1]).unit(); }
        else {                                      v = (open->points[i+1] - open->points[i]).unit(); }
        
        GLdouble thisLength = u.magn();
        u /= thisLength;
        
//        currentLength + thisLength;
        
        offset = (*lambda)((currentLength + thisLength)/open->length())/2;
    }
    
    VECTOR dir = -(u.perpCCW() + v.perpCCW()).unit() / sqrt((1 + u * v)/2);
    
//    open->points[i-1].printNL();
//    open->points[i].printNL();
//    open->points[i+1].printNL();
//    dir.printNL();
    
    if (offset > 0) {
        switch (side) {
            case -1:    closed->add(open->points[i] + dir*(offset + PADDING));   break;
            case 0:     closed->add(open->points[i] + dir*offset);               break;
            case 1:     closed->add(open->points[i] - dir*offset);               break;
        }
    }

    if (i < open->size()-1) {
//        printf("recursing! - %i\n", i);
        thickenRecurse(open, closed, lambda, side, minstep, i+1, currentLength + thisLength);
    }
    
    if (offset > 0) {
        switch (side) {
            case -1:    closed->add(open->points[i] + dir*offset);               break;
            case 0:     closed->add(open->points[i] - dir*offset);               break;
            case 1:     closed->add(open->points[i] - dir*(offset + PADDING));   break;
        }
    }
}


