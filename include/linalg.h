#pragma once
#include <Eigen>
#include <iostream>

using VEC2 = Eigen::Vector2f;
using VEC3 = Eigen::Vector3f;
using VEC4 = Eigen::Vector4f;
using MAT3 = Eigen::Matrix3f;
using MAT4 = Eigen::Matrix4f;

static const MAT3 ROTATION_X = (MAT3() << 1, 0, 0, 0, 0, -1, 0, 1, 0).finished();
static const MAT3 ROTATION_Y = (MAT3() << 0, 0, 1, 0, 1, 0, -1, 0, 0).finished();
static const MAT3 ROTATION_Z = (MAT3() << 0, -1, 0, 1, 0, 0, 0, 0, 1).finished();

// rotate vector around x, y, z axes
inline VEC3 rotateVector(VEC3 vector, const VEC3& rotation)
{
    MAT3 rotationMatrix;
    rotationMatrix =  Eigen::AngleAxisf(rotation[0], VEC3::UnitX())
                    * Eigen::AngleAxisf(rotation[1], VEC3::UnitY())
                    * Eigen::AngleAxisf(rotation[2], VEC3::UnitZ());
    return rotationMatrix * vector;
}

inline void printVec(const VEC3& vec)
{
    std::cout << vec[0] << " " << vec[1] << " " << vec[2] << std::endl;
}
