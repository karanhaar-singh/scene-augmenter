#include "gtest/gtest.h"

#include "core/CircleBuilder.hpp"
#include "core/Definitions.hpp"


/**
 * Verifies correctness when building circles of various radii.
 */
TEST(typicalCircleBuilder, variousRadii) {
    EXPECT_EQ(core::CircleBuilder::execute(0u), std::vector<cv::Point>(
            {{0, 0}}));
    EXPECT_EQ(core::CircleBuilder::execute(1u), std::vector<cv::Point>(
            {{1, 0}, {0, -1}, {-1, 0}, {0, 1}}));
    EXPECT_EQ(core::CircleBuilder::execute(2u), std::vector<cv::Point>(
            {{2, 0}, {2, -1}, {1, -2},
            {0, -2}, {-1, -2}, {-2, -1},
            {-2, 0}, {-2, 1}, {-1, 2},
            {0, 2}, {1, 2}, {2, 1}}));
    EXPECT_EQ(core::CircleBuilder::execute(3u), std::vector<cv::Point>(
            {{3, 0}, {3, -1}, {2, -2}, {1, -3},
            {0, -3}, {-1, -3}, {-2, -2}, {-3, -1},
            {-3, 0}, {-3, 1}, {-2, 2}, {-1, 3},
            {0, 3}, {1, 3}, {2, 2}, {3, 1}}));
}

