# BUILD file modified from https://github.com/hatstand/symmetrical-octo-fiesta
cc_library(
    name = "opencv_core",

    srcs = glob(["modules/core/src/**/*.cpp",
            "modules/core/src/**/*.hpp",
            "modules/core/include/**/*.hpp",
            "modules/core/include/**/*.h"]) +
            ["opencv2/opencv_modules.hpp",
            "cvconfig.h",
            "custom_hal.hpp",
            "opencl_kernels_core.hpp",
            "version_string.inc"],
    hdrs = ["modules/core/include/opencv2/core/opencl/ocl_defs.hpp"],

    copts = ["-D__OPENCV_BUILD",
            "-Iexternal/opencv/3rdparty/zlib"],
    deps = [":zlib"],
    includes = ["modules/core/include"],
    linkopts = ["-ldl", "-lpthread"],

    visibility = ["//visibility:public"],
    linkstatic = 1,
)

cc_library(
    name = "opencv_imgproc",

    srcs = glob(["modules/imgproc/src/**/*.cpp",
            "modules/imgproc/src/**/*.hpp",
            "modules/imgproc/src/**/*.h",
            "modules/imgproc/include/**/*.hpp",
            "modules/imgproc/include/**/*.h"]) + 
            ["opencl_kernels_imgproc.hpp"],

    copts = ["-D__OPENCV_BUILD"],
    deps = [":opencv_core"],
    includes = ["modules/imgproc/include"],

    visibility = ["//visibility:public"],
    linkstatic = 1,
)

cc_library(
    name = "opencv_imgcodecs",

    srcs = glob(["modules/imgcodecs/src/**/*.cpp",
            "modules/imgcodecs/src/**/*.hpp",
            "modules/imgcodecs/include/**/*.hpp",
            "modules/imgcodecs/include/**/*.h"]),

    copts = ["-D__OPENCV_BUILD",
            "-Iexternal/opencv/3rdparty/libpng",
            "-Iexternal/opencv/3rdparty/libjpeg",
            "-Iexternal/opencv/3rdparty/zlib"],
    includes = ["modules/imgcodecs/include"],
    deps = [":opencv_core",
            ":opencv_imgproc",
            ":libpng",
            ":libjpeg",
            ":zlib"],

    visibility = ["//visibility:public"],
    linkstatic = 1,
)

cc_library(
    name = "libjpeg",

    srcs = glob(["3rdparty/libjpeg/*.c", "3rdparty/libjpeg/*.h"]),

    copts = ["-Wno-cast-align",
            "-Wno-shadow",
            "-Wno-unused"],

    visibility = ["//visibility:public"],
    linkstatic = 1,
)

cc_library(
    name = "libpng",

    srcs = glob(["3rdparty/libpng/*.c", "3rdparty/libpng/*.h"]),
    hdrs = ["3rdparty/libpng/png.h"],

    copts = ["-Wno-cast-align"],
    deps = [":zlib"],

    visibility = ["//visibility:public"],
    linkstatic = 1,
)

cc_library(
    name = "zlib",

    srcs = glob(["3rdparty/zlib/*.c", "3rdparty/zlib/*.h"]),
    hdrs = ["3rdparty/zlib/zconf.h"] +
            ["3rdparty/zlib/zlib.h"],

    copts = ["-Wno-shorten-64-to-32",
            "-Wno-attributes",
            "-Wno-strict-prototypes",
            "-Wno-missing-prototypes",
            "-Wno-missing-declarations"],

    visibility = ["//visibility:public"],
    linkstatic = 1,
)


genrule(
    name = "opencv_modules",
    cmd = """
        echo '#define HAVE_OPENCV_CORE' > $@
        echo '#define HAVE_OPENCV_IMGPROC' >> $@
        echo '#define HAVE_OPENCV_IMGCODECS' >> $@
    """,
    outs = ["opencv2/opencv_modules.hpp"],
)

genrule(
    name = "cvconfig",
    outs = ["cvconfig.h"],
    cmd = """
        echo '#define HAVE_PNG' > $@
        echo '#define HAVE_JPEG' >> $@
    """,
)

genrule(
    name = "custom_hal",
    outs = ["custom_hal.hpp"],
    cmd = "touch $@",
)

genrule(
    name = "opencv_core_kernels",
    outs = ["opencl_kernels_core.hpp"],
    cmd = """
        echo '#include "opencv2/core/ocl.hpp"' > $@
        echo '#include "opencv2/core/ocl_genbase.hpp"' >> $@
        echo '#include "opencv2/core/opencl/ocl_defs.hpp"' >> $@
    """,
)

genrule(
    name = "version_string",
    outs = ["version_string.inc"],
    cmd = """
        echo '\"OpenCV 3.1.0\"' > $@
    """,
)

genrule(
    name = "opencv_imgproc_kernels",
    outs = ["opencl_kernels_imgproc.hpp"],
    cmd = """
        echo '#include "opencv2/core/ocl.hpp"' > $@
        echo '#include "opencv2/core/ocl_genbase.hpp"' >> $@
        echo '#include "opencv2/core/opencl/ocl_defs.hpp"' >> $@
    """,
)

genrule(
    name = "zconf",
    srcs = ["3rdparty/zlib/zconf.h.cmakein"],
    outs = ["3rdparty/zlib/zconf.h"],
    cmd = """
        sed '/^#cmakedefine/ d' $< > $@
    """,
)

