cc_binary(
    name = "scene_augmenter_sample",

    srcs = ["src/sceneAugmenterImageList.cpp"] +
            ["//lib:libsceneaugmenter.so"],

    data = glob(["assets/source.png",
            "assets/replacement.jpg",
            "assets/targets.txt",
            "assets/model.bin",
            "assets/targets/*"]),

    deps = ["//lib:exposed_headers"] + 
            ["@opencv//:opencv_imgcodecs",
            "@opencv//:opencv_imgproc",
            "@opencv//:opencv_core"],

    args = ["samples/assets/source.png",
            "samples/assets/replacement.jpg",
            "samples/assets/targets.txt",
            "samples/assets/model.bin",
            "samples/assets/output/"],

    visibility = ["//visibility:public"],
)

cc_binary(
    name = "scene_augmenter_image_list",

    srcs = ["src/sceneAugmenterImageList.cpp"] +
            ["//lib:libsceneaugmenter.so"],

    data = glob(["assets/source.png",
            "assets/replacement.jpg",
            "assets/targets.txt",
            "assets/model.bin",
            "assets/targets/*"]),

    deps = ["//lib:exposed_headers"] + 
            ["@opencv//:opencv_imgcodecs",
            "@opencv//:opencv_imgproc",
            "@opencv//:opencv_core"],

    visibility = ["//visibility:public"],
)

cc_binary(
    name = "feature_model_generator",

    srcs = ["src/featureModelGenerator.cpp"],

    deps = ["//lib:scene_augmenter_internal",
            "@opencv//:opencv_imgproc",
            "@opencv//:opencv_core"],

    visibility = ["//visibility:public"],
)

