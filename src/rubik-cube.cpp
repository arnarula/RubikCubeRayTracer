#include "rubik-cube.h"
#include "linalg.h"

RubikCube::RubikCube() {
    // calculates center of top left block given the center of the Rubik's cube
    const VEC3 offset = VEC3(CUBE_DIMENSIONS_SIZE - 1, CUBE_DIMENSIONS_SIZE - 1, CUBE_DIMENSIONS_SIZE - 1);
    const VEC3 posOffset = offset * (CUBE_BLOCK_SCALE + CUBE_BLOCK_GAP) / 2.0f;
    VEC3 cornerBlockPos = CUBE_CENTER - posOffset;

    // set positions of the cube blocks
    for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                cube[i][j][k].setPosition(
                    cornerBlockPos + VEC3(i, j, k) * (CUBE_BLOCK_SCALE + CUBE_BLOCK_GAP)
                );
            }
        }
    }
}

void RubikCube::RotateFace(enum Face face)
{
    MAT3 rotations[CUBE_DIMENSIONS_SIZE][CUBE_DIMENSIONS_SIZE];

    if (face == Face::BACK) {
        int k = 0;
        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
                rotations[i][j] = cube[i][j][k].axes;
            }
        }

        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
                cube[i][j][k].setRotation(ROTATION_Z * rotations[j][CUBE_DIMENSIONS_SIZE - 1 - i]);
            }
        }
    }

    if (face == Face::FRONT) {
        int k = CUBE_DIMENSIONS_SIZE - 1;
        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
                rotations[i][j] = cube[i][j][k].axes;
            }
        }

        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
                cube[i][j][k].setRotation(ROTATION_Z.transpose() * rotations[CUBE_DIMENSIONS_SIZE - 1 - j][i]);
            }
        }
    }

    if (face == Face::LEFT) {
        int i = 0;
        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                rotations[j][k] = cube[i][j][k].axes;
            }
        }

        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                cube[i][j][k].setRotation(ROTATION_X * rotations[k][CUBE_DIMENSIONS_SIZE - 1 - j]);
            }
        }
    }

    if (face == Face::RIGHT) {
        int i = CUBE_DIMENSIONS_SIZE - 1;
        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                rotations[j][k] = cube[i][j][k].axes;
            }
        }

        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                cube[i][j][k].setRotation(ROTATION_X.transpose() * rotations[CUBE_DIMENSIONS_SIZE - 1 - k][j]); 
            }
        }
    }

    if (face == Face::BOTTOM) {
        int j = 0;
        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                rotations[i][k] = cube[i][j][k].axes;
            }
        }

        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                cube[i][j][k].setRotation(ROTATION_Y * rotations[CUBE_DIMENSIONS_SIZE - 1 - k][i]);
            }
        }
    }

    if (face == Face::TOP) {
        int j = CUBE_DIMENSIONS_SIZE - 1;
        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                rotations[i][k] = cube[i][j][k].axes;
            }
        }

        for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                cube[i][j][k].setRotation(ROTATION_Y.transpose() * rotations[k][CUBE_DIMENSIONS_SIZE - 1 - i]);
            }
        }
    }

}

RubikCube::~RubikCube() {
    
}