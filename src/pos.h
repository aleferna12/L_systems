//
// Created by aleferna on 19/09/24.
//

#ifndef L_SYSTEMS_POS_H
#define L_SYSTEMS_POS_H

#include <cstddef>

// This is used to check if two positions are the same within a precision range
struct CollisionPos {
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const CollisionPos& other) const = default;
};

// This is used when we need the real position of the object
struct Pos {
    double x = 0;
    double y = 0;
    double z = 0;

    Pos(
        const CollisionPos &pos,
        unsigned int precision
    ) : x(pos.x / (double) precision),
        y(pos.y / (double) precision),
        z(pos.z / (double) precision) {}
};

struct DevState {
    CollisionPos pos = {};
    double ax = 0.;
    double ay = 0;
};

struct pos_hash {
    std::size_t operator()(const CollisionPos &pos) const {
        return pos.x ^ pos.y ^ pos.z;
    }
};


#endif //L_SYSTEMS_POS_H
