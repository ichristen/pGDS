#include "device.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECTORINT::VECTORINT() {
    x = 0;
    y = 0;
}
VECTORINT::VECTORINT(VECTOR v, GLdouble scalar) {
    x = endianSwap((int32_t)round(v.x*scalar));
    y = endianSwap((int32_t)round(v.y*scalar));
}
VECTORINT::VECTORINT(VECTOR v, GLdouble scalar, AFFINE m) {
    x = endianSwap((int32_t)round((m.a*v.x + m.b*v.y + m.e)*scalar));
    y = endianSwap((int32_t)round((m.c*v.x + m.d*v.y + m.f)*scalar));
}

VECTOR int2vec(VECTORINT vi, GLdouble scalar) {
    return VECTOR(endianSwap(vi.x)/scalar, endianSwap(vi.y)/scalar);
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
    if (connections.find(connection.name) == connections.end()) {    // If a connection of this key has not yet been added...
        connections[connection.name] = connection;
    } else {
//        if (connections[connection.name] == connection) {
//            // Do nothing.
//        } else {
//            connection.name = connection.name + std::string("-");
//            add(connection);        // Change the key such that it does work... (change in the future?)
//        }
    }
}

POLYLINES DEVICE::getLayer(uint16_t l1) const {
    return getLayer(l1, l1);
}
POLYLINES DEVICE::getLayer(uint16_t l1, uint16_t l2) const {
    POLYLINES toReturn = polylines.getLayer(l1);
    
//    toReturn.print();
    
    for (int i = 0; i < devices.size(); i++){   toReturn.add( devices[i].device->getLayer(l1) * devices[i].transformation ); }
//    for (int i = 0; i < polylines.size(); i++){
//        if (polylines[i].layer == l1) {         toReturn.add( polylines[i] ); }
//    }
    
    if (l1 != l2) { toReturn.setLayer(l2); }
    
    return toReturn;
}
//POLYLINES DEVICE::removeLayer(uint16_t l) {
//
//}
//POLYLINES DEVICE::setLayer(uint16_t from, uint16_t to) {
//
//}
//POLYLINES DEVICE::exchangeLayers(uint16_t l1, uint16_t l2) {
//
//}

void DEVICE::setLayer(uint16_t layer) {
//    for (int i = 0; i < devices.size();     i++){ devices[i].device->setLayer(layer); }
    for (int i = 0; i < polylines.size();   i++){ polylines[i].setLayer(layer); }
}
void DEVICE::setConnectionName(std::string prev, std::string next) {
    CONNECTION c = operator[](prev);
    
    if (c.isEmpty()) {
        printf("DEVICE::setConnectionName(std::string^2): Connection name '%s' does not exist.", prev.c_str());
    } else {
        c.name = next;
        add(c);
        eraseConnection(prev);
    }
}
void DEVICE::eraseConnection(std::string name) {
    connections.erase(name);
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
        if (!polylines.polylines[i].isCCW()) { polylines.polylines[i].reverse(); }
//        if () {
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
                               [](VECTOR v) -> VECTORINT { return VECTORINT(v, DBUNIT); });   // Assuming dbUnit = .001

                intPoints.push_back(VECTORINT(polylines[i].points[0], DBUNIT));
            } else {
                std::transform(polylines.polylines[i].points.begin(), polylines.polylines[i].points.end(),
                               std::back_inserter(intPoints),
                               [transformation](VECTOR v) -> VECTORINT {
                                   return VECTORINT(v, DBUNIT, transformation);
                               });   // Assuming dbUnit = .001

                intPoints.push_back(VECTORINT(polylines.polylines[i].points[0], DBUNIT, transformation));
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
            
#ifdef DEVICE_DEBUG
            for (int j = 0; j < polylines.polylines[i].size(); j++){
                // TEXT
                putc(0x00, f);
                putc(0x04, f);              // LENGTH = 4 bytes
            
                putc(0x0C, f);              // RECORD TYPE  = TEXT
                putc(0x00, f);              // DATA TYPE    = null
                
                // LAYER
                putc(0x00, f);
                putc(0x06, f);              // LENGTH = 6 = 4 + 2 bytes
                
                putc(0x0D, f);              // RECORD TYPE  = LAYER
                putc(0x02, f);              // DATA TYPE    = 2-int
                
                uint16_t layer = endianSwap(-1);
                
                fwrite(&layer, sizeof(uint16_t), 1, f);
                
                // TEXTTYPE
                putc(0x00, f);
                putc(0x06, f);              // LENGTH = 6 = 4 + 2 bytes
                
                putc(0x16, f);              // RECORD TYPE  = TEXTTYPE
                putc(0x02, f);              // DATA TYPE    = 2-int
                
                uint16_t texttype = endianSwap(0);
                
                fwrite(&texttype, sizeof(uint16_t), 1, f);
                
                // XY
                uint16_t size = endianSwap((uint16_t)(8 + 4));
                
                fwrite(&size, sizeof(uint16_t), 1, f);
                
                putc(0x10, f);              // RECORD TYPE  = XY
                putc(0x03, f);              // DATA TYPE    = 4-int
                
                VECTORINT v = VECTORINT(polylines.polylines[i].points[j], DBUNIT, transformation);
                
                fwrite(&v,   sizeof(VECTORINT), 1, f);
            
                // STRING
                char str[8];    // This will break at > "[99999]"
                sprintf(str, "[%i]", j);
                
                putc(0x00, f);
                putc((uint8_t)(8+4), f);
            
                putc(0x19, f);              // RECORD TYPE  = STRING
                putc(0x06, f);              // DATA TYPE    = ASCII string
            
                fwrite(str,   sizeof(char), 8, f);
            
                // ENDEL
                putc(0x00, f);
                putc(0x04, f);              // LENGTH = 4 bytes
            
                putc(0x11, f);              // RECORD TYPE  = ENDEL
                putc(0x00, f);              // DATA TYPE    = null
            }
#endif
        }
//    }

    if (!transformation.isZero()) {
        for (int i = 0; i < devices.size(); i++) {
            devices[i].device->exportNoStructureGDS(f, transformation*devices[i].transformation);
        }
    }

    return true;
}
bool DEVICE::exportLibraryGDS(std::string fname, bool flatten) {
//#ifdef MATLAB_MEX_FILE
//    mexPrintf("HERE!\n");
//    
//    FILE* f1 = fopen("/Users/i/Desktop/hyper1.gds", "w");  mexPrintf("1 - 0x%X\n", f1);
//    FILE* f2 = fopen("/Users/i/Desktop/hyper2.gds", "wb"); mexPrintf("2 - 0x%X\n", f2);
//    FILE* f3 = fopen("/Users/i/Desktop/hyper3.gds", "w+"); mexPrintf("3 - 0x%X\n", f3);
//    
//    mexPrintf("HERE!\n");
//    
//    mexPrintf("%s\n", fname.c_str());
//    
//#endif
    
    // return exportLibraryGDS(fopen("/Users/i/Desktop/hyper.gds", "wb"), flatten);
    return exportLibraryGDS(fopen(fname.c_str(), "w+"), flatten);
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

    GLdouble dbUnitUser =       1.0/DBUNIT;
    GLdouble dbUnitsMeters =    (1e-6)/DBUNIT;
    
    // printf("dbUnitUser = %.10f\n",     dbUnitUser);
    // printf("dbUnitsMeters = %.10f\n",  dbUnitsMeters);

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

    uint32_t header =       0;
    size_t size =           0;

    size_t bufsize =        200*8;  // Expected maximum buffer. If we ever need more, it will be allocated.
    void* buffer =          malloc(bufsize);

    POLYLINE polyline;
    DEVICE* subdevice = NULL;

    uint64_t user2dbUnits =    0;
    uint64_t db2metricUnits =  0;

    while (true) {
        
        std::vector<VECTORINT> intPoints;
        
        fread(&header, sizeof(uint32_t), 1, f);
        
        header = endianSwap(header);
        
        uint16_t length =      ((header & 0xFFFF0000) >> 16) - 4;   // The first 4 bytes are the length of record.
        uint8_t record =       ((header & 0x0000FF00) >> 8);       // The type of the record.
        uint8_t token =         (header & 0x000000FF);              // The last two bytes are the data type (token).
        
        uint16_t rt =           (header & 0x0000FFFF);              // The record and the token.
        
        printf("Head = 0x%X, Record = 0x%X, Token = 0x%X, Length = 0x%X = %i\n", header, record, token, length, length);

        if (token == 0 && length > 0) {
            throw std::runtime_error("DEVICE::importGDS(std::string): Header says that we should not expect data, but gives non-zero length");
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
                throw std::runtime_error("DEVICE::importGDS(std::string): Unexpected token; 4-reals are not used in GDSII.");   break;
            case 0x05:      // (8-sem)
                size = 8;   break;
            case 0x06:      // (ASCII string)
                size = 1;   break;
        }

        if (size*length > bufsize) {
            while (size*length > bufsize) { bufsize *= 2; }
            
            free(buffer);
            buffer = malloc(bufsize);
        }

        if (length) { fread(buffer, size, length/size, f); }

        switch (record) {
            case 0x00:    // HEADER       (2-int)
                            // This can be 0, 3, 4, 5, or 600. 600 means 6. It uses the 100s digit for possible subversion control (e.g. 601 == v6.0.1)
                            //                gdsVersion = *((int*)buffer);
                break;
            case 0x01:    // BGNLIB       (2-int)     Begin Library.      BGNLIB and BGNSTR both contain the creation and modification dates of the structure.
            case 0x05:    // BGNSTR       (2-int)     Begin Structure.
                if (length == 24) {

                    if (record == 0x01) {
                        modification =  ((GDSDATE*)buffer)[0];
                        access =        ((GDSDATE*)buffer)[1];
                    } else if (record == 0x05) {
                        subdevice = new DEVICE("");     // Create a new device without a description. Description will be added in STRNAME.

                        subdevice->modification =  ((GDSDATE*)buffer)[0];
                        subdevice->access =        ((GDSDATE*)buffer)[1];
                    }
                } else {
                    throw std::runtime_error("DEVICE::importGDS(std::string): Expected two 12-byte dates.");
                }

                break;
            case 0x02:    // LIBNAME      (str)       Library Name.
                description =               std::string((char*)buffer, length);     break;  // constructor std::string(char& cstr, size_t n)
            case 0x03:    // UNITS        (sem)
                user2dbUnits =      sem2num( ((uint64_t*)buffer)[0] );
                db2metricUnits =    sem2num( ((uint64_t*)buffer)[1] );
                break;
            case 0x04:    // ENDLIB       (null)
//                polylines.print();
//                polylines.bb.print();
                bb.enlarge(polylines.bb);
                return true;
//                break;
            case 0x06:    // STRNAME      (str)       Structure Name.
                subdevice->description =    std::string((char*)buffer, length);     break;
            case 0x07:    // ENDSTR       (null)      End Structure.
                break;
            case 0x08:    // BOUNDARY     (null)
                if (!polyline.isEmpty()) { throw std::runtime_error("Did not expect nested boundaries..."); }
            case 0x09:    // PATH         (null)
                break;
            case 0x0A:    // SREF         (null)
                break;
            case 0x0B:    // AREF         (null)
                break;
            case 0x0C:    // TEXT         (null)
                break;
            case 0x0D:    // LAYER        (2-int)     In [0, 63].
                polyline.setLayer( endianSwap( *((uint16_t*)buffer) ) ); break;
            case 0x0E:    // DATATYPE     (4-int)
                break;
            case 0x0F:    // WIDTH        (4-int)
                break;
            case 0x10:    // XY           (4-int)
//                std::vector<VECTORINT> intPoints; //((VECTORINT*)buffer, length/size);
//                intPoints.clear();
//                intPoints.resize(length/size);
//                intPoints.resize(length/size/2 - 1, VECTORINT());
//                printf("SIZE0=%i\n", intPoints.size());
//                intPoints.reserve(length/size/2 - 1);
                intPoints.resize(length/size/2 - 1, VECTORINT());
//                printf("SIZE1=%i\n", intPoints.size());
                memcpy(&(intPoints[0]), buffer, length - 2*size);
                
                std::transform(intPoints.begin(), intPoints.begin() + length/size/2 - 1,
                               std::back_inserter(polyline.points),
                               [](VECTORINT v) -> VECTOR { return int2vec(v, DBUNIT); });   // Assuming dbUnit = .001

                polyline.close();
//                polyline.print();
//                printf("AREA = %f\n", polyline.area());
                
                if (polyline.area() < 0) { polyline.reverse(); }
                
                polyline.recomputeBoundingBox();
                
                polylines.add(polyline);
                
                break;
            case 0x11:    // ENDEL        (null)
                if (polyline.isEmpty()) { throw std::runtime_error("Expected a boundary to have been written..."); }
                else                    { polyline.clear(); }
                    
                break;
            case 0x12:    // SNAME        (str)       Inserts the strucuture of this name. Used with sref?

            case 0x13:    // COLROW       (2-int)

            case 0x14:    // ?

            case 0x15:    // NODE         (null)
            case 0x16:    // TEXTTYPE     (4-int)     In [0, 63].
            case 0x17:    // PRESENTATION (bitarr)

            case 0x18:    // ?

            case 0x19:    // STRING       (str)       Up to 512 chars long
            case 0x1A:    // STRANS       (bitarr)
            case 0x1B:    // MAG          (sem)
            case 0x1C:    // REFLIBS      (sem)

            case 0x1D:    // ?
            case 0x1E:    // ?
            case 0x1F:    // ?

            case 0x20:    // FONTS        (str)
            case 0x21:    // PATHTYPE     (2-int)
            case 0x22:    // GENERATIONS  (2-int)
            case 0x23:    // ATTRTABLE    (str)

            case 0x24:    // ?
            case 0x25:    // ?

            case 0x26:    // EFLAGS       (bitarr)

            case 0x27:    // ?
            case 0x28:    // ?
            case 0x29:    // ?

            case 0x2A:    // NODETYPE     (2-int)    In [0, 63].
            case 0x2B:    // PROPATTR     (2-int)
            case 0x2C:    // PROPVALUE    (str)

                // The following records are not supported by Stream Release 3.0:

            case 0x2D:    // BOX          (null)
            case 0x2E:    // BOXTYPE      (2-int)
            case 0x2F:    // PLEX         (4-int)
            case 0x30:    // BGNEXTN      (4-int)
            case 0x31:    // EXDEXTN      (4-int)

            case 0x32:    // ?
            case 0x33:    // ?
            case 0x34:    // ?
            case 0x35:    // ?
            case 0x36:    // ?

            case 0x37:    // MASK         (str)
            case 0x38:    // ENDMASKS     (null)
            case 0x39:    // LIBDIRSIZE   (2-int)
            case 0x3A:    // SRFNAME      (str)
            case 0x3B:    // LIBSECUR     (2-int)
                
            default:
                printf("DEVICE::importGDS(std::string): Unknown record or unknown record-token pairing: 0x%X\n", rt);
//                throw std::runtime_error();

                break;
        }
    }

    return true;
}
//POLYLINES DEVICE::getLayer(uint8_t l) {
//    POLYLINES toReturn;
//
//    for (int i = 0; i < polylines.polylines.size(); i++) {
//        if (polylines.polylines[i].layer == l) { toReturn.add(polylines.polylines[i]); }
//    }
//
//    for (int i = 0; i < devices.size(); i++) {
//        toReturn.add(devices[i].device->getLayer(l) * devices[i].transformation);
//    }
//
//    return toReturn;
//}

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
DEVICEPTR DEVICEPTR::operator*(AFFINE m)    const { return DEVICEPTR(device, m * transformation); }
DEVICEPTR DEVICEPTR::operator*(GLdouble s)  const { return DEVICEPTR(device, transformation * s); }
DEVICEPTR DEVICEPTR::operator/(GLdouble s)  const { return DEVICEPTR(device, transformation / s); }

DEVICEPTR DEVICEPTR::operator+=(VECTOR v) {         transformation += v; return *this; }
DEVICEPTR DEVICEPTR::operator-=(VECTOR v) {         transformation -= v; return *this; }
DEVICEPTR DEVICEPTR::operator*=(AFFINE m) {         transformation = m * transformation; return *this; }
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




