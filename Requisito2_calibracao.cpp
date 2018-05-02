#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>


// Square side size in millimeter
const float square_size = 28.5;

// Chessboard pattern (8x6)
const cv::Size board_size(8, 6);

// Calibration file
const std::string parameters_file("parameters.xml");


void readImages(const char *images_path, std::vector<std::vector<cv::Mat>>& images)
{
    cv::Mat new_image;

    // Read all .jpg files
    struct dirent* dp;
    DIR* dirp = opendir(images_path);
    while ((dp = readdir(dirp)) != nullptr)
    {
        if (std::string(dp->d_name).find("R.jpg") != std::string::npos)
        {
            std::string right_image_path = std::string(images_path) + "/" + dp->d_name;
            std::string left_image_path(right_image_path);
            std::replace(left_image_path.begin(), left_image_path.end(), 'R', 'L');

            std::cout << "Adicionando as imagens " << right_image_path << " e " << left_image_path << std::endl;

            new_image = cv::imread(right_image_path, CV_LOAD_IMAGE_COLOR);
            if (new_image.empty())
            {
                std::cout << "Não foi possível abrir a imagem " << right_image_path << std::endl;
                exit(-1);
            }
            images.at(0).push_back(new_image);

            new_image = cv::imread(left_image_path, CV_LOAD_IMAGE_COLOR);
            if (new_image.empty())
            {
                std::cout << "Não foi possível abrir a imagem " << left_image_path << std::endl;
                exit(-1);
            }
            images.at(1).push_back(new_image);
        }
    }
}


int main(int argc, char** argv)
{
    std::vector<std::vector<cv::Mat>> images(2);
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<std::vector<cv::Point2f>>> image_points(2);

    if (argc != 2)
    {
        std::cout << "Uso: " << argv[0] << " calibracao_dir" << std::endl;
        return -1;
    }

    const char *images_path = argv[1];

    readImages(images_path, images);

    // Calculate the object expected points
    std::vector<cv::Point3f> board_points;
    for (int i = 0; i < board_size.height; i++)
        for (int j = 0; j < board_size.width; j++)
            board_points.push_back(cv::Point3f(j * square_size, i * square_size, 0));

    for (uint i = 0; i < images.size(); i++)
    {
        for (uint j = 0; j < images.at(i).size(); j++)
        {
            std::vector<cv::Point2f> chess_points;
            bool found = cv::findChessboardCorners(images.at(i).at(j), board_size, chess_points);

            if (!found)
            {
                std::cout << "Não foi possível encontrar o tabuleiro na imagem " << i << " x " << j << std::endl;
                return -1;
            }

            image_points.at(i).push_back(chess_points);

            if (i == 0)
                object_points.push_back(board_points);
        }
    }

    // Calibrate
    cv::Mat M1 = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat M2 = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat D1, D2, R, T, E, F;
    cv::stereoCalibrate(object_points, image_points.at(0), image_points.at(1), M1, D1, M2, D2, images.at(0).at(0).size(),
                        R, T, E, F, cv::CALIB_FIX_ASPECT_RATIO | cv::CALIB_ZERO_TANGENT_DIST | cv::CALIB_SAME_FOCAL_LENGTH);
    
    std::cout << "\nCalibração concluida, salvando resultados em " << parameters_file << std::endl;
    cv::FileStorage fs(parameters_file, cv::FileStorage::WRITE);

    if (fs.isOpened())
    {
        fs << "left_camera_matrix" << M1;
        fs << "right_camera_matrix" << M2;
        fs << "left_coefficients_vector" << D1;
        fs << "right_coefficients_vector" << D2;
        fs << "stereo_rotation_matrix" << R;
        fs << "stereo_translation_vector" << T;
        fs << "stereo_essential_matrix" << E;
        fs << "stereo_fundamental_matrix" << F;
        fs.release();
    }
    else
    {
        std::cout << "Não foi possível salvar no arquivo " << parameters_file << std::endl;
    }


    cv::waitKey(0);

    return 0;
}