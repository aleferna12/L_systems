//
// Created by alefe on 08/04/2025.
//

#include "libraries/pico_display_2/pico_display_2.hpp"
#include "graphics.h"

Graphics::Graphics(
    uint16_t width,
    uint16_t height,
    pimoroni::Rotation rot
) : screen(
        width,
        height,
        rot,
        false,
        get_spi_pins(pimoroni::BG_SPI_FRONT)
    ),
    pen(
        screen.width,
        screen.height,
        nullptr
    ),
    foreground(pen.create_pen(255, 255, 255)),
    background(pen.create_pen(0, 0, 0)) {}

void Graphics::line(const Pos &p1, const Pos &p2, uint16_t width) {
    pen.set_pen(foreground);
    if (width == 1)
        pen.line({(int32_t) p1.x, (int32_t) p1.y}, {(int32_t) p2.x, (int32_t) p2.y});
    else
        pen.thick_line(
            {(int32_t) p1.x, (int32_t) p1.y},
            {(int32_t) p2.x, (int32_t) p2.y},
            width
        );
}

void Graphics::clear() {
    pen.set_pen(background);
    pen.clear();
}

void Graphics::update() {
    screen.update(&pen);
}
