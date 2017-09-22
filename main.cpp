//#define USE_FT_FONT 1
#define USE_GL_RENDER 1

#include "testdevices.hpp"

int main(int argc, const char * argv[]) {
    mainDevice(1);
    
//    auto start = std::chrono::steady_clock::now();
//    
//    DEVICE* toReturn =  getDevice("main");
//    
//    DEVICE* rect2 =      getDevice("rect");
//    
//    CONNECTION c = CONNECTION(VECTOR(.5,2), VECTOR(0,1), .5, "top");
//    
//    rect2->add(rect(VECTOR(0,0), VECTOR(1,2)));
//    rect2->add(c);
//    
//    toReturn->add(rect2, AFFINE(TAU/8, -10,0), 'a');
//    toReturn->add(rect2, AFFINE(10,0), 'b');
//    
//    toReturn->printConnectionNames();
//    
//    connectThickenAndAdd(toReturn, toReturn->connections["topa"], toReturn->connections["topb"], CIRCULAR);
//    
//    auto end = std::chrono::steady_clock::now();
//    
//    printf("Devices were created in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());
//    printf("Devices have area %f um^2 = %f mm^2.\n", toReturn->area(), toReturn->area()/1e6);
//    
//    toReturn->exportLibraryGDS("/Users/i/Desktop/Sandia/GDS/test.gds");

}
