# Focus Stacking

This repository contains simple implementation of _focus stacking_ technique, 
which is used in digital image processing. It combines multiple images taken at different
focus distances to give a resulting image with a greated _depth of field_ than any
of the individual source images. [[Wikipedia]](https://en.wikipedia.org/wiki/Focus_stacking)

## Requirements and Technical Info

To run this project **OpenCV 3** is required (used for images disk operations).


## Build and Run Instruction

**Building:**

In order to build this program, please download and extract this repository and then 
follow below steps:

```bash
cd focus-stacking
mkdir build
cd build

cmake ../
make
```

**Running:**

After compilation two executables should be created: _runAllTests_ and _focusStacking_.

In order to run tests, execute:
 ```bash
 ./runAllTests
 ```

In order to run focus stacking, execute (all arguments are optional):

```bash
./focusStacking [images_directory [bg_threshold [kernels_size [gaussian_sigma]]]]
```

Example:
```bash
./focusStacking ../example-images/ 70 7 5.0
```

After processing results are stored in the same director as executable. Results are two JPG images, 
i.e. `focus_image.jpg` and `depth_map.jpg`. 

**Parameters:**

You can specify following arguments:
- `images_directory` [string] - relative path to directory where images are 
stored (default=../example-images/)
- `bg_threshold` [unsigned int] - threshold value used to decide whether edges 
are meaningful or whether they belong to image background (default=70)
- `kernels_size` [unsigned int] - size of gaussian and laplacian filters (default=7)
- `gaussian_sigma` [float] - sigma parameter of gaussian kernel (default=5.0)

## Example Results

|All in Focus Image|Depth Map|
|:---:|:---:|
|`bg_threshold=25`, `kernels_size=3`, `gaussian_sigma=2.0`||
|![all-in-focus](http://luniak.io/focus-stacking-repo-images/focus_image_25_3_2.jpg)|![depth-map](http://luniak.io/focus-stacking-repo-images/depth_map_25_3_2.jpg)|
|`bg_threshold=70`, `kernels_size=7`, `gaussian_sigma=5.0`||
|![all-in-focus](http://luniak.io/focus-stacking-repo-images/focus_image_70_7_5.jpg)|![depth-map](http://luniak.io/focus-stacking-repo-images/depth_map_70_7_5.jpg)|
|`bg_threshold=160`, `kernels_size=13`, `gaussian_sigma=7.0`||
|![all-in-focus](http://luniak.io/focus-stacking-repo-images/focus_image_160_13_7.jpg)|![depth-map](http://luniak.io/focus-stacking-repo-images/depth_map_160_13_7.jpg)|