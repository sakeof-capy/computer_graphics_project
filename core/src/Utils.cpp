#include "Utils.hpp"


// Bresenham-style line drawing
void line(
    int x0,
    int y0,
    int x1,
    int y1,
    cv::Mat& image,
    cv::Vec3b color
)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++)
    {
        float t = (x - x0) / static_cast<float>(x1 - x0 + 1e-5);
        int y = static_cast<int>(y0 * (1. - t) + y1 * t);
        if (x >= 0 && y >= 0 && x < image.cols && y < image.rows)
        {
            if (steep)
                image.at<cv::Vec3b>(x, y) = color;
            else
                image.at<cv::Vec3b>(y, x) = color;
        }
    }
}

void triangle(
    cv::Point p0,
    cv::Point p1,
    cv::Point p2,
    cv::Mat& image,
    cv::Vec3b color
)
{
    int minX = std::max(0, std::min({p0.x, p1.x, p2.x}));
    int maxX = std::min(image.cols - 1, std::max({p0.x, p1.x, p2.x}));
    int minY = std::max(0, std::min({p0.y, p1.y, p2.y}));
    int maxY = std::min(image.rows - 1, std::max({p0.y, p1.y, p2.y}));

    const int dx01 = p0.x - p1.x, dy01 = p0.y - p1.y;
    const int dx12 = p1.x - p2.x, dy12 = p1.y - p2.y;
    const int dx20 = p2.x - p0.x, dy20 = p2.y - p0.y;

    const int area2 = dx20 * dy01 - dy20 * dx01;
    if (area2 == 0)
        return;

    int w0_row = (minX - p1.x) * dy12 - (minY - p1.y) * dx12;
    int w1_row = (minX - p2.x) * dy20 - (minY - p2.y) * dx20;
    int w2_row = (minX - p0.x) * dy01 - (minY - p0.y) * dx01;

    for (int y = minY; y <= maxY; y++)
    {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        uchar* row = image.ptr(y);

        for (int x = minX; x <= maxX; x++)
        {
            if ((w0 | w1 | w2) >= 0 || (w0 & w1 & w2) < 0)
            {
                uchar* px = row + x * 3;
                px[0] = color[0];
                px[1] = color[1];
                px[2] = color[2];
            }

            w0 += dy12;
            w1 += dy20;
            w2 += dy01;
        }

        w0_row -= dx12;
        w1_row -= dx20;
        w2_row -= dx01;
    }
}

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
)
{
    int minX = std::max(0,            std::min({p0.x, p1.x, p2.x}));
    int maxX = std::min(image.cols-1, std::max({p0.x, p1.x, p2.x}));
    int minY = std::max(0,            std::min({p0.y, p1.y, p2.y}));
    int maxY = std::min(image.rows-1, std::max({p0.y, p1.y, p2.y}));

    const int dx01 = p0.x-p1.x, dy01 = p0.y-p1.y;
    const int dx12 = p1.x-p2.x, dy12 = p1.y-p2.y;
    const int dx20 = p2.x-p0.x, dy20 = p2.y-p0.y;

    const int area2 = dx20*dy01 - dy20*dx01;
    if (area2 == 0) return;

    const float inv_area2 = 1.f / static_cast<float>(area2);

    int w0_row = (minX-p1.x)*dy12 - (minY-p1.y)*dx12;
    int w1_row = (minX-p2.x)*dy20 - (minY-p2.y)*dx20;
    int w2_row = (minX-p0.x)*dy01 - (minY-p0.y)*dx01;

    for (int y = minY; y <= maxY; y++)
    {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        uchar* row = image.ptr(y);

        for (int x = minX; x <= maxX; x++)
        {
            if ((w0|w1|w2) >= 0 || (w0&w1&w2) < 0)
            {
                const float z = (w0*z0 + w1*z1 + w2*z2) * inv_area2;

                float& zval = zbuf[y * image.cols + x];
                if (z > zval)
                {
                    zval = z;
                    uchar* px = row + x*3;
                    px[0] = color[0];
                    px[1] = color[1];
                    px[2] = color[2];
                }
            }

            w0 += dy12;
            w1 += dy20;
            w2 += dy01;
        }

        w0_row -= dx12;
        w1_row -= dx20;
        w2_row -= dx01;
    }
}
