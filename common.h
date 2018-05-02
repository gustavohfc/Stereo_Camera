// Common functions

#ifndef COMMON_H
#define COMMON_H

void calculateDisparity(const cv::Mat& image_left, const cv::Mat& image_right, cv::Mat& disparity_map);

void calculateDepth(const cv::Mat& disparity_map, double baseline, double focal_distance, cv::Mat& depth_map);

void readParameters(const char* file, cv::Mat& M1, cv::Mat& M2, cv::Mat& D1, cv::Mat& D2, cv::Mat& R, cv::Mat& T, cv::Mat& E, cv::Mat& F);

void rectify(const cv::Mat& M1, const cv::Mat& M2, const cv::Mat& D1, const cv::Mat& D2, const cv::Mat& R, const cv::Mat& T,
             cv::Mat& image_left, cv::Mat& image_right);

#endif  // COMMON_H
