#ifndef DEVICE_HPP
#define DEVICE_HPP

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

#ifdef MATLAB_MEX_FILE
#include <mex.h>
#endif

#define DBUNIT 1000

//#define DEVICE_DEBUG 1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper structures/functions for GDSII writing...

struct VECTORINT {  // Vectors are stored as integers in GDSII. This helper structure converts our `GLdouble` `VECTOR`s to integers.
    int32_t x;
    int32_t y;
    
    VECTORINT();
    VECTORINT(VECTOR v, GLdouble scalar);
    VECTORINT(VECTOR v, GLdouble scalar, AFFINE m);
};

VECTOR int2vec(VECTORINT vi, GLdouble scalar);

struct GDSDATE {    // Helper struture encapsulating the GDSII date structure.
    int16_t year;
    int16_t month;
    int16_t day;
    int16_t hour;
    int16_t minute;
    int16_t second;
    
    void setLocal();
    GDSDATE es();   // Endian swap.
};

//std::string replaceIllegal(std::string str, size_t maxLength=std::string::npos);    // Removes illegal characters
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FONT;         // Forward declaration of `FONT` for use in `DEVICE`.
class DEVICEPTR;    // Forward declaration of `DEVICEPTR` for use in `DEVICE`.

class DEVICE {
public:
    uint16_t version = 600;
    
    bool exported = false;
    
    static std::map<std::string, DEVICE*> allDevices;
    
    POLYLINES               polylines;
    std::vector<DEVICEPTR>  devices;
    
    BOUNDINGBOX bb;
    
    std::map<std::string, CONNECTION> connections;
    
    std::string description;
    
    std::string generatingCode;
    std::string generatingFunction;
    
//    bool isLocked = true;
//    bool hasRendered = false;
    
    double area_ = 0;
    
    GDSDATE modification;   // Date of last modification (not really supported)
    GDSDATE access;         // Date of last access (not really supported)
    
    DEVICE(std::string description_);   // Makes an empty device with the appropriate `description`. Note that there should only be *one* device object for each `description`
    
    void add(POLYLINE p);                                   // Add a single polyline to our device.
    void add(POLYLINES p);                                  // Add a set of polylines to our device.
    void add(std::vector<POLYLINE> p);                      // Add a vector of polylines to our device.
    void add(DEVICEPTR device, char c=0);                   // Add a pointer to a transformed device. Suffix all `CONNECTION` in this device with `c` (does not suffix if `c == 0`). Note that all `CONNECTION`s are appropriately transformed.
    void add(DEVICE* device, AFFINE m=AFFINE(), char c=0);  // Same as above.
    void add(CONNECTION connection);                        // Add a `CONNECTION`. If a `CONNECTION` of this name already exists, replace the existing `CONNECTION`.
    
    POLYLINES getLayer(uint16_t l1) const;
    POLYLINES getLayer(uint16_t l1, uint16_t l2) const;
//    POLYLINES removeLayer(uint16_t l);
//    POLYLINES setLayer(uint16_t from, uint16_t to);
//    POLYLINES exchangeLayers(uint16_t l1, uint16_t l2);
    
    void setLayer(uint16_t layer);
    void setConnectionName(std::string prev, std::string next);
    void eraseConnection(std::string name);
    
//    void render();
//    void flush();
    
    double area();                          // Returns the summed and transformed area of this device.
//    void refreshConnections();
//    void checkOverlap();
    
    bool initialized();                     // Returns whether `POLYLINE`s or `DEVICEPTR` have already been added to this device.
//    void unlock();
    
    DEVICEPTR operator+(VECTOR v)   const;  // Returns a pointer to this device, translated by `v`.
    DEVICEPTR operator-(VECTOR v)   const;  // Returns a pointer to this device, translated by `v`.
    DEVICEPTR operator*(GLdouble s) const;  // Returns a pointer to this device, scaled by `s`.
    DEVICEPTR operator/(GLdouble s) const;  // Returns a pointer to this device, scaled by `1/s`.
    DEVICEPTR operator*(AFFINE m)   const;  // Returns a pointer to this device, transformed by `m`.
    
    CONNECTION operator[](std::string connectionName)       const;  // Returns the `CONNECTION` corresponding to `connectionName`. If no correspondence, returns empty `CONNECTION`.
    CONNECTION getConnection(std::string connectionName)    const;  // Same as above.
    
    void print();                                                   // Prints the device description, along with the contained `POLYLINE`s and `DEVICEPTR`s.
    void printConnectionNames();                                    // Prints the names of the contained `CONNECTION`s.
    
    bool importGDS(std::string fname);                              // Unfinished import function.
    
    bool exportLibraryGDS(std::string fname, bool flatten=true);    // Export as .gds...
    bool exportLibraryGDS(FILE* f, bool flatten=true);              // Generally for internal use only...
    
//    POLYLINES getLayer(uint8_t l);
    
private:
    bool exportStructureGDS(FILE* f);                               // For internal use only...
    bool exportNoStructureGDS(FILE* f, AFFINE tranformation);       // For internal use only...
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEVICE* getDevice(std::string description); // Accesses the `static` variable inside `DEVICE` to retrieve the pointer to the device with description `description`. If no device exists, a pointer to an empty device with description `description` is returned.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DEVICEPTR {
public:
    DEVICE* device;
    
    AFFINE transformation;
    
    DEVICEPTR();
    DEVICEPTR(DEVICE* device_,          AFFINE transformation_=AFFINE());
    DEVICEPTR(DEVICEPTR const &device_, AFFINE transformation_=AFFINE());
    
    DEVICEPTR operator+(VECTOR v)   const;  // Transforms affine `transformation` appropriately.
    DEVICEPTR operator-(VECTOR v)   const;  // Same as above.
    DEVICEPTR operator+=(VECTOR v);         // Same as above.
    DEVICEPTR operator-=(VECTOR v);         // Same as above.
    
    DEVICEPTR operator*(AFFINE m)   const;  // Transforms affine `transformation` appropriately.
    DEVICEPTR operator*(GLdouble s) const;  // Same as above.
    DEVICEPTR operator/(GLdouble s) const;  // Same as above.
    DEVICEPTR operator*=(AFFINE m);         // Same as above.
    DEVICEPTR operator*=(GLdouble s);       // Same as above.
    DEVICEPTR operator/=(GLdouble s);       // Same as above.
    
    CONNECTION operator[](std::string connectionName)       const;  // Returns the `CONNECTION` corresponding to `connectionName`. If no correspondence, returns empty `CONNECTION`.
    CONNECTION getConnection(std::string connectionName)    const;  // Same as above.
    
    AFFINE getTransformation()      const;  // Getter for transformation.
    void setTransformation(AFFINE m);       // Setter for transformation.
    
    bool isEmpty()                  const;  // Returns whether device == nullptr.
    
    DEVICEPTR copy()                const;  // Makes a copy of this transformation of the device pointer.
    
    GLdouble area();                        // Returns the area of the device, multiplied by a scaling factor corresponding with the affine `transformation`.
    
    BOUNDINGBOX bb()                const;  // Returns the transformed bounding box of the device.
    
    void print()                    const;  // Prints the transformation and the pointed device.
    
    void render(AFFINE m, bool fill=true, bool outline=true);
};

#include "font.hpp"

#endif
