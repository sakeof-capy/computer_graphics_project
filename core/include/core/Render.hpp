#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "Lighting.hpp"
#include "Model.hpp"
#include "Projection.hpp"
#include "Transform.hpp"
#include "Utils.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

enum class RenderMode { Lines, Triangles };

void drawFaces(
    const model::Model& model,
    cv::Mat& image,
    const Mat4& transform,
    const std::vector<cv::Vec3b>& face_colors,
    RenderMode render,
    ProjectionMode proj,
    const PerspectiveParams& pp,
    const AxonometricParams& ap,
    const cv::Vec3f& light_dir,
    const cv::Vec3f& view_dir,
    const PhongParams& phong
);

#endif // RENDER_HPP_
