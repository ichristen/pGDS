#ifdef MATLAB_MEX_FILE
    #include "mex.h"
#endif

#include "class_handle.hpp"

#include "testdevices.hpp"
#include "vector.hpp"
#include "math.hpp"
#include "polyline.hpp"
#include "device.hpp"
#include "font.hpp"
#include "primitives.hpp"
#include "material.hpp"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    if (nlhs < 0 || nrhs < 0) {  mexErrMsgTxt("mex_bridge.cpp: Something has gone horribly wrong; expected natural numbers to be non-negative."); }

    if (nlhs > 1) {  mexErrMsgTxt("mex_bridge.cpp: C functions cannot return more than one value."); }
    if (nrhs < 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected at least one argument."); }

    int cmd = (int)*mxGetPr(prhs[0]);

    if (cmd == 1) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<VECTOR>(prhs[1]);

        return;
    }
    if (cmd == 2) { // VECTOR(GLdouble x_=0, GLdouble y_=0, bool isCylindrical=false); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 1 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 1 and 4 arguments."); }

        if (nrhs == 1) {
            // GLdouble x_ = 0;
            // GLdouble y_ = 0;
            // bool isCylindrical = false;

            plhs[0] = convertPtr2Mat<VECTOR>( new VECTOR() );
        }
        if (nrhs == 2) {
            GLdouble x_ = (GLdouble)*mxGetPr(prhs[1]);
            // GLdouble y_ = 0;
            // bool isCylindrical = false;

            plhs[0] = convertPtr2Mat<VECTOR>( new VECTOR(x_) );
        }
        if (nrhs == 3) {
            GLdouble x_ = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble y_ = (GLdouble)*mxGetPr(prhs[2]);
            // bool isCylindrical = false;

            plhs[0] = convertPtr2Mat<VECTOR>( new VECTOR(x_, y_) );
        }
        if (nrhs == 4) {
            GLdouble x_ = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble y_ = (GLdouble)*mxGetPr(prhs[2]);
            bool isCylindrical = (bool)*mxGetPr(prhs[3]);

            plhs[0] = convertPtr2Mat<VECTOR>( new VECTOR(x_, y_, isCylindrical) );
        }

        return;
    }
    if (cmd == 3) { // bool isZero() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        bool toReturn = ptr->isZero();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 4) { // bool operator!() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        bool toReturn = ptr->operator!();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 5) { // bool operator==(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->operator==(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 6) { // bool operator!=(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->operator!=(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 7) { // bool operator< (VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->operator<(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 8) { // bool operator<=(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->operator<=(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 9) { // bool operator> (VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->operator>(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 10) { // bool operator>=(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->operator>=(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 11) { // bool inLine(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->inLine(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 12) { // VECTOR operator=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->operator=(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 13) { // VECTOR operator-() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR toReturn = ptr->operator-();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 14) { // VECTOR operator+(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->operator+(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 15) { // VECTOR operator-(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->operator-(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 16) { // VECTOR operator*(GLdouble factor) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble factor = (GLdouble)*mxGetPr(prhs[2]);

        VECTOR toReturn = ptr->operator*(factor);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 17) { // VECTOR operator/(GLdouble factor) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble factor = (GLdouble)*mxGetPr(prhs[2]);

        VECTOR toReturn = ptr->operator/(factor);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 18) { // VECTOR operator+=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->operator+=(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 19) { // VECTOR operator-=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->operator-=(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 20) { // VECTOR operator*=(GLdouble factor); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble factor = (GLdouble)*mxGetPr(prhs[2]);

        VECTOR toReturn = ptr->operator*=(factor);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 21) { // VECTOR operator/=(GLdouble factor); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble factor = (GLdouble)*mxGetPr(prhs[2]);

        VECTOR toReturn = ptr->operator/=(factor);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 22) { // VECTOR operator*(AFFINE m) - Equivalent to m*v
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        VECTOR toReturn = ptr->operator*(*m);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 23) { // VECTOR operator*=(AFFINE m); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        VECTOR toReturn = ptr->operator*=(*m);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 24) { // GLdouble magn() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble toReturn = ptr->magn();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 25) { // GLdouble magn2() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble toReturn = ptr->magn2();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 26) { // void unitEquals(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        ptr->unitEquals();

        return;
    }
    if (cmd == 27) { // VECTOR unit() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR toReturn = ptr->unit();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 28) { // bool isUnit() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        bool toReturn = ptr->isUnit();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 29) { // void zero(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        ptr->zero();

        return;
    }
    if (cmd == 30) { // VECTOR perpCCW() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR toReturn = ptr->perpCCW();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 31) { // VECTOR perpCW() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR toReturn = ptr->perpCW();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 32) { // GLdouble operator*(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        GLdouble toReturn = ptr->operator*(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 33) { // GLdouble dot(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        GLdouble toReturn = ptr->dot(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 34) { // GLdouble perpCCWdot(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        GLdouble toReturn = ptr->perpCCWdot(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 35) { // GLdouble cross(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        GLdouble toReturn = ptr->cross(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 36) { // VECTOR rotate(GLdouble radians) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble radians = (GLdouble)*mxGetPr(prhs[2]);

        VECTOR toReturn = ptr->rotate(radians);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 37) { // void rotateEquals(GLdouble radians); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble radians = (GLdouble)*mxGetPr(prhs[2]);

        ptr->rotateEquals(radians);

        return;
    }
    if (cmd == 38) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 39) { // void printNL() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        ptr->printNL();

        return;
    }
    if (cmd == 40) { // void render() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        ptr->render();

        return;
    }
    if (cmd == 41) { // void render(GLdouble x_, GLdouble y_) - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        GLdouble x_ = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble y_ = (GLdouble)*mxGetPr(prhs[3]);

        ptr->render(x_, y_);

        return;
    }
    if (cmd == 42) { // void render(VECTOR v) - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        ptr->render(*v);

        return;
    }
    if (cmd == 43) { // void glVertex() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        VECTOR* ptr = convertMat2Ptr<VECTOR>(prhs[1]);

        ptr->glVertex();

        return;
    }
    if (cmd == 44) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<LABEL>(prhs[1]);

        return;
    }
    if (cmd == 45) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<CONNECTION>(prhs[1]);

        return;
    }
    if (cmd == 46) { // CONNECTION(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<CONNECTION>( new CONNECTION() );

        return;
    }
    if (cmd == 47) { // CONNECTION(VECTOR v_, VECTOR dv_, GLdouble w_=1, std::string name_="Connection"); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 5 arguments."); }

        if (nrhs == 3) {
            VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* dv_ = convertMat2Ptr<VECTOR>(prhs[2]);
            // GLdouble w_ = 1;
            // std::string name_ = "Connection";

            plhs[0] = convertPtr2Mat<CONNECTION>( new CONNECTION(*v_, *dv_) );
        }
        if (nrhs == 4) {
            VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* dv_ = convertMat2Ptr<VECTOR>(prhs[2]);
            GLdouble w_ = (GLdouble)*mxGetPr(prhs[3]);
            // std::string name_ = "Connection";

            plhs[0] = convertPtr2Mat<CONNECTION>( new CONNECTION(*v_, *dv_, w_) );
        }
        if (nrhs == 5) {
            VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* dv_ = convertMat2Ptr<VECTOR>(prhs[2]);
            GLdouble w_ = (GLdouble)*mxGetPr(prhs[3]);
            char str4[64];
            mxGetString(prhs[4], str4, 64);
            std::string name_ = std::string(str4);

            plhs[0] = convertPtr2Mat<CONNECTION>( new CONNECTION(*v_, *dv_, w_, name_) );
        }

        return;
    }
    if (cmd == 48) { // CONNECTION operator=(CONNECTION c); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        CONNECTION* c = convertMat2Ptr<CONNECTION>(prhs[2]);

        CONNECTION toReturn = ptr->operator=(*c);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 49) { // CONNECTION operator-() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        CONNECTION toReturn = ptr->operator-();
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 50) { // CONNECTION operator+ (VECTOR v_) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[2]);

        CONNECTION toReturn = ptr->operator+(*v_);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 51) { // CONNECTION operator+=(VECTOR v_); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[2]);

        CONNECTION toReturn = ptr->operator+=(*v_);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 52) { // CONNECTION operator- (VECTOR v_) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[2]);

        CONNECTION toReturn = ptr->operator-(*v_);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 53) { // CONNECTION operator-=(VECTOR v_); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        VECTOR* v_ = convertMat2Ptr<VECTOR>(prhs[2]);

        CONNECTION toReturn = ptr->operator-=(*v_);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 54) { // CONNECTION operator* (GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        CONNECTION toReturn = ptr->operator*(s);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 55) { // CONNECTION operator*=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        CONNECTION toReturn = ptr->operator*=(s);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 56) { // CONNECTION operator/ (GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        CONNECTION toReturn = ptr->operator/(s);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 57) { // CONNECTION operator/=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        CONNECTION toReturn = ptr->operator/=(s);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 58) { // CONNECTION operator* (AFFINE m) - Equivalent to m*c
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        CONNECTION toReturn = ptr->operator*(*m);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 59) { // CONNECTION operator*=(AFFINE m); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        CONNECTION toReturn = ptr->operator*=(*m);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 60) { // bool operator==(CONNECTION c) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        CONNECTION* c = convertMat2Ptr<CONNECTION>(prhs[2]);

        bool toReturn = ptr->operator==(*c);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 61) { // CONNECTION setName(std::string name_); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        char str2[64];
        mxGetString(prhs[2], str2, 64);
        std::string name_ = std::string(str2);

        CONNECTION toReturn = ptr->setName(name_);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 62) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 63) { // CONNECTION copy() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        CONNECTION toReturn = ptr->copy();
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 64) { // void render() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        CONNECTION* ptr = convertMat2Ptr<CONNECTION>(prhs[1]);

        ptr->render();

        return;
    }
    if (cmd == 65) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<AFFINE>(prhs[1]);

        return;
    }
    if (cmd == 66) { // AFFINE(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE() );

        return;
    }
    if (cmd == 67) { // AFFINE(double radians); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        double radians = (double)*mxGetPr(prhs[1]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(radians) );

        return;
    }
    if (cmd == 68) { // AFFINE(double radians, VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        double radians = (double)*mxGetPr(prhs[1]);
        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(radians, *v) );

        return;
    }
    if (cmd == 69) { // AFFINE(double radians, GLdouble e_, GLdouble f_); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        double radians = (double)*mxGetPr(prhs[1]);
        GLdouble e_ = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble f_ = (GLdouble)*mxGetPr(prhs[3]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(radians, e_, f_) );

        return;
    }
    if (cmd == 70) { // AFFINE(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[1]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(*v) );

        return;
    }
    if (cmd == 71) { // AFFINE(GLdouble e_, GLdouble f_); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        GLdouble e_ = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble f_ = (GLdouble)*mxGetPr(prhs[2]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(e_, f_) );

        return;
    }
    if (cmd == 72) { // AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 6) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 6 arguments."); }

        GLdouble a_ = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble b_ = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble c_ = (GLdouble)*mxGetPr(prhs[3]);
        GLdouble d_ = (GLdouble)*mxGetPr(prhs[4]);
        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[5]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(a_, b_, c_, d_, *v) );

        return;
    }
    if (cmd == 73) { // AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, GLdouble e_=0, GLdouble f_=0); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 5 || nrhs > 7) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 5 and 7 arguments."); }

        if (nrhs == 5) {
            GLdouble a_ = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble b_ = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble c_ = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble d_ = (GLdouble)*mxGetPr(prhs[4]);
            // GLdouble e_ = 0;
            // GLdouble f_ = 0;

            plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(a_, b_, c_, d_) );
        }
        if (nrhs == 6) {
            GLdouble a_ = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble b_ = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble c_ = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble d_ = (GLdouble)*mxGetPr(prhs[4]);
            GLdouble e_ = (GLdouble)*mxGetPr(prhs[5]);
            // GLdouble f_ = 0;

            plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(a_, b_, c_, d_, e_) );
        }
        if (nrhs == 7) {
            GLdouble a_ = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble b_ = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble c_ = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble d_ = (GLdouble)*mxGetPr(prhs[4]);
            GLdouble e_ = (GLdouble)*mxGetPr(prhs[5]);
            GLdouble f_ = (GLdouble)*mxGetPr(prhs[6]);

            plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(a_, b_, c_, d_, e_, f_) );
        }

        return;
    }
    if (cmd == 74) { // AFFINE(CONNECTION from, CONNECTION to); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        CONNECTION* from = convertMat2Ptr<CONNECTION>(prhs[1]);
        CONNECTION* to = convertMat2Ptr<CONNECTION>(prhs[2]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(*from, *to) );

        return;
    }
    if (cmd == 75) { // AFFINE(BOUNDINGBOX from, BOUNDINGBOX to); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* from = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);
        BOUNDINGBOX* to = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        plhs[0] = convertPtr2Mat<AFFINE>( new AFFINE(*from, *to) );

        return;
    }
    if (cmd == 76) { // bool isIdentity() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        bool toReturn = ptr->isIdentity();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 77) { // bool isZero() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        bool toReturn = ptr->isZero();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 78) { // VECTOR operator*(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->operator*(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 79) { // VECTOR linearTimes(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        VECTOR toReturn = ptr->linearTimes(*v);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 80) { // CONNECTION operator*(CONNECTION c) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        CONNECTION* c = convertMat2Ptr<CONNECTION>(prhs[2]);

        CONNECTION toReturn = ptr->operator*(*c);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 81) { // CONNECTION linearTimes(CONNECTION c) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        CONNECTION* c = convertMat2Ptr<CONNECTION>(prhs[2]);

        CONNECTION toReturn = ptr->linearTimes(*c);
        CONNECTION* toReturnPtr = new CONNECTION();
        memcpy(toReturnPtr, &toReturn, sizeof(CONNECTION));
        plhs[0] = convertPtr2Mat<CONNECTION>(toReturnPtr);

        return;
    }
    if (cmd == 82) { // BOUNDINGBOX operator*(BOUNDINGBOX bb) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator*(*bb);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 83) { // AFFINE operator*(AFFINE m) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        AFFINE toReturn = ptr->operator*(*m);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 84) { // AFFINE operator*=(AFFINE m); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        AFFINE toReturn = ptr->operator*=(*m);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 85) { // AFFINE operator+(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        AFFINE toReturn = ptr->operator+(*v);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 86) { // AFFINE operator-(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        AFFINE toReturn = ptr->operator-(*v);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 87) { // AFFINE operator*(GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        AFFINE toReturn = ptr->operator*(s);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 88) { // AFFINE operator/(GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        AFFINE toReturn = ptr->operator/(s);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 89) { // AFFINE operator+=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        AFFINE toReturn = ptr->operator+=(*v);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 90) { // AFFINE operator-=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        AFFINE toReturn = ptr->operator-=(*v);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 91) { // AFFINE operator*=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        AFFINE toReturn = ptr->operator*=(s);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 92) { // AFFINE operator/=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        AFFINE toReturn = ptr->operator/=(s);
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 93) { // VECTOR translation() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        VECTOR toReturn = ptr->translation();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 94) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 95) { // GLdouble det() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        GLdouble toReturn = ptr->det();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 96) { // AFFINE inv() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        AFFINE toReturn = ptr->inv();
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 97) { // bool islinear() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        bool toReturn = ptr->islinear();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 98) { // void glTransform() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        AFFINE* ptr = convertMat2Ptr<AFFINE>(prhs[1]);

        ptr->glTransform();

        return;
    }
    if (cmd == 99) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<BOUNDINGBOX>(prhs[1]);

        return;
    }
    if (cmd == 100) { // BOUNDINGBOX(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<BOUNDINGBOX>( new BOUNDINGBOX() );

        return;
    }
    if (cmd == 101) { // BOUNDINGBOX(VECTOR a); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        VECTOR* a = convertMat2Ptr<VECTOR>(prhs[1]);

        plhs[0] = convertPtr2Mat<BOUNDINGBOX>( new BOUNDINGBOX(*a) );

        return;
    }
    if (cmd == 102) { // BOUNDINGBOX(VECTOR a, VECTOR b); - Makes the smallest bounding box that contains both a and b (a != ur, b != ll, generally).
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* a = convertMat2Ptr<VECTOR>(prhs[1]);
        VECTOR* b = convertMat2Ptr<VECTOR>(prhs[2]);

        plhs[0] = convertPtr2Mat<BOUNDINGBOX>( new BOUNDINGBOX(*a, *b) );

        return;
    }
    if (cmd == 103) { // GLdouble width(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble toReturn = ptr->width();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 104) { // GLdouble height(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble toReturn = ptr->height();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 105) { // BOUNDINGBOX copy() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX toReturn = ptr->copy();
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 106) { // void clear(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        ptr->clear();

        return;
    }
    if (cmd == 107) { // bool isEmpty(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        bool toReturn = ptr->isEmpty();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 108) { // bool doesContain(VECTOR v); - Checks whether the bounding box contains v.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->doesContain(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 109) { // bool doesContainX(GLdouble x); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble x = (GLdouble)*mxGetPr(prhs[2]);

        bool toReturn = ptr->doesContainX(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 110) { // bool doesContainY(GLdouble y); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble y = (GLdouble)*mxGetPr(prhs[2]);

        bool toReturn = ptr->doesContainY(y);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 111) { // bool doesContain(BOUNDINGBOX bb); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        bool toReturn = ptr->doesContain(*bb);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 112) { // bool enlarge(VECTOR v); - Insist that the boundary box contain v. Returns whether enlargement was neccessary.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->enlarge(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 113) { // bool enlarge(BOUNDINGBOX bb); - Insist that the boundary box contain b. Returns whether enlargement was neccessary.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        bool toReturn = ptr->enlarge(*bb);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 114) { // bool doesIntersect(BOUNDINGBOX bb); - Checks whether this bounding box intersects with another.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        bool toReturn = ptr->doesIntersect(*bb);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 115) { // BOUNDINGBOX operator&(BOUNDINGBOX bb); - Boolean and.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator&(*bb);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 116) { // BOUNDINGBOX operator&=(BOUNDINGBOX bb); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator&=(*bb);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 117) { // BOUNDINGBOX operator|(BOUNDINGBOX bb); - Boolean or.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator|(*bb);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 118) { // BOUNDINGBOX operator|=(BOUNDINGBOX bb); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator|=(*bb);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 119) { // BOUNDINGBOX operator|(VECTOR v); - Same as enlarge.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator|(*v);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 120) { // BOUNDINGBOX operator|=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator|=(*v);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 121) { // BOUNDINGBOX operator=(BOUNDINGBOX bb); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator=(*bb);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 122) { // BOUNDINGBOX operator/ (GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator/(s);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 123) { // BOUNDINGBOX operator* (GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator*(s);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 124) { // BOUNDINGBOX operator/=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator/=(s);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 125) { // BOUNDINGBOX operator*=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        BOUNDINGBOX toReturn = ptr->operator*=(s);
        BOUNDINGBOX* toReturnPtr = new BOUNDINGBOX();
        memcpy(toReturnPtr, &toReturn, sizeof(BOUNDINGBOX));
        plhs[0] = convertPtr2Mat<BOUNDINGBOX>(toReturnPtr);

        return;
    }
    if (cmd == 126) { // GLdouble area() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        GLdouble toReturn = ptr->area();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 127) { // bool operator==(BOUNDINGBOX bb) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        BOUNDINGBOX* bb = convertMat2Ptr<BOUNDINGBOX>(prhs[2]);

        bool toReturn = ptr->operator==(*bb);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 128) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        BOUNDINGBOX* ptr = convertMat2Ptr<BOUNDINGBOX>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 129) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<POLYLINE>(prhs[1]);

        return;
    }
    if (cmd == 130) { // POLYLINE(size_t size_=200); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 1 || nrhs > 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 1 and 2 arguments."); }

        if (nrhs == 1) {
            // size_t size_ = 200;

            plhs[0] = convertPtr2Mat<POLYLINE>( new POLYLINE() );
        }
        if (nrhs == 2) {
            size_t size_ = (size_t)*mxGetPr(prhs[1]);

            plhs[0] = convertPtr2Mat<POLYLINE>( new POLYLINE(size_) );
        }

        return;
    }
    if (cmd == 131) { // POLYLINE(POLYLINE p, int b, int e); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[1]);
        int b = (int)*mxGetPr(prhs[2]);
        int e = (int)*mxGetPr(prhs[3]);

        plhs[0] = convertPtr2Mat<POLYLINE>( new POLYLINE(*p, b, e) );

        return;
    }
    if (cmd == 132) { // VECTOR  operator[](int i) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        int i = (int)*mxGetPr(prhs[2]);

        VECTOR toReturn = ptr->operator[](i);
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 133) { // bool    insert(int i, VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        int i = (int)*mxGetPr(prhs[2]);
        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[3]);

        bool toReturn = ptr->insert(i, *v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 134) { // void add(VECTOR v); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        ptr->add(*v);

        return;
    }
    if (cmd == 135) { // void add(POLYLINE p); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        ptr->add(*p);

        return;
    }
    if (cmd == 136) { // void recomputeBoundingBox(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        ptr->recomputeBoundingBox();

        return;
    }
    if (cmd == 137) { // POLYLINE clip(int b, int e); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        int b = (int)*mxGetPr(prhs[2]);
        int e = (int)*mxGetPr(prhs[3]);

        POLYLINE toReturn = ptr->clip(b, e);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 138) { // POLYLINE operator-(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE toReturn = ptr->operator-();
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 139) { // POLYLINE reverse(); - Changes some flags such that the enclosed datapoints are treated as if they are reversed.
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE toReturn = ptr->reverse();
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 140) { // void makeforward(); - Actually reverses the data points if reversed is flaged. Turns off reversed flags.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        ptr->makeforward();

        return;
    }
    if (cmd == 141) { // VECTOR getBeginPoint(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR toReturn = ptr->getBeginPoint();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 142) { // VECTOR getEndPoint(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR toReturn = ptr->getEndPoint();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 143) { // VECTOR getBeginDirection(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR toReturn = ptr->getBeginDirection();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 144) { // VECTOR getEndDirection(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR toReturn = ptr->getEndDirection();
        VECTOR* toReturnPtr = new VECTOR();
        memcpy(toReturnPtr, &toReturn, sizeof(VECTOR));
        plhs[0] = convertPtr2Mat<VECTOR>(toReturnPtr);

        return;
    }
    if (cmd == 145) { // POLYLINE operator+ (VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        POLYLINE toReturn = ptr->operator+(*v);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 146) { // POLYLINE operator- (VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        POLYLINE toReturn = ptr->operator-(*v);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 147) { // POLYLINE operator+=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        POLYLINE toReturn = ptr->operator+=(*v);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 148) { // POLYLINE operator-=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        POLYLINE toReturn = ptr->operator-=(*v);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 149) { // POLYLINE operator/ (double s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINE toReturn = ptr->operator/(s);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 150) { // POLYLINE operator* (double s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINE toReturn = ptr->operator*(s);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 151) { // POLYLINE operator* (AFFINE m) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        POLYLINE toReturn = ptr->operator*(*m);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 152) { // POLYLINE operator/=(double s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINE toReturn = ptr->operator/=(s);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 153) { // POLYLINE operator*=(double s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINE toReturn = ptr->operator*=(s);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 154) { // POLYLINE operator*=(AFFINE m); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        POLYLINE toReturn = ptr->operator*=(*m);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 155) { // POLYLINE operator+ (POLYLINE p) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINE toReturn = ptr->operator+(*p);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 156) { // POLYLINE operator- (POLYLINE p) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINE toReturn = ptr->operator-(*p);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 157) { // POLYLINE operator+=(POLYLINE p); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINE toReturn = ptr->operator+=(*p);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 158) { // POLYLINE operator-=(POLYLINE p); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINE toReturn = ptr->operator-=(*p);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 159) { // POLYLINES operator&(POLYLINE p) - Boolean AND
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator&(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 160) { // POLYLINES operator|(POLYLINE p) - Boolean OR
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator|(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 161) { // POLYLINES operator^(POLYLINE p) - Boolean XOR
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator^(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 162) { // GLdouble area(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        GLdouble toReturn = ptr->area();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 163) { // size_t size() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        size_t toReturn = ptr->size();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 164) { // POLYLINE close(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE toReturn = ptr->close();
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 165) { // POLYLINE open(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE toReturn = ptr->open();
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 166) { // bool isCCW(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        bool toReturn = ptr->isCCW();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 167) { // bool isEmpty() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        bool toReturn = ptr->isEmpty();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 168) { // bool contains(VECTOR v) - Note that this imperfectly considers the boundary...
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->contains(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 169) { // GLdouble length(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        GLdouble toReturn = ptr->length();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 170) { // POLYLINE setLayer(uint16_t l); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        uint16_t l = (uint16_t)*mxGetPr(prhs[2]);

        POLYLINE toReturn = ptr->setLayer(l);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 171) { // POLYLINE copy() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        POLYLINE toReturn = ptr->copy();
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 172) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINE* ptr = convertMat2Ptr<POLYLINE>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 173) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<POLYLINES>(prhs[1]);

        return;
    }
    if (cmd == 174) { // POLYLINES(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<POLYLINES>( new POLYLINES() );

        return;
    }
    if (cmd == 175) { // POLYLINES(POLYLINE polyline); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        POLYLINE* polyline = convertMat2Ptr<POLYLINE>(prhs[1]);

        plhs[0] = convertPtr2Mat<POLYLINES>( new POLYLINES(*polyline) );

        return;
    }
    if (cmd == 176) { // POLYLINE    operator[](int i) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        int i = (int)*mxGetPr(prhs[2]);

        POLYLINE toReturn = ptr->operator[](i);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 177) { // bool        insert(int i, POLYLINE p); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        int i = (int)*mxGetPr(prhs[2]);
        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[3]);

        bool toReturn = ptr->insert(i, *p);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 178) { // void        clear(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        ptr->clear();

        return;
    }
    if (cmd == 179) { // void add(POLYLINE p); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        ptr->add(*p);

        return;
    }
    if (cmd == 180) { // void add(POLYLINES p); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        ptr->add(*p);

        return;
    }
    if (cmd == 181) { // void recomputeBoundingBox(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        ptr->recomputeBoundingBox();

        return;
    }
    if (cmd == 182) { // size_t size() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        size_t toReturn = ptr->size();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 183) { // bool isEmpty() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        bool toReturn = ptr->isEmpty();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 184) { // POLYLINES   operator/ (double s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINES toReturn = ptr->operator/(s);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 185) { // POLYLINES   operator* (double s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINES toReturn = ptr->operator*(s);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 186) { // POLYLINES   operator* (AFFINE m) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator*(*m);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 187) { // POLYLINES   operator/=(double s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINES toReturn = ptr->operator/=(s);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 188) { // POLYLINES   operator*=(double s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        double s = (double)*mxGetPr(prhs[2]);

        POLYLINES toReturn = ptr->operator*=(s);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 189) { // POLYLINES   operator*=(AFFINE m); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator*=(*m);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 190) { // POLYLINES   operator-(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES toReturn = ptr->operator-();
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 191) { // POLYLINES   reverse(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES toReturn = ptr->reverse();
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 192) { // POLYLINES   operator&(POLYLINE p) - Boolean AND
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator&(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 193) { // POLYLINES   operator|(POLYLINE p) - Boolean OR
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator|(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 194) { // POLYLINES   operator^(POLYLINE p) - Boolean XOR
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator^(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 195) { // POLYLINES   operator&=(POLYLINE p); - Boolean AND equals
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator&=(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 196) { // POLYLINES   operator|=(POLYLINE p); - Boolean OR equals
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator|=(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 197) { // POLYLINES   operator^=(POLYLINE p); - Boolean XOR equals
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        POLYLINES toReturn = ptr->operator^=(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 198) { // POLYLINES   operator&(POLYLINES p) - Boolean AND
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        POLYLINES toReturn = ptr->operator&(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 199) { // POLYLINES   operator|(POLYLINES p) - Boolean OR
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        POLYLINES toReturn = ptr->operator|(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 200) { // POLYLINES   operator^(POLYLINES p) - Boolean XOR
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        POLYLINES toReturn = ptr->operator^(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 201) { // POLYLINES   operator&=(POLYLINES p); - Boolean AND equals
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        POLYLINES toReturn = ptr->operator&=(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 202) { // POLYLINES   operator|=(POLYLINES p); - Boolean OR equals
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        POLYLINES toReturn = ptr->operator|=(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 203) { // POLYLINES   operator^=(POLYLINES p); - Boolean XOR equals
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        POLYLINES toReturn = ptr->operator^=(*p);
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 204) { // double area(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        double toReturn = ptr->area();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 205) { // bool contains(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        bool toReturn = ptr->contains(*v);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 206) { // POLYLINES copy() - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        POLYLINES toReturn = ptr->copy();
        POLYLINES* toReturnPtr = new POLYLINES();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINES));
        plhs[0] = convertPtr2Mat<POLYLINES>(toReturnPtr);

        return;
    }
    if (cmd == 207) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        POLYLINES* ptr = convertMat2Ptr<POLYLINES>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 208) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<DEVICEPTR>(prhs[1]);

        return;
    }
    if (cmd == 209) { // DEVICEPTR(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<DEVICEPTR>( new DEVICEPTR() );

        return;
    }
    if (cmd == 210) { // DEVICEPTR(DEVICE* device_, AFFINE transformation_=AFFINE()); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 2 || nrhs > 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 2 and 3 arguments."); }

        if (nrhs == 2) {
            DEVICE* device_ = convertMat2Ptr<DEVICE>(prhs[1]);
            // AFFINE* transformation_ = AFFINE();

            plhs[0] = convertPtr2Mat<DEVICEPTR>( new DEVICEPTR(device_) );
        }
        if (nrhs == 3) {
            DEVICE* device_ = convertMat2Ptr<DEVICE>(prhs[1]);
            AFFINE* transformation_ = convertMat2Ptr<AFFINE>(prhs[2]);

            plhs[0] = convertPtr2Mat<DEVICEPTR>( new DEVICEPTR(device_, *transformation_) );
        }

        return;
    }
    if (cmd == 211) { // DEVICEPTR operator+(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        DEVICEPTR toReturn = ptr->operator+(*v);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 212) { // DEVICEPTR operator-(VECTOR v) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        DEVICEPTR toReturn = ptr->operator-(*v);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 213) { // DEVICEPTR operator*(AFFINE m) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        DEVICEPTR toReturn = ptr->operator*(*m);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 214) { // DEVICEPTR operator*(GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        DEVICEPTR toReturn = ptr->operator*(s);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 215) { // DEVICEPTR operator/(GLdouble s) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        DEVICEPTR toReturn = ptr->operator/(s);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 216) { // DEVICEPTR operator+=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        DEVICEPTR toReturn = ptr->operator+=(*v);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 217) { // DEVICEPTR operator-=(VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        DEVICEPTR toReturn = ptr->operator-=(*v);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 218) { // DEVICEPTR operator*=(AFFINE m); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        AFFINE* m = convertMat2Ptr<AFFINE>(prhs[2]);

        DEVICEPTR toReturn = ptr->operator*=(*m);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 219) { // DEVICEPTR operator*=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        DEVICEPTR toReturn = ptr->operator*=(s);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 220) { // DEVICEPTR operator/=(GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        DEVICEPTR toReturn = ptr->operator/=(s);
        DEVICEPTR* toReturnPtr = new DEVICEPTR();
        memcpy(toReturnPtr, &toReturn, sizeof(DEVICEPTR));
        plhs[0] = convertPtr2Mat<DEVICEPTR>(toReturnPtr);

        return;
    }
    if (cmd == 221) { // GLdouble area(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        GLdouble toReturn = ptr->area();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 222) { // void print() - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        DEVICEPTR* ptr = convertMat2Ptr<DEVICEPTR>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 223) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<DEVICE>(prhs[1]);

        return;
    }
    if (cmd == 224) { // DEVICE(std::string description_); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        char str1[64];
        mxGetString(prhs[1], str1, 64);
        std::string description_ = std::string(str1);

        plhs[0] = convertPtr2Mat<DEVICE>( new DEVICE(description_) );

        return;
    }
    if (cmd == 225) { // void add(POLYLINE p); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        POLYLINE* p = convertMat2Ptr<POLYLINE>(prhs[2]);

        ptr->add(*p);

        return;
    }
    if (cmd == 226) { // void add(POLYLINES p); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        POLYLINES* p = convertMat2Ptr<POLYLINES>(prhs[2]);

        ptr->add(*p);

        return;
    }
    if (cmd == 227) { // void add(DEVICEPTR device, char c=0); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        if (nrhs == 3) {
            DEVICEPTR* device = convertMat2Ptr<DEVICEPTR>(prhs[2]);
            // char c = 0;

            ptr->add(*device);
        }
        if (nrhs == 4) {
            DEVICEPTR* device = convertMat2Ptr<DEVICEPTR>(prhs[2]);
            char c = (char)*mxGetPr(prhs[3]);

            ptr->add(*device, c);
        }

        return;
    }
    if (cmd == 228) { // void add(DEVICE* device, AFFINE m=AFFINE()); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        if (nrhs == 3) {
            DEVICE* device = convertMat2Ptr<DEVICE>(prhs[2]);
            // AFFINE* m = AFFINE();

            ptr->add(device);
        }
        if (nrhs == 4) {
            DEVICE* device = convertMat2Ptr<DEVICE>(prhs[2]);
            AFFINE* m = convertMat2Ptr<AFFINE>(prhs[3]);

            ptr->add(device, *m);
        }

        return;
    }
    if (cmd == 229) { // void add(CONNECTION connection); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        CONNECTION* connection = convertMat2Ptr<CONNECTION>(prhs[2]);

        ptr->add(*connection);

        return;
    }
    if (cmd == 230) { // double area(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        double toReturn = ptr->area();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 231) { // bool initialized(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        bool toReturn = ptr->initialized();
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 232) { // void print(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        ptr->print();

        return;
    }
    if (cmd == 233) { // void printKeys(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        ptr->printKeys();

        return;
    }
    if (cmd == 234) { // bool importGDS(std::string fname); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        char str2[64];
        mxGetString(prhs[2], str2, 64);
        std::string fname = std::string(str2);

        bool toReturn = ptr->importGDS(fname);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 235) { // bool exportLibraryGDS(std::string fname, bool flatten=true); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        DEVICE* ptr = convertMat2Ptr<DEVICE>(prhs[1]);

        if (nrhs == 3) {
            char str2[64];
            mxGetString(prhs[2], str2, 64);
            std::string fname = std::string(str2);
            // bool flatten = true;

            bool toReturn = ptr->exportLibraryGDS(fname);
            plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
            double* toReturnDouble = mxGetPr(plhs[0]);
            *toReturnDouble = (double)toReturn;
        }
        if (nrhs == 4) {
            char str2[64];
            mxGetString(prhs[2], str2, 64);
            std::string fname = std::string(str2);
            bool flatten = (bool)*mxGetPr(prhs[3]);

            bool toReturn = ptr->exportLibraryGDS(fname, flatten);
            plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
            double* toReturnDouble = mxGetPr(plhs[0]);
            *toReturnDouble = (double)toReturn;
        }

        return;
    }
    if (cmd == 236) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<FONT>(prhs[1]);

        return;
    }
    if (cmd == 237) { // FONT(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<FONT>( new FONT() );

        return;
    }
    if (cmd == 238) { // FONT(uint16_t l); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        uint16_t l = (uint16_t)*mxGetPr(prhs[1]);

        plhs[0] = convertPtr2Mat<FONT>( new FONT(l) );

        return;
    }
    if (cmd == 239) { // FONT(uint16_t l, GLdouble s); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        uint16_t l = (uint16_t)*mxGetPr(prhs[1]);
        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        plhs[0] = convertPtr2Mat<FONT>( new FONT(l, s) );

        return;
    }
    if (cmd == 240) { // FONT(uint16_t l, GLdouble t, GLdouble h); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        uint16_t l = (uint16_t)*mxGetPr(prhs[1]);
        GLdouble t = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble h = (GLdouble)*mxGetPr(prhs[3]);

        plhs[0] = convertPtr2Mat<FONT>( new FONT(l, t, h) );

        return;
    }
    if (cmd == 241) { // void setThick(GLdouble t); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        GLdouble t = (GLdouble)*mxGetPr(prhs[2]);

        ptr->setThick(t);

        return;
    }
    if (cmd == 242) { // void setSpace(GLdouble s); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);

        ptr->setSpace(s);

        return;
    }
    if (cmd == 243) { // void setHeight(GLdouble h); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        GLdouble h = (GLdouble)*mxGetPr(prhs[2]);

        ptr->setHeight(h);

        return;
    }
    if (cmd == 244) { // void setWidth(GLdouble w); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        GLdouble w = (GLdouble)*mxGetPr(prhs[2]);

        ptr->setWidth(w);

        return;
    }
    if (cmd == 245) { // void recalculate(); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        ptr->recalculate();

        return;
    }
    if (cmd == 246) { // DEVICE* getChar(char c); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        char c = (char)*mxGetPr(prhs[2]);

        DEVICE* toReturn = ptr->getChar(c);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 247) { // DEVICE* getText(std::string text, std::string devname=""); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        FONT* ptr = convertMat2Ptr<FONT>(prhs[1]);

        if (nrhs == 3) {
            char str2[64];
            mxGetString(prhs[2], str2, 64);
            std::string text = std::string(str2);
            // std::string devname = "";

            DEVICE* toReturn = ptr->getText(text);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 4) {
            char str2[64];
            mxGetString(prhs[2], str2, 64);
            std::string text = std::string(str2);
            char str3[64];
            mxGetString(prhs[3], str3, 64);
            std::string devname = std::string(str3);

            DEVICE* toReturn = ptr->getText(text, devname);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 248) { // delete() - Deletes pointer reference.
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 argument."); }

        destroyObject<MATERIAL>(prhs[1]);

        return;
    }
    if (cmd == 249) { // MATERIAL(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        plhs[0] = convertPtr2Mat<MATERIAL>( new MATERIAL() );

        return;
    }
    if (cmd == 250) { // MATERIAL(std::string name_, int16_t layer_, std::array<int, 3> colorFill_={255,255,255}, std::array<int, 3> colorOutline_={255,255,255}, GLdouble fontThick=1, GLdouble fontHeight=7); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        char str1[64];
        mxGetString(prhs[1], str1, 64);
        std::string name_ = std::string(str1);
        int16_t layer_ = (int16_t)*mxGetPr(prhs[2]);

        plhs[0] = convertPtr2Mat<MATERIAL>( new MATERIAL(name_, layer_) );

        return;
    }
    if (cmd == 251) { // MATERIAL getLayer(uint16_t l); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        MATERIAL* ptr = convertMat2Ptr<MATERIAL>(prhs[1]);

        uint16_t l = (uint16_t)*mxGetPr(prhs[2]);

        MATERIAL toReturn = ptr->getLayer(l);
        MATERIAL* toReturnPtr = new MATERIAL();
        memcpy(toReturnPtr, &toReturn, sizeof(MATERIAL));
        plhs[0] = convertPtr2Mat<MATERIAL>(toReturnPtr);

        return;
    }
    if (cmd == 252) { // inline int sign(double x) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        double x = (double)*mxGetPr(prhs[1]);

        int toReturn = sign(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 253) { // inline GLdouble max(GLdouble x, GLdouble y) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        GLdouble x = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble y = (GLdouble)*mxGetPr(prhs[2]);

        GLdouble toReturn = max(x, y);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 254) { // inline GLdouble min(GLdouble x, GLdouble y) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        GLdouble x = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble y = (GLdouble)*mxGetPr(prhs[2]);

        GLdouble toReturn = min(x, y);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 255) { // bool getRoot(GLdouble* root, GLdouble a, GLdouble b, GLdouble c, GLdouble s=1); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 5 || nrhs > 6) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 5 and 6 arguments."); }

        if (nrhs == 5) {
            GLdouble* root = (GLdouble*)mxGetPr(prhs[1]);
            GLdouble a = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble b = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble c = (GLdouble)*mxGetPr(prhs[4]);
            // GLdouble s = 1;

            bool toReturn = getRoot(root, a, b, c);
            plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
            double* toReturnDouble = mxGetPr(plhs[0]);
            *toReturnDouble = (double)toReturn;
        }
        if (nrhs == 6) {
            GLdouble* root = (GLdouble*)mxGetPr(prhs[1]);
            GLdouble a = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble b = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble c = (GLdouble)*mxGetPr(prhs[4]);
            GLdouble s = (GLdouble)*mxGetPr(prhs[5]);

            bool toReturn = getRoot(root, a, b, c, s);
            plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
            double* toReturnDouble = mxGetPr(plhs[0]);
            *toReturnDouble = (double)toReturn;
        }

        return;
    }
    if (cmd == 256) { // AFFINE mirrorX(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        AFFINE toReturn = mirrorX();
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 257) { // AFFINE mirrorY(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        AFFINE toReturn = mirrorY();
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 258) { // AFFINE mirrorYeX(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        AFFINE toReturn = mirrorYeX();
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 259) { // AFFINE mirrorYemX(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        AFFINE toReturn = mirrorYemX();
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 260) { // AFFINE zeroAFFINE(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        AFFINE toReturn = zeroAFFINE();
        AFFINE* toReturnPtr = new AFFINE();
        memcpy(toReturnPtr, &toReturn, sizeof(AFFINE));
        plhs[0] = convertPtr2Mat<AFFINE>(toReturnPtr);

        return;
    }
    if (cmd == 261) { // std::string replaceIllegalLIBNAME(std::string str); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        char str1[64];
        mxGetString(prhs[1], str1, 64);
        std::string str = std::string(str1);

        std::string toReturn = replaceIllegalLIBNAME(str);
        plhs[0] = mxCreateString(toReturn.c_str());

        return;
    }
    if (cmd == 262) { // std::string replaceIllegalSTRNAME(std::string str); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        char str1[64];
        mxGetString(prhs[1], str1, 64);
        std::string str = std::string(str1);

        std::string toReturn = replaceIllegalSTRNAME(str);
        plhs[0] = mxCreateString(toReturn.c_str());

        return;
    }
    if (cmd == 263) { // inline uint16_t endianSwap(uint16_t x) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        uint16_t x = (uint16_t)*mxGetPr(prhs[1]);

        uint16_t toReturn = endianSwap(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 264) { // inline uint32_t endianSwap(uint32_t x) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        uint32_t x = (uint32_t)*mxGetPr(prhs[1]);

        uint32_t toReturn = endianSwap(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 265) { // inline uint64_t endianSwap(uint64_t x) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        uint64_t x = (uint64_t)*mxGetPr(prhs[1]);

        uint64_t toReturn = endianSwap(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 266) { // inline int16_t endianSwap(int16_t x) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        int16_t x = (int16_t)*mxGetPr(prhs[1]);

        int16_t toReturn = endianSwap(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 267) { // inline int32_t endianSwap(int32_t x) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        int32_t x = (int32_t)*mxGetPr(prhs[1]);

        int32_t toReturn = endianSwap(x);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 268) { // inline GLdouble sem2num(uint64_t sem) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        uint64_t sem = (uint64_t)*mxGetPr(prhs[1]);

        GLdouble toReturn = sem2num(sem);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 269) { // inline uint64_t num2sem(GLdouble num) - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        GLdouble num = (GLdouble)*mxGetPr(prhs[1]);

        uint64_t toReturn = num2sem(num);
        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* toReturnDouble = mxGetPr(plhs[0]);
        *toReturnDouble = (double)toReturn;

        return;
    }
    if (cmd == 270) { // DEVICE* getDevice(std::string description); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        char str1[64];
        mxGetString(prhs[1], str1, 64);
        std::string description = std::string(str1);

        DEVICE* toReturn = getDevice(description);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 271) { // POLYLINE rect(VECTOR u, VECTOR v); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        VECTOR* u = convertMat2Ptr<VECTOR>(prhs[1]);
        VECTOR* v = convertMat2Ptr<VECTOR>(prhs[2]);

        POLYLINE toReturn = rect(*u, *v);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 272) { // POLYLINE rect(GLdouble x, GLdouble y, GLdouble w, GLdouble h); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 5 arguments."); }

        GLdouble x = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble y = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble w = (GLdouble)*mxGetPr(prhs[3]);
        GLdouble h = (GLdouble)*mxGetPr(prhs[4]);

        POLYLINE toReturn = rect(x, y, w, h);
        POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
        plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);

        return;
    }
    if (cmd == 273) { // POLYLINE arc(GLdouble r, GLdouble t1_, GLdouble t2_, bool CCW=true); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 4 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 4 and 5 arguments."); }

        if (nrhs == 4) {
            GLdouble r = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble t1_ = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble t2_ = (GLdouble)*mxGetPr(prhs[3]);
            // bool CCW = true;

            POLYLINE toReturn = arc(r, t1_, t2_);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 5) {
            GLdouble r = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble t1_ = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble t2_ = (GLdouble)*mxGetPr(prhs[3]);
            bool CCW = (bool)*mxGetPr(prhs[4]);

            POLYLINE toReturn = arc(r, t1_, t2_, CCW);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }

        return;
    }
    if (cmd == 274) { // GLdouble getArcAngle(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest=true); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 4 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 4 and 5 arguments."); }

        if (nrhs == 4) {
            VECTOR* c = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* b = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* e = convertMat2Ptr<VECTOR>(prhs[3]);
            // bool chooseShortest = true;

            GLdouble toReturn = getArcAngle(*c, *b, *e);
            plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
            double* toReturnDouble = mxGetPr(plhs[0]);
            *toReturnDouble = (double)toReturn;
        }
        if (nrhs == 5) {
            VECTOR* c = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* b = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* e = convertMat2Ptr<VECTOR>(prhs[3]);
            bool chooseShortest = (bool)*mxGetPr(prhs[4]);

            GLdouble toReturn = getArcAngle(*c, *b, *e, chooseShortest);
            plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
            double* toReturnDouble = mxGetPr(plhs[0]);
            *toReturnDouble = (double)toReturn;
        }

        return;
    }
    if (cmd == 275) { // POLYLINE arc(VECTOR c, VECTOR b, VECTOR e, bool chooseShortest=true); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 4 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 4 and 5 arguments."); }

        if (nrhs == 4) {
            VECTOR* c = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* b = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* e = convertMat2Ptr<VECTOR>(prhs[3]);
            // bool chooseShortest = true;

            POLYLINE toReturn = arc(*c, *b, *e);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 5) {
            VECTOR* c = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* b = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* e = convertMat2Ptr<VECTOR>(prhs[3]);
            bool chooseShortest = (bool)*mxGetPr(prhs[4]);

            POLYLINE toReturn = arc(*c, *b, *e, chooseShortest);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }

        return;
    }
    if (cmd == 276) { // POLYLINE linear(VECTOR p0, VECTOR p1, size_t steps=5); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        if (nrhs == 3) {
            VECTOR* p0 = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* p1 = convertMat2Ptr<VECTOR>(prhs[2]);
            // size_t steps = 5;

            POLYLINE toReturn = linear(*p0, *p1);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 4) {
            VECTOR* p0 = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* p1 = convertMat2Ptr<VECTOR>(prhs[2]);
            size_t steps = (size_t)*mxGetPr(prhs[3]);

            POLYLINE toReturn = linear(*p0, *p1, steps);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }

        return;
    }
    if (cmd == 277) { // POLYLINE qBezier(VECTOR p0, VECTOR p1, VECTOR p2, size_t steps=5); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 4 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 4 and 5 arguments."); }

        if (nrhs == 4) {
            VECTOR* p0 = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* p1 = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* p2 = convertMat2Ptr<VECTOR>(prhs[3]);
            // size_t steps = 5;

            POLYLINE toReturn = qBezier(*p0, *p1, *p2);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 5) {
            VECTOR* p0 = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* p1 = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* p2 = convertMat2Ptr<VECTOR>(prhs[3]);
            size_t steps = (size_t)*mxGetPr(prhs[4]);

            POLYLINE toReturn = qBezier(*p0, *p1, *p2, steps);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }

        return;
    }
    if (cmd == 278) { // POLYLINE cBezier(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, size_t steps=5); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 5 || nrhs > 6) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 5 and 6 arguments."); }

        if (nrhs == 5) {
            VECTOR* p0 = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* p1 = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* p2 = convertMat2Ptr<VECTOR>(prhs[3]);
            VECTOR* p3 = convertMat2Ptr<VECTOR>(prhs[4]);
            // size_t steps = 5;

            POLYLINE toReturn = cBezier(*p0, *p1, *p2, *p3);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 6) {
            VECTOR* p0 = convertMat2Ptr<VECTOR>(prhs[1]);
            VECTOR* p1 = convertMat2Ptr<VECTOR>(prhs[2]);
            VECTOR* p2 = convertMat2Ptr<VECTOR>(prhs[3]);
            VECTOR* p3 = convertMat2Ptr<VECTOR>(prhs[4]);
            size_t steps = (size_t)*mxGetPr(prhs[5]);

            POLYLINE toReturn = cBezier(*p0, *p1, *p2, *p3, steps);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }

        return;
    }
    if (cmd == 279) { // POLYLINE thicken(POLYLINE open, GLdouble width, int side=0, GLdouble minstep=1); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 5 arguments."); }

        if (nrhs == 3) {
            POLYLINE* open = convertMat2Ptr<POLYLINE>(prhs[1]);
            GLdouble width = (GLdouble)*mxGetPr(prhs[2]);
            // int side = 0;
            // GLdouble minstep = 1;

            POLYLINE toReturn = thicken(*open, width);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 4) {
            POLYLINE* open = convertMat2Ptr<POLYLINE>(prhs[1]);
            GLdouble width = (GLdouble)*mxGetPr(prhs[2]);
            int side = (int)*mxGetPr(prhs[3]);
            // GLdouble minstep = 1;

            POLYLINE toReturn = thicken(*open, width, side);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }
        if (nrhs == 5) {
            POLYLINE* open = convertMat2Ptr<POLYLINE>(prhs[1]);
            GLdouble width = (GLdouble)*mxGetPr(prhs[2]);
            int side = (int)*mxGetPr(prhs[3]);
            GLdouble minstep = (GLdouble)*mxGetPr(prhs[4]);

            POLYLINE toReturn = thicken(*open, width, side, minstep);
            POLYLINE* toReturnPtr = new POLYLINE();
        memcpy(toReturnPtr, &toReturn, sizeof(POLYLINE));
            plhs[0] = convertPtr2Mat<POLYLINE>(toReturnPtr);
        }

        return;
    }
    if (cmd == 280) { // void mainDevice(int i=0); - 
        if (nlhs != 0) {  mexErrMsgTxt("mex_bridge.cpp: Expected no return values; void type."); }
        if (nrhs < 1 || nrhs > 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 1 and 2 arguments."); }

        if (nrhs == 1) {
            // int i = 0;

            mainDevice();
        }
        if (nrhs == 2) {
            int i = (int)*mxGetPr(prhs[1]);

            mainDevice(i);
        }

        return;
    }
    if (cmd == 281) { // DEVICE* directionalCoupler(GLdouble a, GLdouble d, GLdouble L, GLdouble a0, GLdouble t=90, GLdouble r=MINRADIUS); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 5 || nrhs > 7) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 5 and 7 arguments."); }

        if (nrhs == 5) {
            GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble d = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble a0 = (GLdouble)*mxGetPr(prhs[4]);
            // GLdouble t = 90;
            // GLdouble r = MINRADIUS;

            DEVICE* toReturn = directionalCoupler(a, d, L, a0);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 6) {
            GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble d = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble a0 = (GLdouble)*mxGetPr(prhs[4]);
            GLdouble t = (GLdouble)*mxGetPr(prhs[5]);
            // GLdouble r = MINRADIUS;

            DEVICE* toReturn = directionalCoupler(a, d, L, a0, t);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 7) {
            GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble d = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble a0 = (GLdouble)*mxGetPr(prhs[4]);
            GLdouble t = (GLdouble)*mxGetPr(prhs[5]);
            GLdouble r = (GLdouble)*mxGetPr(prhs[6]);

            DEVICE* toReturn = directionalCoupler(a, d, L, a0, t, r);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 282) { // DEVICE* yjunction(GLdouble a, GLdouble s, GLdouble L); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble L = (GLdouble)*mxGetPr(prhs[3]);

        DEVICE* toReturn = yjunction(a, s, L);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 283) { // DEVICE* yjunction2(GLdouble a, GLdouble s, GLdouble L); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble s = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble L = (GLdouble)*mxGetPr(prhs[3]);

        DEVICE* toReturn = yjunction2(a, s, L);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 284) { // DEVICE* grating(int nm, GLdouble scaleTimes=1, GLdouble offsetPlus=0); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 2 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 2 and 4 arguments."); }

        if (nrhs == 2) {
            int nm = (int)*mxGetPr(prhs[1]);
            // GLdouble scaleTimes = 1;
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grating(nm);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 3) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble scaleTimes = (GLdouble)*mxGetPr(prhs[2]);
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grating(nm, scaleTimes);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble scaleTimes = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble offsetPlus = (GLdouble)*mxGetPr(prhs[3]);

            DEVICE* toReturn = grating(nm, scaleTimes, offsetPlus);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 285) { // DEVICE* ring(GLdouble r, GLdouble a, GLdouble d, GLdouble a0, GLdouble L=0); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 5 || nrhs > 6) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 5 and 6 arguments."); }

        if (nrhs == 5) {
            GLdouble r = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble a = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble d = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble a0 = (GLdouble)*mxGetPr(prhs[4]);
            // GLdouble L = 0;

            DEVICE* toReturn = ring(r, a, d, a0);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 6) {
            GLdouble r = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble a = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble d = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble a0 = (GLdouble)*mxGetPr(prhs[4]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[5]);

            DEVICE* toReturn = ring(r, a, d, a0, L);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 286) { // DEVICE* MIT(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        DEVICE* toReturn = MIT();
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 287) { // DEVICE* edgeCoupling1(GLdouble a, GLdouble a0, GLdouble L); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 4 arguments."); }

        GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble a0 = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble L = (GLdouble)*mxGetPr(prhs[3]);

        DEVICE* toReturn = edgeCoupling1(a, a0, L);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 288) { // DEVICE* edgeCouplingLabels(GLdouble L, GLdouble p, GLdouble t, GLdouble lhz); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 5 arguments."); }

        GLdouble L = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble p = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble t = (GLdouble)*mxGetPr(prhs[3]);
        GLdouble lhz = (GLdouble)*mxGetPr(prhs[4]);

        DEVICE* toReturn = edgeCouplingLabels(L, p, t, lhz);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 289) { // DEVICE* edgeCouplingN(GLdouble a, GLdouble a0, GLdouble L, GLdouble lhz, int n); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 6) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 6 arguments."); }

        GLdouble a = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble a0 = (GLdouble)*mxGetPr(prhs[2]);
        GLdouble L = (GLdouble)*mxGetPr(prhs[3]);
        GLdouble lhz = (GLdouble)*mxGetPr(prhs[4]);
        int n = (int)*mxGetPr(prhs[5]);

        DEVICE* toReturn = edgeCouplingN(a, a0, L, lhz, n);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 290) { // DEVICE* grateCouplingN(int nm, int n, GLdouble scaleTimes=1, GLdouble offsetPlus=0); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 5 arguments."); }

        if (nrhs == 3) {
            int nm = (int)*mxGetPr(prhs[1]);
            int n = (int)*mxGetPr(prhs[2]);
            // GLdouble scaleTimes = 1;
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grateCouplingN(nm, n);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            int n = (int)*mxGetPr(prhs[2]);
            GLdouble scaleTimes = (GLdouble)*mxGetPr(prhs[3]);
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grateCouplingN(nm, n, scaleTimes);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 5) {
            int nm = (int)*mxGetPr(prhs[1]);
            int n = (int)*mxGetPr(prhs[2]);
            GLdouble scaleTimes = (GLdouble)*mxGetPr(prhs[3]);
            GLdouble offsetPlus = (GLdouble)*mxGetPr(prhs[4]);

            DEVICE* toReturn = grateCouplingN(nm, n, scaleTimes, offsetPlus);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 291) { // DEVICE* grateLoop(int nm, GLdouble L, GLdouble adiabat, bool wide=false, GLdouble scaleTimes=1, GLdouble offsetPlus=0); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 4 || nrhs > 7) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 4 and 7 arguments."); }

        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[3]);
            // bool wide = false;
            // GLdouble scaleTimes = 1;
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grateLoop(nm, L, adiabat);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 5) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[3]);
            bool wide = (bool)*mxGetPr(prhs[4]);
            // GLdouble scaleTimes = 1;
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grateLoop(nm, L, adiabat, wide);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 6) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[3]);
            bool wide = (bool)*mxGetPr(prhs[4]);
            GLdouble scaleTimes = (GLdouble)*mxGetPr(prhs[5]);
            // GLdouble offsetPlus = 0;

            DEVICE* toReturn = grateLoop(nm, L, adiabat, wide, scaleTimes);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 7) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[3]);
            bool wide = (bool)*mxGetPr(prhs[4]);
            GLdouble scaleTimes = (GLdouble)*mxGetPr(prhs[5]);
            GLdouble offsetPlus = (GLdouble)*mxGetPr(prhs[6]);

            DEVICE* toReturn = grateLoop(nm, L, adiabat, wide, scaleTimes, offsetPlus);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 292) { // DEVICE* grateLoopArray(int nm, GLdouble adiabat, bool wide=false); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        if (nrhs == 3) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[2]);
            // bool wide = false;

            DEVICE* toReturn = grateLoopArray(nm, adiabat);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[2]);
            bool wide = (bool)*mxGetPr(prhs[3]);

            DEVICE* toReturn = grateLoopArray(nm, adiabat, wide);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 293) { // DEVICE* edgeLoop(int nm, int L, GLdouble adiabat, bool wide=false); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 4 || nrhs > 5) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 4 and 5 arguments."); }

        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            int L = (int)*mxGetPr(prhs[2]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[3]);
            // bool wide = false;

            DEVICE* toReturn = edgeLoop(nm, L, adiabat);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 5) {
            int nm = (int)*mxGetPr(prhs[1]);
            int L = (int)*mxGetPr(prhs[2]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[3]);
            bool wide = (bool)*mxGetPr(prhs[4]);

            DEVICE* toReturn = edgeLoop(nm, L, adiabat, wide);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 294) { // DEVICE* loopSegment(int nm, GLdouble adiabat); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        int nm = (int)*mxGetPr(prhs[1]);
        GLdouble adiabat = (GLdouble)*mxGetPr(prhs[2]);

        DEVICE* toReturn = loopSegment(nm, adiabat);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 295) { // DEVICE* edgeLoopArray(int nm, GLdouble adiabat, bool wide=false); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 3 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 3 and 4 arguments."); }

        if (nrhs == 3) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[2]);
            // bool wide = false;

            DEVICE* toReturn = edgeLoopArray(nm, adiabat);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble adiabat = (GLdouble)*mxGetPr(prhs[2]);
            bool wide = (bool)*mxGetPr(prhs[3]);

            DEVICE* toReturn = edgeLoopArray(nm, adiabat, wide);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 296) { // DEVICE* diamond(GLdouble ac, GLdouble ae); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        GLdouble ac = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble ae = (GLdouble)*mxGetPr(prhs[2]);

        DEVICE* toReturn = diamond(ac, ae);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 297) { // DEVICE* diamond2(GLdouble ac, GLdouble ae); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 3 arguments."); }

        GLdouble ac = (GLdouble)*mxGetPr(prhs[1]);
        GLdouble ae = (GLdouble)*mxGetPr(prhs[2]);

        DEVICE* toReturn = diamond2(ac, ae);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 298) { // DEVICE* device1(int nm); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        int nm = (int)*mxGetPr(prhs[1]);

        DEVICE* toReturn = device1(nm);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 299) { // DEVICE* device2(int nm); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        int nm = (int)*mxGetPr(prhs[1]);

        DEVICE* toReturn = device2(nm);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 300) { // DEVICE* device4(int nm); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        int nm = (int)*mxGetPr(prhs[1]);

        DEVICE* toReturn = device4(nm);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 301) { // DEVICE* device4across(int nm); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 2) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 2 arguments."); }

        int nm = (int)*mxGetPr(prhs[1]);

        DEVICE* toReturn = device4across(nm);
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 302) { // DEVICE* testD2(int nm, GLdouble scale=1, bool grate=false); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 2 || nrhs > 4) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 2 and 4 arguments."); }

        if (nrhs == 2) {
            int nm = (int)*mxGetPr(prhs[1]);
            // GLdouble scale = 1;
            // bool grate = false;

            DEVICE* toReturn = testD2(nm);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 3) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble scale = (GLdouble)*mxGetPr(prhs[2]);
            // bool grate = false;

            DEVICE* toReturn = testD2(nm, scale);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 4) {
            int nm = (int)*mxGetPr(prhs[1]);
            GLdouble scale = (GLdouble)*mxGetPr(prhs[2]);
            bool grate = (bool)*mxGetPr(prhs[3]);

            DEVICE* toReturn = testD2(nm, scale, grate);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 303) { // DEVICE* testY2(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        DEVICE* toReturn = testY2();
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
    if (cmd == 304) { // DEVICE* testYN(int n, bool grate=false); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 2 || nrhs > 3) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 2 and 3 arguments."); }

        if (nrhs == 2) {
            int n = (int)*mxGetPr(prhs[1]);
            // bool grate = false;

            DEVICE* toReturn = testYN(n);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 3) {
            int n = (int)*mxGetPr(prhs[1]);
            bool grate = (bool)*mxGetPr(prhs[2]);

            DEVICE* toReturn = testYN(n, grate);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 305) { // DEVICE* linesNgaps(GLdouble dw, GLdouble s, GLdouble L, int steps, GLdouble add=0); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs < 5 || nrhs > 6) {  mexErrMsgTxt("mex_bridge.cpp: Expected between 5 and 6 arguments."); }

        if (nrhs == 5) {
            GLdouble dw = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble s = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[3]);
            int steps = (int)*mxGetPr(prhs[4]);
            // GLdouble add = 0;

            DEVICE* toReturn = linesNgaps(dw, s, L, steps);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }
        if (nrhs == 6) {
            GLdouble dw = (GLdouble)*mxGetPr(prhs[1]);
            GLdouble s = (GLdouble)*mxGetPr(prhs[2]);
            GLdouble L = (GLdouble)*mxGetPr(prhs[3]);
            int steps = (int)*mxGetPr(prhs[4]);
            GLdouble add = (GLdouble)*mxGetPr(prhs[5]);

            DEVICE* toReturn = linesNgaps(dw, s, L, steps, add);
            plhs[0] = convertPtr2Mat<DEVICE>(toReturn);
        }

        return;
    }
    if (cmd == 306) { // DEVICE* astrix(); - 
        if (nlhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected 1 return value."); }
        if (nrhs != 1) {  mexErrMsgTxt("mex_bridge.cpp: Expected exactly 1 argument."); }

        DEVICE* toReturn = astrix();
        plhs[0] = convertPtr2Mat<DEVICE>(toReturn);

        return;
    }
}