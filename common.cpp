#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common.h"

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


void readParameters(const char* file, cv::Mat& M1, cv::Mat& M2, cv::Mat& D1, cv::Mat& D2, cv::Mat& R, cv::Mat& T, cv::Mat& E, cv::Mat& F)
{
    cv::FileStorage fs(file, cv::FileStorage::READ);
    if (fs.isOpened())
    {
        fs["left_camera_matrix"] >> M1;
        fs["right_camera_matrix"] >> M2;
        fs["left_coefficients_vector"] >> D1;
        fs["right_coefficients_vector"] >> D2;
        fs["stereo_rotation_matrix"] >> R;
        fs["stereo_translation_vector"] >> T;
        fs["stereo_essential_matrix"] >> E;
        fs["stereo_fundamental_matrix"] >> F;
        fs.release();
    }
    else
    {
        std::cout << "Não foi possível salvar no arquivo " << file << std::endl;
        exit(-1);
    }
}


void rectify(const cv::Mat& M1, const cv::Mat& M2, const cv::Mat& D1, const cv::Mat& D2, const cv::Mat& R, const cv::Mat& T,
             cv::Mat& image_left, cv::Mat& image_right)
{
    cv::Mat R1, R2, P1, P2, Q, map11, map12, map21, map22;

    cv::stereoRectify(M1, D1, M2, D2, image_left.size(), R, T, R1, R2, P1, P2, Q);

    cv::initUndistortRectifyMap(M1, D1, R1, P1, image_left.size(), CV_16SC2, map11, map12);
    cv::initUndistortRectifyMap(M2, D2, R2, P2, image_left.size(), CV_16SC2, map21, map22);

    cv::remap(image_left, image_left, map11, map12, cv::INTER_LINEAR);
    cv::remap(image_right, image_right, map21, map22, cv::INTER_LINEAR);

    cv::Mat disparity_map, depth;
    calculateDisparity(image_left, image_right, disparity_map);
    
    calculateDepth(disparity_map, P2.at<double>(0,3), 1, depth);
}