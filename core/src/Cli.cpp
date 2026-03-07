#include "Cli.hpp"

std::optional<RenderMode> parseRenderMode(std::string_view arg)
{
    if (arg == "--lines")     return RenderMode::Lines;
    if (arg == "--triangles") return RenderMode::Triangles;
    return std::nullopt;
}

std::optional<TransformMode> parseTransformMode(std::string_view arg)
{
    if (arg == "--default")   return TransformMode::Default;
    if (arg == "--translate") return TransformMode::Translate;
    if (arg == "--scale")     return TransformMode::Scale;
    if (arg == "--rotate-x")  return TransformMode::RotateX;
    if (arg == "--rotate-y")  return TransformMode::RotateY;
    if (arg == "--rotate-z")  return TransformMode::RotateZ;
    if (arg == "--combined")  return TransformMode::Combined;
    return std::nullopt;
}

std::optional<ProjectionMode> parseProjectionMode(std::string_view arg)
{
    if (arg == "--orthographic") return ProjectionMode::Orthographic;
    if (arg == "--perspective")  return ProjectionMode::Perspective;
    if (arg == "--axonometric")  return ProjectionMode::Axonometric;
    return std::nullopt;
}

std::optional<float> parseFloatParam(std::string_view arg, std::string_view key)
{
    if (arg.substr(0, key.size()) != key) return std::nullopt;
    try   { return std::stof(std::string(arg.substr(key.size()))); }
    catch (...) { return std::nullopt; }
}
