#ifndef WAVELENGTHS_H
#define WAVELENGTHS_H

#include "math.hpp"
#include "device.hpp"
//#include "device.hpp"
#include <OpenGL/gl3.h>         //OpenGL
#include <map>


//#define COUP_LEN    120
//#define ADIABAT     15
//
//#define WG_W_MULT   3

//#define CORRECTION .020
#define CORRECTION .020
//DEVICEPTR directionalCoupler2(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t=90, GLdouble r=MINRADIUS, bool text=true, bool straights=true);

class DCINFO {
public:
    GLdouble a;
    GLdouble d;
    GLdouble l;
    GLdouble r;
    GLdouble t;
    
    DCINFO();
    DCINFO(GLdouble a, GLdouble d, GLdouble l, GLdouble r, GLdouble t);
};

class WAVELENGTH {
public:
    GLdouble nm;        // Wavelength
    GLdouble neff;      // Effecitive index of single mode waveguide (200nm LN, 40% etch)
    GLdouble sm;        // Width of single mode waveguide
    GLdouble fc;        // Width of optimal fiber couple
    GLdouble rad;       // Bend radius with minimal loss
    
    GLdouble lossS;     // Loss length[s]tep
    GLdouble lossL;     // Loss [l]ength
    GLdouble lossN;     // Loss ring [n]umber
    GLdouble lossD;     // Loss ring spacing
    
//    GLdouble dcA;       // 50:50 directional coupler width
//    GLdouble dcD;       // 50:50 directional coupler spacing
//    GLdouble dcL;       // 50:50 directional coupler length
    
    GLdouble eoW;
    GLdouble eoS;
    GLdouble eoL;
    
    std::map<GLdouble, DCINFO> directionalCouplers;
    
    WAVELENGTH();
    WAVELENGTH(GLdouble nm_, GLdouble neff_, GLdouble sm_, GLdouble fc_, GLdouble rad_);
    void setLoss(GLdouble s, GLdouble l, GLdouble n, GLdouble d);
    void makeDC(GLdouble trans, GLdouble a, GLdouble d, GLdouble l, GLdouble r, GLdouble t);
    void setEO(GLdouble w, GLdouble s, GLdouble l);
};

#define NUMWAVELENGTHS 9

class WAVELENGTHS {
public:
    WAVELENGTH wavelengths[NUMWAVELENGTHS];
    
    WAVELENGTHS();
    
    WAVELENGTH getWavelength(int nm);
};

POLYLINES dcTest(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t, GLdouble r, GLdouble sep);
DEVICEPTR directionalCoupler(WAVELENGTH wl, GLdouble trans, GLdouble t=0, bool negResist=true);
DEVICE* directionalCoupler(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t=90, GLdouble r=MINRADIUS, bool text=true, bool straights=true, bool bend=false);
DEVICEPTR directionalCoupler2(GLdouble a, GLdouble d, GLdouble L, GLdouble r=MINRADIUS, GLdouble t=TAU/8, bool bend=false, bool text=true);

//WAVELENGTHS wavelengths = WAVELENGTHS();

#endif
