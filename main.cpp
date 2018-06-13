#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cstdint>

#include "src/images_utils.h"
#include "src/focus_stack.h"

struct Params {
    std::string images_dir{"../example-images/"};
    int bg_threshold{70};
    unsigned short kernels_size{7};
    float gaussian_sigma{5.0f};

    void setParameters(int argc, char *argv[]);
};

int main(int argc, char* argv[]) {

    Params params;
    params.setParameters(argc, argv);

    FocusStack focus_stack;

    try {
        images_utils::readImagesFromDirToFocusStack(params.images_dir, focus_stack);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    auto start = std::chrono::steady_clock::now();

    focus_stack.computeAllInFocusAndDepthMap(params.kernels_size, params.gaussian_sigma, params.bg_threshold);

    auto duration = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start);
    std::cout << "processing time: " << duration.count() << " ms" << std::endl;

    Matrix<uint8_t> depth_map = focus_stack.getDepthMap();
    Matrix<uint8_t> all_in_focus_image = focus_stack.getAllInFocusImage();

    images_utils::storeImageOnDisk("depth_map.jpg", depth_map);
    images_utils::storeImageOnDisk("focus_image.jpg", all_in_focus_image);

    return 0;
}

void Params::setParameters(int argc, char *argv[]) {
    if (argc >= 2)
        images_dir = std::string(argv[1]);
    if (argc >= 3)
        bg_threshold = atoi(argv[2]);
    if (argc >= 4)
        kernels_size = static_cast<unsigned short>(atoi(argv[3]));
    if (argc >= 5)
        gaussian_sigma = static_cast<float>(atof(argv[4]));
}