/**
 * Contains useful high-level constants, aliases for complex data structures and functions.
 */

#pragma once

#include <stdexcept>

namespace shared {

static constexpr float TWO_PI = 2.0f*CV_PI;
static constexpr float MAX_CHAR_VALUE = 255.0f;
static constexpr float INVERSE_MAX_CHAR_VALUE = 1.0f/MAX_CHAR_VALUE;

inline void VALIDATE_ARGUMENT(bool predicate, const std::string& errorMessage) {
    if (!predicate) {
        throw std::invalid_argument(errorMessage);
    }
}

}

