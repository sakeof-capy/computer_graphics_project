#include "Model.hpp"
#include "Error.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using model::Model;
using model::ModelParsingError;

// Bresenham-style line drawing
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
        float t = (x - x0) / static_cast<float>(x1 - x0 + 1e-5);
        int y = static_cast<int>(y0 * (1. - t) + y1 * t);
        if (x >= 0 && y >= 0 && x < image.cols && y < image.rows)
        {
            if (steep)
                image.at<cv::Vec3b>(x, y) = color;
            else
                image.at<cv::Vec3b>(y, x) = color;
        }
    }
}

cv::Vec3f rotateXYZ(const cv::Vec3f& v, float angleX, float angleY, float angleZ)
{
    const float cosX = std::cos(angleX); 
    const float sinX = std::sin(angleX);
    const float cosY = std::cos(angleY);
    const float sinY = std::sin(angleY);
    const float cosZ = std::cos(angleZ);
    const float sinZ = std::sin(angleZ);

    // Rotate around X
    cv::Vec3f rx = cv::Vec3f(v[0],
                             v[1] * cosX - v[2] * sinX,
                             v[1] * sinX + v[2] * cosX);
    // Rotate around Y
    cv::Vec3f ry = cv::Vec3f(rx[0] * cosY + rx[2] * sinY,
                             rx[1],
                             -rx[0] * sinY + rx[2] * cosY);
    // Rotate around Z
    cv::Vec3f rz = cv::Vec3f(ry[0] * cosZ - ry[1] * sinZ,
                             ry[0] * sinZ + ry[1] * cosZ,
                             ry[2]);
    return rz;
}

int main()
{
    const int width = 800;
    const int height = 800;
    const cv::Vec3b white { 255, 255, 255 };

    const std::expected<Model, ModelParsingError> maybe_model 
        = model::parse_model("./models/african_head.obj");
    
    if (!maybe_model.has_value())
    {
        std::cerr << "Error: " << stringify_error_variant(maybe_model.error()) << "\n";
        return EXIT_FAILURE;
    }

    const Model& model = maybe_model.value();

    cv::namedWindow("3D Rotation", cv::WINDOW_AUTOSIZE);

    // Animation loop
    int frame = 0;
    while (true)
    {
        float t = frame * 0.02f; // time parameter
        float angleX = 0.3f * std::sin(t * 0.7f);   // tilt up/down
        float angleY = t;                            // main spin
        float angleZ = 0.2f * std::cos(t * 1.3f);   // twist

        cv::Mat image(height, width, CV_8UC3, cv::Scalar(50, 100, 20));

        for (const auto& face : model.faces)
        {
            for (int j = 0; j < 3; j++)
            {
                cv::Vec3f v0 = rotateXYZ(model.verts[face[j]], angleX, angleY, angleZ);
                cv::Vec3f v1 = rotateXYZ(model.verts[face[(j + 1) % 3]], angleX, angleY, angleZ);

                int x0 = static_cast<int>((1.f + v0[0]) * width / 2.f);
                int y0 = static_cast<int>((1.f - v0[1]) * height / 2.f);
                int x1 = static_cast<int>((1.f + v1[0]) * width / 2.f);
                int y1 = static_cast<int>((1.f - v1[1]) * height / 2.f);

                line(x0, y0, x1, y1, image, white);
            }
        }

        cv::imshow("3D Rotation", image);

        const int key = cv::waitKey(20);
        
        if (key >= 0) 
        {
            break;
        }

        frame++;
    }

    return EXIT_SUCCESS;
}