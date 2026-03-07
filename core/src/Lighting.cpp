#include "Lighting.hpp"

cv::Vec3f faceNormal(const cv::Vec3f& v0, const cv::Vec3f& v1, const cv::Vec3f& v2)
{
    const cv::Vec3f e0 = v1 - v0;
    const cv::Vec3f e1 = v2 - v0;

    const cv::Vec3f n(
        e0[1]*e1[2] - e0[2]*e1[1],
        e0[2]*e1[0] - e0[0]*e1[2],
        e0[0]*e1[1] - e0[1]*e1[0]
    );

    const float len = std::sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    if (len < 1e-6f) return cv::Vec3f(0.f, 0.f, 1.f);
    return n * (1.f / len);
}

float diffuse(const cv::Vec3f& N, const cv::Vec3f& L, float I, float Kd)
{
    const float cos_theta = N[0]*L[0] + N[1]*L[1] + N[2]*L[2];
    return I * Kd * std::max(0.f, cos_theta);
}

float specular(const cv::Vec3f& N, const cv::Vec3f& L,
                      const cv::Vec3f& V, float I, float Ks, float p)
{
    const float N_dot_L = N[0]*L[0] + N[1]*L[1] + N[2]*L[2];
    if (N_dot_L <= 0.f) return 0.f;

    const float N_dot_N = N[0]*N[0] + N[1]*N[1] + N[2]*N[2];
    const float scale   = 2.f * N_dot_L / N_dot_N;
    const cv::Vec3f R(
        N[0]*scale - L[0],
        N[1]*scale - L[1],
        N[2]*scale - L[2]
    );

    const float R_len = std::sqrt(R[0]*R[0] + R[1]*R[1] + R[2]*R[2]);
    if (R_len < 1e-6f) return 0.f;

    const float cos_alpha = (R[0]*V[0] + R[1]*V[1] + R[2]*V[2]) / R_len;
    if (cos_alpha <= 0.f) return 0.f;

    return I * Ks * std::pow(cos_alpha, p);
}

cv::Vec3b applyLighting(const cv::Vec3b& color,
                               const cv::Vec3f& N,
                               const cv::Vec3f& L,
                               const cv::Vec3f& V,
                               const PhongParams& ph)
{
    const float I_ambient  = ph.Ia * ph.Ka;
    const float I_diffuse  = diffuse(N, L, ph.I, ph.Kd);
    const float I_specular = specular(N, L, V, ph.I, ph.Ks, ph.p);

    const float diffuse_factor  = I_ambient + I_diffuse;
    const float specular_factor = I_specular;

    return cv::Vec3b(
        static_cast<uchar>(std::min(255.f, color[0] * diffuse_factor + 255.f * specular_factor)),
        static_cast<uchar>(std::min(255.f, color[1] * diffuse_factor + 255.f * specular_factor)),
        static_cast<uchar>(std::min(255.f, color[2] * diffuse_factor + 255.f * specular_factor))
    );
}
