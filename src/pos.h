//
// Created by aleferna on 19/09/24.
//

#ifndef L_SYSTEMS_POS_H
#define L_SYSTEMS_POS_H


// This is used to check if two positions are the same within a precision range
struct CollisionPos {
    int16_t x = 0;
    int16_t y = 0;

    bool operator==(const CollisionPos& other) const = default;
};

// This is used when we need the real position of the object
struct Pos {
    float x = 0;
    float y = 0;

    Pos(float x, float y) : x(x), y(y) {}

    Pos(
        const CollisionPos &pos,
        const uint16_t precision
    ) : x(pos.x / (float) precision),
        y(pos.y / (float) precision) {}
};

#endif //L_SYSTEMS_POS_H
