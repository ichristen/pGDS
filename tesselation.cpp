#include "tesselation.hpp"

void tessCTL(POLYLINE& p, FILE* f) {                tess(POLYLINES(p), f); }
void tessCTL(POLYLINES& p, FILE* f) {               tess(p, f); }

void tessCTL(POLYLINE& p, std::string output) {     FILE* f = fopen(output.c_str(), "w+"); tess(POLYLINES(p), f); }
void tessCTL(POLYLINES& p, std::string output) {    FILE* f = fopen(output.c_str(), "w+"); tess(p, f); }

#ifdef MATLAB_MEX_FILE
void tessCTL(POLYLINE& p) {                         FILE* f = fopen("/Users/i/Desktop/MPB/wmc/wmcgeo.ctl", "w+"); tess(POLYLINES(p), f); }
void tessCTL(POLYLINES& p) {                        FILE* f = fopen("/Users/i/Desktop/MPB/wmc/wmcgeo.ctl", "w+"); tess(p, f); }
#endif

void tessGL(POLYLINE& p) {                          tess(p, nullptr, false); }
//void tessGL(POLYLINES& p) {                         tess(p, nullptr, false); }

//void tess(POLYLINES plines, FILE* f) {
void tess(POLYLINES plines, FILE* f) {
#ifdef TESSDEBUG
    DEVICE* tessTest = getDevice("tessTest");
#endif

    if (f) { fprintf(f, "(set! geometry (list\n"); }

    for (int j = 0; j < plines.size(); j++) {
        tess(plines[j], f, true
#ifdef TESSDEBUG
             , tessTest
#endif
             );
    }
    
#ifdef TESSDEBUG
    tessTest->exportLibraryGDS("/Users/i/Desktop/Sandia/GDS/2018_05_08_tessTest.gds");
    tessTest->clear();
//    delete tessTest;
#endif
    
    
    if (f) { fprintf(f, "))"); }
    
    fclose(f);
}
void tess(POLYLINE p, void* ptr, bool isCTL
#ifdef TESSDEBUG
          , DEVICE* tessTest
#endif
          ) {
    
    FILE* f = nullptr;
    std::vector<unsigned int>* v = nullptr;
    std::vector<unsigned int> ii;
    
    if (isCTL) {   f = (FILE*)ptr; }
    else {
        v = (std::vector<unsigned int>*)ptr;
        ii = std::vector<unsigned int>(p.size());
        std::iota(std::begin(ii), std::end(ii), 0);
    }
        
    int i = 0;

    if (!p.isClosed) {  throw std::runtime_error("void tess(POLYLINE, FILE*): Can't tesselate an open polyline..."); }
//    if (p.area() < 0) { throw std::runtime_error("void tess(POLYLINE, FILE*): Don't want to tesselate negative polylines..."); }
    if (p.area() < 0) { p.reverse(); }
              
    while (p.size() >= 3 && i < 1000) {
//        printf("%i - %f\n", i, p.area() * ( (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i])) );
//        bool angleGood = p.area() * ( (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i]) ) >= 0;   // Check whether the current angle is convex...
        bool angleGood = ((p.isReversed)?(-1):(1)) * ( (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i]) ) <= 0;   // Check whether the current angle is convex...
        
        bool angle2Good = (p[i+1] - p[i]) * ( (p[i+2] - p[i+1]).unit() - (p[i-1] - p[i+1]).unit() ) >= 0; // This might break in certain situations...
        
        bool intersectionGood = angleGood && angle2Good;
        
        if (angleGood) {

            VECTOR v1 = p[i-1];
            VECTOR v2 = p[i+1];

            VECTOR v;
            VECTOR* vptr = &v;

            int k = i % p.size();

            for (int j = 0; j < p.size() && intersectionGood; j++) {
                if (!(j >= k-2 && j+1 <= k+2) && !(j >= k-2-p.size() && j+1 <= k+2-p.size()) && !(j >= k-2+p.size() && j+1 <= k+2+p.size())) {
                    intersectionGood = intersectionGood && !intersect(v1, v2, p[j], p[j+1], &vptr);
                }
            }
        }
        
        if (intersectionGood) {
#ifdef TESSDEBUG
            if (tessTest) {
                POLYLINE toAdd;
                
                toAdd.add(p[i-1]);
                toAdd.add(p[i]);
                toAdd.add(p[i+1]);
                
                tessTest->add(toAdd);
            }
#endif
            if (f) { outputTriangle(p[i-1],  p[i],   p[i+1], f); }
            
            p.erase(i);     // Is there a faster way to do this?
            
            if (v) {
//                v->push_back(ii[(i-1) % p.size()]);
//                v->push_back(ii[(i)   % p.size()]);
//                v->push_back(ii[(i+1) % p.size()]);
//
//                ii.erase(ii.begin() + (i % p.size()));
                
                v->push_back(ii[p.returnValidIndex(i-1)]);
                v->push_back(ii[p.returnValidIndex(i)  ]);
                v->push_back(ii[p.returnValidIndex(i+1)]);
                
                ii.erase(ii.begin() + p.returnValidIndex(i));
            }
            
            i--;
        } else {
            // GL_TRIANGLE_STRIP logic...
        }
        
        i++;

    }
}
void outputTriangle(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f) {
    if (f) {    // CTL
        outputQuadrilateral(v1, v2, v3, f);
        outputQuadrilateral(v2, v3, v1, f);
        outputQuadrilateral(v3, v1, v2, f);
        
        outputQuadrilateral(v1 * mirrorX(), v2 * mirrorX(), v3 * mirrorX(), f);
        outputQuadrilateral(v2 * mirrorX(), v3 * mirrorX(), v1 * mirrorX(), f);
        outputQuadrilateral(v3 * mirrorX(), v1 * mirrorX(), v2 * mirrorX(), f);

        outputQuadrilateral(v1 * mirrorY(), v2 * mirrorY(), v3 * mirrorY(), f);
        outputQuadrilateral(v2 * mirrorY(), v3 * mirrorY(), v1 * mirrorY(), f);
        outputQuadrilateral(v3 * mirrorY(), v1 * mirrorY(), v2 * mirrorY(), f);

        outputQuadrilateral(-v1, -v2, -v3, f);
        outputQuadrilateral(-v2, -v3, -v1, f);
        outputQuadrilateral(-v3, -v1, -v2, f);
    } else {    // OpenGL
        
    }
}
void outputQuadrilateral(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f) {
    VECTOR e1 = v1 - v2;
    VECTOR e2 = v1 - v3;
    
    if (!e1.isZero() && !e2.isZero() && e1 != e2) {
        VECTOR c = (v1*2 + v2 + v3)/4;
    
        fprintf(f, "\t(make block ");
    
        fprintf(f, "(center %.15f %.15f 0) ", c.x, c.y);
    
        fprintf(f, "(e1 %.15f %.15f 0) ", e1.x, e1.y);
        fprintf(f, "(e2 %.15f %.15f 0) ", e2.x, e2.y);
    
        fprintf(f, "(size %.15f %.15f h) ", e1.magn()/2, e2.magn()/2);
    
        fprintf(f, "(material GDS))\n");
    }
}
//void fprintCTL(FILE* f, VECTOR v, std::string name) {
//    fprintf(f, "(%s %f %f 0)", name.c_str(), v.x, v.y);
//}





