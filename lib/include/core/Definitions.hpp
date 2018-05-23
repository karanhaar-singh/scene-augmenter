/**
 * Contains useful constants, aliases for complex data structures and functions.
 */

#pragma once

#include <bitset>
#include <vector>

#include "opencv2/core.hpp"

namespace core {

static constexpr uint32_t NUM_BRIEF_BITS = 128u;
using FeatureVector = std::bitset<NUM_BRIEF_BITS>;

}

