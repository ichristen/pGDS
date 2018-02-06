#include "tesselation.hpp"

void tessCTL(POLYLINE& p, FILE* f) {
    tess(p, f);
}
void tessCTL(POLYLINE& p, std::string output) {
    FILE* f = fopen(output.c_str(), "w");
    
    tessCTL(p, f);
}
void tessGL(POLYLINE& p) {
    tess(p, nullptr);
}
void tess(POLYLINE p, FILE* f) {
    if (!p.isClosed) {  throw std::runtime_error("void tess(POLYLINE, FILE*): Can't tesselate an open polyline..."); }
    if (p.area() > 0) { throw std::runtime_error("void tess(POLYLINE, FILE*): Don't want to tesselate negative polylines..."); }
    
    if (f) { fprintf(f, "(set! geometry (list\n"); }
    
    int i = 0;
    
    while (p.size() > 3) {
        bool angleGood = (p[i] - p[i-1]).perpCW() * (p[i+1] - p[i]) <= 0;   // Check whether the current angle is convex...
        
        bool intersectionGood = angleGood;
        
        if (angleGood) {
            
            VECTOR v1 = p[i-1];
            VECTOR v2 = p[i+1];
            
            VECTOR v;
            VECTOR* vptr = &v;
            
            int k = i % p.size();
            
            for (int j = 0; j < p.size() && intersectionGood; j++) {
                if (!(j+1 >= k-2 && j <= k+2) && !(j+1 >= k-2-p.size() && j <= k+2-p.size()) && !(j+1 >= k-2+p.size() && j <= k+2+p.size())) {
                    intersectionGood = intersectionGood && !intersect(v1, v2, p[j], p[j+1], &vptr);
                }
            }
        }
        
        if (intersectionGood) {
            if (f) {
                outputTriangle(p[i-1],  p[i],   p[i+1], f);
                outputTriangle(p[i],    p[i+1], p[i-1], f);
                outputTriangle(p[i+1],  p[i-1], p[i],   f);
                
                p.erase(i);
            } else {
                
            }
        }
        
        i++;
    }
    
    if (f) { fprintf(f, "))"); }
}
void outputTriangle(VECTOR v1, VECTOR v2, VECTOR v3, FILE* f) {
    if (f) {    // CTL
        VECTOR e1 = v1 - v2;
        VECTOR e2 = v1 - v3;
        
        VECTOR c = (v1*2 + v2 + v3)/4;
        
        fprintf(f, "\t(make block ");
        
        fprintf(f, "(center %f %f 0) ", c.x, c.y);
        
        fprintf(f, "(e1 %f %f 0) ", e1.x, e1.y);
        fprintf(f, "(e2 %f %f 0) ", e2.x, e2.y);
        
        fprintf(f, "(size %f %f h) ", e1.magn(), e2.magn());
        
        fprintf(f, "(material GDS))\n");
    } else {    // OpenGL
        
    }
}
//void fprintCTL(FILE* f, VECTOR v, std::string name) {
//    fprintf(f, "(%s %f %f 0)", name.c_str(), v.x, v.y);
//}





