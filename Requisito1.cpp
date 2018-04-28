#include <iostream>
#include <cmath>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void calculateDisparity(const cv::Mat& image_left, const cv::Mat& image_right, cv::Mat& disparity_map)
{
    cv::Mat gray_image_left, gray_image_right, disparity_map_8bits, col_sum;

    cv::cvtColor(image_left, gray_image_left, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image_right, gray_image_right, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(/* minDisparity = */ 0,
                                                          /* numDisparities = */ 192,
                                                          /* blockSize = */ 9,
                                                          /* P1 = */ 216,
                                                          /* P2 = */ 864,
                                                          /* disp12MaxDiff = */ 1,
                                                          /* preFilterCap = */ 63,
                                                          /* uniquenessRatio = */ 10,
                                                          /* speckleWindowSize = */ 100,
                                                          /* speckleRange = */ 32);

    sgbm->compute(gray_image_left, gray_image_right, disparity_map);


    // Remove the left part of the image that is present only on the left image, therefore the sum
    // of the values in these columns are equal to zero
    normalize(disparity_map, disparity_map_8bits, 0, 255, CV_MINMAX, CV_8U);    
    cv::reduce(disparity_map_8bits, col_sum, 0, CV_REDUCE_SUM, CV_32S);
    for (int i = 0; col_sum.at<int>(0, i) == 0; i++)
        disparity_map = disparity_map.colRange(1, disparity_map.cols);

    normalize(disparity_map, disparity_map_8bits, 0, 255, CV_MINMAX, CV_8U);
    cv::imshow("disparity_map", disparity_map_8bits);
}


void calculateDepth(const cv::Mat& disparity_map, double baseline, double focal_distance, cv::Mat& depth_map)
{
    depth_map = cv::Mat(disparity_map.size(), CV_64F);

    disparity_map.forEach<int16_t>
    (
        [&] (const int16_t &pixel, const int * position) -> void
        {
            double depth = (baseline * focal_distance) / pixel;

            if (std::isinf(depth))
                depth = 0;

            depth_map.at<double>(position[0], position[1]) = depth;
        }
    );

    cv::Mat depth_8bits;
    normalize(depth_map, depth_8bits, 0, 255, CV_MINMAX, CV_8U);
    cv::imshow("depth_map", depth_8bits);
}


int main(int argc, char** argv)
{
    cv::Mat disparity_map, depth;
    float baseline, focal_distance;

    if (argc != 5)
    {
        std::cout << "N" << std::endl;
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
