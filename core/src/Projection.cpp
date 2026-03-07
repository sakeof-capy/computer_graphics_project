#include "Projection.hpp"

cv::Point projectOrthographic(const cv::Vec3f& v, int w, int h)
{
    return {
        static_cast<int>((1.f + v[0]) * w / 2.f),
        static_cast<int>((1.f - v[1]) * h / 2.f)
    };
}

cv::Point projectPerspective(const cv::Vec3f& v, int w, int h,
                                    const PerspectiveParams& p)
{
    const float denom = p.zc - v[2];
    if (std::abs(denom) < 1e-5f) return projectOrthographic(v, w, h);

    const float scale = (p.zc - p.zsp) / denom;
    return {
        static_cast<int>((1.f + v[0] * scale) * w / 2.f),
        static_cast<int>((1.f - v[1] * scale) * h / 2.f)
    };
}

cv::Point projectAxonometric(const cv::Vec3f& v, int w, int h,
                                    const AxonometricParams& p)
{
    const float ca = std::cos(p.alpha), sa = std::sin(p.alpha);
    const float cb = std::cos(p.beta),  sb = std::sin(p.beta);

    const float X = v[0]*ca - v[1]*sa;
    const float Y = v[0]*sa*cb + v[1]*ca*cb - v[2]*sb;

    return {
        static_cast<int>((1.f + X) * w / 2.f),
        static_cast<int>((1.f - Y) * h / 2.f)
    };
}

cv::Point project(const cv::Vec3f& v, int w, int h,
                         ProjectionMode proj,
                         const PerspectiveParams& pp,
                         const AxonometricParams& ap)
{
    switch (proj)
    {
        case ProjectionMode::Perspective:  return projectPerspective(v, w, h, pp);
        case ProjectionMode::Axonometric:  return projectAxonometric(v, w, h, ap);
        default:                           return projectOrthographic(v, w, h);
    }
}
