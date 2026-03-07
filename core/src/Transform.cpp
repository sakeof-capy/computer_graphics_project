#include "Transform.hpp"

// clang-format off
Mat4 translationMatrix(float tx, float ty, float tz)
{
    return Mat4(
        1, 0, 0, tx,
        0, 1, 0, ty,
        0, 0, 1, tz,
        0, 0, 0,  1
    );
}

Mat4 scaleMatrix(float sx, float sy, float sz)
{
    return Mat4(
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1
    );
}

Mat4 rotateXMatrix(float a)
{
    const float c = std::cos(a), s = std::sin(a);
    return Mat4(
        1,  0,  0, 0,
        0,  c, -s, 0,
        0,  s,  c, 0,
        0,  0,  0, 1
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
        s,  c, 0, 0,
        0,  0, 1, 0,
        0,  0, 0, 1
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
    case TransformMode::RotateX: return rotateXMatrix(t);
    case TransformMode::RotateY: return rotateYMatrix(t);
    case TransformMode::RotateZ: return rotateZMatrix(t);
    case TransformMode::Combined:
        return translationMatrix(
                   0.3f * std::sin(t * 0.5f),
                   0.2f * std::cos(t * 0.7f),
                   0.f)
             * rotateXYZMatrix(
                   0.3f * std::sin(t * 0.7f),
                   t,
                   0.2f * std::cos(t * 1.3f))
             * scaleMatrix(
                   1.f + 0.15f * std::sin(t * 2.f),
                   1.f + 0.15f * std::cos(t * 2.3f),
                   1.f);
    }
    return Mat4::eye();
}
