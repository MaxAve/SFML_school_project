#include <iostream>
#include "../../include/resources/Fonts.hpp"

namespace Fonts {
sf::Font pixel;

void initFonts() {
    if (!pixel.openFromFile("resources/fonts/Pix32.ttf")) {
        std::cerr << "Error in Fonts::initFonts()" << std::endl;
    }
}
}