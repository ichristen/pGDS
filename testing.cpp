#include "testing.hpp"

void testAll() {
    testPrimitives();
    testBooleans();
}

void testPrimitives() {
    
}
void testBooleans() {
    std::string description = "Bool Test";
    
    DEVICE* toReturn = getDevice(description);  // if (toReturn->initialized()) { return toReturn; }
    
    auto start = std::chrono::steady_clock::now();
    
    GLdouble y = 0;
    GLdouble s = 12;
    
    POLYLINE a, b;
    
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                a = rect(VECTOR( 1, 1), 4);
                b = rect(VECTOR(-1,-1), 2);
//                a = rect(VECTOR( 2, 2), 6);
//                b = rect(VECTOR(-2,-2), 4);
                break;
            case 1:
                a = rect(VECTOR( 2, 2), 6);
                b = rect(VECTOR(-2,-2), 4);
                break;
            case 2:
                a = circle(3, VECTOR( 1, 1), 81);
                b = circle(2, VECTOR(-1,-1), 80);
                break;
            case 3:
                a = circle(3, VECTOR( 1, 1), 10);
                b = circle(2, VECTOR(-1,-1), 10);
                break;
        }
        
        toReturn->add(a         * AFFINE(1*s,y));
//        printf("\n\na & b ===== ===== ===== ===== ===== ===== ===== ===== ===== =====\n\n");
        toReturn->add((a & b)   * AFFINE(2*s,y));
//        printf("\n\na | b ===== ===== ===== ===== ===== ===== ===== ===== ===== =====\n\n");
        toReturn->add((a | b)   * AFFINE(3*s,y));
//        printf("\n\na ^ b ===== ===== ===== ===== ===== ===== ===== ===== ===== =====\n\n");
        toReturn->add((a ^ b)   * AFFINE(4*s,y));
//        printf("\n\n-a & b ===== ===== ===== ===== ===== ===== ===== ===== ===== =====\n\n");
        toReturn->add((-a &  b)   * AFFINE(5*s,y));
//        printf("\n\na & -b ===== ===== ===== ===== ===== ===== ===== ===== ===== =====\n\n");
        toReturn->add(( a & -b)   * AFFINE(6*s,y));
        toReturn->add(b.setLayer(1) * AFFINE(1*s,y));
        
        y += s;
    }
    
    auto end = std::chrono::steady_clock::now();
    
    printf("Devices were created in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());
    printf("Devices have area %f um^2 = %f mm^2.\n", toReturn->area(), toReturn->area()/1e6);
    
    toReturn->exportLibraryGDS("/Users/i/Desktop/Sandia/GDS/2017_01_03 " + description + ".gds");
}




