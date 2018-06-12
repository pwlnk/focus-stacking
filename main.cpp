#include <iostream>

#include "src/images_utils.h"
#include "src/focus_stack.h"
#include "src/gaussian_kernel.h"

std::string getImagesDir(int argc, char* argv[], std::string default_images_dir);

int main(int argc, char* argv[]) {

    std::string images_dir = getImagesDir(argc, argv, "input/");

    FocusStack focus_stack;
    images_utils::readImagesFromDirToFocusStack(images_dir, focus_stack);

    std::clock_t start;
    double duration;
    start = std::clock();

    focus_stack.computeAllInFocusAndDepthMap();

    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << "processing time: " << duration << std::endl;

    Matrix<uint8_t> depth_map = focus_stack.getDepthMap();
    Matrix<uint8_t> all_in_focus_image = focus_stack.getAllInFocusImage();

    images_utils::showImage("Depth Map", depth_map);
    images_utils::showImage("All in Focus Image", all_in_focus_image);

    images_utils::storeImageOnDisk("../../depth_map.jpg", depth_map);
    images_utils::storeImageOnDisk("../../focus_image.jpg", all_in_focus_image);
    
    cv::waitKey(0);

    return 0;
}

std::string getImagesDir(int argc, char* argv[], std::string default_images_dir) {
    if (argc == 2) {
        return std::string(argv[1]);
    }

    return default_images_dir;
}