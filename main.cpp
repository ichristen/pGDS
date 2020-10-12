//#define USE_FT_FONT 1
#define USE_GL_RENDER 1
//#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>

//#define pGDSGLFW 1

//#include "window.hpp"

//#include "testdevices.hpp"
//#include "diamonddevices.hpp"
//#include "eodevices.hpp"
//#include "testing.hpp"
//#include "harvarddevices.hpp"
//#include "quantumdevices.hpp"
//#include "vapordevices.hpp"
//#include "mitredevices.hpp"
//#include "tesselation.hpp"
//#include "nemsdevices.hpp"

//#include "straindevices.hpp"
//#include "interposerdevices.hpp"

#include "lincolndevices.hpp"
#include "csemdevices.hpp"

extern double SPACEPAD(0);

int main(int argc, const char * argv[]) {
//    mainDiamondDevice(0);
//    DEVICE* c = getDevice("Circle");
//    c->add(circle(5));
    
//    DEVICE* c = grating(637);
//    DEVICE* c = testY2();
//    DEVICEPTR c = DEVICEPTR(superDuperDevice2(637));
//    DEVICEPTR c = DEVICEPTR(loss(2));
    //testD2(637);
    
//    SiNStrain();
//    return 0;

//    CSEM();
//    return 0;
    
//    LincolnIon();
//    return 0;
    
    LincolnShort();
    return 0;
    
//    GANHARV();
//    interposerv2();
//    GANHARVinterposer();
//    return 0;
    
//    NEMS();
//    return 0;
//    
//    LNX_LOSS_2();
//    return 0;
////    VAPORTEST();
//    
////    WINDOW w;
////    w.focus =
////    ALNHARV();
////    SiN();
//    
//    DCSI();
//    return 0;
//
////    MITRELN();
////    return 0;
//    
//    DEVICEPTR dlist[3] = { LNXEOQ(), LNXEO(), LNX_HARV_6() };
//    
//    DEVICE* mask = getDevice("MASK");
//    
//    char str[16];
//    snprintf(str, 15, "+%inm", int(SPACEPAD*1000));
//    
//    DEVICEPTR font = MATERIAL::layers[0].font.getText(str);
//    //    toReturn->add(font.device->getLayer(0,2) * (AFFINE(-(w-2.5)*500, 0) * s * AFFINE(TAU/4)  * font.transformation) );
//    //    toReturn->add(font.device->getLayer(0,3) * (AFFINE( 0, (h-2.5)*500) * s *                  font.transformation) );
//    //    toReturn->add(font.device->getLayer(0,4) * (AFFINE( (w-2.5)*500, 0) * s * AFFINE(-TAU/4) * font.transformation) );
//    
//    
////    for (int l = 2; l <= 4; l++) {
//    
//    GLdouble s = 1000/7.;
//        mask->add(font * (AFFINE(-18*500, 16000*2.5-6000 ) * AFFINE(TAU/4) * s  * font.transformation) );
////        toReturn->add(font.device->getLayer(0,l) * (AFFINE( 15*500, 0 ) * AFFINE(-TAU/4) * s * font.transformation) );
////    }
//    
//    int N = 3;
//    
//    for (int i = 0; i < N; i++) {
//        for (int j = 2; j < 5; j++) {
//            mask->add(dlist[i % 3].device->getLayer(j, 0) * AFFINE(i*15000, j*16000));
//        }
//    }
//    
//    int N2 = 6;
//    int dx = 64;
//    
//    GLdouble w[6] = { 1, 2, 4, 8, 16, 32 };
////    GLdouble g[N2] = {  };
//    
//    for (int i = 1; i < 4; i++) {
//        VECTOR c = VECTOR(N*15000-3500, i*16000);
//        
//        mask->add(thicken(rect(c, 7000+360, 15000+360).setLayer(0), 280));
//        
//        GLdouble x = -3000;
//        int j = 0;
//        
//        while (x < -100) {
//            mask->add(rect(c + VECTOR(x+w[j%N2],  2000), w[j%N2], 4000).setLayer(0));
//            mask->add(rect(c + VECTOR(x, -2000), w[j%N2], 4000).setLayer(0));
//            x += dx;
//            j++;
//        }
//        
//        j = 0;
//        x = 100;
//        
//        while (x < 3000) {
//            mask->add(rect(c + VECTOR(x+dx/4,  2000), dx/2, 4000).setLayer(0));
//            mask->add(rect(c + VECTOR(x-dx/4-w[j%N2],           2000), dx/2, 4000).setLayer(0));
//            mask->add(rect(c + VECTOR(x+dx/4+w[j%N2],  -2000), dx/2, 4000).setLayer(0));
//            mask->add(rect(c + VECTOR(x-dx/4,           -2000), dx/2, 4000).setLayer(0));
//            x += dx;
//            j++;
//        }
//    }
//    
////    for (int i = 0; i < 3; i++) {
////        mask->add();
////    }
//    
//    char str2[128];
//    snprintf(str2, 127, "/Users/i/Desktop/Sandia/GDS/2018_10_08_W01009_PHOTOMASK_%i.gds", int(SPACEPAD*1000));
//    
//    mask->exportLibraryGDS(str2);
//    
//    
////    w.loop();
//    
////    dcTest(.2, .4, 4, .3, 30, 20, 4);
//    
}




