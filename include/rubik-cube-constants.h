#pragma once
#include "linalg.h"
#include <array>

constexpr int CUBE_DIMENSIONS_SIZE = 4; // number of blocks in each dimension of rubik's cube

const VEC3 CUBE_CENTER = VEC3(0, 0, 0); // center of the cube
constexpr double CUBE_BLOCK_SCALE = 0.5f; // block scale of the cube
constexpr double CUBE_BLOCK_GAP = 0.1f; // gap between blocks of the cube (for visibility)

const VEC3 CUBE_FACE_COLORS[6] = {
    VEC3(0, 1, 0),          // right (x-axis) - green
    VEC3(0, 0, 1),          // left - blue
    VEC3(1, 0, 0),          // top (y-axis) - red
    VEC3(1, 0.5, 0),        // bottom - orange
    VEC3(0.8, 0.8, 0.8),    // front (z-axis) - white
    VEC3(1, 1, 0)           // back - yellow
};

constexpr double TURN_RADIUS = M_PI / 2.0f; 
constexpr double SELECTED_BLOCK_DAMPENING_PCT = 0.6; 
