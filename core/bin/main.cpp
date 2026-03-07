#include "Error.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <string_view>

using model::Model;
using model::ModelParsingError;

using Mat4 = cv::Matx44f;

enum class RenderMode
{
    Lines,
    Triangles
};
enum class TransformMode
{
    Default,
    Translate,
    Scale,
    RotateX,
    RotateY,
    RotateZ,
    Combined
};

// clang-format off
Mat4 translationMatrix(float tx, float ty, float tz)
{
    return Mat4(
        1, 0, 0, tx, 
        0, 1, 0, ty, 
        0, 0, 1, tz, 
        0, 0, 0, 1
    );
}

Mat4 scaleMatrix(float sx, float sy, float sz)
{
    return Mat4(
        sx, 0, 0, 0, 
        0, sy, 0, 0, 
        0, 0, sz, 0, 
        0, 0, 0, 1
    );
}

Mat4 rotateXMatrix(float a)
{
    const float c = std::cos(a), s = std::sin(a);
    
    return Mat4(
        1, 0, 0, 0, 
        0, c, -s, 0, 
        0, s, c, 0, 
        0, 0, 0, 1
    );
}

Mat4 rotateYMatrix(float a)
{
    const float c = std::cos(a), s = std::sin(a);
    
    return Mat4(
        c, 0, s, 0, 
        0, 1, 0, 0, 
        -s, 0, c, 0, 
        0, 0, 0, 1
    );
}

Mat4 rotateZMatrix(float a)
{
    const float c = std::cos(a), s = std::sin(a);
    return Mat4(
        c, -s, 0, 0, 
        s, c, 0, 0, 
        0, 0, 1, 0, 
        0, 0, 0, 1
    );
}

Mat4 rotateXYZMatrix(float angleX, float angleY, float angleZ)
{
    const float cx = std::cos(angleX), sx = std::sin(angleX);
    const float cy = std::cos(angleY), sy = std::sin(angleY);
    const float cz = std::cos(angleZ), sz = std::sin(angleZ);

    return Mat4(
         cy*cz,             -cy*sz,              sy,    0,
         sx*sy*cz + cx*sz,  -sx*sy*sz + cx*cz,  -sx*cy, 0,
        -cx*sy*cz + sx*sz,   cx*sy*sz + sx*cz,   cx*cy, 0,
         0,                  0,                  0,     1
    );
}

// clang-format on

cv::Vec3f applyTransform(const Mat4& m, const cv::Vec3f& v)
{
    const cv::Vec4f h(v[0], v[1], v[2], 1.f);
    const cv::Vec4f r = m * h;
    return cv::Vec3f(r[0], r[1], r[2]);
}

cv::Point project(const cv::Vec3f& v, int width, int height)
{
    return {
        static_cast<int>((1.f + v[0]) * width / 2.f),
        static_cast<int>((1.f - v[1]) * height / 2.f)
    };
}

void drawFaces(
    const Model& model,
    cv::Mat& image,
    const Mat4& transform,
    const std::vector<cv::Vec3b>& face_colors,
    RenderMode render
)
{
    const int w = image.cols, h = image.rows;
    const cv::Vec3b white{255, 255, 255};

    for (size_t i = 0; i < model.faces.size(); i++)
    {
        const auto& face = model.faces[i];

        const cv::Point p0 = project(
            applyTransform(transform, model.verts[face[0]]),
            w,
            h
        );
        const cv::Point p1 = project(
            applyTransform(transform, model.verts[face[1]]),
            w,
            h
        );
        const cv::Point p2 = project(
            applyTransform(transform, model.verts[face[2]]),
            w,
            h
        );

        if (render == RenderMode::Triangles)
        {
            triangle(p0, p1, p2, image, face_colors[i]);
        }
        else
        {
            line(p0.x, p0.y, p1.x, p1.y, image, white);
            line(p1.x, p1.y, p2.x, p2.y, image, white);
            line(p2.x, p2.y, p0.x, p0.y, image, white);
        }
    }
}

Mat4 buildTransform(TransformMode mode, float t)
{
    switch (mode)
    {
    case TransformMode::Default:
        return rotateXYZMatrix(
            0.3f * std::sin(t * 0.7f),
            t,
            0.2f * std::cos(t * 1.3f)
        );

    case TransformMode::Translate:
        return translationMatrix(
            0.4f * std::sin(t),
            0.3f * std::cos(t * 1.3f),
            0.f
        );

    case TransformMode::Scale:
    {
        const float s = 1.f + 0.5f * std::sin(t * 2.f);
        return rotateYMatrix(t * 0.5f) * scaleMatrix(s, s, s);
    }

    case TransformMode::RotateX:
        return rotateXMatrix(t);
    case TransformMode::RotateY:
        return rotateYMatrix(t);
    case TransformMode::RotateZ:
        return rotateZMatrix(t);

    case TransformMode::Combined:
        return translationMatrix(
                   0.3f * std::sin(t * 0.5f),
                   0.2f * std::cos(t * 0.7f),
                   0.f
               ) *
               rotateXYZMatrix(
                   0.3f * std::sin(t * 0.7f),
                   t,
                   0.2f * std::cos(t * 1.3f)
               ) *
               scaleMatrix(
                   1.f + 0.15f * std::sin(t * 2.f),
                   1.f + 0.15f * std::cos(t * 2.3f),
                   1.f
               );
    }
    return Mat4::eye();
}

std::optional<RenderMode> parseRenderMode(std::string_view arg)
{
    if (arg == "--lines")
        return RenderMode::Lines;
    if (arg == "--triangles")
        return RenderMode::Triangles;
    return std::nullopt;
}

std::optional<TransformMode> parseTransformMode(std::string_view arg)
{
    if (arg == "--default")
        return TransformMode::Default;
    if (arg == "--translate")
        return TransformMode::Translate;
    if (arg == "--scale")
        return TransformMode::Scale;
    if (arg == "--rotate-x")
        return TransformMode::RotateX;
    if (arg == "--rotate-y")
        return TransformMode::RotateY;
    if (arg == "--rotate-z")
        return TransformMode::RotateZ;
    if (arg == "--combined")
        return TransformMode::Combined;
    return std::nullopt;
}

static constexpr std::string_view USAGE =
    "Usage: <program> [--lines|--triangles]"
    " [--default|--translate|--scale|--rotate-x|--rotate-y|--rotate-"
    "z|--combined]\n"
    "  Arguments can be in any order.\n";

int main(int argc, char* argv[])
{
    if (argc != 3) [[unlikely]]
    {
        std::cerr << USAGE;
        return EXIT_FAILURE;
    }

    std::optional<RenderMode> render;
    std::optional<TransformMode> transform;

    for (int i = 1; i < argc; i++)
    {
        if (const auto r = parseRenderMode(argv[i]))
        {
            render = r;
        }
        else if (const auto t = parseTransformMode(argv[i]))
        {
            transform = t;
        }
        else [[unlikely]]
        {
            std::cerr << "Unknown argument: " << argv[i] << "\n"
                      << USAGE;
            return EXIT_FAILURE;
        }
    }

    if (!render) [[unlikely]]
    {
        std::cerr << "Missing render mode (--lines or --triangles)\n"
                  << USAGE;
        return EXIT_FAILURE;
    }

    if (!transform) [[unlikely]]
    {
        std::cerr << "Missing transform mode\n" << USAGE;
        return EXIT_FAILURE;
    }

    const std::expected<Model, ModelParsingError> maybe_model =
        model::parse_model("./models/african_head.obj");

    if (!maybe_model.has_value()) [[unlikely]]
    {
        std::cerr << "Error: "
                  << stringify_error_variant(maybe_model.error())
                  << "\n";
        return EXIT_FAILURE;
    }

    const Model& model = maybe_model.value();

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 255);
    std::vector<cv::Vec3b> face_colors(model.faces.size());

    for (auto& c : face_colors)
    {
        c = cv::Vec3b(dist(rng), dist(rng), dist(rng));
    }

    const int width = 800, height = 800;
    cv::namedWindow("3D Rotation", cv::WINDOW_AUTOSIZE);

    int frame = 0;
    while (true)
    {
        const float t = frame * 0.02f;

        cv::Mat
            image(height, width, CV_8UC3, cv::Scalar(50, 100, 20));

        drawFaces(
            model,
            image,
            buildTransform(*transform, t),
            face_colors,
            *render
        );

        cv::imshow("3D Rotation", image);

        if (cv::waitKey(20) >= 0) [[unlikely]]
        {
            break;
        }

        frame++;
    }

    return EXIT_SUCCESS;
}
