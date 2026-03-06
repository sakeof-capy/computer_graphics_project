#include "Error.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

using model::Model;
using model::ModelParsingError;

enum class DrawMode
{
    Lines,
    Triangles
};

void drawModelLines(
    const Model& model,
    cv::Mat& image,
    float angleX,
    float angleY,
    float angleZ
)
{
    const int width = image.cols;
    const int height = image.rows;
    const cv::Vec3b white{255, 255, 255};

    for (const auto& face : model.faces)
    {
        for (int j = 0; j < 3; j++)
        {
            const cv::Vec3f v0 = rotateXYZ(
                model.verts[face[j]],
                angleX,
                angleY,
                angleZ
            );
            const cv::Vec3f v1 = rotateXYZ(
                model.verts[face[(j + 1) % 3]],
                angleX,
                angleY,
                angleZ
            );

            const int x0 =
                static_cast<int>((1.f + v0[0]) * width / 2.f);
            const int y0 =
                static_cast<int>((1.f - v0[1]) * height / 2.f);
            const int x1 =
                static_cast<int>((1.f + v1[0]) * width / 2.f);
            const int y1 =
                static_cast<int>((1.f - v1[1]) * height / 2.f);

            line(x0, y0, x1, y1, image, white);
        }
    }
}

void drawModelTriangles(
    const Model& model,
    cv::Mat& image,
    const float angleX,
    const float angleY,
    const float angleZ,
    const std::vector<cv::Vec3b>& face_colors
)
{
    const int width = image.cols;
    const int height = image.rows;

    for (size_t i = 0; i < model.faces.size(); i++)
    {
        const auto& face = model.faces[i];

        const cv::Vec3f v0 =
            rotateXYZ(model.verts[face[0]], angleX, angleY, angleZ);
        const cv::Vec3f v1 =
            rotateXYZ(model.verts[face[1]], angleX, angleY, angleZ);
        const cv::Vec3f v2 =
            rotateXYZ(model.verts[face[2]], angleX, angleY, angleZ);

        const cv::Point p0(
            static_cast<int>((1.f + v0[0]) * width / 2.f),
            static_cast<int>((1.f - v0[1]) * height / 2.f)
        );
        const cv::Point p1(
            static_cast<int>((1.f + v1[0]) * width / 2.f),
            static_cast<int>((1.f - v1[1]) * height / 2.f)
        );
        const cv::Point p2(
            static_cast<int>((1.f + v2[0]) * width / 2.f),
            static_cast<int>((1.f - v2[1]) * height / 2.f)
        );

        triangle(p0, p1, p2, image, face_colors[i]);
    }
}

int main()
{
    const int width = 800;
    const int height = 800;

    const std::expected<Model, ModelParsingError> maybe_model =
        model::parse_model("./models/african_head.obj");

    if (!maybe_model.has_value())
    {
        std::cerr << "Error: "
                  << stringify_error_variant(maybe_model.error())
                  << "\n";
        return EXIT_FAILURE;
    }

    const Model& model = maybe_model.value();

    // Modern C++ random: Mersenne Twister seeded from hardware
    // entropy
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 255);

    std::vector<cv::Vec3b> face_colors(model.faces.size());
    for (auto& c : face_colors)
        c = cv::Vec3b(dist(rng), dist(rng), dist(rng));

    DrawMode mode = DrawMode::Triangles;

    cv::namedWindow("3D Rotation", cv::WINDOW_AUTOSIZE);
    std::cout << "Press T for triangles, L for lines, any other key "
                 "to quit.\n";

    int frame = 0;
    while (true)
    {
        float t = frame * 0.02f;
        float angleX = 0.3f * std::sin(t * 0.7f);
        float angleY = t;
        float angleZ = 0.2f * std::cos(t * 1.3f);

        cv::Mat
            image(height, width, CV_8UC3, cv::Scalar(50, 100, 20));

        switch (mode)
        {
        case DrawMode::Lines:
            drawModelLines(model, image, angleX, angleY, angleZ);
            break;
        case DrawMode::Triangles:
            drawModelTriangles(
                model,
                image,
                angleX,
                angleY,
                angleZ,
                face_colors
            );
            break;
        }

        cv::imshow("3D Rotation", image);

        const int key = cv::waitKey(20);
        if (key == 't' || key == 'T')
        {
            mode = DrawMode::Triangles;
        }
        else if (key == 'l' || key == 'L')
        {
            mode = DrawMode::Lines;
        }
        else if (key >= 0)
        {
            break;
        }

        frame++;
    }

    return EXIT_SUCCESS;
}