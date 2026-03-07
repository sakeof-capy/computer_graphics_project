#ifndef PROJECTION_HPP_
#define PROJECTION_HPP_

#include <cmath>
#include <opencv2/opencv.hpp>

struct PerspectiveParams
{
    float zc  = 5.f;
    float zsp = 0.f;
};

struct AxonometricParams
{
    float alpha = static_cast<float>(M_PI) / 4.f;
    float beta  = static_cast<float>(M_PI) / 4.f;
};

enum class ProjectionMode { Orthographic, Perspective, Axonometric };

cv::Point projectOrthographic(const cv::Vec3f& v, int w, int h);

cv::Point projectPerspective(
    const cv::Vec3f& v, int w, int h,
    const PerspectiveParams& p
);

cv::Point projectAxonometric(
    const cv::Vec3f& v, int w, int h,
    const AxonometricParams& p
);

cv::Point project(
    const cv::Vec3f& v, int w, int h,
    ProjectionMode proj,
    const PerspectiveParams& pp,
    const AxonometricParams& ap
);

#endif // PROJECTION_HPP_
