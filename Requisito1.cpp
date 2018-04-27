#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void calculateDepth(const cv::Mat& image_left, const cv::Mat& image_right, cv::Mat& depth)
{
    cv::Mat gray_image_left, gray_image_right;

    cv::cvtColor(image_left, gray_image_left, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image_right, gray_image_right, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create();

    // bm->setMinDisparity(0);
    // bm->setNumDisparities(128);
    bm->setBlockSize(21);
    bm->setSpeckleRange(32);
    bm->setSpeckleWindowSize(100);

    bm->compute(gray_image_left, gray_image_right, depth);

    cv::imshow("teste1", depth);
    depth.convertTo(depth, CV_8U);
    cv::imshow("teste2", depth);
}


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "N" << std::endl;
        return -1;
    }

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

    cv::Mat depth;
    calculateDepth(image_left, image_right, depth);

    cv::waitKey(0);

    return 0;
}
