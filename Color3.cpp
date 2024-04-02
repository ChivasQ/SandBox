#include "Color3.h"

Color3::Color3(float R, float G, float B) {
    this->R = R;
    this->G = G;
    this->B = B;
}
Color3::Color3() = default;

//Color3 Color3::add(Color3 color)
//{
//    return Color3(this->R + color.R, this->B + color.B, this->G + color.G);
//}