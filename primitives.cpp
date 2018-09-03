#include "primitives.hpp"

// CLOSED PRIMITIVES ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        printf("primitives.hpp::rect(u,v): Points defining rectangle are inline! Empty POLYLINE returned.\n");  // Make this more official.
    }
    
//    toReturn.print();
    
    return toReturn;
}
POLYLINE path(VECTOR u, VECTOR v, GLdouble p) {
    POLYLINE toReturn = POLYLINE(4);
    
    if (p != 0) {
        
        VECTOR d =  (u - v).unit() * (p * sqrt(2));
        
//        d.printNL();
        
        VECTOR a =  d.rotate(-TAU/8);
        VECTOR b =  d.rotate(TAU/8);
        
        toReturn.add(u + a);
        toReturn.add(u + b);
        toReturn.add(v - a);
        toReturn.add(v - b);
    }
    
    return toReturn;
}
//POLYLINE rect(GLdouble x, GLdouble y, GLdouble w, GLdouble h) {
//    return rect(VECTOR(x,y), VECTOR(x+w, y+h));
//}
POLYLINE rect(VECTOR c, GLdouble w, GLdouble h, int anchorx, int anchory) {
    if (h == 0) { h = w; }
    
    return rect(c - VECTOR((1+anchorx)*w/2, (1+anchory)*h/2), c + VECTOR((1-anchorx)*w/2, (1-anchory)*h/2));
}

POLYLINE roundRect(VECTOR u, VECTOR v, GLdouble r) {
    POLYLINE toReturn = POLYLINE(4);
    toReturn.isClosed = true;
    
    //    u.printNL();
    //    v.printNL();
    
    if (!u.inLine(v)) {
        VECTOR ll = VECTOR(min(u.x, v.x), min(u.y, v.y));
        VECTOR ur = VECTOR(max(u.x, v.x), max(u.y, v.y));
        
        GLdouble w = ur.x - ll.x;
        GLdouble h = ur.y - ll.y;
        
        r = min(r, min(w/2, h/2));
        
        POLYLINE round = arc(VECTOR(r,r), VECTOR(0,r), VECTOR(r,0));
        
        toReturn.add(round * (AFFINE(ll)));
        toReturn.add(round * (AFFINE(VECTOR(ur.x, ll.y)) * AFFINE(TAU/4)));
        toReturn.add(round * (AFFINE(ur)                 * AFFINE(TAU/2)));
        toReturn.add(round * (AFFINE(VECTOR(ll.x, ur.y)) * AFFINE(-TAU/4)));
        
//        if (toReturn.area() < 0) {
//            toReturn.reverse();
//        }
    } else {
        printf("primitives.hpp::roundRect(u,v): Points defining rectangle are inline! Empty POLYLINE returned.\n");  // Make this more official.
    }
    
    //    toReturn.print();
    
    return toReturn;
}
POLYLINE roundRect(VECTOR c, GLdouble r, GLdouble w, GLdouble h, int anchorx, int anchory) {
    if (h == 0) { h = w; }
    
    return roundRect(c - VECTOR((1+anchorx)*w/2, (1+anchory)*h/2), c + VECTOR((1-anchorx)*w/2, (1-anchory)*h/2), r);
}
//POLYLINE rect(VECTOR c, VECTOR s, int anchorx=0, int anchory=0) {
//    return rect(c, s.x, s.y, anchorx, anchory);
//}

POLYLINE circle(GLdouble r, VECTOR c, int steps) {
    
    POLYLINE toReturn;
    
    if (!steps) {
        steps = ceil(TAU/acos(1 - EPSILON/r));
    }
    
    AFFINE m = AFFINE(TAU/steps);
    
    VECTOR s = VECTOR(0, r);
    
    toReturn.add(s + c);
    for (int i = 0; i < steps-1; i++) {
        s = m*s;
        toReturn.add(s + c);
    }
    
    toReturn.close();
    
    return toReturn;
}

POLYLINE ellipse(GLdouble a, GLdouble b, VECTOR c, VECTOR semimajorunit) {
    return circle(1) * (AFFINE(semimajorunit, semimajorunit.perpCCW(), c) * AFFINE(a,0,0,b));
    
//    POLYLINE toReturn;
//    
//    int steps = ceil(TAU/acos(1 - EPSILON/a));
//    
//    AFFINE m = AFFINE(TAU/steps);
//    
//    VECTOR s = semimajorunit;
//    
//    toReturn.add(s + c);
//    for (int i = 0; i < steps-1; i++) {
//        s = m*s;
//        toReturn.add(s + c);
//    }
//    
//    toReturn.close();
//    
//    return toReturn;
}
POLYLINE ellipse(VECTOR focus1, VECTOR focus2, GLdouble L) {
    VECTOR semimajorunit = (focus1 - focus2);
    
    GLdouble magn2 = semimajorunit.magn2();
    
    if (magn2 > L*L) { return POLYLINE(); }     // Cannot make a polyline with such a small L...
    
    semimajorunit /= sqrt(magn2);
    
    VECTOR c = (focus1 + focus2)/2;
    
    GLdouble a = L/2;
    GLdouble b = sqrt((L*L - magn2)/4);
    
    return ellipse(a, b, c, semimajorunit);
}

// PATHS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

POLYLINE arc(GLdouble r, GLdouble t1_, GLdouble t2_, bool CCW, int steps, GLdouble stepMultiplier) {
    GLdouble t1 = fmod(t1_, TAU);   // What happens in t1 or t2 are negative?
    GLdouble t2 = fmod(t2_, TAU);
    
    GLdouble dt = fmod(t2 - t1, TAU);
    
    if (dt < 0) {
        dt += TAU;
    }
    
//    printf("t1: %f, t2: %f, dt: %f\n", t1, t2, dt);
    
    if (r == 0) {
        return POLYLINE();  // Return empty...
    } else if (r < 0) {
        r = -r;
    }
    
    if (t1 == t2) {
        throw std::runtime_error("Use circle() instead of arc() for arcs returning to the same angle.");
    }
    
    if (!steps) {
        steps = ceil(dt/acos(1 - EPSILON/r))*stepMultiplier;
    }
    
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

POLYLINE parabola(GLdouble x0, GLdouble x1, VECTOR focus, VECTOR vertex, int steps) {
    return parabola(x0, x1, 1/(4 * (focus-vertex).magn()), vertex, (focus-vertex).unit(), steps);
}
POLYLINE parabola(GLdouble x0, GLdouble x1, GLdouble a, VECTOR center, VECTOR direction, int steps) {
    if (!steps) { steps = ceil(std::abs(x0 - x1)) + 10; }
    
    VECTOR perp = direction.perpCW();
    
    return parametric([x0,x1,a,center,direction,perp] (GLdouble t) -> VECTOR {
        return direction * (a * ( x0 + (x1-x0)*t ) * ( x0 + (x1-x0)*t )) + perp * ( x0 + (x1-x0)*t ) + center;
    }, steps);
}
POLYLINE parabola(GLdouble x0, GLdouble x1, GLdouble a, GLdouble b, GLdouble c, int steps) {
    if (!steps) { steps = ceil(std::abs(x0 - x1)) + 10; }
    
    return parametric([x0,x1,a,b,c] (GLdouble t) -> VECTOR {
        return a * ( x0 + (x1-x0)*t ) * ( x0 + (x1-x0)*t ) + b* ( x0 + (x1-x0)*t ) + c;
    }, steps);
}

//POLYLINE hyperbola(GLdouble x0, GLdouble x1, VECTOR focus, VECTOR vertex, VECTOR c, int steps) {
//    GLdouble a = 0;
//    GLdouble b = 0;
//    
//    return hyperbola(x0, x1, <#GLdouble a#>, <#GLdouble b#>)
//}
POLYLINE hyperbola(GLdouble x0, GLdouble x1, GLdouble a, GLdouble b, VECTOR c, VECTOR direction, int steps) {
    if (!steps) { steps = ceil(std::abs(x0 - x1)) + 10; }
    
    VECTOR perp = direction.perpCW();
    
    return parametric([x0,x1,a,b,c,direction,perp] (GLdouble t) -> VECTOR {
        return direction * ( (a/b) * sqrt( ( x0 + (x1-x0)*t ) * ( x0 + (x1-x0)*t ) + b*b ) ) + perp * ( x0 + (x1-x0)*t ) + c;
    }, steps);
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
            
        steps = ceil(3*(dp1.magn() + dp2.magn()) / (dot + 2));
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
        
        steps = ceil(5*(dp1.magn() + dp2.magn() + dp3.magn()) / (dot1 + dot2 + 3));
    }
    
    return parametric([p0,p1,p2,p3] (GLdouble t) -> VECTOR {
        return p0*((1.0-t)*(1.0-t)*(1.0-t)) + p1*(3*(1.0-t)*(1.0-t)*t) + p2*(3*(1.0-t)*t*t) + p3*(t*t*t);
    }, steps);
}

POLYLINE qBezier(std::vector<VECTOR> pts) {
//    if (isQuadratic) {
        if (pts.size() == 3) {
            return qBezier(pts[0], pts[1], pts[2], 0); // We only need to define one segment...
        } else if (pts.size() < 3) {
            return POLYLINE();                      // Return empty.
        } else {
            POLYLINE toReturn = qBezier(pts[0], pts[1], (pts[1] + pts[2])/2, 0);
            
            for (int i = 1; i < pts.size()-2; i++) {
                toReturn.add(qBezier((pts[i-1] + pts[i])/2, pts[i], (pts[i] + pts[i+1])/2, 0));     // Make this more efficient!
            }
            
            toReturn.add(qBezier((pts[pts.size()-3] + pts[pts.size()-2])/2, pts[pts.size()-2], pts[pts.size()-1], 0));
                
            return toReturn;
        }
//    } else {
//        throw std::runtime_error("longBezier(std::vector<VECTOR>, false): Cubic longBezier NotImplemented.");
//    }
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
        printf("Arcs are equidistant, choosing CCW as shortest...\n");      // Change this!
    } else {
        ang = dir*acos(((b - c).unit()) * ((e - c).unit()));
        //        ang = acos(((b - c).unit()) * ((e - c).unit()));
    }
    
    if (!chooseShortest) {
        if (ang > 0) {  } // ang -= TAU; }
                          //        if (ang > 0) { ang -= TAU; }
        else {          ang += TAU; }
    }
    
    return ang;
}

GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, VECTOR db) {
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
    
//    GLdouble dir = (b - c).cross(e - c);
    GLdouble ang;
    
    int dir1 = sign((b-c).perpCCWdot(db));
    int dir2 = sign(db*(e-c));
    
    if (dir2 == 0) {
        ang = dir1*TAU/2;
    } else {
        ang = acos( ((b - c).unit()) * ((e - c).unit()) );
        
//        if (dir2 < 0) {
//            ang = TAU - ang;
//        }
//
//        ang *= -dir1;
        
        if (dir2 < 0) {
            ang = TAU - ang;
        }
        
        ang *= -dir1;
    }
//    else if (dir1*dir2 < 0) {
//        ang = acos( ((b - c).unit()) * ((e - c).unit()) );
//    } else {
//        ang = acos( ((b - c).unit()) * ((e - c).unit()) ) - TAU/2;
//    }
    
//    if (dir1 == 0) {
//        //        dir = 1;
//        ang = -PI;
//        printf("Arcs are equidistant, choosing CCW as shortest...\n");      // Change this!
//    } else {
//        ang = dir*acos(((b - c).unit()) * ((e - c).unit()));
////        ang = acos(((b - c).unit()) * ((e - c).unit()));
//    }
    
//    if (db*(e-c) < 0){
////        if (ang > 0) {
//        ang = TAU - ang;
////        } else {
////            ang = -ang - TAU;
////        }
//    }
    
//    if (!chooseShortest) {
//        if (ang > 0) {  } // ang -= TAU; }
                          //        if (ang > 0) { ang -= TAU; }
//        else {          ang += TAU; }
//        if (ang < 0) { ang += TAU; }
//    }
    
    return ang;
}

POLYLINE arc(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest, int steps, GLdouble stepMutliplier) {
    GLdouble ang = getArcAngle(c, b, e, chooseShortest);
    GLdouble r = (b - c).magn();
    
//    if (ang > TAU || r > 2000000) { return POLYLINE(); }
    
//    printf("ang: %f\n", ang);
    
    if (!steps) {
        steps = ceil(std::abs(ang)/acos(1 - EPSILON/r))*stepMutliplier + 2;
    }
    
    if (ang > TAU) { return POLYLINE(); }
    
    if (steps > 1e6 || steps < 0) { return POLYLINE(); }
    
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
    
    toReturn.setBeginDirection( (b-c).perpCCW().unit()*sign(ang) );
    toReturn.setEndDirection(   (e-c).perpCCW().unit()*sign(ang) );
    
    return toReturn;
}

POLYLINE connect(CONNECTION i, CONNECTION f, CONNECTIONTYPE type, int numPointsDuringLinear, GLdouble stepMutliplier) {
//    if (b.type != e.type) {
//        
//    }
    POLYLINE toReturn;
    
    if (i.v == f.v) { return toReturn; }
    
    toReturn.setBeginDirection(i.dv);
    toReturn.setEndDirection(-f.dv);
    
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
        if (numPointsDuringLinear) {
            toReturn += linear(i.v, f.v, numPointsDuringLinear);
        } else {
            toReturn += i.v;
            toReturn += f.v;
        }
        
        toReturn.setBeginDirection(i.dv);
        toReturn.setEndDirection(-f.dv);
        
        return toReturn;
    }
    
    VECTOR* v = nullptr;
    
    switch (type) {
        case QBEZIER:
        case CBEZIER:
            throw std::runtime_error("CONNECTIONTYPE not currently supported...");
            
        case LINEAR:
            toReturn.add(i.v);
            toReturn.add(f.v);
            
            toReturn.setBeginDirection(i.dv);
            toReturn.setEndDirection(f.dv);
            
            break;
        case MONOCIRCULAR:
//            v = new VECTOR();
//
//            intersect(i, f, &v);
//
//            if (v) {
//
//
//                toReturn.add(arc(<#VECTOR c#>, <#VECTOR b#>, <#VECTOR e#>))
//
//                if ((i.v - *v).magn2() < (f.v - *v).magn2()) {  // If we need to make a straight segment from f
//
//                } else {                                        // If we need to make a straight segment from i
//
//                }
//
//                delete v;
//            } else {
//
//            }
            
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
                            
                            toReturn.add(arc(ci, i.v, m, (m-i.v)*i.dv > 0, 0, stepMutliplier));
                            toReturn.add(arc(cf, m, f.v, (m-f.v)*f.dv > 0, 0, stepMutliplier));
                            
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
    
    toReturn.setBeginDirection(i.dv);
    toReturn.setEndDirection(-f.dv);
    
    return toReturn;
}

// THICKENED CONNECTIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble a0, GLdouble tb, GLdouble te, GLdouble lb, GLdouble le) {
    CONNECTION b1 = bendRadius(b, tb);  if (tb != 0) { b1.w = -a0; }
    CONNECTION e1 = bendRadius(e, te);  if (te != 0) { e1.w = -a0; }
    
    CONNECTION b2 = b1 + lb;
    CONNECTION e2 = e1 + le;
    
    connectThickenAndAdd(addto, b, -b1, CIRCULAR);
    //    connectThickenAndAdd(addto, b1, -b2, CIRCULAR);
    addto->add(connectThickenShortestDistance(b1, -b2, MINRADIUS));
    //    connectThickenAndAdd(addto, b1, e1,  CIRCULAR);
    if ((e.v - b.v).magn() > 30) {
        addto->add(connectThickenShortestDistance(b2, e2, MINRADIUS));
    } else {
        connectThickenAndAdd(addto, b2, e2,  CIRCULAR);
    }
    //    connectThickenAndAdd(addto, e1, b1,  CIRCULAR);
    //    addto->add(connectThickenShortestDistance(b1, e1, MINRADIUS));
    //    connectThickenAndAdd(addto, b1, e1,  CIRCULAR);
    //    connectThickenAndAdd(addto, e1, -e2, CIRCULAR);
    addto->add(connectThickenShortestDistance(e1, -e2, MINRADIUS));
    connectThickenAndAdd(addto, e, -e1, CIRCULAR);
}

void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, WAVELENGTH wl, GLdouble tb, GLdouble te, GLdouble lb, GLdouble le) {
    CONNECTION b1 = bendRadius(b, tb);  if (tb != 0) { b1.w = -wl.sm; }
    CONNECTION e1 = bendRadius(e, te);  if (te != 0) { e1.w = -wl.sm; }
    
    CONNECTION b2 = b1 + lb;
    CONNECTION e2 = e1 + le;
    
    connectThickenAndAdd(addto, b, -b1, CIRCULAR);
    //    connectThickenAndAdd(addto, b1, -b2, CIRCULAR);
    addto->add(connectThickenShortestDistance(b1, -b2, wl.rad));
    //    connectThickenAndAdd(addto, b1, e1,  CIRCULAR);
    if ((e.v - b.v).magn() > 30) {
        addto->add(connectThickenShortestDistance(b2, e2, wl.rad));
    } else {
        connectThickenAndAdd(addto, b2, e2,  CIRCULAR);
    }
    //    connectThickenAndAdd(addto, e1, b1,  CIRCULAR);
    //    addto->add(connectThickenShortestDistance(b1, e1, MINRADIUS));
    //    connectThickenAndAdd(addto, b1, e1,  CIRCULAR);
    //    connectThickenAndAdd(addto, e1, -e2, CIRCULAR);
    addto->add(connectThickenShortestDistance(e1, -e2, wl.rad));
    connectThickenAndAdd(addto, e, -e1, CIRCULAR);
}
    
void connectThickenAndAdd(DEVICE* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep, GLdouble padding) {
    std::function<GLdouble(GLdouble t)> lambda;
    
    if ( (b.w > 0 && e.w < 0) || (b.w < 0 && e.w > 0) ) {
        return;
    }
    
    bool outer = b.w + e.w < 0;
    
    if (b.l != e.l) {
        printf("connectThickenAndAdd(): Warning! Connections have different layers. Choosing the layer of the first connection.\n");
    }
    
    if (b.w != e.w) {
        GLdouble w1 = std::abs(b.w);
        GLdouble w2 = std::abs(e.w);
        
        lambda = [w1,w2] (GLdouble t) -> GLdouble { return ((w2 - w1)*(3*t*t - 2*t*t*t) + w1); };
    } else {
        GLdouble w = std::abs(b.w);
        
        lambda = [w] (GLdouble t) -> GLdouble { return w; };
    }
    
    b.print();
    e.print();
    
    POLYLINE p = connect(b, e, type, (b.w != e.w)?((int)((e.v-b.v).magn()/minstep) + 2):(0));
    
//    printf("%i", p.size());
    
//    if (p.size() > 5000) { return; }
    
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
            addto->add(thicken(polylines[j], lambda, padding*i, minstep).setLayer(b.l));
        }
    }
}
POLYLINES connectThicken(CONNECTION b, CONNECTION e, CONNECTIONTYPE type) {
    POLYLINES toReturn;
    
    connectThickenAndAdd(&toReturn, b, e, type);
    
    return toReturn;
}

void connectThickenAndAdd(POLYLINES* addto, CONNECTION b, CONNECTION e, CONNECTIONTYPE type, GLdouble minstep, GLdouble padding) {
    std::function<GLdouble(GLdouble t)> lambda;
    
    if ( (b.w > 0 && e.w < 0) || (b.w < 0 && e.w > 0) ) {
        return;
    }
    
    bool outer = b.w + e.w < 0;
    
    if (b.l != e.l) {
        printf("connectThickenAndAdd(): Warning! Connections have different layers. Choosing the layer of the first connection.\n");
    }
    
    if (b.w != e.w) {
        GLdouble w1 = std::abs(b.w);
        GLdouble w2 = std::abs(e.w);
        
        lambda = [w1,w2] (GLdouble t) -> GLdouble { return ((w2 - w1)*(3*t*t - 2*t*t*t) + w1); };
    } else {
        GLdouble w = std::abs(b.w);
        
        lambda = [w] (GLdouble t) -> GLdouble { return w; };
    }
    
    POLYLINE p = connect(b, e, type, (b.w != e.w)?((int)((e.v-b.v).magn()/minstep) + 2):(0));   //
    
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
            addto->add(thicken(polylines[j], lambda, i*padding, minstep).setLayer(b.l));
        }
    }
}

POLYLINES connectThickenShortestDistance(CONNECTION b, CONNECTION e, GLdouble r, GLdouble adiabat, GLdouble mult, GLdouble padding) {
    POLYLINES toReturn;
    
    VECTOR bl = b.v + b.dv.perpCCW()*r;
    VECTOR br = b.v + b.dv.perpCW()*r;
    VECTOR el = e.v + e.dv.perpCCW()*r;
    VECTOR er = e.v + e.dv.perpCW()*r;
    
    VECTOR ll = -(bl - el);
    VECTOR lr = -(bl - er);
    VECTOR rl = -(br - el);
    VECTOR rr = -(br - er);
    
//    bl.printNL();
//    br.printNL();
//    el.printNL();
//    er.printNL();
//
//    ll.printNL();
//    lr.printNL();
//    rl.printNL();
//    rr.printNL();
    
    if ((e.v - b.v).magn() < 2*r) {
        connectThickenAndAdd(&toReturn, b, e, CIRCULAR);
        
        return toReturn;
    }
    
//    bool chooseshortest = false;
//    bool chooseshortest = b.dv * e.dv < 0;
    VECTOR chooseshortest1 = b.dv;
    VECTOR chooseshortest2 = e.dv;
    
    // Straight Cases...
    VECTOR blr = bl + lr.perpCW().unit()*r;
    VECTOR elr = er + lr.perpCW().unit()*r;
    GLdouble angblr = (getArcAngle(bl, b.v, blr, chooseshortest1));
    GLdouble angelr = (getArcAngle(er, e.v, elr, chooseshortest2));
//    GLdouble angelr = std::abs(getArcAngle(er, elr, e.v));
    GLdouble Llr = ( std::abs(angblr) + std::abs(angelr) )*r + lr.magn();
    
    VECTOR brl = br + rl.perpCCW().unit()*r;
    VECTOR erl = el + rl.perpCCW().unit()*r;
    GLdouble angbrl = (getArcAngle(br, b.v, brl, chooseshortest1));
    GLdouble angerl = (getArcAngle(el, e.v, erl, chooseshortest2));
//    GLdouble angerl = std::abs(getArcAngle(el, erl, e.v));
    GLdouble Lrl = ( std::abs(angbrl) + std::abs(angerl) )*r + rl.magn();
    
    // Diagonal Cases...
    GLdouble asin2rll = asin(2*r/ll.magn());
//    GLdouble asin2rll = -asin(2*r/ll.magn());
//    printf("ANG1 = %f pi\n", 2*asin2rll/TAU);
//    VECTOR bll = bl + ll.perpCW().unit().rotate(asin2rll)*r;
//    VECTOR ell = el - ll.perpCW().unit().rotate(asin2rll)*r;
    VECTOR bll = bl + ll.perpCW().unit().rotate(asin2rll)*r;
    VECTOR ell = el - ll.perpCW().unit().rotate(asin2rll)*r;
    GLdouble angbll = (getArcAngle(bl, b.v, bll, chooseshortest1)); // + asin2rll;
    GLdouble angell = (getArcAngle(el, e.v, ell, chooseshortest2)); // + asin2rll;
//    GLdouble angell = std::abs(getArcAngle(er, elr, e.v)) + asin2rll;
    GLdouble Lll = ( std::abs(angbll) + std::abs(angell) )*r + sqrt(ll.magn2() - 4*r*r);
    
//    GLdouble asin2rrr = -asin(2*r/rr.magn());
    GLdouble asin2rrr = asin(2*r/rr.magn());
//    printf("ANG2 = %f pi\n", 2*asin2rrr/TAU);
//    VECTOR brr = br - rr.perpCW().unit().rotate(asin2rrr)*r;
//    VECTOR err = er + rr.perpCW().unit().rotate(asin2rrr)*r;
    VECTOR brr = br - rr.perpCW().unit().rotate(-asin2rrr)*r;
    VECTOR err = er + rr.perpCW().unit().rotate(-asin2rrr)*r;
    GLdouble angbrr = (getArcAngle(br, b.v, brr, chooseshortest1)); // + asin2rrr;
    GLdouble angerr = (getArcAngle(er, e.v, err, chooseshortest2)); // + asin2rrr;
//    GLdouble angerr = std::abs(getArcAngle(el, erl, e.v)) + asin2rrr;
    GLdouble Lrr = ( std::abs(angbrr) + std::abs(angerr) )*r + sqrt(rr.magn2() - 4*r*r);
    
    if (isnan(Llr)) { Llr = INFINITY; }
    if (isnan(Lrl)) { Lrl = INFINITY; }
    if (isnan(Lll)) { Lll = INFINITY; }
    if (isnan(Lrr)) { Lrr = INFINITY; }
    
//    Llr = INFINITY;
//    Lrl = INFINITY;
////    Lll = INFINITY;
//    Lrr = INFINITY;
    
//    // Straight Cases...
//    VECTOR blr = bl + lr.perpCW().unit()*r;
//    VECTOR elr = er + lr.perpCW().unit()*r;
//    GLdouble angblr = (getArcAngle(bl, b.v, blr));
//    GLdouble angelr = (getArcAngle(er, e.v, elr));
//    //    GLdouble angelr = std::abs(getArcAngle(er, elr, e.v));
//    GLdouble Llr = ( std::abs(angblr) + std::abs(angelr) )*r + lr.magn();
//
//    VECTOR brl = br + rl.perpCW().unit()*r;
//    VECTOR erl = el + rl.perpCW().unit()*r;
//    GLdouble angbrl = (getArcAngle(br, b.v, brl));
//    GLdouble angerl = (getArcAngle(el, e.v, erl));
//    //    GLdouble angerl = std::abs(getArcAngle(el, erl, e.v));
//    GLdouble Lrl = ( std::abs(angbrl) + std::abs(angerl) )*r + rl.magn();
//
//    // Diagonal Cases...
//    GLdouble asin2rll = asin(2*r/ll.magn());
//    GLdouble angbll = (getArcAngle(bl, b.v, brl)) + asin2rll;
//    GLdouble angell = (getArcAngle(er, e.v, erl)) + asin2rll;
//    //    GLdouble angell = std::abs(getArcAngle(er, elr, e.v)) + asin2rll;
//    GLdouble Lll = ( std::abs(angbll) + std::abs(angell) )*r + sqrt(ll.magn2() - 4*r*r);
//
//    GLdouble asin2rrr = -asin(2*r/rr.magn());
//    GLdouble angbrr = (getArcAngle(br, b.v, blr)) + asin2rrr;
//    GLdouble angerr = (getArcAngle(el, e.v, elr)) + asin2rrr;
//    //    GLdouble angerr = std::abs(getArcAngle(el, erl, e.v)) + asin2rrr;
//    GLdouble Lrr = ( std::abs(angbrr) + std::abs(angerr) )*r + sqrt(rr.magn2() - 4*r*r);
    
    // Now find the shortest case...
    GLdouble Lmin = min(min(Llr, Lrl), min(Lll, Lrr));
    
//    printf("min( %f, ")
    
//    GLdouble longWidth = mult*(b.w + e.w)/2.;
    GLdouble longWidth = sign(b.w)*mult*max(abs(b.w), abs(e.w));
    
    GLdouble angb, ange;
    
//    Lll += 1;
    
    if      (Lll == Lmin) { angb = angbll; ange = angell; }
    else if (Llr == Lmin) { angb = angblr; ange = angelr; }
    else if (Lrl == Lmin) { angb = angbrl; ange = angerl; }
    else if (Lrr == Lmin) { angb = angbrr; ange = angerr; }
    else { throw std::runtime_error("connectThickenShortestDistance(CONNECTION^2, GLdouble r^2): Something went horribly wrong..."); }
    
#ifdef CONNECTTHICKEN_DEBUG
    std::string debug;
    
    if      (Lll == Lmin) { debug = "ll"; }
    else if (Llr == Lmin) { debug = "lr"; }
    else if (Lrl == Lmin) { debug = "rl"; }
    else if (Lrr == Lmin) { debug = "rr"; }
    
    debug += "\n";
    debug += " " + std::to_string(Lll) + " " + std::to_string(angbll/DEG2RAD) + " " + std::to_string(angell/DEG2RAD) + "\n";
    debug += " " + std::to_string(Llr) + " " + std::to_string(angblr/DEG2RAD) + " " + std::to_string(angelr/DEG2RAD) + "\n";
    debug += " " + std::to_string(Lrl) + " " + std::to_string(angbrl/DEG2RAD) + " " + std::to_string(angerl/DEG2RAD) + "\n";
    debug += " " + std::to_string(Lrr) + " " + std::to_string(angbrr/DEG2RAD) + " " + std::to_string(angerr/DEG2RAD) + "\n";
#endif
    
//    angb = angbrr; ange = angerr;
//    angb = angblr; ange = angelr;
//    angb = angbll; ange = angell;
    
    // And make the connections...
    CONNECTION b1 = bendRadius(b, angb, r);
    CONNECTION e1 = bendRadius(e, ange, r);
    
    if (mult <= 0) {
        POLYLINE fin = connect(b, -b1);
        fin += connect(-e1, e);
        toReturn.add(fin);
        
        return toReturn;
    }
    
    GLdouble Lminfin = (b1.v - e1.v).norm();
    
    if (abs(Lmin - Lminfin) > .1) { printf("Lmin=%f, Lminfin=%f\n", Lmin, Lminfin); Lmin = Lminfin; }
    
    if (Lmin > 2*adiabat) {
        connectThickenAndAdd(&toReturn, b, -b1, CIRCULAR);  // Change this to thicken the arc?
        connectThickenAndAdd(&toReturn, e, -e1, CIRCULAR);
        
        CONNECTION b2 = b1 + adiabat; b2.w = longWidth;
        CONNECTION e2 = e1 + adiabat; e2.w = longWidth;
        
#ifdef CONNECTTHICKEN_DEBUG
        DEVICEPTR d = MATERIAL::layers[0].font.getText(debug);
        
        toReturn.add(d.device->getLayer(0) * AFFINE((b2.v + e2.v)/2));
#endif
        
        if (b2.w < 0) {
            for (int i = -1; i < 2; i += 2) {
                POLYLINE p;
                
                p.add(b2.v + b2.dv.perpCCW()*i*(-b2.w/2));
                p.add(b2.v + b2.dv.perpCCW()*i*(-b2.w/2 + padding));
                p.add(e2.v - e2.dv.perpCCW()*i*(-e2.w/2 + padding));
                p.add(e2.v - e2.dv.perpCCW()*i*(-e2.w/2));
                
                p.close().setLayer(b.l);
                
                if (p.area() < 0) { p.reverse(); }
                
                toReturn.add(p);
            }
        } else {
            POLYLINE p;

            p.add(b2.right());
            p.add(b2.left());
            p.add(e2.right());
            p.add(e2.left());

            p.close().setLayer(b.l);

            toReturn.add(p);
        }
        
//        p.print();
//
//        thicken(p, longWidth,  1, 100).print();
//
//        toReturn.add(thicken(p, longWidth,  1, 100));
//        toReturn.add(thicken(p, longWidth, -1, 100));
//        connectThickenAndAdd(&toReturn, b2, e2, CIRCULAR);
//        connectThickenAndAdd(&toReturn, b2, e2, CIRCULAR);
        
        connectThickenAndAdd(&toReturn, b1, -b2, CIRCULAR);
        connectThickenAndAdd(&toReturn, e1, -e2, CIRCULAR);
    } else {
        connectThickenAndAdd(&toReturn, b, e, CIRCULAR);
    }
    
    return toReturn;
}

POLYLINES thicken(POLYLINES open, GLdouble width, GLdouble side, GLdouble minstep) {
    POLYLINES toReturn;
    
    for (int i = 0; i < open.size(); i++) {
        toReturn.add(thicken(open[i], [width] (GLdouble t) -> GLdouble { return width; }, side, minstep));
    }
    
    return toReturn;
}
POLYLINES thicken(POLYLINES open, std::function<GLdouble(GLdouble t)> lambda, GLdouble side, GLdouble minstep) {
    POLYLINES toReturn;
    
    for (int i = 0; i < open.size(); i++) {
        toReturn.add(thicken(open[i], lambda, side, minstep));
    }
    
    return toReturn;
}

POLYLINES thicken(POLYLINE open, std::vector<GLdouble> widths, GLdouble minstep) {
    if (widths.size() % 2) {    throw std::runtime_error("thicken"); }
    if (open.size() < 2) {      throw std::runtime_error("thicken"); }
    
    POLYLINES toReturn;
    
    for (int i = 0; i < widths.size(); i += 2) {
        POLYLINE step = POLYLINE(open.size()*2).setLayer(open.layer);
        
        GLdouble l = widths[i];
        GLdouble r = widths[i+1];
        
        std::function<GLdouble(GLdouble t)> lambdal = [l] (GLdouble t) -> GLdouble { return l; };
        std::function<GLdouble(GLdouble t)> lambdar = [r] (GLdouble t) -> GLdouble { return r; };
        
        thickenRecurse(&open, &step, &lambdal, &lambdar, minstep, 0, 0);
        
        toReturn.add(step);
    }
    
    return toReturn;
//    return thicken(open, [width] (GLdouble t) -> GLdouble { return width; }, side, minstep);
}

POLYLINE thicken(POLYLINE open, GLdouble width, GLdouble side, GLdouble minstep) {
    return thicken(open, [width] (GLdouble t) -> GLdouble { return width; }, side, minstep);
}
POLYLINE thicken(POLYLINE open, std::function<GLdouble(GLdouble t)> lambda, GLdouble side, GLdouble minstep) {
    bool closed = open.isClosed;
    
    if (closed) {
//        printf("CLOSED!!!\n");
        
        if (open[0] != open[-1]) { open.add(open[0]); }
        
        VECTOR v0 = open[-2];
//        VECTOR v1 = open[0];
        VECTOR v2 = open[1];
        
        open.open();
        
        open.insert(0, v0);
        open.add(v2);
        
//        VECTOR d = ( (v1-v2).unit() - (v1-v0).unit() ).unit();
//
//        v0.printNL();
//        v1.printNL();
//        v2.printNL();
//        d.printNL();
//
//        open.open();
//
//        open.setBeginDirection( d );
//        open.setEndDirection(  -d );
    }
    
    POLYLINE toReturn = POLYLINE(open.size()*2).setLayer(open.layer);
    
    if (open.size() >= 2) {
        thickenRecurse(&open, &toReturn, &lambda, side, minstep, 0, 0);
    }
    
//    toReturn.print();
    
    if (closed) {
        int mid = (int)toReturn.size()/2;   // Optimize?
        
        toReturn.erase((int)toReturn.size()-1);
        toReturn.erase(mid);
        toReturn.erase(mid-1);
        toReturn.erase(0);
    }
    
    toReturn.close();
//    printf("recursing! - AREATHICKEN: %f\n", toReturn.area());
    
    return toReturn;
}
void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambda, GLdouble side, GLdouble minstep, int i, GLdouble currentLength) {
    VECTOR u;
    VECTOR v;
    
    GLdouble offset;
    GLdouble thisLength = 0;// = minstep;
    
    if (i == open->size()-1) {
        if (!open->getEndDirection().isZero()) {    u = open->getEndDirection(); }
        else {  u = (open->points[open->size()-1] - open->points[open->size()-2]).unit(); }
        
        v = u;
        
        offset = (*lambda)(1)/2;
    } else if (i == 0) {
        if (!open->getBeginDirection().isZero()) {  u = open->getBeginDirection(); }
        else {  u = (open->points[1] - open->points[0]).unit(); }
        
        u.printNL();
        
        v = u;
        
        offset = (*lambda)(0)/2;
    } else {
        if (open->points[i] == open->points[i-1]) { u = open->points[i-1] - open->points[i-2]; }
        else {                                      u = open->points[i] - open->points[i-1]; }
        
        if (open->points[i+1] == open->points[i]) { v = (open->points[i+2] - open->points[i+1]).unit(); }
        else {                                      v = (open->points[i+1] - open->points[i]).unit(); }
        
        thisLength = u.magn();
        u /= thisLength;
        
        offset = (*lambda)((currentLength + thisLength)/open->length())/2;
    }
    
    VECTOR dir = -(u.perpCCW() + v.perpCCW()).unit() / sqrt((1 + u * v) / 2);
    
    if (offset > 0) {
        //        closed->add(open->points[i] + dir*(side + offset));
        switch (sign(side)) {
            case -1:    closed->add(open->points[i] + dir*(offset - side));     break;
            case 0:     closed->add(open->points[i] + dir*offset);              break;
            case 1:     closed->add(open->points[i] - dir*offset);              break;
        }
    }
    
    if (i < open->size()-1) {
        thickenRecurse(open, closed, lambda, side, minstep, i+1, currentLength + thisLength);
    }
    
    if (offset > 0) {
        //        closed->add(open->points[i] + dir*(side - offset));
        switch (sign(side)) {
            case -1:    closed->add(open->points[i] + dir*offset);              break;
            case 0:     closed->add(open->points[i] - dir*offset);              break;
            case 1:     closed->add(open->points[i] - dir*(offset + side));     break;
        }
    }
}
void thickenRecurse(POLYLINE* open, POLYLINE* closed, std::function<GLdouble(GLdouble t)>* lambdal, std::function<GLdouble(GLdouble t)>* lambdar, GLdouble minstep, int i, GLdouble currentLength) {
    VECTOR u;
    VECTOR v;
    
    GLdouble l;
    GLdouble r;
    GLdouble thisLength = 0;// = minstep;
    
    if (i == open->size()-1) {
        if (!open->getEndDirection().isZero()) {    u = open->getEndDirection(); }
        else {  u = (open->points[open->size()-1] - open->points[open->size()-2]).unit(); }
        
        v = u;
        
        l = (*lambdal)(1);
        r = (*lambdar)(1);
    } else if (i == 0) {
        if (!open->getBeginDirection().isZero()) {  u = open->getBeginDirection(); }
        else {  u = (open->points[1] - open->points[0]).unit(); }
        
        u.printNL();
        
        v = u;
        
        l = (*lambdal)(0);
        r = (*lambdar)(0);
    } else {
        if (open->points[i] == open->points[i-1]) { u = open->points[i-1] - open->points[i-2]; }
        else {                                      u = open->points[i] - open->points[i-1]; }
        
        if (open->points[i+1] == open->points[i]) { v = (open->points[i+2] - open->points[i+1]).unit(); }
        else {                                      v = (open->points[i+1] - open->points[i]).unit(); }
        
        thisLength = u.magn();
        u /= thisLength;
        
        l = (*lambdal)((currentLength + thisLength)/open->length());
        r = (*lambdar)((currentLength + thisLength)/open->length());
    }
    
    VECTOR dir = -(u.perpCCW() + v.perpCCW()).unit() / sqrt((1 + u * v) / 2);
    
    closed->add(open->points[i] + dir*r);
    
    if (i < open->size()-1) {
        thickenRecurse(open, closed, lambdal, lambdar, minstep, i+1, currentLength + thisLength);
    }
    
    closed->add(open->points[i] + dir*l);
}

POLYLINES rfThicken(POLYLINE open, GLdouble width, GLdouble gap, GLdouble groundWidth, uint16_t layer, uint16_t groundlayer) {
    POLYLINES toReturn;
    
    toReturn.add(thicken(open, width, 0).setLayer(layer));
    toReturn.add(thicken(open, width+2*gap,  TRANSPADDING).setLayer(layer));
    toReturn.add(thicken(open, width+2*gap, -TRANSPADDING).setLayer(layer));
    
    return toReturn;
}
POLYLINES rfThicken(POLYLINE open, RFTYPE type) {
    switch (type) {
        case UPPER:             return rfThicken(open, RF_WID, RF_GAP, RF_GND, RF_LAY);
        case UPPERwithGROUND:   return rfThicken(open, RF_WID, RF_GAP, RF_GND, RF_LAY, 2);
    }
}
POLYLINES rfConnectThicken(CONNECTION b, CONNECTION e, RFTYPE type) {
    return rfThicken(connect(b, e), type);
}




