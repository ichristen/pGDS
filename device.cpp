#include "device.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECTORINT::VECTORINT(VECTOR v, GLdouble scalar) {
    x = endianSwap((int32_t)round(v.x*scalar));
    y = endianSwap((int32_t)round(v.y*scalar));
}
VECTORINT::VECTORINT(VECTOR v, GLdouble scalar, AFFINE m) {
    x = endianSwap((int32_t)round((m.a*v.x + m.b*v.y + m.e)*scalar));
    y = endianSwap((int32_t)round((m.c*v.x + m.d*v.y + m.f)*scalar));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string replaceIllegalSTRNAME(std::string str) {    // Replaces invalid characters for STRNAME.
    std::string s = str.substr(0, 31);  // Limit the string to 32 characters (31 + '\0' at the end).
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) {
                       if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '?' || c == '$') {
                           return c;
                       } else {
                           return (unsigned char)'_';
                       }
                   });
    return s;
}
std::string replaceIllegalLIBNAME(std::string str) {    // Replaces invalid characters for LIBNAME. This follows UNIX filename conventions.
    std::string s = str.substr(0, 254);      // Limiting filenames to 255 characters is common?
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) {
                       if (c < 32 || c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|' || c == '\b' || c == '\t') {
                           return (unsigned char)'_';
                       } else {
                           return c;
                       }
                   });
    return s;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GDSDATE::setLocal() {
    time_t T = time(NULL);
    struct tm *t = localtime(&T);

    year =      t->tm_year + 1900;
    month =     t->tm_mon + 1;
    day =       t->tm_mday;
    hour =      t->tm_hour;
    minute =    t->tm_min;
    second =    t->tm_sec;
}
GDSDATE GDSDATE::es() {
    GDSDATE toReturn;

    toReturn.year =     endianSwap(year);
    toReturn.month =    endianSwap(month);
    toReturn.day =      endianSwap(day);
    toReturn.hour =     endianSwap(hour);
    toReturn.minute =   endianSwap(minute);
    toReturn.second =   endianSwap(second);

    return toReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEVICE::DEVICE(std::string description_) {
    description = description_;

    modification.setLocal();
    access.setLocal();
}
void DEVICE::add(POLYLINE p) {
    polylines.add(p);

    area_ += p.area();

    bb.enlarge(p.bb);
}
void DEVICE::add(POLYLINES p) {
    polylines.add(p);

    area_ += p.area();

    bb.enlarge(p.bb);
}
void DEVICE::add(std::vector<POLYLINE> p) {
    for (int i = 0; i < p.size(); i++) {
        add(p[i]);
    }
}
void DEVICE::add(DEVICEPTR device, char c) {
    devices.push_back(device);

    bb.enlarge(device.transformation * device.device->bb);

    std::map<std::string, CONNECTION> map = device.device->connections;

    for (std::map<std::string, CONNECTION>::iterator it = map.begin(); it != map.end(); ++it) {
        CONNECTION connection = it->second;
        if (c) { connection.name += c; }
        add(device.transformation * connection);
    }

    area_ += device.area();
}
void DEVICE::printConnectionNames() {
    for (std::map<std::string, CONNECTION>::iterator it = connections.begin(); it != connections.end(); ++it) {
        printf("%s\n", it->first.c_str());
    }
}
void DEVICE::add(DEVICE* device, AFFINE m, char c) {
    add(DEVICEPTR(device, m), c);
}
void DEVICE::add(CONNECTION connection) {
    if (connections.find(connection.name) == connections.end()) {    // If a connections of this key has not yet been added...
        connections[connection.name] = connection;
    } else {
        if (connections[connection.name] == connection) {
            // Do nothing.
        } else {
            connection.name = connection.name + std::string("-");
            add(connection);        // Change the key such that it does work... (change in the future?)
        }
    }
}

double DEVICE::area() {
    if (area_ == 0) {
        area_ += polylines.area();
        for (int i = 0; i < devices.size();   i++){ area_ += devices[i].area(); }
    }
    return area_;
}

bool DEVICE::initialized() {
    return polylines.size() + devices.size() > 0;
}
//bool DEVICE::unlock() {
//    isLocked = false;
//}

DEVICEPTR DEVICE::operator+(VECTOR v)   const { return DEVICEPTR((DEVICE*)this, AFFINE(v)); }
DEVICEPTR DEVICE::operator-(VECTOR v)   const { return DEVICEPTR((DEVICE*)this, AFFINE(-v)); }
DEVICEPTR DEVICE::operator*(GLdouble s) const { return DEVICEPTR((DEVICE*)this, AFFINE(s,   0, 0, s)); }
DEVICEPTR DEVICE::operator/(GLdouble s) const { return DEVICEPTR((DEVICE*)this, AFFINE(1/s, 0, 0, 1/s)); }
DEVICEPTR DEVICE::operator*(AFFINE m)   const { return DEVICEPTR((DEVICE*)this, m); }

CONNECTION DEVICE::operator[](std::string connectionName)   const {
    if (connections.find(connectionName) == connections.end()) {    // If a connection of this name has not yet been added...
        return CONNECTION();                    // Empty `CONNECTION`.
    } else {
        return connections.at(connectionName);  // `CONNECTION` corresponding to `connectionName`.
    }
}

void DEVICE::print() {
    printf("DEVICE with description: {\n");
    printf("    %s\n", description.c_str());
    printf("} containing POLYLINEs: {\n");
    for (int i = 0; i < polylines.size(); i++){ polylines[i].print(); }
    printf("} and subDEVICEPTRs: {\n");
    for (int i = 0; i < devices.size(); i++){   devices[i].print(); }
    printf("}\n");
}

bool DEVICE::exportNoStructureGDS(FILE* f, AFFINE transformation=AFFINE()) {
    for (int i = 0; i < polylines.polylines.size(); i++) {
        if (polylines.polylines[i].isCCW()) {
            // BOUNDARY
            putc(0x00, f);
            putc(0x04, f);              // LENGTH = 4 bytes

            putc(0x08, f);              // RECORD TYPE  = BOUNDARY
            putc(0x00, f);              // DATA TYPE    = null


            // LAYER
            putc(0x00, f);
            putc(0x06, f);              // LENGTH = 6 = 4 + 2 bytes

            putc(0x0D, f);              // RECORD TYPE  = LAYER
            putc(0x02, f);              // DATA TYPE    = 2-int

            uint16_t layer = endianSwap(polylines.polylines[i].layer);

            fwrite(&layer, sizeof(uint16_t), 1, f);


            // DATATYPE                 "The DATATYPE record contains unimportant information and its argument should be zero."
            putc(0x00, f);
            putc(0x06, f);              // LENGTH = 6 = 4 + 2 bytes

            putc(0x0E, f);              // RECORD TYPE  = DATATYPE
            putc(0x02, f);              // DATA TYPE    = 2-int

            putc(0x00, f);
            putc(0x00, f);


            // XY
            std::vector<VECTORINT> intPoints;

            if (transformation.isIdentity() || transformation.isZero()) {
                std::transform(polylines.polylines[i].points.begin(), polylines.polylines[i].points.end(),
                               std::back_inserter(intPoints),
                               [](VECTOR v) -> VECTORINT { return VECTORINT(v, 1000); });   // Assuming dbUnit = .001

                intPoints.push_back(VECTORINT(polylines[i].points[0], 1000));
            } else {
                std::transform(polylines.polylines[i].points.begin(), polylines.polylines[i].points.end(),
                               std::back_inserter(intPoints),
                               [transformation](VECTOR v) -> VECTORINT {
                                   return VECTORINT(v, 1000, transformation);
                               });   // Assuming dbUnit = .001

                intPoints.push_back(VECTORINT(polylines.polylines[i].points[0], 1000, transformation));
            }

            uint16_t size = endianSwap((uint16_t)(8*intPoints.size() + 4));

            fwrite(&size, sizeof(uint16_t), 1, f);

            putc(0x10, f);              // RECORD TYPE  = XY
            putc(0x03, f);              // DATA TYPE    = 4-int

            fwrite(&(intPoints[0]),   sizeof(VECTORINT), intPoints.size(), f);


            // ENDEL
            putc(0x00, f);
            putc(0x04, f);              // LENGTH = 4 bytes

            putc(0x11, f);              // RECORD TYPE  = ENDEL
            putc(0x00, f);              // DATA TYPE    = null
        }
    }

    if (!transformation.isZero()) {
        for (int i = 0; i < devices.size(); i++) {
            devices[i].device->exportNoStructureGDS(f, transformation*devices[i].transformation);
        }
    }

    return true;
}
bool DEVICE::exportLibraryGDS(std::string fname, bool flatten) {
    return exportLibraryGDS(fopen(fname.c_str(), "wb"), flatten);
}
bool DEVICE::exportLibraryGDS(FILE* f, bool flatten) {
    // References:  http://www.cnf.cornell.edu/cnf_spie9.html
    //              http://www.rulabinsky.com/cavd/text/chapc.html
    //              http://boolean.klaasholwerda.nl/interface/bnf/gdsformat.html


    if (!f) {
        printf("DEVICE::exportGDSLibrary(FILE*): File pointer is null. File cannot be written");
        return false;
    }


    auto start = std::chrono::steady_clock::now();


    // HEADER
    putc(0x00, f);
    putc(0x06, f);              // LENGTH = 6 = 4 + 2 bytes

    putc(0x00, f);              // RECORD TYPE  = HEADER
    putc(0x02, f);              // DATA TYPE    = 2-int

    uint16_t v = endianSwap((uint16_t)600);

    fwrite(&v, 2, 1, f);  // Write the version (default 600)


    // BGNLIB
    putc(0x00, f);
    putc(0x1C, f);              // LENGTH = 28 = 4 + 24 bytes

    putc(0x01, f);              // RECORD TYPE  = BGNLIB
    putc(0x02, f);              // DATA TYPE    = 2-int

    GDSDATE modification;   modification.setLocal();
    GDSDATE access;         access.setLocal();

    GDSDATE modificationES =    modification.es();
    GDSDATE accessES =          access.es();

    fwrite(&modificationES,     sizeof(GDSDATE), 1, f);
    fwrite(&accessES,           sizeof(GDSDATE), 1, f);


    // LIBDIRSIZE   (Optional)  "Contains the number of pages in the Library directory." We do not care.


    // SRFNAME      (Optional)  "Contains the name of the Sticks Rules File." We do not care.


    // LIBSECUR     (Optional)  "Contains an array of Access Control List (ACL) data." We do not care.


    // LIBNAME                  "The string must adhere to CDOS file name conventions for length and valid characters"
    std::string legalDescription = replaceIllegalLIBNAME(description);

    putc(0x00, f);
    putc((uint8_t)(legalDescription.length()+1+4), f);

    putc(0x02, f);              // RECORD TYPE  = LIBNAME
    putc(0x06, f);              // DATA TYPE    = ASCII string

    fwrite(legalDescription.c_str(),   sizeof(char), legalDescription.length()+1, f);

//    printf("description: %s\n", description.c_str());
//    printf("legalDescription: %s\n", legalDescription.c_str());


    // REFLIBS      (Optional)  Don't Care.


    // FONTS        (Optional)  Don't Care.


    // ATTRTABLE    (Optional)  Don't Care.


    // GENERATIONS  (Optional)  Don't Care.


    // FORMAT       (Optional)  Don't Care.


    // UNITS
    putc(0x00, f);
    putc(0x14, f);              // LENGTH = 20 = 4 + 16 bytes

    putc(0x03, f);              // RECORD TYPE  = UNITS
    putc(0x05, f);              // DATA TYPE    = 8-sem

    GLdouble dbUnitUser =       .001;
    GLdouble dbUnitsMeters =    1e-9;

    uint64_t dbUnitUserSEM =    num2sem(dbUnitUser);
    uint64_t dbUnitsMetersSEM = num2sem(dbUnitsMeters);

    //    printf("dbUnitUser    = %f\n", dbUnitUser);
    //    printf("dbUnitsMeters = %f\n", dbUnitsMeters);
    //
    //    printf("dbUnitUserSEM    = %llu\n", dbUnitUserSEM);
    //    printf("dbUnitsMetersSEM = %llu\n", dbUnitsMetersSEM);

    fwrite(&dbUnitUserSEM,      8, 1, f);
    fwrite(&dbUnitsMetersSEM,   8, 1, f);

    if (flatten) {
        // BGNSTR
        putc(0x00, f);
        putc(0x1C, f);              // LENGTH = 28 = 4 + 24 bytes

        putc(0x05, f);              // RECORD TYPE  = BGNSTR
        putc(0x02, f);              // DATA TYPE    = 2-int

        fwrite(&modificationES,   sizeof(GDSDATE), 1, f);
        fwrite(&accessES,         sizeof(GDSDATE), 1, f);


        // STRNAME
        legalDescription = replaceIllegalSTRNAME(description);

        putc(0x00, f);
        putc((uint8_t)(legalDescription.length()+1+4), f);

        putc(0x06, f);              // RECORD TYPE  = STRNAME
        putc(0x06, f);              // DATA TYPE    = ASCII string

        fwrite(legalDescription.c_str(),   sizeof(char), legalDescription.length()+1, f);


        exportNoStructureGDS(f);


        // ENDSTR
        putc(0x00, f);
        putc(0x04, f);              // LENGTH = 4 bytes

        putc(0x07, f);              // RECORD TYPE  = ENDSTR
        putc(0x00, f);              // DATA TYPE    = null
    }


    // ENDLIB
    putc(0x00, f);
    putc(0x04, f);              // LENGTH = 4 bytes

    putc(0x04, f);              // RECORD TYPE  = ENDLIB
    putc(0x00, f);              // DATA TYPE    = null


    fclose(f);

    auto end = std::chrono::steady_clock::now();

    printf("Devices were exported in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());

    return true;
}
bool DEVICE::importGDS(std::string fname) {
    // References:  http://www.cnf.cornell.edu/cnf_spie9.html
    //              http://www.rulabinsky.com/cavd/text/chapc.html
    //              http://boolean.klaasholwerda.nl/interface/bnf/gdsformat.html
    FILE* f = fopen(fname.c_str(), "r");

    if (!f) {
        throw std::runtime_error("File does not exist");
    }

    uint64_t header =       0;
    size_t size =           0;

    size_t bufsize =        200*8;  // Expected maximum buffer. If we ever need more, it will be allocated.
    void* buffer =          malloc(bufsize);

    POLYLINE polyline;
    DEVICE* subdevice = NULL;

    uint64_t user2dbUnits =    0;
    uint64_t db2metricUnits =  0;

    while (true) { //!feof(f)) {
                   //        unsigned long header =  fscanf(f, "%l");                // We read 8 bytes from the file.
        fread(&header, sizeof(unsigned long), 1, f);

        uint16_t length =      ((header & 0xFFFF0000) >> 32) - 4;   // The first 4 bytes are the length of record.
//        unsigned char record =  (header & 0x0000FF00) >> 16;        // The type of the record.
        uint8_t token =         (header & 0x000000FF);              // The last two bytes are the data type (token).

        uint16_t rt =           (header & 0x0000FFFF);              // The record and the token.

        if (token == 0 && length <= 0) {
            throw std::runtime_error("Header says that we should not expect data, but gives non-zero length");
            // Error; expected no data.
        }

//        if (!(length % record)) {
//            // Error; length not divisible by type.
//        }

        // Need to malloc the memory!

        switch (token) {
            case 0x00:      // (null)
                size = 0;   break;
            case 0x01:      // (2-bitarr)
                size = 2;   break;
            case 0x02:      // (2-int)
                size = 2;   break;
            case 0x03:      // (4-int)
                size = 4;   break;
            case 0x04:      // (4-real; unused)
                throw std::runtime_error("Unexpected token; 4-reals are not used in GDSII.");   break;
            case 0x05:      // (8-sem)
                size = 8;   break;
            case 0x06:      // (ASCII string)
                size = 1;   break;
        }

        if (size*length > bufsize) {
            bufsize *= 2;
            free(buffer);
            buffer = malloc(bufsize);
        }

        fread(buffer, size, length, f);

        switch (rt) {
            case 0x0002:    // HEADER       (2-int)
                            // This can be 0, 3, 4, 5, or 600. 600 means 6. It uses the 100s digit for possible subversion control (e.g. 601 == v6.0.1)
                            //                gdsVersion = *((int*)buffer);
                break;
            case 0x0102:    // BGNLIB       (2-int)     Begin Library.      BGNLIB and BGNSTR both contain the creation and modification dates of the structure.
            case 0x0502:    // BGNSTR       (2-int)     Begin Structure.
                if (length == 12) {

                    if (rt == 0x0102) {
                        modification =  ((GDSDATE*)buffer)[0];
                        access =        ((GDSDATE*)buffer)[1];
                    } else if (rt == 0x0502) {
                        subdevice = new DEVICE("");     // Create a new device without a description. Description will be added in STRNAME.

                        subdevice->modification =  ((GDSDATE*)buffer)[0];
                        subdevice->access =        ((GDSDATE*)buffer)[1];
                    }
                } else {
                    throw std::runtime_error("Expected two 12-byte dates.");
                }

                break;
            case 0x0206:    // LIBNAME      (str)       Library Name.
                description =               std::string((char*)buffer, length);     break;  // constructor std::string(char& cstr, size_t n)
            case 0x0305:    // UNITS        (sem)
                user2dbUnits =      sem2num( ((uint64_t*)buffer)[0] );
                db2metricUnits =    sem2num( ((uint64_t*)buffer)[1] );
            case 0x0400:    // ENDLIB       (null)
            case 0x0606:    // STRNAME      (str)       Structure Name.
                subdevice->description =    std::string((char*)buffer, length);     break;
            case 0x0700:    // ENDSTR       (null)      End Structure.
            case 0x0800:    // BOUNDARY     (null)
            case 0x0900:    // PATH         (null)
            case 0x0A00:    // SREF         (null)
            case 0x0B00:    // AREF         (null)
            case 0x0C00:    // TEXT         (null)
            case 0x0D02:    // LAYER        (2-int)     In [0, 63].
            case 0x0F03:    // WIDTH        (4-int)
            case 0x1003:    // XY           (4-int)
//                if (element){
//                    //                    element->add(vec2(((int*)buffer)[0], ((int*)buffer)[1]));
//                }
            case 0x1100:    // ENDEL        (null)
                return true;    // Success?
            case 0x1206:    // SNAME        (str)       Inserts the strucuture of this name. Used with sref?

            case 0x1302:    // COLROW       (2-int)

            case 0x1400:    // ?

            case 0x1500:    // NODE         (null)
            case 0x1602:    // TEXTTYPE     (4-int)     In [0, 63].
            case 0x1701:    // PRESENTATION (bitarr)

            case 0x1800:    // ?

            case 0x1906:    // STRING       (str)       Up to 512 chars long
            case 0x1A01:    // STRANS       (bitarr)
            case 0x1B05:    // MAG          (sem)
            case 0x1C05:    // REFLIBS      (sem)

            case 0x1D00:    // ?
            case 0x1E00:    // ?
            case 0x1F00:    // ?

            case 0x2006:    // FONTS        (str)
            case 0x2102:    // PATHTYPE     (2-int)
            case 0x2202:    // GENERATIONS  (2-int)
            case 0x2306:    // ATTRTABLE    (str)

            case 0x2400:    // ?
            case 0x2500:    // ?

            case 0x2601:    // EFLAGS       (bitarr)

            case 0x2700:    // ?
            case 0x2800:    // ?
            case 0x2900:    // ?

            case 0x2A02:    // NODETYPE     (2-int)    In [0, 63].
            case 0x2B02:    // PROPATTR     (2-int)
            case 0x2C06:    // PROPVALUE    (str)

                // The following records are not supported by Stream Release 3.0:

            case 0x2D00:    // BOX          (null)
            case 0x2E02:    // BOXTYPE      (2-int)
            case 0x2F03:    // PLEX         (4-int)
            case 0x3003:    // BGNEXTN      (4-int)
            case 0x3103:    // EXDEXTN      (4-int)

            case 0x3200:    // ?
            case 0x3300:    // ?
            case 0x3400:    // ?
            case 0x3500:    // ?
            case 0x3600:    // ?

            case 0x3706:    // MASK         (str)
            case 0x3800:    // ENDMASKS     (null)
            case 0x3902:    // LIBDIRSIZE   (2-int)
            case 0x3A06:    // SRFNAME      (str)
            case 0x3B02:    // LIBSECUR     (2-int)
            default:
                throw std::runtime_error("Unknown record or unknown record-token pairing.");

                break;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, DEVICE*> DEVICE::allDevices;  // Declaration of empty global device map.

DEVICE* getDevice(std::string description) {
    DEVICE d("");   // (Profanely) get a path to the static allDevices.

//    d.allDevices

    if (d.allDevices.find(description) == d.allDevices.end()) {    // If a device of this description has not yet been added...
        DEVICE* toReturn = new DEVICE(description);
        d.allDevices[description] = toReturn;
        return toReturn;
    } else {
        return d.allDevices[description];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEVICEPTR::DEVICEPTR() { device = nullptr; transformation=AFFINE(); }
DEVICEPTR::DEVICEPTR(DEVICE* device_, AFFINE transformation_) { device = device_; transformation = transformation_; }
DEVICEPTR::DEVICEPTR(DEVICEPTR const &device_, AFFINE transformation_) { device = device_.device; transformation = transformation_ * device_.transformation; }

DEVICEPTR DEVICEPTR::operator+(VECTOR v)    const { return DEVICEPTR(device, transformation + v); }
DEVICEPTR DEVICEPTR::operator-(VECTOR v)    const { return DEVICEPTR(device, transformation - v); }
DEVICEPTR DEVICEPTR::operator*(AFFINE m)    const { return DEVICEPTR(device, transformation * m); }
DEVICEPTR DEVICEPTR::operator*(GLdouble s)  const { return DEVICEPTR(device, transformation * s); }
DEVICEPTR DEVICEPTR::operator/(GLdouble s)  const { return DEVICEPTR(device, transformation / s); }

DEVICEPTR DEVICEPTR::operator+=(VECTOR v) {         transformation += v; return *this; }
DEVICEPTR DEVICEPTR::operator-=(VECTOR v) {         transformation -= v; return *this; }
DEVICEPTR DEVICEPTR::operator*=(AFFINE m) {         transformation *= m; return *this; }
DEVICEPTR DEVICEPTR::operator*=(GLdouble s) {       transformation *= s; return *this; }
DEVICEPTR DEVICEPTR::operator/=(GLdouble s) {       transformation /= s; return *this; }

GLdouble DEVICEPTR::area() {                  return device->area() * transformation.det(); }

DEVICEPTR DEVICEPTR::copy() const {           return DEVICEPTR(device, transformation); }

void DEVICEPTR::print() const {
    printf("DEVICEPTR consisting of DEVICE: {\n");
    device->print();
    printf("} transformed by AFFINE transformation: {\n");
    transformation.print();
    printf("}\n");
}




