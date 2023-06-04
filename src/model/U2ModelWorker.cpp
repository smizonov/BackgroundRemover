#include "U2ModelWorker.h"

#include <iostream>

namespace backgroundRemover {

namespace {

auto modelPath{ std::filesystem::path("model") / "u2net.onnx" };

}

U2ModelWorker::U2ModelWorker()
    : ModelWorker(std::filesystem::current_path() / modelPath)
{}


cv::Size U2ModelWorker::inputImageSize()
{
    return cv::Size(320, 320);
}

}
