cc_test(
    name = "scene_augmenter_tests",

    srcs = ["src/core/CircleBuilder.cpp",
            "src/core/FeatureModel.cpp",
            "src/core/FeatureExtractor.cpp",
            "src/core/FeatureMatcher.cpp",
            "src/core/KeypointDetector.cpp",
            "src/core/Transformation.cpp",
            "src/shared/ImageConversionUtils.cpp",
            "src/CorrespondenceFinder.cpp",
            "src/MatchingPoints.cpp",
            "src/SceneAugmenterPri.cpp",
            "src/SceneFeatureExtractor.cpp",
            "src/TransformationFitter.cpp"],

    data = ["assets/images/test.jpg",
            "assets/feature_models/invalid_only_metadata.bin",
            "assets/feature_models/invalid_pts_out_of_range.bin",
            "assets/feature_models/invalid_too_few_bytes.bin",
            "assets/feature_models/invalid_too_many_bytes.bin",
            "assets/feature_models/valid.bin"],

    copts = ["-Iexternal/gtest/googletest/include"],
    deps = ["//lib:scene_augmenter_internal",
            "@opencv//:opencv_imgproc",
            "@opencv//:opencv_core",
            "@gtest//:main",],

    visibility = ["//visibility:public"],
)

