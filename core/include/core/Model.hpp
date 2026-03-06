#ifndef __MODEL_H__
#define __MODEL_H__

#include <expected>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <variant>
#include <vector>

namespace model
{

struct FileNotFound
{
public:
    std::string file_path;

public:
    [[nodiscard]]
    std::string stringify() const noexcept;
};

using ModelParsingError = std::variant<FileNotFound>;

struct Model
{
    std::vector<cv::Vec3f> verts;
    std::vector<std::vector<int>> faces;
};

[[nodiscard]]
std::expected<Model, ModelParsingError> parse_model(
    const char* filename
);

} // namespace model

#endif //__MODEL_H__
