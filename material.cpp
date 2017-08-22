#include "material.hpp"

MATERIAL::MATERIAL() {
    
}
MATERIAL::MATERIAL(std::string name_, int16_t layer_, std::array<int, 3> colorFill_, std::array<int, 3> colorOutline_, GLdouble fontThick, GLdouble fontHeight) {
    name = name_;
    layer = layer_;
    
    font = FONT(layer, fontThick, fontHeight);
    
    setFill(colorFill_);
    setOutline(colorOutline_);
}

MATERIAL MATERIAL::getLayer(uint16_t l) {
    return layers[l];
}

void MATERIAL::setFill(   std::array<int, 3> colorFill_) {
    colorFill[0] = colorFill_[0];
    colorFill[1] = colorFill_[1];
    colorFill[2] = colorFill_[2];
}
void MATERIAL::setOutline(std::array<int, 3> colorOutline_) {
    colorOutline[0] = colorOutline_[0];
    colorOutline[1] = colorOutline_[1];
    colorOutline[2] = colorOutline_[2];
}

uint16_t MATERIAL::currentLayer = 1;

std::map<uint16_t, MATERIAL> MATERIAL::layers =   { {1, MATERIAL("LiNbO_3",             1, {0,0,127},       {0,0,255}, 1./2, 7./2)},
                                                    {2, MATERIAL("Au",                  2, {255, 188, 38},  {255, 210, 110})},
                                                    {3, MATERIAL("Oxide Uncovering",    3, {127, 127, 127}, {255, 255, 255})},
                                                    {4, MATERIAL("Au2",                 4, {205, 151, 29},  {207, 170, 88})} };

//uint16_t currentMaterial = 1;




