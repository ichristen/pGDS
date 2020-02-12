#include "material.hpp"

MATERIAL::MATERIAL() {
    
}
MATERIAL::MATERIAL(std::string name_, int16_t layer_, std::array<GLfloat, 4> colorFill_, std::array<GLfloat, 4> colorOutline_, GLdouble fontThick, GLdouble fontHeight) {
    name = name_;
    layer = layer_;
    
    font = FONT(layer, fontThick, fontHeight);
    
    setFill(colorFill_);
    setOutline(colorOutline_);
}

MATERIAL MATERIAL::getLayer(uint16_t l) {
    return layers[l];
}

void MATERIAL::setLayer(MATERIAL m, uint16_t l) {
    layers[l] = m;
}

void MATERIAL::setFill(   std::array<GLfloat, 4> colorFill_) {
    colorFill[0] = colorFill_[0];
    colorFill[1] = colorFill_[1];
    colorFill[2] = colorFill_[2];
    colorFill[3] = colorFill_[3];
}
void MATERIAL::setOutline(std::array<GLfloat, 4> colorOutline_) {
    colorOutline[0] = colorOutline_[0];
    colorOutline[1] = colorOutline_[1];
    colorOutline[2] = colorOutline_[2];
    colorOutline[3] = colorOutline_[3];
}
void MATERIAL::print() {
    printf("MATERIAL %s for layer %i", name.c_str(), layer);
}
void MATERIAL::glFillColor() {
#ifdef pGDSGLFW
    GLuint cID = glGetUniformLocation(shaders, "c");
//    glUniform4fv(cID, 4, colorFill);
    glUniform4f(cID, colorFill[0], colorFill[1], colorFill[2], colorFill[3]);
#endif
}
void MATERIAL::glOutlineColor() {
#ifdef pGDSGLFW
    GLuint cID = glGetUniformLocation(shaders, "c");
//    glUniform4fv(cID, 4, colorOutline);
    glUniform4f(cID, colorOutline[0], colorOutline[1], colorOutline[2], colorOutline[3]);
//    printf("%i, [%f,%f,%f,%f]\n", shaders, colorOutline[0], colorOutline[1], colorOutline[2], colorOutline[3]);
#endif
}

uint16_t MATERIAL::currentLayer = 1;

std::map<uint16_t, MATERIAL> MATERIAL::layers =   { {0, MATERIAL("Helper",              0, {.8, .8, .8, 1}, {.5, .5, .5, 1}, 1., 7.)},
                                                    {1, MATERIAL("LiNbO_3",             1, {.12, .6, 1, 1}, {.56, .8, 1, 1}, 1./2, 7./2)},
                                                    {2, MATERIAL("Au",                  2, {1,  .73, .15,  1}, {1, .82, .43, 1}, 3., 18.)},
//                                                    {2, MATERIAL("Au",                  2, {1,  .73, .15,  1}, {1, .82, .43, 1}, 1., 7.)},
                                                    {3, MATERIAL("Oxide Window",        3, {.5, .5, .5, 1}, {1, 1, 1, 1}, 2., 14.)},
                                                    {4, MATERIAL("Au2",                 4, {.8, .6, .11,  1}, {.81, .66, .35,  1}, 3., 18.)},
//                                                    {4, MATERIAL("Au2",                 4, {.8, .6, .11,  1}, {.81, .66, .35,  1}, 2., 14.)},
                                                    {5, MATERIAL("Diamond",             5, {.8, .8, .8, 1}, {.8, .8, .8, 1}, 1./7., 1)},
                                                    {6, MATERIAL("AuSi",                6, {1, .6, .11,  1}, {1, .66, .35,  1}, 2., 14.)},
                                                    {7, MATERIAL("SiHelper",            7, {.8, 0, 0, 1}, {1, 0, 0, 1}, 1./14, .5)} };

GLuint MATERIAL::shaders = 0;

//uint16_t currentMaterial = 1;




