#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "MlRemover.h"
#include <DisModelWorker.h>
#include <U2ModelWorker.h>
#include <iostream>
#include <fstream>
#include <iomanip>

namespace backgroundRemover{

cv::Mat convertToMask(cv::Mat& ort_outs, const cv::Mat& inputImage)
{
    cv::Mat pred(ort_outs.size[2], ort_outs.size[3], CV_32F, ort_outs.ptr<float>());

    double minValue = 0.0;
    double maxValue = 255.0;

    cv::Mat uint8Mat;
    cv::normalize(pred, uint8Mat, minValue, maxValue, cv::NORM_MINMAX, CV_8UC1);
    cv::resize(uint8Mat, uint8Mat, inputImage.size(), cv::INTER_NEAREST);
    return uint8Mat;
}

MlRemover::MlRemover(ModelWorkerUniquePtr && model)
    : model_(std::move(model))
{
    if (!model_)
        throw std::runtime_error("MlRemover::MlRemover: model is nullptr");
}

cv::Mat MlRemover::getObjectMask(const cv::Mat &srcImage)
{
    cv::Mat blob;
    cv::dnn::blobFromImage(srcImage, blob, 1.0 / 255, model_->inputImageSize(), cv::Scalar(124, 116, 104));

    model_->net().setInput(blob);
    cv::Mat output;

    try
    {
        output = model_->net().forward(model_->outputLayerName());
        // Process the output
    } catch (const cv::Exception& e) {
        std::cout << "Exception = " << e.what();
    }

    return convertToMask(output, srcImage);
}

}
