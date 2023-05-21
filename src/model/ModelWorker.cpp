#include "ModelWorker.h"

#include <iostream>


namespace backgroundRemover {

ModelWorker::ModelWorker(std::filesystem::path path)
    : net_(cv::dnn::readNetFromONNX(path.string()))
    , layerNames_(net_.getUnconnectedOutLayersNames())
{}

ModelWorker & ModelWorker::getInstance()
{
    std::filesystem::path currentPath = std::filesystem::current_path()/ "model" / "u2net.onnx";
    static ModelWorker model(currentPath);
    return model;
}

cv::dnn::Net &ModelWorker::operator()()
{
    return net_;
}

const std::string &ModelWorker::outputLayerName()
{
    return layerNames_[0];
}

}
