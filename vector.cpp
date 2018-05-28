#include "vector.hpp"

// VECTOR ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECTOR::VECTOR(GLdouble x_, GLdouble y_, bool isCylindrical) {
    if (isCylindrical) {
        x = x_*cos(y_); y = x_*sin(y_);
    } else {
        x = x_; y = y_;
    }
}

//VECTOR VECTOR:setX(GLdouble x_)             const { return *this; }
//VECTOR VECTOR:setY(GLdouble y_)             const { return *this; }

bool VECTOR::isZero()                       const { return x == 0 && y == 0; }
bool VECTOR::operator!()                    const { return isZero(); }

bool VECTOR::operator!=(VECTOR v)           const { return !operator==(v); }

#define SQUISHYLOGIC 1

#ifdef SQUISHYLOGIC
bool VECTOR::operator==(VECTOR v)           const { return (x == v.x && y == v.y) || (std::abs(x - v.x) < ERROR && std::abs(y - v.y) < ERROR); }

bool VECTOR::operator< (VECTOR v)           const { return (x == v.x || std::abs(x - v.x) < ERROR)?(y <   v.y - ERROR):(x <   v.x - ERROR); }
bool VECTOR::operator<=(VECTOR v)           const { return (x == v.x || std::abs(x - v.x) < ERROR)?(y <=  v.y - ERROR):(x <=  v.x - ERROR); }
bool VECTOR::operator> (VECTOR v)           const { return (x == v.x || std::abs(x - v.x) < ERROR)?(y >   v.y - ERROR):(x >   v.x - ERROR); }
bool VECTOR::operator>=(VECTOR v)           const { return (x == v.x || std::abs(x - v.x) < ERROR)?(y >=  v.y - ERROR):(x >=  v.x - ERROR); }

bool VECTOR::inLine(VECTOR v)               const { return x == v.x || y == v.y || std::abs(x - v.x) < ERROR || std::abs(y - v.y) < ERROR; }
#else
bool VECTOR::operator==(VECTOR v)           const { return (x == v.x && y == v.y); }
    
bool VECTOR::operator< (VECTOR v)           const { return (x == v.x)?(y <  v.y):(x <  v.x); }
bool VECTOR::operator<=(VECTOR v)           const { return (x == v.x)?(y <= v.y):(x <= v.x); }
bool VECTOR::operator> (VECTOR v)           const { return (x == v.x)?(y >  v.y):(x >  v.x); }
bool VECTOR::operator>=(VECTOR v)           const { return (x == v.x)?(y >= v.y):(x >= v.x); }

bool VECTOR::inLine(VECTOR v)               const { return x == v.x || y == v.y; }
#endif

VECTOR VECTOR::operator=(VECTOR v)                { x = v.x; y = v.y; return *this; }
VECTOR VECTOR::operator-()                  const { return VECTOR(-x, -y);  }

VECTOR VECTOR::operator+=(VECTOR v)               { x += v.x; y += v.y; return *this; }
VECTOR VECTOR::operator-=(VECTOR v)               { x -= v.x; y -= v.y; return *this; }
VECTOR VECTOR::operator*=(GLdouble factor)        { x *= factor; y *= factor; return *this; }
VECTOR VECTOR::operator/=(GLdouble factor)        { x /= factor; y /= factor; return *this; }

VECTOR VECTOR::operator+(VECTOR v)          const { return VECTOR(x + v.x, y + v.y); }
VECTOR VECTOR::operator-(VECTOR v)          const { return VECTOR(x - v.x, y - v.y); }
VECTOR VECTOR::operator*(GLdouble factor)   const { return VECTOR(x * factor, y * factor); }
VECTOR VECTOR::operator/(GLdouble factor)   const { return (factor == 0)?
                                                        (VECTOR(0, 0)):(VECTOR(x / factor, y / factor)); }

VECTOR VECTOR::operator*(AFFINE m)          const { return m*(*this); }
VECTOR VECTOR::operator*=(AFFINE m)               { *this = m*(*this); return *this; };

GLdouble VECTOR::magn()                     const { return sqrt(x*x + y*y); }
GLdouble VECTOR::magn2()                    const { return x*x + y*y; }

GLdouble VECTOR::norm()                     const { return magn(); }
GLdouble VECTOR::norm2()                    const { return magn2(); }

void VECTOR::zero() { x = 0; y = 0; }

void VECTOR::unitEquals(){
    GLdouble magn2_ = magn2();

    if (magn2_ != 1 && magn2_ != 0) {
        *this /= sqrt(magn2_);
    }
}
VECTOR VECTOR::unit() const {
    GLdouble magn2_ = magn2();

    if (magn2_ != 1 && magn2_ != 0) {
        return VECTOR(x, y) / sqrt(magn2_);
    } else {
        return VECTOR(x, y);
    }
}
bool VECTOR::isUnit()                   const { return x*x + y*y == 1; }

VECTOR VECTOR::perpCCW()                    const { return VECTOR(-y, x); }
VECTOR VECTOR::perpCW()                     const { return VECTOR(y, -x); }

GLdouble VECTOR::operator*(VECTOR v)    const { return x*v.x + y*v.y; }
GLdouble VECTOR::dot(VECTOR v)          const { return x*v.x + y*v.y; }
GLdouble VECTOR::perpCCWdot(VECTOR v)   const { return y*v.x - x*v.y; }
GLdouble VECTOR::cross(VECTOR v)        const { return sign(x*v.y - y*v.x); }

VECTOR VECTOR::rotate(GLdouble radians) const {
    GLdouble c = cos(radians);
    GLdouble s = sin(radians);

    return VECTOR(c*x - s*y, s*x + c*y);
}
void VECTOR::rotateEquals(GLdouble radians) {
    GLdouble c = cos(radians);
    GLdouble s = sin(radians);

    GLdouble xtemp = x;

    x = c*x - s*y;
    y = s*xtemp + c*y;
}

void VECTOR::print()                    const {
#ifdef MATLAB_MEX_FILE
    mexPrintf("[ %.2f, %.2f ]",     x, y);
#else
    printf("[ %.2f, %.2f ]",        x, y);
#endif
}
void VECTOR::printNL()                  const {
#ifdef MATLAB_MEX_FILE
    mexPrintf("[ %.2f, %.2f ]\n",   x, y);
#else
    printf("[ %.2f, %.2f ]\n",      x, y);
#endif
}
std::string VECTOR::str()               const {
    char toReturn[64];
    
    snprintf(toReturn, 64, "[ %.2f, %.2f ]\n", x, y);
    
    return std::string(toReturn);
}

long VECTOR::hash()                    const {
    return ( ((long)x) << 1 ) ^ ( ((long)y) >> 1 );     // Change?
}

void VECTOR::render()                   const { render(0,0); }
void VECTOR::render(VECTOR v)           const { render(v.x, v.y); }
void VECTOR::render(GLdouble x_, GLdouble y_) const {
#ifdef USE_GL_RENDER
    glBegin(GL_LINE);

    glColor3ub(255, 0, 0);

    glVertex2d(x_,      y_);
    glVertex2d(x_ + x,  y_ + y);

    glEnd();
#endif
}

void VECTOR::glVertex() const {
#ifdef USE_GL_RENDER
    glVertex2d(x, y);
//    glVertex2dv((GLdouble*)this);
#endif
}

// LABEL ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LABEL::LABEL(VECTOR v, std::string label_) {
    x = v.x;
    y = v.y;
    label = label_;
}
LABEL::LABEL(GLdouble x_, GLdouble y_, std::string label_) {
    x = x_;
    y = y_;
    label = label_;
}

void LABEL::render(GLdouble x_, GLdouble y_) const {

}

// AFFINE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AFFINE::AFFINE() { a = 1; b = 0; c = 0; d = 1; e = 0; f = 0; }
AFFINE::AFFINE(GLdouble radians) {
    GLdouble c_ = cos(radians);
    GLdouble s_ = sin(radians);

    a = c_; b = -s_; c = s_; d = c_; e = 0; f = 0;
}
AFFINE::AFFINE(GLdouble radians, VECTOR v) {
    GLdouble c_ = cos(radians);
    GLdouble s_ = sin(radians);

    a = c_; b = -s_; c = s_; d = c_; e = v.x; f = v.y;
}
AFFINE::AFFINE(GLdouble radians, GLdouble e_, GLdouble f_) {
    GLdouble c_ = cos(radians);
    GLdouble s_ = sin(radians);

    a = c_; b = -s_; c = s_; d = c_; e = e_; f = f_;
}

AFFINE::AFFINE(VECTOR v) {                      a = 1;   b = 0;   c = 0;   d = 1;   e = v.x; f = v.y; }
AFFINE::AFFINE(GLdouble e_, GLdouble f_) {      a = 1;   b = 0;   c = 0;   d = 1;   e = e_;  f = f_; }

AFFINE::AFFINE(VECTOR u, VECTOR v) {            a = u.x; b = v.x; c = u.y; d = v.y; e = 0;   f = 0; }
AFFINE::AFFINE(VECTOR u, VECTOR v, VECTOR w) {  a = u.x; b = v.x; c = u.y; d = v.y; e = w.x; f = w.y; }

AFFINE::AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, GLdouble e_, GLdouble f_) {
    a = a_; b = b_; c = c_; d = d_; e = e_; f = f_;
}
AFFINE::AFFINE(GLdouble a_, GLdouble b_, GLdouble c_, GLdouble d_, VECTOR v) {
    a = a_; b = b_; c = c_; d = d_; e = v.x; f = v.y;
}

AFFINE::AFFINE(CONNECTION from, CONNECTION to) {
    GLdouble radians = sign(from.dv.cross(-to.dv))*acos(from.dv * -to.dv);

    if (radians == 0) {
        if (from.dv * -to.dv < 0) {
            radians = TAU/2;
        }
    }
    
    operator=(AFFINE(radians, to.v)*AFFINE(-from.v));
}
AFFINE::AFFINE(BOUNDINGBOX from, BOUNDINGBOX to, int noskew) {
    if (from.area() && to.area()) {
        GLdouble sx =  to.width()/from.width();
        GLdouble sy = to.height()/from.height();
        
//        printf("%f, %f\n", sx, sy);
        
        if          (noskew == 0) {
            operator=( AFFINE(to.center()) * AFFINE(sx,0,0,sy) * AFFINE(-from.center()) );
        } else if   ( (noskew == 1 && sx > sy) || (noskew ==  -1 && sx < sy) ) {
            operator=( AFFINE(to.center()) * AFFINE(sx,0,0,sx) * AFFINE(-from.center()) );
        } else {
            operator=( AFFINE(to.center()) * AFFINE(sy,0,0,sy) * AFFINE(-from.center()) );
        }
    } else {
        a = 1; b = 0; c = 0; d = 1; e = 0; f = 0;   // Identity...
    }
}

bool AFFINE::isIdentity()               const { return a == 1 && b == 0 && c == 0 && d == 1 && e == 0 && f == 0; }
bool AFFINE::isZero()                   const { return a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0; }
//bool AFFINE::isIdentity()               const { return std::abs(a-1) < E && std::abs(b) < E && std::abs(c) < E && std::abs(d-1) < E && std::abs(e) < E && std::abs(f) < E; }
//bool AFFINE::isZero()                   const { return std::abs(a) < E && std::abs(b) < E && std::abs(c) < E && std::abs(d) < E && std::abs(e) < E && std::abs(f) < E; }

VECTOR AFFINE::operator*(VECTOR v)      const { return VECTOR(a*v.x + b*v.y + e, c*v.x + d*v.y + f); }
VECTOR AFFINE::linearTimes(VECTOR v)    const { return VECTOR(a*v.x + b*v.y, c*v.x + d*v.y); }

CONNECTION AFFINE::operator*(CONNECTION c) const {
    VECTOR dv = linearTimes(c.dv.perpCCW()).perpCW() * det();
    GLdouble magn = dv.magn();

    return CONNECTION(operator*(c.v), // + translation(),
                      dv/std::abs(magn),          // Make dv a unit vector.
                      c.w*magn,         // Grow/shrink width appropriately.
                      c.name,
                      c.l);
}
CONNECTION AFFINE::linearTimes(CONNECTION c) const {
    VECTOR dv = linearTimes(c.dv.perpCCW()).perpCW() * det();
    GLdouble magn = dv.magn();

    return CONNECTION(c.v,
                      dv/magn,          // Make dv a unit vector.
                      c.w*magn,         // Grow/shrink width appropriately.
                      c.name,
                      c.l);
}
BOUNDINGBOX AFFINE::operator*(BOUNDINGBOX bb)   const {
    BOUNDINGBOX toReturn;
    
    toReturn.enlarge(operator*(bb.ll));
    toReturn.enlarge(operator*(VECTOR(bb.ll.x, bb.ur.y)));
    toReturn.enlarge(operator*(bb.ur));
    toReturn.enlarge(operator*(VECTOR(bb.ur.x, bb.ll.y)));
    
    return toReturn;
}
BOUNDINGBOX AFFINE::linearTimes(BOUNDINGBOX bb) const {
    BOUNDINGBOX toReturn;
    
    toReturn.enlarge(linearTimes(bb.ll));
    toReturn.enlarge(linearTimes(VECTOR(bb.ll.x, bb.ur.y)));
    toReturn.enlarge(linearTimes(bb.ur));
    toReturn.enlarge(linearTimes(VECTOR(bb.ur.x, bb.ll.y)));
    
    return toReturn;
}
AFFINE AFFINE::operator*(AFFINE m) const {
    return AFFINE(a*m.a + b*m.c, a*m.b + b*m.d, c*m.a + d*m.c, c*m.b + d*m.d, a*m.e + b*m.f + e, c*m.e + d*m.f + f);
}
AFFINE AFFINE::operator*=(AFFINE m) {
    GLdouble a_= a*m.a + b*m.c;
    GLdouble b_= a*m.b + b*m.d;
    GLdouble c_= c*m.a + d*m.c;
    GLdouble d_= c*m.b + d*m.d;
    GLdouble e_= a*m.e + b*m.f + e;
    GLdouble f_= c*m.e + d*m.f + f;

    a = a_; b = b_; c = c_; d = d_; e = e_; f = f_;

    return *this;
}


AFFINE AFFINE::operator+(VECTOR v)      const { return AFFINE(a, b, c, d, e+v.x, f+v.y); }
AFFINE AFFINE::operator-(VECTOR v)      const { return AFFINE(a, b, c, d, e-v.x, f-v.y); }
//AFFINE AFFINE::operator*(GLdouble s)    const { return AFFINE(a*s, b*s, c*s, d*s, e, f); }          // Should these affect e and f?
//AFFINE AFFINE::operator/(GLdouble s)    const { return AFFINE(a/s, b/s, c/s, d/s, e, f); }
AFFINE AFFINE::operator*(GLdouble s)    const { return copy() *= s; }
AFFINE AFFINE::operator/(GLdouble s)    const { return copy() /= s; }

//DEVICEPTR AFFINE::operator*(DEVICE* ptr)    const { return DEVICEPTR(ptr, *this); }
//DEVICEPTR AFFINE::operator*(DEVICEPTR ptr)  const { return ptr * (*this); }

AFFINE AFFINE::operator+=(VECTOR v) {   e += v.x; f += v.y; return *this; }
AFFINE AFFINE::operator-=(VECTOR v) {   e -= v.x; f -= v.y; return *this; }
//AFFINE AFFINE::operator*=(GLdouble s) { a *= s; b *= s; c *= s; d *= s; return *this; }
//AFFINE AFFINE::operator/=(GLdouble s) { a /= s; b /= s; c /= s; d /= s; return *this; }
AFFINE AFFINE::operator*=(GLdouble s) { operator*=(AFFINE(s,   0, 0, s));   return *this; }
AFFINE AFFINE::operator/=(GLdouble s) { operator*=(AFFINE(1/s, 0, 0, 1/s)); return *this; }

AFFINE AFFINE::copy()                   const { return *this; }
VECTOR AFFINE::translation()            const { return VECTOR(e, f); }
AFFINE AFFINE::linear()                 const { return AFFINE(a, b, c, d); }
void AFFINE::print() const {
    printf("[ %.2f  %.2f ] [ x ]  +  [ %.2f ]\n",a, b, e);
    printf("[ %.2f  %.2f ] [ y ]  +  [ %.2f ]\n",c, d, f);
}
GLdouble AFFINE::det()                  const { return (a*d - b*c); }
AFFINE AFFINE::inv() const {
    GLdouble dd = det();
    if (dd == 0) {
//        throw std::runtime_error("AFFINE is not invertible");
    }

    if (islinear()) {
        return AFFINE(d/dd, -b/dd, -c/dd, a/dd);
    } else {
        return AFFINE(d/dd, -b/dd, -c/dd, a/dd, (d*e - b*f)/dd, (a*f - c*e)/dd);
    }
}
bool AFFINE::islinear()                 const { return e == 0 && f == 0; }  // Compare with E (squishy logic?)
void AFFINE::glMatrix() const {
//#ifdef USE_GL_RENDER
    const GLfloat m[16] = { (GLfloat)a, (GLfloat)c, 0, 0, (GLfloat)b, (GLfloat)d, 0, 0, 0, 0, 1, 0, (GLfloat)e, (GLfloat)f, 0, 1 };    // OpenGL is column-major!
//    glLoadMatrixd(m);
//    printf("HERE!");
//    print();
    
#ifdef pGDSGLFW
    GLuint mID = glGetUniformLocation(MATERIAL::shaders, "m");
    glUniformMatrix4fv(mID, 1, GL_FALSE, m);
#endif
    
//    gluniformMa
    
//    glMultMatrixd(m);   // Change cullface for transformation with negative determinant? Use both sides?
//#endif
}

AFFINE mirrorX() {      return AFFINE( 1,  0,  0, -1); }
AFFINE mirrorY() {      return AFFINE(-1,  0,  0,  1); }
AFFINE mirrorYeX() {    return AFFINE( 0,  1,  1,  0); }
AFFINE mirrorYemX() {   return AFFINE( 0, -1, -1,  0); }
AFFINE zeroAFFINE() {   return AFFINE( 0,  0,  0,  0); }

// CONNECTION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CONNECTION::CONNECTION() {
    v = VECTOR(0,0); dv = VECTOR(1,0); w = 1; name = "default";
}
CONNECTION::CONNECTION(VECTOR v_, VECTOR dv_, GLdouble w_, std::string name_, int16_t l_) {
//    v = v_; dv = dv_; w = std::abs(w_); name = name_;
    v = v_; dv = dv_; w = w_; name = name_;
    
    if (l_ == -1)   { l = 1; } //l = MATERIAL::currentLayer; }
    else            { l = l_; }
}
bool CONNECTION::isEmpty() const { return v.isZero() && dv ==  VECTOR(1,0) && w == 1 && name == "default"; }
CONNECTION CONNECTION::operator=(CONNECTION c) {        v = c.v; dv = c.dv; w = c.w; name = c.name; l = c.l; return *this; }
CONNECTION CONNECTION::operator-()          const {     return CONNECTION(v, -dv, w, name, l); }

//CONNECTION CONNECTION::operator+(VECTOR v_) const { return CONNECTION(v + v_, dv, w, name); }
//CONNECTION CONNECTION::operator-(VECTOR v_) const { return CONNECTION(v - v_, dv, w, name); }
CONNECTION CONNECTION::operator+(VECTOR v_) const {     return copy() += v_; }
CONNECTION CONNECTION::operator-(VECTOR v_) const {     return copy() -= v_; }
CONNECTION CONNECTION::operator+(GLdouble x) const {    return copy() += x; }
CONNECTION CONNECTION::operator-(GLdouble x) const {    return copy() -= x; }

CONNECTION CONNECTION::operator+=(VECTOR v_) {          v += v_;    return *this; }
CONNECTION CONNECTION::operator-=(VECTOR v_) {          v -= v_;    return *this; }
CONNECTION CONNECTION::operator+=(GLdouble x) {         v += dv*x;  return *this; }
CONNECTION CONNECTION::operator-=(GLdouble x) {         v -= dv*x;  return *this; }

//CONNECTION CONNECTION::operator*(GLdouble s) const {    return CONNECTION(v*s, dv*sign(s), w*std::abs(s), name); }
//CONNECTION CONNECTION::operator*=(GLdouble s) {         v *= s; dv *= sign(s); w *= std::abs(s); return *this; }

CONNECTION CONNECTION::operator* (GLdouble s) const {   return copy() *= s; }
CONNECTION CONNECTION::operator*=(GLdouble s) {         v *= s; dv *= sign(s); w *= std::abs(s); return *this; }

CONNECTION CONNECTION::operator/ (GLdouble s) const {   return copy() /= s; }
CONNECTION CONNECTION::operator/=(GLdouble s) {         v /= s; dv *= sign(s); w /= std::abs(s); return *this; }

CONNECTION CONNECTION::operator* (AFFINE m) const { return copy() *= m; }
CONNECTION CONNECTION::operator*=(AFFINE m) {
//    VECTOR dv_ = m.linearTimes(dv.perpCCW()).perpCW();
    VECTOR dv_ = m.linearTimes(dv.perpCCW()).perpCW() ;
    GLdouble magn = dv_.magn();
    
//    return CONNECTION(c.v,
//                      dv/magn,          // Make dv a unit vector.
//                      c.w*magn,         // Grow/shrink width appropriately.
//                      c.name);
//    printf("FISHFISH");
    
    dv /= magn * m.det();       // FIX ME!!!
    w *= magn;
    
    return *this;
}

bool CONNECTION::operator==(CONNECTION c)    const { return v == c.v && dv == c.dv && w == c.w; }

CONNECTION CONNECTION::setWidth(GLdouble w_) {      w = w_;         return *this; }

CONNECTION CONNECTION::setName(std::string name_){  name = name_;   return *this; }
CONNECTION CONNECTION::setLayer(int16_t l_){        l = l_;         return *this; }

void CONNECTION::print()    const {
    printf("CONNECTION %s at ", name.c_str());
    v.print();
    printf(" pointing toward ");
    dv.print();
    printf(" with width %f and layer %i\n", w, l);
}
CONNECTION  CONNECTION::copy()  const { return CONNECTION(v, dv, w, name, l); }

VECTOR      CONNECTION::left()  const { return v + dv.perpCW()*(w/2); }
VECTOR      CONNECTION::right() const { return v + dv.perpCCW()*(w/2); }

void CONNECTION::render()   const {
#ifdef USE_GL_RENDER
    VECTOR offset = dv.perpCW()*(w/2);
    VECTOR length = dv*(2*w);

    // Make an arrow...
    glBegin(GL_LINE_LOOP);

    glColor3ub(255, 255, 0);

    (v - offset).glVertex();
    (v + length - offset).glVertex();
    (v + length - offset*1.5).glVertex();
    (v + length*1.375).glVertex();
    (v + length + offset*1.5).glVertex();
    (v + length + offset).glVertex();
    (v + offset).glVertex();

    glEnd();
#endif
}

//GLdouble getAng(VECTOR u, VECTOR v) {
//    return acos(u*v);
//}

CONNECTION bendRadius(CONNECTION start, GLdouble ang, GLdouble radius) {
    VECTOR pivot = start.v + start.dv.perpCCW() * (radius * sign(ang));
    
    VECTOR dv = AFFINE(ang) * start.dv;
    //    dv.printNL();
    VECTOR v = pivot + dv.perpCW() * (radius * sign(ang));
    
    return CONNECTION(v, dv, start.w, start.name + "+", start.l);
}

CONNECTION bendTowards(CONNECTION start, GLdouble ang, GLdouble radius) {
    return bendTowards(start, VECTOR(ang), radius);
}
CONNECTION bendTowards(CONNECTION start, VECTOR towards, GLdouble radius) {
    bool dir = start.dv.perpCCWdot(towards) > 0;
    
//    printf("%f\n", acos(start.dv * towards.unit()));
//    printf("%f\n", ((dir)?(-1):(1)) * acos(start.dv * towards.unit()));
    
    return bendRadius(start, ((dir)?(-1):(1)) * acos(start.dv * towards.unit()), radius);
}

CONNECTION bendLength(CONNECTION start, GLdouble length, GLdouble ang) {
    if (ang == 0) { return start; }
    
    GLdouble radius = length/ang;
    
    if (radius < 0) {   return bendRadius(start, ang, -radius); }
    else {              return bendRadius(start, ang,  radius); }
}
CONNECTION bendLeft(CONNECTION start, GLdouble radius) {    return bendRadius(start,  TAU/4, radius); }
CONNECTION bendRight(CONNECTION start, GLdouble radius) {   return bendRadius(start, -TAU/4, radius); }

//bool intersect(CONNECTION c1, CONNECTION c2, VECTOR** i) {
//
//}

CONNECTION bendHorizontal(CONNECTION start, GLdouble horizontal) {
    VECTOR* i = new VECTOR();
    
//    printf("FISH!: %f\n\n", horizontal);
//
//    start.print();
    
    
    if (intersect(start, CONNECTION(VECTOR(start.v.x, horizontal), VECTOR(1, 0)), &i)) {
//        i->printNL();
        VECTOR path = *i - start.v;
        
//        path.printNL();
        
        if (path * start.dv > 0) {
            GLdouble d = path.norm();
            int dir = sign(start.dv.x);
            
            return CONNECTION(*i + VECTOR(dir*d, 0), VECTOR(-dir, 0), start.w, start.name + "+");
        }
    }
    
    delete i;
    return CONNECTION();
}
CONNECTION bendVertical(CONNECTION start,   GLdouble vertical) {
    VECTOR* i = new VECTOR();
    
//    printf("FISH!\n\n");
    
    if (intersect(start, CONNECTION(VECTOR(vertical, start.v.y), VECTOR(0, 1)), &i)) {
//        i->printNL();
        VECTOR path = *i - start.v;
        
//        path.printNL();
        
        if (path * start.dv > 0) {
            GLdouble d = path.norm();
            int dir = sign(start.dv.y);
            
            return CONNECTION(*i + VECTOR(0, dir*d), VECTOR(0, -dir), start.w, start.name + "+");
        }
    }
    
    delete i;
    return CONNECTION();
}

bool intersect(CONNECTION a, CONNECTION b, VECTOR** i, bool onlyForward) {
    if (a.dv == b.dv || a.dv == -b.dv) { *i = nullptr; return false; }  // Lines are parallel, no valid intersection...
    
    if (a.dv.x == 0 && b.dv.x == 0) {   // If both are vertical...
        throw std::runtime_error("intersect(CONNECTION^2, VECTOR**, bool): Both-vertical case should have been caught by parallel check...");
    } else if (a.dv.x == 0) {           // If `a` is vertical...
        *i = new VECTOR( a.v.x, (b.dv.y / b.dv.x) * (a.v.x - b.v.x) + b.v.y );
    } else if (b.dv.x == 0) {           // If `b` is vertical...
        *i = new VECTOR( b.v.x, (a.dv.y / a.dv.x) * (b.v.x - a.v.x) + a.v.y );
    } else {                            // If neither `a` nor `b` are vertical...
        GLdouble x = ( (a.dv.y / a.dv.x) * a.v.x - (b.dv.y / b.dv.x) * b.v.x - a.v.y + b.v.y ) / ( (a.dv.y / a.dv.x) - (b.dv.y / b.dv.x) );
        
        *i = new VECTOR( x, (a.dv.y / a.dv.x) * (x - a.v.x) + a.v.y );
    }
    
    if (onlyForward) {
        if ( (**i - a.v) * a.dv <= 0 || (**i - b.v) * b.dv <= 0 ) {
            delete *i;  *i = nullptr; return false;
        }
    }
    
    return true;
}



