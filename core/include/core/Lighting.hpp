#ifndef LIGHTING_HPP_
#define LIGHTING_HPP_

#include <cmath>
#include <opencv2/opencv.hpp>

struct PhongParams
{
    float Ia = 1.f;
    float I  = 1.f;
    float Ka = 0.15f;
    float Kd = 0.85f;
    float Ks = 0.5f;
    float p  = 32.f;
};

cv::Vec3f faceNormal(const cv::Vec3f& v0, const cv::Vec3f& v1, const cv::Vec3f& v2);

float diffuse(const cv::Vec3f& N, const cv::Vec3f& L, float I, float Kd);

float specular(
    const cv::Vec3f& N, const cv::Vec3f& L,
    const cv::Vec3f& V, float I, float Ks, float p
);

cv::Vec3b applyLighting(
    const cv::Vec3b& color,
    const cv::Vec3f& N,
    const cv::Vec3f& L,
    const cv::Vec3f& V,
    const PhongParams& ph
);

#endif // LIGHTING_HPP_
