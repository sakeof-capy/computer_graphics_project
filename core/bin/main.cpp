#include "Cli.hpp"
#include "Error.hpp"
#include "Model.hpp"
#include "Render.hpp"
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

using model::Model;
using model::ModelParsingError;

std::vector<cv::Vec3b> generate_random_colours(const Model& model)
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 255);
    std::vector<cv::Vec3b> face_colors(model.faces.size());
    
    for (auto& c : face_colors)
    {
        c = cv::Vec3b(dist(rng), dist(rng), dist(rng));
    }

    return face_colors;
}

int main(int argc, char* argv[])
{
    if (argc < 4) [[unlikely]]
    {
        std::cerr << USAGE;
        return EXIT_FAILURE;
    }

    std::optional<RenderMode>     render;
    std::optional<TransformMode>  transform;
    std::optional<ProjectionMode> projection;

    PerspectiveParams pp;
    AxonometricParams ap;
    PhongParams       phong;

    float lx = 0.f, ly = 0.f, lz = 1.f;
    const cv::Vec3f view_dir(0.f, 0.f, 1.f);

    for (int i = 1; i < argc; i++)
    {
        const std::string_view arg(argv[i]);

        if (const auto r = parseRenderMode(arg))
            render = r;
        else if (const auto t = parseTransformMode(arg))
            transform = t;
        else if (const auto p = parseProjectionMode(arg))
            projection = p;
        else if (const auto v = parseFloatParam(arg, "--zc="))
            pp.zc  = *v;
        else if (const auto v = parseFloatParam(arg, "--zsp="))
            pp.zsp = *v;
        else if (const auto v = parseFloatParam(arg, "--alpha="))
            ap.alpha = *v * static_cast<float>(M_PI) / 180.f;
        else if (const auto v = parseFloatParam(arg, "--beta="))
            ap.beta  = *v * static_cast<float>(M_PI) / 180.f;
        else if (const auto v = parseFloatParam(arg, "--lx=")) lx = *v;
        else if (const auto v = parseFloatParam(arg, "--ly=")) ly = *v;
        else if (const auto v = parseFloatParam(arg, "--lz=")) lz = *v;
        else if (const auto v = parseFloatParam(arg, "--Ia=")) phong.Ia = *v;
        else if (const auto v = parseFloatParam(arg, "--I="))  phong.I  = *v;
        else if (const auto v = parseFloatParam(arg, "--Ka=")) phong.Ka = *v;
        else if (const auto v = parseFloatParam(arg, "--Kd=")) phong.Kd = *v;
        else if (const auto v = parseFloatParam(arg, "--Ks=")) phong.Ks = *v;
        else if (const auto v = parseFloatParam(arg, "--p="))  phong.p  = *v;
        else [[unlikely]]
        {
            std::cerr << "Unknown argument: " << arg << "\n" << USAGE;
            return EXIT_FAILURE;
        }
    }

    if (!render) [[unlikely]]
    {
        std::cerr << "Missing render mode (--lines or --triangles)\n" << USAGE;
        return EXIT_FAILURE;
    }
    if (!transform) [[unlikely]]
    {
        std::cerr << "Missing transform mode\n" << USAGE;
        return EXIT_FAILURE;
    }
    if (!projection) [[unlikely]]
    {
        std::cerr << "Missing projection mode\n" << USAGE;
        return EXIT_FAILURE;
    }

    const float llen = std::sqrt(lx*lx + ly*ly + lz*lz);
    const cv::Vec3f light_dir = (llen < 1e-6f)
        ? cv::Vec3f(0.f, 0.f, 1.f)
        : cv::Vec3f(lx/llen, ly/llen, lz/llen);

    const std::expected<Model, ModelParsingError> maybe_model =
        model::parse_model("./models/african_head.obj");

    if (!maybe_model.has_value()) [[unlikely]]
    {
        std::cerr << "Error: "
                  << stringify_error_variant(maybe_model.error()) << "\n";
        return EXIT_FAILURE;
    }

    const Model& model = maybe_model.value();

    std::vector<cv::Vec3b> face_colors = generate_random_colours(model);    

    const int width = 800, height = 800;
    cv::namedWindow("3D Rotation", cv::WINDOW_AUTOSIZE);

    int frame = 0;

    while (true)
    {
        const float t = frame * 0.02f;

        cv::Mat image(height, width, CV_8UC3, cv::Scalar(50, 100, 20));
        drawFaces(model, image, buildTransform(*transform, t),
                  face_colors, *render, *projection, pp, ap,
                  light_dir, view_dir, phong);

        cv::imshow("3D Rotation", image);
        if (cv::waitKey(20) >= 0) [[unlikely]] break;

        frame++;
    }

    return EXIT_SUCCESS;
}
