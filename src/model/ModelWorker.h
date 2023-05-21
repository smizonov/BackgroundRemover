#pragma once
#include <opencv2/dnn.hpp>

#include <filesystem>

namespace backgroundRemover {

class ModelWorker
{
private:
    ModelWorker(std::filesystem::path);

public:
    static ModelWorker& getInstance();

    cv::dnn::Net & operator()();

public:
    std::string const & outputLayerName();

private:
    cv::dnn::Net net_;
    std::vector<std::string> const layerNames_;
};

}
