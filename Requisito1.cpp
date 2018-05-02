#include <iostream>
#include <cmath>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common.h"

int main(int argc, char** argv)
{
    cv::Mat disparity_map, depth;
    float baseline, focal_distance;

    if (argc != 5)
    {
        std::cout << "Uso: " << argv[0] << " baseline distancia_focal imagem_esquerda imagem_direita" << std::endl;
        return -1;
    }

    baseline = atoi(argv[1]);
    focal_distance = atoi(argv[2]);

    cv::Mat image_left = cv::imread(argv[3], CV_LOAD_IMAGE_COLOR);
    if (image_left.empty())
    {
        std::cout << "Não foi possível abrir a imagem " << argv[3] << std::endl;
        return -1;
    }

    cv::Mat image_right = cv::imread(argv[4], CV_LOAD_IMAGE_COLOR);
    if (image_right.empty())
    {
        std::cout << "Não foi possível abrir a imagem " << argv[4] << std::endl;
        return -1;
    }

    calculateDisparity(image_left, image_right, disparity_map);

    calculateDepth(disparity_map, baseline, focal_distance, depth);

    cv::waitKey(0);

    return 0;
}
