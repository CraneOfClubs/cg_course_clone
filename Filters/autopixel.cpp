#include "autopixel.h"

AutoPixel::AutoPixel()
{
    colors._red = 0;
    colors._green = 0;
    colors._blue = 0;
    colors._alpha = 0;
}

AutoPixel::AutoPixel(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha)
{
    colors._red = Red;
    colors._green = Green;
    colors._blue = Blue;
    colors._alpha = Alpha;
}

AutoPixel::AutoPixel(uint8_t Red, uint8_t Green, uint8_t Blue)
{
    colors._red = Red;
    colors._green = Green;
    colors._blue = Blue;
    colors._alpha = 0;
}

AutoPixel::AutoPixel(uint32_t Packed)
{
    colors._red = Packed >> 24 & 0xFF;
    colors._green = Packed >> 16 & 0xFF;
    colors._blue = Packed >> 8 & 0xFF;
    colors._alpha = Packed & 0xFF;
}

void AutoPixel::setPixel(uint32_t Packed) {
    colors._red = Packed >> 24 & 0xFF;
    colors._green = Packed >> 16 & 0xFF;
    colors._blue = Packed >> 8 & 0xFF;
    colors._alpha = Packed & 0xFF;
}

void AutoPixel::setPixel(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha)
{
    colors._red = Red;
    colors._green = Green;
    colors._blue = Blue;
    colors._alpha = Alpha;
}

void AutoPixel::setPixel(uint8_t Red, uint8_t Green, uint8_t Blue)
{
    colors._red = Red;
    colors._green = Green;
    colors._blue = Blue;
    colors._alpha = 0;
}

uint32_t AutoPixel::getPacked() {
    uint32_t buf = 0;
    buf = buf | (colors._alpha);
    buf = buf | (colors._red << 24);
    buf = buf | (colors._green << 16);
    buf = buf | (colors._blue << 8);
    return buf;
}

uint8_t AutoPixel::getRed() {
    return colors._red;
}

uint8_t AutoPixel::getBlue() {
    return colors._blue;
}

uint8_t AutoPixel::getGreen() {
    return colors._green;
}

uint8_t AutoPixel::getAlpha() {
    return colors._alpha;
}

