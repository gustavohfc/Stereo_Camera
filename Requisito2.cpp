#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "common.h"

// Calibration file
const char* parameters_file = "parameters.xml";

int main(int argc, char** argv)
{
    cv::Mat disparity_map, depth;
    cv::Mat M1, M2, D1, D2, R, T, E, F;

    cv::Mat image_left = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if (image_left.empty())
    {
        std::cout << "Não foi possível abrir a imagem " << argv[1] << std::endl;
        return -1;
    }

    cv::Mat image_right = cv::imread(argv[2], CV_LOAD_IMAGE_COLOR);
    if (image_right.empty())
    {
        std::cout << "Não foi possível abrir a imagem " << argv[2] << std::endl;
        return -1;
    }

    readParameters(parameters_file, M1, M2, D1, D2, R, T, E, F);

    rectify(M1, M2, D1, D2, R, T, image_left, image_right);

    // calculateDisparity(image_left, image_right, disparity_map);

    cv::waitKey(0);

    return 0;
}