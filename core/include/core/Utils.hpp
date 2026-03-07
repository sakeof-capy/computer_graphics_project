#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <opencv2/opencv.hpp>

void line(
    int x0,
    int y0,
    int x1,
    int y1,
    cv::Mat& image,
    cv::Vec3b color
);

void triangle(
    cv::Point p0,
    cv::Point p1,
    cv::Point p2,
    cv::Mat& image,
    cv::Vec3b color
);

void triangle_zbuf(
    cv::Point p0,
    cv::Point p1,
    cv::Point p2,
    float z0,
    float z1,
    float z2,
    cv::Mat& image,
    std::vector<float>& zbuf,
    cv::Vec3b color
);

#endif // !UTILS_HPP_
