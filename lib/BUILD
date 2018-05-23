source_prefix = "src/"
PUBLIC_SOURCES = ["SceneAugmenter.cpp"]
SOURCES = ["shared/ImageConversionUtils.cpp",
        "core/CircleBuilder.cpp",
        "core/FeatureExtractor.cpp",
        "core/FeatureModelGenerator.cpp",
        "core/FeatureModel.cpp",
        "core/KeypointDetector.cpp",
        "core/Transformation.cpp",
        "core/homography/SanityChecker.cpp",
        "core/homography/Builder.cpp",
        "core/homography/Evaluator.cpp",
        "CorrespondenceFinder.cpp",
        "MatchingPoints.cpp",
        "SceneAugmenterPri.cpp",
        "SceneFeatureExtractor.cpp",
        "TransformationFitter.cpp"]

header_prefix = "include/"
PUBLIC_HEADERS = ["SceneAugmenter.hpp"]
HEADERS = ["shared/Definitions.hpp",
        "shared/ImageConversionUtils.hpp",
        "core/CircleBuilder.hpp",
        "core/Definitions.hpp",
        "core/FeatureExtractor.hpp",
        "core/FeatureMatcher.hpp",
        "core/FeatureModelGenerator.hpp",
        "core/FeatureModel.hpp",
        "core/KeypointDetector.hpp",
        "core/Transformation.hpp",
        "core/homography/Definitions.hpp",
        "core/homography/SanityChecker.hpp",
        "core/homography/Builder.hpp",
        "core/homography/Evaluator.hpp",
        "CorrespondenceFinder.hpp",
        "Definitions.hpp",
        "MatchingPoints.hpp",
        "SceneAugmenterPri.hpp",
        "SceneFeatureExtractor.hpp",
        "TransformationFitter.hpp"]
        
cc_library(
    name = "scene_augmenter_internal",

    srcs = [source_prefix + source for source in SOURCES],
    hdrs = [header_prefix + header for header in HEADERS],

    deps = ["@opencv//:opencv_imgproc",
            "@opencv//:opencv_core"],
    includes = ["include"],

    linkstatic = 1,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "exposed_headers",
    srcs = [header_prefix + header for header in PUBLIC_HEADERS],
    includes = ["include"],

    linkstatic = 1,
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "libsceneaugmenter.so",

    srcs = [source_prefix + source for source in PUBLIC_SOURCES] +
            [header_prefix + header for header in PUBLIC_HEADERS] +
            [source_prefix + source for source in SOURCES] +
            [header_prefix + header for header in HEADERS],

    deps = ["@opencv//:opencv_imgproc",
            "@opencv//:opencv_core"],
    includes = ["include"],

    linkshared = 1,
    visibility = ["//visibility:public"],
)

