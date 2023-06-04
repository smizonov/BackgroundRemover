#include "DisModelWorker.h"

#include <iostream>


namespace backgroundRemover {

namespace {

auto modelPath{ std::filesystem::path("model") / "onnx.onnx" };

}

DisModelWorker::DisModelWorker()
    : ModelWorker(std::filesystem::current_path() / modelPath)
{}


cv::Size DisModelWorker::inputImageSize()
{
    return cv::Size(1024, 1024);
}


}
