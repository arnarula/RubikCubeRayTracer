#pragma once
#include "math.h"
#include "rubik-cube-constants.h"

// building components of the cube
struct Block
{
    VEC3 position = CUBE_CENTER;
    const double scale = CUBE_BLOCK_SCALE;

    MAT3 axes = MAT3::Identity(); // local axes of the block
    bool selected = false;

    Block() {}
    Block(VEC3 position, double scale) : position(position), scale(scale) {}

    void setPosition(VEC3 pos) {
        position = pos;
    }

    void setRotation(MAT3 rotation) {
        axes = rotation;
    }
};
