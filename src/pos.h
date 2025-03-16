//
// Created by aleferna on 19/09/24.
//

#ifndef L_SYSTEMS_POS_H
#define L_SYSTEMS_POS_H


// This is used to check if two positions are the same within a precision range
struct CollisionPos {
    int x = 0;
    int y = 0;

    bool operator==(const CollisionPos& other) const = default;
};

// This is used when we need the real position of the object
struct Pos {
    double x = 0;
    double y = 0;

    Pos(
        const CollisionPos &pos,
        const unsigned int precision
    ) : x(pos.x / (double) precision),
        y(pos.y / (double) precision) {}
};

#endif //L_SYSTEMS_POS_H
