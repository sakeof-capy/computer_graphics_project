#include "Model.hpp"
#include "Error.hpp"
#include <iostream>

using model::Model;
using model::ModelParsingError;

void line(int x0, int y0, int x1, int y1, cv::Mat& image, cv::Vec3b color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++)
    {
        float t = (x - x0) / (float) (x1 - x0);
        int y = y0 * (1. - t) + y1 * t;
        if (x > 0 && y > 0)
        {
            if (steep)
            {
                image.at<cv::Vec3b>(x, y) = color;
            }
            else
            {
                image.at<cv::Vec3b>(y, x) = color;
            }
        }
    }
}

int main()
{
    std::string imageName("D:/Images/07/1.tif");

    const int width = 800;
    const int height = 800;

    cv::Mat image_readed = cv::imread(imageName.c_str(), cv::IMREAD_COLOR);

    cv::Mat image(width, height, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::imshow("Show empty image", image);

    const std::expected<Model, ModelParsingError> maybe_model 
        = model::parse_model("./models/african_head.obj");
    
    if (!maybe_model.has_value()) [[unlikely]]
    {
        std::cerr << "Error: " << stringify_error_variant(maybe_model.error());
        std::cerr.flush();
        return EXIT_FAILURE;
    }

    const Model& model = maybe_model.value();
    const cv::Vec3b white { 255, 255, 255 };

    for (int i = 0; i < model.faces.size(); i++)
    {
        const std::vector<int>& face = model.faces[i];
        for (int j = 0; j < 3; j++)
        {
            cv::Vec3f v0 = model.verts[face[j]];
            cv::Vec3f v1 = model.verts[face[(j + 1) % 3]];
            int x0 = (1. - v0[0]) * width / 2.;
            int y0 = (1. - v0[1]) * height / 2.;
            int x1 = (1. - v1[0]) * width / 2.;
            int y1 = (1. - v1[1]) * height / 2.;
            line(x0, y0, x1, y1, image, white);
        }
    }
    std::cout.flush();
    cv::imshow("Display window", image);

    cv::waitKey(0);

    system("Pause");
    return EXIT_SUCCESS;
}
