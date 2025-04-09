//
// Created by alefe on 08/04/2025.
//

#ifndef L_SYSTEMS_GRAPHICS_H
#define L_SYSTEMS_GRAPHICS_H

#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "pos.h"

class Graphics {
public:
    Graphics(uint16_t width, uint16_t height, pimoroni::Rotation rot);

    void clear();

    void line(const Pos &p1, const Pos &p2, uint16_t width=1);

    void update();

    pimoroni::ST7789 screen;
    pimoroni::PicoGraphics_PenRGB332 pen;
    pimoroni::Pen foreground;
    pimoroni::Pen background;
};


#endif //L_SYSTEMS_GRAPHICS_H
