#pragma once
#include "linalg.h"
#include "rubik-cube-constants.h"

const VEC3 EYE = VEC3(0, 0, 5); // camera position
const VEC3 LOOKAT = CUBE_CENTER; // camera look at
const VEC3 UP = VEC3(0, 1, 0); // camera up (for orientation)

const double FOV = M_PI / 2.0f; // field of view
const double ASPECT = 1.0f; // aspect ratio of screen (width / height)

constexpr double ZOOM_PAN_SPEED = 0.1f; // speed of zooming in/out
constexpr double ROTATION_SPEED = M_PI / 20.0f; // speed of rotating the camera
