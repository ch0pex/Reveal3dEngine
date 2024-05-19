/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file matrix_test.cpp
 * @version 1.0
 * @date 13/05/2024
 * @brief Short description
 *
 * Longer description
 */

#include <gtest/gtest.h>
#include "math/math.hpp"

LogLevel loglevel = logDEBUG;

namespace reveal3d {

class Matrix3Test : public testing::Test {
protected:
    Matrix3Test() {
    }

    math::mat3 v1_;
    math::mat3 v2_;
    math::mat3 v3_;
};

class Matrix4Test : public testing::Test {
protected:
    Matrix4Test() {
    }

    math::mat4 v1_;
    math::mat4 v2_;
    math::mat4 v3_;
};

TEST(Matrix3Test, Init) {

}

TEST(Matrix3Test, Add) {

}

TEST(Matrix3Test, Sub) {

}

TEST(Matrix3Test, Mul) {

}

TEST(Matrix3Test, Div) {

}

TEST(Matrix4Test, Init) {

}

TEST(Matrix4Test, Add) {

}

TEST(Matrix4Test, Sub) {

}

TEST(Matrix4Test, Mul) {

}

TEST(Matrix4Test, Div) {

}

}
