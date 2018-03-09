#include "tesselation.hpp"

void tessCTL(POLYLINE& p, FILE* f) {                tess(POLYLINES(p), f); }
void tessCTL(POLYLINES& p, FILE* f) {               tess(p, f); }

void tessCTL(POLYLINE& p, std::string output) {     FILE* f = fopen(output.c_str(), "w"); tess(POLYLINES(p), f); }
void tessCTL(POLYLINES& p, std::string output) {    FILE* f = fopen(output.c_str(), "w"); tess(p, f); }

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
    tessTest->exportLibraryGDS("/Users/i/Desktop/Sandia/GDS/2018_03_06_tessTest.gds");
#endif
    
    
    if (f) { fprintf(f, "))"); }
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
    
    while (p.size() >= 3 && i < 1000) {
//        printf("%i - %f\n", i, p.area() * ( (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i])) );
//        bool angleGood = p.area() * ( (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i]) ) >= 0;   // Check whether the current angle is convex...
        bool angleGood = ((p.isReversed)?(-1):(1)) * ( (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i]) ) <= 0;   // Check whether the current angle is convex...
        
//        (p[i] - p[i-1]).printNL();
//        (p[i+1] - p[i]).printNL();
        
//        angleGood = true;
        
        bool intersectionGood = angleGood;
        
//        if (angleGood) {
//
//            VECTOR v1 = p[i-1];
//            VECTOR v2 = p[i+1];
//
//            VECTOR v;
//            VECTOR* vptr = &v;
//
//            int k = i % p.size();
//
//            for (int j = 0; j < p.size() && intersectionGood; j++) {
//                if (!(j+1 >= k-2 && j <= k+2) && !(j+1 >= k-2-p.size() && j <= k+2-p.size()) && !(j+1 >= k-2+p.size() && j <= k+2+p.size())) {
//                    intersectionGood = intersectionGood && !intersect(v1, v2, p[j], p[j+1], &vptr);
//                }
//            }
//        }
        
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
    } else {    // OpenGL
        
    }
}
void outputQuadrilateral(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f) {
    VECTOR e1 = v1 - v2;
    VECTOR e2 = v1 - v3;
    
    VECTOR c = (v1*2 + v2 + v3)/4;
    
    fprintf(f, "\t(make block ");
    
    fprintf(f, "(center %f %f 0) ", c.x, c.y);
    
    fprintf(f, "(e1 %f %f 0) ", e1.x, e1.y);
    fprintf(f, "(e2 %f %f 0) ", e2.x, e2.y);
    
    fprintf(f, "(size %f %f h) ", e1.magn(), e2.magn());
    
    fprintf(f, "(material GDS))\n");
}
//void fprintCTL(FILE* f, VECTOR v, std::string name) {
//    fprintf(f, "(%s %f %f 0)", name.c_str(), v.x, v.y);
//}





