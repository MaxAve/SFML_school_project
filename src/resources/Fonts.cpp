#include <iostream>
#include "resources/Fonts.hpp"

namespace Fonts {
sf::Font pixel;

void initFonts() {
    if (!pixel.openFromFile("resources/fonts/Jersey10-Regular.ttf")) {
        std::cerr << "Error in Fonts::initFonts()" << std::endl;
    }
}
}