#include "Model.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace model
{

std::string FileNotFound::stringify() const noexcept
{
    return std::format(
        "Unable to load model from a non-existent file: {}",
        this->file_path
    );
}

std::expected<Model, ModelParsingError> parse_model(
    const char* filename
)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);

    if (in.fail()) [[unlikely]]
    {
        return std::unexpected{
            ModelParsingError{FileNotFound{.file_path = filename}}
        };
    }

    std::string line{};
    Model model{};

    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;

        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            cv::Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            model.verts.push_back(v);
        }
        else if (!line.compare(0, 2, "f "))
        {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash)
            {
                idx--; // in wavefront obj all indices start at 1, not
                       // zero
                f.push_back(idx);
            }
            model.faces.push_back(std::move(f));
        }
    }

    return model;
}

} // namespace model
