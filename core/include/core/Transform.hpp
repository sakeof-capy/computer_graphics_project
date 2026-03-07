#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <cmath>
#include <opencv2/opencv.hpp>

using Mat4 = cv::Matx44f;

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

Mat4 translationMatrix(float tx, float ty, float tz);

Mat4 scaleMatrix(float sx, float sy, float sz);

Mat4 rotateXMatrix(float a);

Mat4 rotateYMatrix(float a);

Mat4 rotateZMatrix(float a);

Mat4 rotateXYZMatrix(float angleX, float angleY, float angleZ);

cv::Vec3f applyTransform(const Mat4& m, const cv::Vec3f& v);

Mat4 buildTransform(TransformMode mode, float t);

#endif // TRANSFORM_HPP_
