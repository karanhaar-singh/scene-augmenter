# Scene Augmenter 

Scene Augmenter is a library that can automatically find and replace planar objects in a scene.  For example, in a scene containing a certain book on chess (the "source" object), you can replace its cover with an image of your choosing, such as the Mona Lisa (the "replacement" object).

| Original Scene                   | Augmented Scene                     |
|----------------------------------|-------------------------------------|
| ![](/assets/target.png?raw=true) | ![](/assets/augmented.png?raw=true) |

## Getting Started

This project uses the `bazel` build system, which handles building, unit testing, running, and managing third-party dependencies automatically.  You can install `bazel` by following the instructions provided by the [Official Documentation](https://docs.bazel.build/versions/master/install.html).

After installing `bazel`, the rest is very simple:

Building the library and running all unit tests:
```sh
bazel test :scene_augmenter_tests
```

Building the library and running the sample with default arguments:
```sh
bazel run samples:scene_augmenter_sample
```

Location of the output "augmented" images:
```sh
bazel-bin/samples/scene_augmenter_sample.runfiles/__main__/samples/assets/output
```

The Scene Augmenter uses [OpenCV 3.1.0](https://opencv.org/opencv-3-1.html).  You don't have to worry about installation or managing potential conflicts with any other OpenCV installation on your machine, `bazel` takes care of that for you by downloading and building OpenCV 3.1.0 to a localized sandbox.

## Using The Library

### Running on your own images

To use the library on your own images, run
```sh
bazel run samples:scene_augmenter_image_list <input source image path> <input replacement image path> <input target image list path> <model path> <output folder>
```
with the arguments of your choice.  The default arguments are as follows:

* `samples/assets/source.png`
* `samples/assets/replacement.jpg`
* `samples/assets/targets.txt`
* `samples/assets/model.bin`
* `samples/assets/output/`

Do note that in order for `bazel` to recognize arguments using local paths, they need to be visible in the localized sandbox.  You can accomplish this by modifying
```sh
samples/BUILD
```
Please see the [documentation](https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary) on `cc_binary` for more information.  Alternatively, you can use arguments using absolute paths without issue.

### The Scene Augmenter API

At the highest level, the Scene Augmenter library is defined by the `SceneAugmenter` class.  The constructor expects a model path, which is provided to you as shown above.  After constructing a new object, you can define a new "source" and "replacement" image using the following API calls:

* `setSourceImage`
* `setReplacementImage`

These API calls expect images that are frontal scans of the respective planar objects.  The following source and replacement images were used for the sample results shown above:

| Source Image                     | Replacement Image                     |
|----------------------------------|---------------------------------------|
| ![](/assets/source.png?raw=true) | ![](/assets/replacement.jpg?raw=true) |

After setting both the source and replacement images, you can now perform augmentation in full-frame scene images by calling `execute` with sample results shown at the beginning of this README.  For more information on the API, please refer to the documentation in the `SceneAugmenter` header file:
```sh
lib/include/SceneAugmenter.hpp
```

## How It Works

The Scene Augmenter library is implemented as a pipeline of more fundamental Computer Vision modules:

* `KeypointDetector`: Detects salient [keypoints](https://en.wikipedia.org/wiki/Interest_point_detection) in an image.
* `SceneFeatureExtractor`: Extracts [feature vectors](https://en.wikipedia.org/wiki/Feature_vector) from the keypoint locations in the image.
* `CorrespondenceFinder`: Finds matches between keypoints from two images using the feature vectors.  Ideally, a match between two keypoints would lie on the same object present in both images.
* `TransformationFitter`: Fits a transformation, in this case a [homography](https://en.wikipedia.org/wiki/Homography_(computer_vision)), to the matches in the presence of outliers.
* `augment`: Given the transformation, we can now transform a "replacement" image onto the "target" scene image replacing the planar object defined by the "source" image

This library provides independent implementations of the aforementioned modules using only OpenCV 3.1.0's core and imgproc modules.  The algorithms used are well known in literature and are referenced in the documentation of the source code.  A more thorough introduction to a similar pipeline is detailed in the [OpenCV: Feature Detection and Description](https://docs.opencv.org/3.3.0/db/d27/tutorial_py_table_of_contents_feature2d.html) documentation.

## Performance

### Speed

All benchmarks are evaluated on a server with the following specs:

* OS: Ubuntu 16.04.3 LTS 64-bit Server edition
* gcc: 5.4.0
* CPU: Intel Core i7-6700K CPU @ 4.00GHz
* RAM: 16GB

Speed on included sample images:

* Latency: 0.10 to 0.20 seconds
* Throughput: 5 to 10 FPS

The library is internally multithreaded which allows for improved latency on a frame-by-frame basis.

### Accuracy

When searching for the source object in a target scene image, the algorithm is:

* Naturally shift-invariant
* Reasonably rotation-invariant
* Minorly scale-invariant

Certain algorithms in this pipeline have non-deterministic implementations meaning that final results on identical inputs can vary from run to run.

## Limitations

This library was designed as part of a personal project to explore the properties of various simple but powerful computer vision algorithms and their interactions.  This library serves more as a modular and flexible framework to facilitate such exploration and is not intended to serve as a refined competitor to similar existing algorithms.  Such algorithms will likely outperform this library in both speed and accuracy.

