#include "Color.h"
#include <stdlib.h>
#include <iostream>

Color::Color(uint8_t r, uint8_t g, uint8_t b)
{
    mR = r; mG = g; mB = b;
}

Color::Color(std::string color)
{
    size_t first = color.find(",");

    if(first != std::string::npos && first != 0)
    {
        size_t second = color.find(",", first+1);

        if(second != std::string::npos)
        {
            std::string r = color.substr(0, first);
            std::string g = color.substr(first+1, second-first-1);
            std::string b = color.substr(second+1);

            if(r.size() != 0 && g.size() != 0 && b.size() != 0)
            {
                std::cout << r << std::endl;
                std::cout << g << std::endl;
                std::cout << b << std::endl;

                mR = std::stoi(r);
                mG = std::stoi(g);
                mB = std::stoi(b);
                return;
            }
        }
    }

    mR = 0;
    mG = 0;
    mB = 0;
}

std::string Color::toString()
{
    return std::to_string(mR) + "," + std::to_string(mG) + "," + std::to_string(mB);
}

uint8_t Color::luminosity()
{
    return (mR+mG+mB)/(3);
}
