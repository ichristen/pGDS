#include <stdio.h>
//#include <SDL2/SDL.h>        //SDL

#include <OpenGL/OpenGL.h>      //OpenGL
#include <OpenGL/gl3.h>      //OpenGL
                             //    #include <OpenGL/gl.h>
//#include <OpenGL/glu.h>         //GLU
#include <chrono>

//#define USE_FT_FONT 1
#define USE_GL_RENDER 1

//#include "vector.hpp"
//#include "polyline.hpp"
//#include "primitives.hpp"
#include "testdevices.hpp"
//#include "font.hpp"

int main(int argc, const char * argv[]) {
//    time_t tstart, tend;
//    tstart = time(0);
    
//    auto start = std::chrono::steady_clock::now();
    
//    FONT font(std::string("/Users/i/Documents/gds-test/fonts/courier-bold.ttf"));//Pilgiche.ttf"));

//    POLYLINE bounds = POLYLINE();
//    bounds.layer = 3;
//    
//    bounds += rect(VECTOR(0,0), VECTOR(10000,10000)).setLayer(3).open();
//    bounds += VECTOR(0,0);
//    bounds += VECTOR(2,2);
//    bounds += rect(VECTOR(2,2), VECTOR(9998,9998)).setLayer(3).reverse().open();
//    
//    bounds.close();
    
    mainDevice();
    
//    DEVICE fin("Library");
//    
////    fin.add(bounds);
////    
////    for (GLdouble x = 1.4; x < 10; x += 7.2) {
////        for (GLdouble y = 1.4; y < 10; y += 7.2) {
////            fin.add(DEVICEPTR(astrix(), AFFINE(x*1000, y*1000)));
////        }
////    }
////    for (GLdouble x = 1.4; x < 10; x += 7.2) {
////        for (GLdouble y = 1.4; y < 10; y += 7.2) {
////            fin.add(DEVICEPTR(astrix(), AFFINE(x*1000, y*1000)));
////        }
////    }
////    for (int x = -1; x < 2; x+=2) {
////        for (int y = -1; y < 2; y+=2) {
////            fin.add(DEVICEPTR(astrix(), AFFINE(x*3.5*SEGMENT, y*7.5*SEGMENT)));
////        }
////    }
//    
//    bool round1 = true;
//    
//    if (round1) {
//        for (int i = -5000/SEGMENT; i < 5000/SEGMENT; i++) {
//            for (int j = -5000/SEGMENT; j < 5000/SEGMENT; j++) {
//                POLYLINE p = rect(i*SEGMENT, j*SEGMENT, SEGMENT, SEGMENT);
//                p.layer = 0;
//                fin.add(p);
//            }
//        }
//        
//        GLdouble h = 127;
//        GLdouble x = 750;
//        
//        for (GLdouble ang = 0; ang < TAU; ang += TAU/2) {
//            
//            fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10), AFFINE(ang)*AFFINE(-SEGMENT*10 + 20, SEGMENT*9) ));
//            
//            for (int i = -1; i < 1; i++) {
//                h = SEGMENT*9*i;
//                
////                fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(1+8*i)) ));
////                fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(3+8*i)) ));
////                fin.add(DEVICEPTR(edgeLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(5+8*i)) ));
////                fin.add(DEVICEPTR(edgeLoopArray(637, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(7+8*i)) ));
////                for (int j = 0; j < 4; j++) {
////                    //                    if ((j+1) % 2) {
////                    fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10, j*.008), AFFINE(ang)*AFFINE(-SEGMENT*10 + 320, SEGMENT*(2*j+8*i)) ));
////                    //                    }
////                }
//                
//                
//                fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10), AFFINE(ang)*AFFINE(-SEGMENT*10 + 20, h) )); h += 100;
//                
//            //    fin.add(DEVICEPTR(device0(637),   AFFINE(0, h))); h += 3.2*127;
//            //    fin.add(DEVICEPTR(device0(737),   AFFINE(0, h))); h += 4.2*127;
//                fin.add(DEVICEPTR(device2(637),           AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 3.2*127;
//                fin.add(DEVICEPTR(device2(737),           AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 3.2*127;
//                fin.add(DEVICEPTR(device1(637),   AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.1*127;
//                fin.add(DEVICEPTR(device1(737),   AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.1*127;
//            //    fin.add(DEVICEPTR(device1_5(637), AFFINE(0, h))); h += 3.2*127;
//            //    fin.add(DEVICEPTR(device1_5(737), AFFINE(0, h))); h += 4.2*127;
//                fin.add(DEVICEPTR(device4(637),           AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 5.2*127;
//                fin.add(DEVICEPTR(device4(737),           AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 5.2*127;
//        //        fin.add(DEVICEPTR(device4across(637),     AFFINE(0, h)));
//        //        fin.add(DEVICEPTR(device4across(637),     AFFINE(TAU/2, 10000, h + 4.7*127))); h += 3.4*127;
//        //        fin.add(DEVICEPTR(device4across(737),     AFFINE(0, h)));
//        //        fin.add(DEVICEPTR(device4across(737),     AFFINE(TAU/2, 10000, h + 4.7*127))); h += 3.4*127;
////                fin.add(DEVICEPTR(testD2(637), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; //fin.add(DEVICEPTR(testD2(637, 1, true), AFFINE(x, h))); h += 2.2*127;
////                fin.add(DEVICEPTR(testD2(737), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; // fin.add(DEVICEPTR(testD2(737, 1, true), AFFINE(x, h))); h += 2.2*127;
////                fin.add(DEVICEPTR(testY2(),               AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127;
////                fin.add(DEVICEPTR(testYN(4),              AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 6.2*127;
//            //    fin.add(DEVICEPTR(testY2(),       AFFINE(0, h))); h += 1.2*127;
//            //    fin.add(DEVICEPTR(device4(737),    AFFINE(0, h))); h += 5.2*127;
//                
//                fin.add(DEVICEPTR(device1(637),   AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.1*127;
//                fin.add(DEVICEPTR(device1(737),   AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.1*127;
//                
//                fin.add(DEVICEPTR(device2(637),           AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 3.1*127;
//                fin.add(DEVICEPTR(device2(737),           AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 3.1*127;
//                fin.add(DEVICEPTR(device1(637),   AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.1*127;
//            }
//        }
//        
////         GLdouble h2 = h;
////        fin.add(DEVICEPTR(grateLoop(637, 0),      AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(637, 500),    AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(637, 1000),    AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(637, 2000),    AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(737, 0),      AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(737, 500),    AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(737, 1000),    AFFINE(x, h2))); h2 += 1.2*127;
////        fin.add(DEVICEPTR(grateLoop(737, 2000),    AFFINE(x, h2))); h2 += 1.2*127;
//        
////        for (GLdouble ang = TAU/4; ang < TAU/2; ang += TAU/2) {
//            //            for (int i = -1; i < 1; i++) {
////                fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(1+8*i)) ));
////                fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(3+8*i)) ));
////                fin.add(DEVICEPTR(edgeLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(5+8*i)) ));
////                fin.add(DEVICEPTR(edgeLoopArray(637, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(7+8*i)) ));
//        
////        GLdouble ang = TAU/4;
////        
////        fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-5)) ));
////        fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-3)) ));
////        fin.add(DEVICEPTR(edgeLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-1)) ));
////        fin.add(DEVICEPTR(edgeLoopArray(637, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(1)) ));
////        fin.add(DEVICEPTR(edgeLoopArray(737, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(3)) ));
////        fin.add(DEVICEPTR(edgeLoopArray(737, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(5)) ));
////        
////        for (int j = 0; j < 4; j++) {
////            fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10, j*.008), AFFINE(ang)*AFFINE(-SEGMENT*10 + 320, SEGMENT*(4*j-6)) ));
////        }
////        
////        ang = 3*TAU/4;
////        
////        //        fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-5)) ));
////        //        fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-3)) ));
////        fin.add(DEVICEPTR(grateLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(1)) ));
////        fin.add(DEVICEPTR(grateLoopArray(637, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-1)) ));
////        fin.add(DEVICEPTR(grateLoopArray(737, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-3)) ));
////        fin.add(DEVICEPTR(grateLoopArray(737, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-5)) ));
//        
//        GLdouble ang = TAU/4;
//        
//        fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-7)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-5)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-3)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(637, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(-1)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(737, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(1)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(737, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(3)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(5)) ));
//        fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(7)) ));
//        
//        for (int j = 0; j < 4; j++) {
//            fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10, j*.008), AFFINE(ang)*AFFINE(-SEGMENT*10 + 320, SEGMENT*(4*j-6)) ));
//        }
//        
//        ang = 3*TAU/4;
//        
//        //        fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-5)) ));
//        //        fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-3)) ));
//        fin.add(DEVICEPTR(grateLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(3)) ));
//        fin.add(DEVICEPTR(grateLoopArray(637, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(1)) ));
//        fin.add(DEVICEPTR(grateLoopArray(737, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-1)) ));
//        fin.add(DEVICEPTR(grateLoopArray(737, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-3)) ));
//        
//        fin.add(DEVICEPTR(grateLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-5)) ));
//        fin.add(DEVICEPTR(grateLoopArray(637, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(-7)) ));
//        fin.add(DEVICEPTR(grateLoopArray(737, 15, true),   AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(7)) ));
//        fin.add(DEVICEPTR(grateLoopArray(737, 15),         AFFINE(ang)*AFFINE(SEGMENT*2, SEGMENT*(5)) ));
//        
//        for (int x = -1; x < 2; x+=2) {
//            for (int y = -1; y < 2; y+=2) {
//                fin.add(DEVICEPTR(astrix(), AFFINE(x*8.5*SEGMENT, y*8.5*SEGMENT)));
//            }
//        }
//        
//        ang = 3*TAU/4;
//        
////        h = 0;
//        for (int i = -1; i < 1; i++) {
//                h = SEGMENT*9*i;
//        
//            if (!i) {
//                fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10), AFFINE(ang)*AFFINE(-SEGMENT*10 + 20, h) ));
//            }
//             h += 100;
//            
//            fin.add(DEVICEPTR(testD2(637), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; //fin.add(DEVICEPTR(testD2(637, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testD2(637), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; //fin.add(DEVICEPTR(testD2(637, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testY2(),               AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testD2(737), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; // fin.add(DEVICEPTR(testD2(737, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testD2(737), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; // fin.add(DEVICEPTR(testD2(737, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testY2(),               AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testYN(5),              AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 7.2*127;
//            
//            fin.add(DEVICEPTR(testD2(637), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; //fin.add(DEVICEPTR(testD2(637, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testD2(637), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; //fin.add(DEVICEPTR(testD2(637, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testY2(),               AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testD2(737), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; // fin.add(DEVICEPTR(testD2(737, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testD2(737), AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127; // fin.add(DEVICEPTR(testD2(737, 1, true), AFFINE(x, h))); h += 2.2*127;
//            fin.add(DEVICEPTR(testY2(),               AFFINE(ang)*AFFINE(-SEGMENT*10, h))); h += 2.2*127;
////        fin.add(DEVICEPTR(testY2(),       AFFINE(0, h))); h += 1.2*127;
//        }
//        
//        auto end = std::chrono::steady_clock::now();
//        
//        printf("Devices were created in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());
//        printf("Devices have area %f um^2 = %f mm^2.\n", fin.area(), fin.area()/1e6);
//        
//        fin.exportLibraryGDS("/Users/i/Desktop/2017_08_01 Ian Christen Sandia LN Round 1.gds");
//    }
//    
//    bool waveguides = false;
//    
//    if (waveguides) {
//        for (int x = -1; x < 2; x+=2) {
//            for (int y = -1; y < 2; y+=2) {
//                fin.add(DEVICEPTR(astrix(), AFFINE(x*4.5*SEGMENT, y*9.5*SEGMENT)));
//            }
//        }
//        
//        for (GLdouble ang = 0; ang < TAU; ang += TAU/2) {
//            for (int i = -1; i < 1; i++) {
//                fin.add(DEVICEPTR(edgeLoopArray(532, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(1+8*i)) ));
//                fin.add(DEVICEPTR(edgeLoopArray(532, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(3+8*i)) ));
//                fin.add(DEVICEPTR(edgeLoopArray(637, 15, true),   AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(5+8*i)) ));
//                fin.add(DEVICEPTR(edgeLoopArray(637, 15),         AFFINE(ang)*AFFINE(-SEGMENT*10, SEGMENT*(7+8*i)) ));
//                for (int j = 0; j < 4; j++) {
////                    if ((j+1) % 2) {
//                        fin.add(DEVICEPTR(linesNgaps(.032, 8, 100, 10, j*.008), AFFINE(ang)*AFFINE(-SEGMENT*10 + 320, SEGMENT*(2*j+8*i)) ));
////                    }
//                }
//            }
//        }
//        
//        
//        for (int i = -10; i < 10; i++) {
//            for (int j = -10; j < 10; j++) {
//                POLYLINE p = rect(i*SEGMENT, j*SEGMENT, SEGMENT, SEGMENT);
//                p.layer = 3;
//                fin.add(p);
//            }
//        }
//        
//        for (int k = -1; k < 2; k += 2) {
//            for (int i = 0; i < 6; i++) {
//                for (int j = 0; j < 6; j++) {
//                    GLdouble scaleTimes = .05*i + .9;
//                    GLdouble offsetPlus = .005*j - .01;
//                    
//                    char str[32];
//                    
//                    snprintf(str, 30, "scale=%.2f, pad=%.2f", scaleTimes, offsetPlus);
//                    
//                    fin.add(DEVICEPTR(grateLoop(637, 0, scaleTimes, offsetPlus), AFFINE(SEGMENT*(i-2.5), SEGMENT*(j-2.5+4*k) - 127/2) ));
//                    fin.add(DEVICEPTR(MATERIAL::layers[1].font.getText(str), AFFINE(SEGMENT*(i-2.5), SEGMENT*(j-2.5+4*k))) );
//                }
//            }
//        }
//        
////        for (int k = -1; k < 2; k += 2) {
////            for (float i = 0; i < 6*3; i++) {
////                for (float j = 0; j < 6*3; j++) {
////                    GLdouble scaleTimes = .01*i + .9;
////                    GLdouble offsetPlus = .001*j - .01;
////                    
////                    char str[32];
////                    
////                    snprintf(str, 30, "scale=%.2f, pad=%.2f", scaleTimes, offsetPlus);
////                    
////                    fin.add(DEVICEPTR(grateLoop(637, 0, scaleTimes, offsetPlus), AFFINE(SEGMENT*(i/3-2.5), SEGMENT*(j/3-2.5+4*k) - 127/2) ));
////                    fin.add(DEVICEPTR(fin.labelFont.getText(str), AFFINE(SEGMENT*(i/3-2.5), SEGMENT*(j/3-2.5+4*k))) );
////                }
////            }
////        }
//        
//        
//        for (int k = -1; k < 2; k += 2) {
//            for (int i = 0; i < 8; i++) {
////                for (int i = -1; i < 5; i++) {
////                if (i % 2) {
////                    fin.add(DEVICEPTR(device1(637), AFFINE(k*TAU/4)*AFFINE(-SEGMENT*10 + 10, 250 - 127 + SEGMENT*(i-2))));
////                } else {
////                    fin.add(DEVICEPTR(device1(737), AFFINE(k*TAU/4)*AFFINE(-SEGMENT*10 + 10, 250 - 127 + SEGMENT*(i-2))));
////                }
////                
//                switch (i % 4) {
//                    case 0:
//                        fin.add(DEVICEPTR(device1(637), AFFINE(k*TAU/4)*AFFINE(-SEGMENT*10 + 10, 250 - 127 + SEGMENT*(i-4))));    break;
//                    case 1:
//                        fin.add(DEVICEPTR(device1(737), AFFINE(k*TAU/4)*AFFINE(-SEGMENT*10 + 10, 250 - 127 + SEGMENT*(i-4))));    break;
//                    case 2:
//                        fin.add(DEVICEPTR(device2(637), AFFINE(k*TAU/4)*AFFINE(-SEGMENT*10 + 10, 200 - 127 + SEGMENT*(i-4))));    break;
//                    case 3:
//                        fin.add(DEVICEPTR(device2(737), AFFINE(k*TAU/4)*AFFINE(-SEGMENT*10 + 10, 200 - 127 + SEGMENT*(i-4))));    break;
//                }
//            }
//        }
//        
//        GLdouble w = 800;
//        GLdouble b = 5000;
//        GLdouble t = .25;
//        GLdouble s = 2.5;
//        
//        //    for (int ang = 0; ang < TAU; ang += TAU/2){
//        for (int i = 0; i < 200; i += s){
//            POLYLINE p = POLYLINE();
//            
//            p += VECTOR(b-w, b-i);
//            p += VECTOR(b-w, b-i-t);
//            p += VECTOR(b-i-t, b-w);
//            p += VECTOR(b-i, b-w);
//            
//            p.close();
//            p.reverse();
//            
//            fin.add(p * AFFINE(-300,-10));
//        }
//        for (int i = 0; i < 200; i += s){
//            POLYLINE p = POLYLINE();
//            
//            p += VECTOR(b-w, b-i).perpCW();
//            p += VECTOR(b-w, b-i-t).perpCW();
//            p += VECTOR(b-i-t, b-w).perpCW();
//            p += VECTOR(b-i, b-w).perpCW();
//            
//            p.close();
//            p.reverse();
//            
//            fin.add(p * AFFINE(-300,10));
//        }
//        for (int i = 0; i < 200; i += s){
//            POLYLINE p = POLYLINE();
//            
//            p += VECTOR(b-w, b-i).perpCW().perpCW();
//            p += VECTOR(b-w, b-i-t).perpCW().perpCW();
//            p += VECTOR(b-i-t, b-w).perpCW().perpCW();
//            p += VECTOR(b-i, b-w).perpCW().perpCW();
//            
//            p.close();
//            p.reverse();
//            
//            fin.add(p * AFFINE(300,10));
//        }
//        for (int i = 0; i < 200; i += s){
//            POLYLINE p = POLYLINE();
//            
//            p += VECTOR(b-w, b-i).perpCCW();
//            p += VECTOR(b-w, b-i-t).perpCCW();
//            p += VECTOR(b-i-t, b-w).perpCCW();
//            p += VECTOR(b-i, b-w).perpCCW();
//            
//            p.close();
//            p.reverse();
//            
//            fin.add(p * AFFINE(300,-10));
//        }
//        
//        
//        auto end = std::chrono::steady_clock::now();
//        
//        printf("Devices were created in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());
//        printf("Devices have area %f um^2 = %f mm^2.\n", fin.area(), fin.area()/1e6);
//        
//        fin.exportLibraryGDS("/Users/i/Desktop/2017_08_01 Ian Christen Sandia LN Waveguide Loss.gds");
//    } else if (!round1) {
//        POLYLINE rect1 = rect(VECTOR(0,0), VECTOR(3,3));
//        POLYLINE rect2 = rect(VECTOR(1,1), VECTOR(4,4));
//        POLYLINE rect3 = rect(VECTOR(2,-1), VECTOR(5,2));
//        
//        rect1.print();
//        rect2.print();
//        
//        printf("r1a = %f\n", rect1.area());
//        printf("r2a = %f\n", rect2.area());
//        
////        fin.add(rect1);
////        fin.add(rect2);
//        
////        std::vector<POLYLINE> plines = (rect1 ^ rect2);
//////        std::vector<POLYLINE> plines = (-rect1 & rect2);
////        
////        for (int i = 0; i < plines.size(); i++) {
////            plines[i].print();
////            
////            fin.add(plines[i]);
////        }
//        
////        fin.add((rect1 | rect2) * AFFINE(0, 10) );
////        
////        fin.add((rect1 | rect2 | rect3));
//        
//        FONT f = FONT(1);
//        
////        fin.add(f.getChar('a'));
////        fin.add(f.getChar('b'));
//        fin.add(f.getText("abcdefghijklmnopqrstuvwxyz"));
//        fin.add(DEVICEPTR(f.getText("The quick brown fox jumps over the lazy dog..."), AFFINE(0,10)));
//        fin.add(DEVICEPTR(f.getText("1+2-3*4/5=6^7%8(9)0"), AFFINE(0,20)));
//        
//        printf("Finished adding...\n");
//        
//        auto end = std::chrono::steady_clock::now();
//        
//        printf("Devices were created in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());
//        printf("Devices have area %f um^2 = %f mm^2.\n", fin.area(), fin.area()/1e6);
//        
//        fin.exportLibraryGDS("/Users/i/Desktop/boolean_test.gds");
//    }
}
