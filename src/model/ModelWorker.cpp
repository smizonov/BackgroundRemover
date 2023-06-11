#include "U2ModelWorker.h"

#include <opencv2/dnn.hpp>

#include <iostream>

namespace backgroundRemover {

ModelWorker::ModelWorker(std::filesystem::path path)
    : net_(cv::dnn::readNetFromONNX(path.string()))
    , layerNames_(net_.getUnconnectedOutLayersNames())
{
//    net_.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
}

cv::dnn::Net & ModelWorker::net()
{
    return net_;
}

const std::string &ModelWorker::outputLayerName()
{
    return layerNames_[0];
}

}
