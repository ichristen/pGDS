#ifndef DEVICE_H
#define DEVICE_H

#include <cstdlib>
#include <cstdio>

#include <cmath>

#include <vector>
#include <algorithm>
#include <map>
#include <new>
#include <string>
#include <bitset>
#include <chrono>

#include "polyline.hpp"
#include "vector.hpp"
#include "math.hpp"

struct VECTORINT {
    int32_t x;
    int32_t y;
    
    VECTORINT(VECTOR v, GLdouble scalar);
    VECTORINT(VECTOR v, GLdouble scalar, AFFINE m);
};

std::string replaceIllegal(std::string str, size_t maxLength=std::string::npos);
std::string replaceIllegalLIBNAME(std::string str);
std::string replaceIllegalSTRNAME(std::string str);

inline uint16_t endianSwap(uint16_t x) {
    return  ( (x & 0x00FF) << 8 ) |
            ( (x & 0xFF00) >> 8 );
}
inline uint32_t endianSwap(uint32_t x) {
    return  ( (x & 0x000000FF) << 24 ) |
            ( (x & 0x0000FF00) << 8  ) |
            ( (x & 0x00FF0000) >> 8  ) |
            ( (x & 0xFF000000) >> 24 );
}
inline uint64_t endianSwap(uint64_t x) {
    return  ( (x & 0x00000000000000FF) << 56 ) |
            ( (x & 0x000000000000FF00) << 40 ) |
            ( (x & 0x0000000000FF0000) << 24 ) |
            ( (x & 0x00000000FF000000) << 8  ) |
            ( (x & 0x000000FF00000000) >> 8  ) |
            ( (x & 0x0000FF0000000000) >> 24 ) |
            ( (x & 0x00FF000000000000) >> 40 ) |
            ( (x & 0xFF00000000000000) >> 56 );
}

inline int16_t endianSwap(int16_t x) {
    return  ( (x & 0x00FF) << 8 ) |
            ( (x & 0xFF00) >> 8 );
}
inline int32_t endianSwap(int32_t x) {
    return  ( (x & 0x000000FF) << 24 ) |
            ( (x & 0x0000FF00) << 8  ) |
            ( (x & 0x00FF0000) >> 8  ) |
            ( (x & 0xFF000000) >> 24 );
}
inline int64_t endianSwap(int64_t x) {
    return  ( (x & 0x00000000000000FF) << 56 ) |
            ( (x & 0x000000000000FF00) << 40 ) |
            ( (x & 0x0000000000FF0000) << 24 ) |
            ( (x & 0x00000000FF000000) << 8  ) |
            ( (x & 0x000000FF00000000) >> 8  ) |
            ( (x & 0x0000FF0000000000) >> 24 ) |
            ( (x & 0x00FF000000000000) >> 40 ) |
            ( (x & 0xFF00000000000000) >> 56 );
}

struct GDSDATE {        // Struture encapsulating the GDSII date structure.
    int16_t year;
    int16_t month;
    int16_t day;
    int16_t hour;
    int16_t minute;
    int16_t second;
    
    void setLocal();
    GDSDATE es();
};

class DEVICE;

class DEVICEPTR {    // Transformed device
public:
    DEVICE* device;
    
    AFFINE transformation;
    
    DEVICEPTR();
    DEVICEPTR(DEVICE* device_, AFFINE transformation_=AFFINE());
    
    DEVICEPTR operator+(VECTOR v)     const;
    DEVICEPTR operator-(VECTOR v)     const;
    DEVICEPTR operator*(AFFINE m)     const;
    DEVICEPTR operator*(GLdouble s)   const;
    DEVICEPTR operator/(GLdouble s)   const;
    
    DEVICEPTR operator+=(VECTOR v);
    DEVICEPTR operator-=(VECTOR v);
    DEVICEPTR operator*=(AFFINE m);
    DEVICEPTR operator*=(GLdouble s);
    DEVICEPTR operator/=(GLdouble s);
    
    GLdouble area();
    
    void print()                    const;
};

class FONT;

class DEVICE {
public:
    uint16_t version = 600;
    
    static std::map<std::string, DEVICE*> allDevices;
//    static FONT labelFont;
    
//    std::vector<POLYLINE>   polylines;
    POLYLINES               polylines;
    std::vector<DEVICEPTR>    devices;
    
    BOUNDINGBOX bb;
    
    std::map<std::string, CONNECTION> connections;
    
    std::string description;
    
    bool hasRendered = false;
    
    double area_ = 0;
    
    GDSDATE modification;
    GDSDATE access;
    
    DEVICE(std::string description_);
    
//    void* operator new (size_t size);
    
//    bool operator!();
    
    void add(POLYLINE p);
    void add(POLYLINES p);
    void add(std::vector<POLYLINE> p);
    void add(DEVICEPTR device, char c=0);
    void add(DEVICE* device, AFFINE m=AFFINE());
    void add(CONNECTION connection);
    
//    void render();
//    void flush();
    
    double area();
//    void refreshConnections();
//    void checkOverlap();
    
    bool initialized();
    
    void print();
    void printKeys();
    
    bool importGDS(std::string fname);
    
    //    bool exportGDS(FILE* f, AFFINE tranformation);
    bool exportLibraryGDS(std::string fname, bool flatten=true);
    bool exportLibraryGDS(FILE* f, bool flatten=true);
    bool exportStructureGDS(FILE* f);
    bool exportNoStructureGDS(FILE* f, AFFINE tranformation);
};

inline GLdouble sem2num(uint64_t sem) {
    return          (sem & 0x8000000000000000)?(-1):(1) *           // Sign.
    std::pow(16, (((sem & 0x7F00000000000000) >> 56) - 64)) *       // Exponent.
    (sem & 0x00FFFFFFFFFFFFFF);                     // Mantissa.
}

inline uint64_t num2sem(GLdouble num) {
    uint8_t exponent = 64;
    
    while (num < .0625 && exponent > 0) {
        num *= 16;
        exponent -= 1;
    }
    
    if (exponent == 0) {
        printf("Error: exponent less than 0");
    }
    
    return  (num == 0)?(0):( (num <  0)?(0x0000000000000080):(0) | (uint64_t)exponent | endianSwap((uint64_t)round( num * std::pow(2, 56) )) );
}

DEVICE* getDevice(std::string description);

#include "font.hpp"

#endif
