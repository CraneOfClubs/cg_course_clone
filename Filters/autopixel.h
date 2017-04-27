#ifndef AUTOPIXEL_H
#define AUTOPIXEL_H
#include <stdint.h>

class AutoPixel
{
public:
    AutoPixel();
    AutoPixel(uint8_t Red, uint8_t Green, uint8_t Blue);
    AutoPixel(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);
    void setPixel(uint32_t Packed);
    void setPixel(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);
    void setPixel(uint8_t Red, uint8_t Green, uint8_t Blue);
    uint32_t getPacked();
    uint8_t getRed();
    uint8_t getBlue();
    uint8_t getGreen();
    uint8_t getAlpha();
    AutoPixel(uint32_t Packed);
private:
    struct {
        uint8_t _red;
        uint8_t _green;
        uint8_t _blue;
        uint8_t _alpha;
    } colors;
};

#endif // AUTOPIXEL_H
