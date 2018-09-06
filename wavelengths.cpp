#include "wavelengths.hpp"

DCINFO::DCINFO() { a = 1; d = 2; l = 0; t = 30; };
DCINFO::DCINFO(GLdouble a_, GLdouble d_, GLdouble l_, GLdouble r_, GLdouble t_) { a = a_; d = d_; l = l_; r = r_; t = t_; };

//POLYLINES dcTest(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t, GLdouble r, GLdouble sep) {
//    std::string description = "DC a" + std::to_string(a) +
//    " d" + std::to_string(d) +
//    " L" + std::to_string(L) +
//    " a0" + std::to_string(a0) +
//    " t" + std::to_string(t) +
//    " r" + std::to_string(r) +
//    " text" + std::to_string(sep);
//    
//    DEVICE* toReturn = getDevice(description);
//    
//    if (toReturn->initialized()) { return toReturn->getLayer(1); }
//    
//    toReturn->add(directionalCoupler(a, d, L, a0, t, r, false, false));
//    
//    toReturn->printConnectionNames();
//    
//    CONNECTION ur = (*toReturn)["ur"];
//    ur.print();
//    CONNECTION ur1 = -bendHorizontal(ur, sep/2); ur1.setWidth(a0);
//    ur1.print();
//    
//    POLYLINES bend;
//    
//    connectThickenAndAdd(&bend, ur, -ur1, CIRCULAR);
//    connectThickenAndAdd(&bend, ur1, -(ur1+10), CIRCULAR);
//    
//    toReturn->add(bend);
//    toReturn->add(bend * mirrorX());
//    toReturn->add(bend * mirrorY());
//    toReturn->add(bend * AFFINE(TAU/2));
//    
//    toReturn->exportLibraryGDS("/Users/i/Desktop/dc.gds");
//    
//    return toReturn->getLayer(1);
//}

POLYLINES dcTest(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t, GLdouble r, GLdouble sep) {
    std::string description = "DC a" + std::to_string(a) +
    " d" + std::to_string(d) +
    " L" + std::to_string(L) +
    " a0" + std::to_string(a0) +
    " t" + std::to_string(t) +
    " r" + std::to_string(r) +
    " text" + std::to_string(sep);
    
    DEVICE* toReturn = getDevice(description);
    
    if (toReturn->initialized()) { return toReturn->getLayer(1); }
    
//    toReturn->add(directionalCoupler(a, d, L, a0, t, r, false, false));
////
////    toReturn->printConnectionNames();
//    
////    CONNECTION base = CONNECTION(VECTOR(0, d/2), VECTOR(1,0), a);
////    
////    CONNECTION base2 = base + L/2;
////    
////    CONNECTION ur = bendRadius(base2, t*TAU/360, r);
//    
//    CONNECTION ur = (*toReturn)["ur"];
////    ur.print();
//    CONNECTION ur1 = -bendHorizontal(ur, sep/2); ur1.setWidth(a0);
////    ur1.print();
//    
//    POLYLINES bend;
//    
//    connectThickenAndAdd(&bend, base, -base2, CIRCULAR);
//    connectThickenAndAdd(&bend, base2, -ur, CIRCULAR);
//    connectThickenAndAdd(&bend, ur, -ur1, CIRCULAR);
//    connectThickenAndAdd(&bend, ur1, -(ur1+10), CIRCULAR);
//    
////    base.setWidth(  -2*PADDING-base.w);
////    base2.setWidth( -2*PADDING-base2.w);
////    ur.setWidth(    -2*PADDING-ur.w);
////    ur1.setWidth(   -2*PADDING-ur1.w);
////    
////    connectThickenAndAdd(&bend, base, -base2, CIRCULAR);
////    connectThickenAndAdd(&bend, base2, -ur, CIRCULAR);
////    connectThickenAndAdd(&bend, ur, -ur1, CIRCULAR);
////    connectThickenAndAdd(&bend, ur1, -(ur1+10), CIRCULAR);
//    
//    toReturn->add(bend);
////    toReturn->add(bend * mirrorX());
////    toReturn->add(bend * mirrorY());
////    toReturn->add(bend * AFFINE(TAU/2));
//    
//    toReturn->exportLibraryGDS("/Users/i/Desktop/MPB/me/me.gds");
    
    for (int i = 1; i <= 40; i++) {
        GLdouble x = .0025*i;
        
        DEVICE* dc = directionalCoupler(a+x, d, L, a0+x, t, r, false, false);
        
        toReturn->add(dc->getLayer(1,i));
        
        CONNECTION ur = (*dc)["ur"]; ur.setLayer(i);
        ur.print();
        CONNECTION ur1 = -bendHorizontal(ur, sep/2); ur1.setWidth(a0+x); ur1.setLayer(i);
        
        POLYLINES bend;
        
//        connectThickenAndAdd(&bend, base, -base2, CIRCULAR);
//        connectThickenAndAdd(&bend, base2, -ur, CIRCULAR);
        connectThickenAndAdd(&bend, ur, -ur1, CIRCULAR);
        connectThickenAndAdd(&bend, ur1, -(ur1+10), CIRCULAR);
        
//        bend.setLayer(1,i);
        
        toReturn->add(bend);
        toReturn->add(bend * mirrorX());
        toReturn->add(bend * mirrorY());
        toReturn->add(bend * AFFINE(TAU/2));
        
//        toReturn->eraseConnection("ur");
    }
    
    toReturn->print();
    
    toReturn->exportLibraryGDS("/Users/i/Desktop/MPB/me/me.gds");
    toReturn->exportLibraryGDS("/Users/i/Desktop/dc.gds");
    
    return toReturn->getLayer(1);
}

DEVICEPTR directionalCoupler(WAVELENGTH wl, GLdouble trans, GLdouble t, bool negResist) {
    DCINFO dc = wl.directionalCouplers[trans];
    
    if (t == 0) { t = dc.t; }

//    return DEVICEPTR(directionalCoupler((negResist?-1:1)*dc.a, dc.d, dc.l, wl.sm, t*360/TAU, dc.r, false, false));
    return DEVICEPTR(directionalCoupler((negResist?-1:1)*dc.a, dc.d, dc.l, wl.sm, t, dc.r, false, false));
}
DEVICE* directionalCoupler(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t, GLdouble r, bool text, bool straights) {
    std::string description = "DC a" + std::to_string(a) +
    " d" + std::to_string(d) +
    " L" + std::to_string(L) +
    " a0" + std::to_string(a0) +
    " t" + std::to_string(t) +
    " r" + std::to_string(r) +
    " text" + std::to_string(text) +
    " straights" + std::to_string(straights);

    DEVICE* toReturn = getDevice(description);

    if (toReturn->initialized()) {
        return toReturn;
    }

    bool negResist = false;
    if (a < 0) { a = -a; negResist = true;  printf("NEG RESIST\n"); }
    else {                                  printf("POS RESIST\n"); }
    
    

    CONNECTION connection;

    if (negResist) {
        VECTOR end = VECTOR(L/2, 0);
        VECTOR up1 = VECTOR(0, (d-a)/2);
        VECTOR up2 = VECTOR(0, (d+a)/2);
        VECTOR up3 = VECTOR(0, (d+a)/2 + PADDING);

        toReturn->add(rect(-end-up1, end+up1).setLayer(1));  // Middle rectangle
        toReturn->add(rect(-end+up2, end+up3).setLayer(1));  // Upper rectangle
        toReturn->add(rect(-end-up3, end-up2).setLayer(1));  // Lower rectangle

        GLdouble paddingInnerR =    r - a/2 - PADDING;
        GLdouble wgInnerR =         r - a/2;
        GLdouble wgOuterR =         r + a/2;
        GLdouble paddingOuterR =    r + a/2 + PADDING;

        GLdouble radiusOffset =     r + d/2;

        POLYLINE arc_ = arc(paddingInnerR, DEG2RAD*270, DEG2RAD*(270+t));

        AFFINE c = AFFINE(L/2, radiusOffset);

        GLdouble t2 = acos( radiusOffset / paddingOuterR );

        //    arc_.print();
        //
        //    ( arc_*( (r-(a/2)-PADDING)/(r-(a/2)) ) ).print();

        POLYLINE ur =   ( (arc_ - arc_*( wgInnerR/paddingInnerR )) * c).close().setLayer(1);

        POLYLINE mur =   arc_*( wgOuterR/paddingInnerR ) - arc(paddingOuterR, DEG2RAD*(270)+t2, DEG2RAD*(270+t));

        POLYLINE mr = ((mur * c) - (mur*mirrorX() * AFFINE(L/2, -radiusOffset))).setLayer(1);

        //    mr.print();

        //    mr.reverse();

        //    mr.print();
        //
        mr.close();

        //    ur.print();

        //    (arc_ + ur).close().print();

        toReturn->add(-ur);
        toReturn->add((ur*mirrorX()).reverse());
        toReturn->add((ur*mirrorY()).reverse());
        toReturn->add(-ur*-1);

        toReturn->add(-mr);
        toReturn->add((mr*mirrorY()).reverse());

        //    printf("ang=%f\n", DEG2RAD*(270+t)/TAU);

        VECTOR v = VECTOR(L/2, radiusOffset) + VECTOR(r, DEG2RAD*(270+t), true);//
                                                                                //    v.printNL();

        connection =     CONNECTION(v, VECTOR(1, DEG2RAD*t, true), -a, "ur");
        CONNECTION connection2;

        if (straights) { connection2 =    connection + connection.dv*5; connection2.w = -a0; }
        else { connection2 = connection; }

        //    connectThickenAndAdd(toReturn, connection, -connection2, CIRCULAR, true);

        toReturn->add(connection2);
        toReturn->add((mirrorX()*connection2).setName("lr"));
        toReturn->add((mirrorY()*connection2).setName("ul"));
        toReturn->add((connection2*(-1)).setName("ll"));
    } else {
        CONNECTION base = CONNECTION(VECTOR(L/2, d/2), VECTOR(1,0), a);

        connectThickenAndAdd(toReturn, -base, base - L, CIRCULAR, true);
        connectThickenAndAdd(toReturn, mirrorX() * -base, mirrorX() * base - L, CIRCULAR, true);

        CONNECTION base2 = bendRadius(base, DEG2RAD*t, r);
        connectThickenAndAdd(toReturn, base,                -base2,             CIRCULAR, true);    toReturn->add(base2.setName("ur"));
        connectThickenAndAdd(toReturn, mirrorX() * base,    mirrorX() * -base2, CIRCULAR, true);    toReturn->add((mirrorX() * base2).setName("lr"));
        connectThickenAndAdd(toReturn, mirrorY() * base,    mirrorY() * -base2, CIRCULAR, true);    toReturn->add((mirrorY() * base2).setName("ul"));
        connectThickenAndAdd(toReturn, mirrorX() * mirrorY() * base,  mirrorX() * mirrorY() * -base2, CIRCULAR, true);  toReturn->add((mirrorX() * mirrorY() * base2).setName("ll"));
    }

    // Clean this up.
    if (straights) {
        connection = toReturn->connections["ur"]; connection.w = -a; connection -= connection.dv*5;
        connectThickenAndAdd(toReturn, connection, -toReturn->connections["ur"], CIRCULAR, true);
        connection = toReturn->connections["lr"]; connection.w = -a; connection -= connection.dv*5;
        connectThickenAndAdd(toReturn, connection, -toReturn->connections["lr"], CIRCULAR, true);
        connection = toReturn->connections["ul"]; connection.w = -a; connection -= connection.dv*5;
        connectThickenAndAdd(toReturn, connection, -toReturn->connections["ul"], CIRCULAR, true);
        connection = toReturn->connections["ll"]; connection.w = -a; connection -= connection.dv*5;
        connectThickenAndAdd(toReturn, connection, -toReturn->connections["ll"], CIRCULAR, true);
    }

    if (text) {
        char str[24];
        snprintf(str, 24, "a=%i, d=%i, L=%i", (int)(a*1000), (int)(d*1000), (int)L);

        toReturn->add(DEVICEPTR(MATERIAL::layers[1].font.getText(str), AFFINE(0, +14)));
    }

    return toReturn;
}

WAVELENGTH::WAVELENGTH() { };
WAVELENGTH::WAVELENGTH(GLdouble nm_, GLdouble neff_, GLdouble sm_, GLdouble fc_, GLdouble rad_) {
    nm =    nm_;
    neff =  neff_;
    sm =    sm_ + CORRECTION;
    fc =    fc_ + CORRECTION;
    rad =   rad_;
}
void WAVELENGTH::setLoss(GLdouble s, GLdouble l, GLdouble n, GLdouble d) {
    lossS = s;
    lossL = l;
    lossN = n;
    lossD = d;
}
void WAVELENGTH::setEO(GLdouble w, GLdouble s, GLdouble l) {
    eoW = w;
    eoS = s;
    eoL = l;
}
void WAVELENGTH::makeDC(GLdouble trans, GLdouble a, GLdouble d, GLdouble l, GLdouble r, GLdouble t) {
    directionalCouplers[trans] = DCINFO(a + CORRECTION, d, l, r, t);

    //DEVICEPTR(directionalCoupler(a, d, l, sm, 30, rad, false, false));
}

WAVELENGTHS::WAVELENGTHS() : wavelengths {
    // BASE     nm      neff    sm      fc      rad
    WAVELENGTH( 369.5,  2.1152, 0.300,  3.5,    16),
    WAVELENGTH( 420.3,  2.0548, 0.325,  3.5,    16),
    WAVELENGTH( 532.0,  1.9106, 0.375,  4.0,    18),
    WAVELENGTH( 575.0,  1.8714, 0.400,  4.5,    18),
    WAVELENGTH( 637.2,  1.8241, 0.450,  5.6,    20),
    WAVELENGTH( 737.0,  1.7629, 0.525,  5.6,    20),
    WAVELENGTH( 946.0,  1.6730, 0.650,  5.8,    26),
    WAVELENGTH( 1550.0, 1.5457, 1.300,  0.2,    30) }
{
    // LOSS                 step    len     N       d
    wavelengths[0].setLoss( 300,    800,    2,      .075);
    wavelengths[1].setLoss( 500,    800,    2,      .125);
    wavelengths[2].setLoss( 1250,   1000,   3,      .200);
    wavelengths[3].setLoss( 1250,   1000,   3,      .225);
    wavelengths[4].setLoss( 1250,   1000,   3,      .250);
    wavelengths[5].setLoss( 1250,   1000,   3,      .325);
    wavelengths[6].setLoss( 1250,   1000,   3,      .450);
    wavelengths[7].setLoss( 1250,   1000,   3,      .800);

    // EO                   w       s       l (umV)
    wavelengths[0].setEO(   0.600,  1.200,  2900);
    wavelengths[1].setEO(   0.600,  1.400,  3900);  // (3.2);  0.6, 1.2 best at (3.5)
    wavelengths[2].setEO(   0.800,  1.800,  6400);  // (5.4);  0.8, 1.4 best at (6.6)
    wavelengths[3].setEO(   0.800,  1.800,  6900);  // (6.8);  1.0, 1.6 best at (8.3)
    wavelengths[4].setEO(   0.800,  1.800,  7700);  // (9.3);  0.9, 1.6 best at (10.9)
    wavelengths[5].setEO(   0.800,  1.800,  8900);  // (14.4); 1.1, 1.8 best at (16.3)
    wavelengths[6].setEO(   0.800,  1.800, 11400);  // (30.2); 1.0, 1.8 best at (32.5)

    // DC                   trans   a       d       l       r
//    wavelengths[0].makeDC(  .5,     0.300,  0.300,  1.000,  16);

//    wavelengths[1].makeDC(  .5,     0.195,  0.275,  0.932,  16);     // 7/20
//    wavelengths[1].makeDC(  .5,     0.155,  0.3,  0.960,  16, 12.5);     // 7/26
//    wavelengths[1].makeDC(  .5,     0.180,  0.300,  0.932,  16);     // 7/13

//    wavelengths[1].makeDC(  .51,    0.150,  0.300,  0.638,  20);     // 7/26
//    0.26, 0.8, 0, 0.325
//    wavelengths[1].makeDC(  .51,    0.260,  0.600,  0,  80);     // 9/04
//    wavelengths[1].makeDC(  .51,    0.175,  0.275,  0.070,  20);     // 7/20
//    wavelengths[1].makeDC(  .51,    0.170,  0.300,  0.615,  20);     // 7/15

//    wavelengths[2].makeDC(  .5,     0.300,  0.300,  1.000,  18);
//    wavelengths[3].makeDC(  .5,     0.180,  0.400,  0.909,  18);     // 7/13
//    wavelengths[3].makeDC( 1.5,     0.190,  0.450,  13.14,  18);     // 7/25
//    wavelengths[4].makeDC(  .5,     0.205,  0.450,  1.035,  20);     // 7/20
//    wavelengths[4].makeDC(  .8,     0.205,  0.475,  3.774,  20);     // 7/25
//    wavelengths[5].makeDC(  .5,     0.240,  0.550,  1.947,  20);     // 7/13
    
//    wavelengths[4].makeDC(  .5,     0.210,  0.450,  1.005,  20);     // 7/13
//    wavelengths[6].makeDC(  .5,     0.300,  0.300,  1.000,  26);
//    wavelengths[7].makeDC(  .5,     0.300,  0.300,  1.000,  30);
    
    wavelengths[1].makeDC(  .51,    0.260,  0.600,  0,  80, 12.5);     // 9/04
    wavelengths[1].makeDC(  .5,     0.260,  0.600,  0,  80, 12.5);     // 9/04
    
    wavelengths[2].makeDC(  .5,     0.200,  0.620,  0,  80, 12.5);     // 9/04
    wavelengths[3].makeDC(  .5,     0.200,  0.620,  0,  80, 12.5);     // 9/04
    wavelengths[4].makeDC(  .5,     0.200,  0.630,  0,  80, 12.5);     // 9/04
    wavelengths[5].makeDC(  .5,     0.200,  0.630,  0,  80, 12.5);     // 9/04
    
    wavelengths[2].makeDC(  .8,     0.200,  0.630,  0,  80, 12.5);     // 9/04
    wavelengths[3].makeDC(  .8,     0.200,  0.640,  0,  80, 12.5);     // 9/04
//    wavelengths[4].makeDC(  .8,     0.200,  0.645,  0,  80, 12.5);     // 9/04
    wavelengths[4].makeDC(  .8,     0.200,  0.645,  0,  80, 12.5);     // 9/04
    wavelengths[5].makeDC(  .8,     0.250,  0.600,  0,  80, 12.5);     // 9/04
    
    wavelengths[1].makeDC(  1.5,    0.260,  0.600,  0,   80, 12.5);     // 9/04
    wavelengths[2].makeDC(  1.5,    0.200,  0.620,  30,  80, 12.5);     // 9/04
    wavelengths[3].makeDC(  1.5,    0.200,  0.620,  25,  80, 12.5);     // 9/04
    wavelengths[4].makeDC(  1.5,    0.200,  0.630,  20,  80, 12.5);     // 9/04
    wavelengths[5].makeDC(  1.5,    0.250,  0.600,  17,  80, 12.5);     // 9/04
    
    wavelengths[3].makeDC(  .1,     0.200,  1,  0,  80, 12.5);     // 9/04
}

WAVELENGTH WAVELENGTHS::getWavelength(int nm) {
    if (nm < NUMWAVELENGTHS) {
        return wavelengths[nm];
    }

    for (int i = 0; i < NUMWAVELENGTHS; i++) {
        if (abs(wavelengths[i].nm - nm) <= 1) {
            return wavelengths[i];
        }
    }

    return WAVELENGTH();
}