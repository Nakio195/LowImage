#ifndef COLOR_H
#define COLOR_H

#include <string>

class Color
{
public:
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    Color(std::string color);

    std::string toString();

    uint8_t luminosity();

public:
    uint8_t mR;
    uint8_t mG;
    uint8_t mB;
};

#endif // COLOR_H
