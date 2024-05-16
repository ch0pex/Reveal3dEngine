/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scalar.cpp
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

class ScalarTest  : public testing::Test {
protected:
    ScalarTest() {
        s1_ = 1.0f;
        s2_ = 32.2f;
        s3_ = -23.f;
    }

    math::scalar s1_;
    math::scalar s2_;
    math::scalar s3_;
};


TEST(ScalarTest, Add) {

}

TEST(ScalarTest, Sub) {

}

TEST(ScalarTest, Mul) {

}

TEST(ScalarTest, Div) {

}


}
