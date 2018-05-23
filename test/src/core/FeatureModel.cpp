#include "gtest/gtest.h"

#include "core/FeatureModel.hpp"

/**
 * Ensure that FeatureModel initializes with a valid model.
 */
TEST(initializeFeatureModel, validModel) {
    EXPECT_NO_THROW(core::FeatureModel("test/assets/feature_models/valid.bin"));
}

/**
 * Ensure that FeatureModel throws an exception when it detects an
 * invalid model.
 */
TEST(initializeFeatureModel, invalidModels) {
    // Invalid paths
    EXPECT_ANY_THROW(core::FeatureModel(""));
    EXPECT_ANY_THROW(core::FeatureModel("does/not/exist.bin"));
    // Path to image rather than a model
    EXPECT_ANY_THROW(core::FeatureModel("test/assets/images/test.jpg"));
    // Path to corrupted models
    EXPECT_ANY_THROW(core::FeatureModel(
            "test/assets/feature_models/invalid_only_metadata.bin"));
    EXPECT_ANY_THROW(core::FeatureModel(
            "test/assets/feature_models/invalid_pts_out_of_range.bin"));
    EXPECT_ANY_THROW(core::FeatureModel(
            "test/assets/feature_models/invalid_too_few_bytes.bin"));
    EXPECT_ANY_THROW(core::FeatureModel(
            "test/assets/feature_models/invalid_too_many_bytes.bin"));
}

