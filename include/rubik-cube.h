#pragma once
#include "rubik-cube-constants.h"
#include "block.h"

enum class Face {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    FRONT,
    BACK
};

class RubikCube {
    public:
        RubikCube();
        ~RubikCube();

        void RotateFace(enum Face face);
        Block cube[CUBE_DIMENSIONS_SIZE][CUBE_DIMENSIONS_SIZE][CUBE_DIMENSIONS_SIZE];
};


