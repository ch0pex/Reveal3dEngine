/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file math.cpp
 * @version 1.0
 * @date 02/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include <gtest/gtest.h>
#include "math/math.hpp"

using namespace reveal3d::math;

const xvec3 vec3_1 = {1.5f,1.5f,5.0f};
const xvec3 vec3_2 = {1.5f,6.0f,1.5f};
const xvec4 vec4_1 = {5.2f, 23.2f, 5.0f,4.5f};
const xvec4 vec4_2 = {2.0f, 23.2f, 2.0f,-4.5f};
mat4 mat4_1(vec3_1, vec3_2, vec4_1, vec4_2);
mat4 mat4_2(vec3_1, vec3_2, vec4_1, vec4_2);


TEST(Math, Vec3Add) { xvec3 result = {3,7.5,6.5};
}

TEST(Math, Vec3Sub) { xvec3 result = {0,4.5,-3.5};
}



TEST(Math, Vec4Add) { xvec4 result = { 7.2, 46.4, 7, 0};
}

TEST(Math, Vec4sub) { xvec4 result = { 3.19999981, 0.0f, 3.0f, 9.0f};
}

TEST(Math, Mat3) {

}

TEST(Math, Mat4) {

}

TEST(Math, Mat4ConstructFromMat3) {

}
