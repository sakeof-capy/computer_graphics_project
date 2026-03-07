#include "Render.hpp"

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
)
{
    const int w = image.cols, h = image.rows;
    const cv::Vec3b white{255, 255, 255};

    std::vector<float> zbuf = (render == RenderMode::Triangles)
        ? std::vector<float>(w * h, -std::numeric_limits<float>::infinity())
        : std::vector<float>{};

    for (size_t i = 0; i < model.faces.size(); i++)
    {
        const auto& face = model.faces[i];

        const cv::Vec3f v0 = applyTransform(transform, model.verts[face[0]]);
        const cv::Vec3f v1 = applyTransform(transform, model.verts[face[1]]);
        const cv::Vec3f v2 = applyTransform(transform, model.verts[face[2]]);

        const cv::Point p0 = project(v0, w, h, proj, pp, ap);
        const cv::Point p1 = project(v1, w, h, proj, pp, ap);
        const cv::Point p2 = project(v2, w, h, proj, pp, ap);

        if (render == RenderMode::Triangles)
        {
            const cv::Vec3f N         = faceNormal(v0, v1, v2);
            const cv::Vec3b lit_color = applyLighting(face_colors[i], N,
                                                      light_dir, view_dir, phong);
            triangle_zbuf(p0, p1, p2, v0[2], v1[2], v2[2], image, zbuf, lit_color);
        }
        else
        {
            line(p0.x, p0.y, p1.x, p1.y, image, white);
            line(p1.x, p1.y, p2.x, p2.y, image, white);
            line(p2.x, p2.y, p0.x, p0.y, image, white);
        }
    }
}
