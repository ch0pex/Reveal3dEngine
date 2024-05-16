/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file math_test.cpp
 * @version 1.0
 * @date 13/05/2024
 * @brief Math module unit testing
 *
 */

#include <gtest/gtest.h>
#include "math/math.hpp"

LogLevel loglevel = logDEBUG;

namespace reveal3d {

class Vector3Test : public testing::Test {
protected:
    Vector3Test() {
        v1_ = {0.0f, 0.0f, 0.0f};
        v2_ = {3.3f, 2.1f, -12.8f};
        v3_ = {-1.2f, 4.0f, 2000.2f};
    }

    math::xvec3 v1_;
    math::xvec3 v2_;
    math::xvec3 v3_;
};

class Vector4Test : public testing::Test {
protected:
    Vector4Test() {
        v1_ = {0.0f, 0.0f, 0.0f, 1.2f};
        v2_ = {3.3f, 2.1f, -12.8f, 3.0f};
        v3_ = {-1.2f, 4.0f, 2000.2f, 4.2f};
    }

    math::xvec4 v1_;
    math::xvec4 v2_;
    math::xvec4 v3_;
};



TEST(Vector3Test, Add) {

}

TEST(Vector3Test, Sub) {

}

TEST(Vector3Test, Mul) {

}

TEST(Vector3Test, Div) {

}

TEST(Vector4Test, Add) {

}

TEST(Vector4Test, Sub) {

}

TEST(Vector4Test, Mul) {

}

TEST(Vector4Test, Div) {

}

}
