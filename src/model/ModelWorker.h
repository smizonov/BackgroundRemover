#pragma once
#include <opencv2/dnn.hpp>

#include <filesystem>

namespace backgroundRemover {

class ModelWorker
{
protected:
    ModelWorker(std::filesystem::path);

public:
    virtual cv::Size inputImageSize() = 0;
    cv::dnn::Net & net();

public:
    std::string const & outputLayerName();

private:
    cv::dnn::Net net_;
    std::vector<std::string> const layerNames_;
};

using ModelWorkerPtr = std::shared_ptr<ModelWorker>;
}
