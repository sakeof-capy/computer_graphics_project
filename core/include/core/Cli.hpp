#ifndef CLI_HPP_
#define CLI_HPP_

#include "Lighting.hpp"
#include "Projection.hpp"
#include "Render.hpp"
#include "Transform.hpp"
#include <optional>
#include <string_view>

std::optional<RenderMode> parseRenderMode(std::string_view arg);

std::optional<TransformMode> parseTransformMode(std::string_view arg);

std::optional<ProjectionMode> parseProjectionMode(std::string_view arg);

std::optional<float> parseFloatParam(std::string_view arg, std::string_view key);

static constexpr std::string_view USAGE =
    "Usage: <program> [--lines|--triangles]"
    " [--default|--translate|--scale|--rotate-x|--rotate-y|--rotate-z|--combined]"
    " [--orthographic|--perspective|--axonometric]\n"
    "  Perspective params (optional): --zc=<float>    (camera Z, default 5.0)\n"
    "                                 --zsp=<float>   (screen plane Z, default 0.0)\n"
    "  Axonometric params (optional): --alpha=<float> (roll angle degrees, default 45.0)\n"
    "                                 --beta=<float>  (pitch angle degrees, default 45.0)\n"
    "  Light direction  (optional):   --lx=<float>    (default 0.0)\n"
    "                                 --ly=<float>    (default 0.0)\n"
    "                                 --lz=<float>    (default 1.0, toward camera)\n"
    "  Phong (I_відб = Ia*Ka + I*(Kd*cosθ + Ks*cos^p α)):\n"
    "                                 --Ia=<float>    (default 1.0)\n"
    "                                 --I=<float>     (default 1.0)\n"
    "                                 --Ka=<float>    (default 0.15)\n"
    "                                 --Kd=<float>    (default 0.85)\n"
    "                                 --Ks=<float>    (default 0.5)\n"
    "                                 --p=<float>     (default 32)\n"
    "  Arguments can be in any order.\n";

#endif // CLI_HPP_
