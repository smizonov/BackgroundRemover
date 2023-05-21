#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "MlRemover.h"
#include <ModelWorker.h>
#include <iostream>
#include <fstream>
#include <iomanip>

namespace backgroundRemover{

cv::Mat processPredictions(cv::Mat& ort_outs, const cv::Mat& inputImage)
{
    cv::Mat pred(ort_outs.size[2], ort_outs.size[3], CV_32F, ort_outs.ptr<float>());

    double minValue = 0.0;
    double maxValue = 255.0;

    cv::Mat uint8Mat;
    cv::normalize(pred, uint8Mat, minValue, maxValue, cv::NORM_MINMAX, CV_8UC1);
    cv::resize(uint8Mat, uint8Mat, inputImage.size(), cv::INTER_LANCZOS4);
    return uint8Mat;
}

cv::Mat MlRemover::getObjectMask(const cv::Mat &srcImage)
{
    auto & net = ModelWorker::getInstance();
    cv::Mat blob;
    cv::dnn::blobFromImage(srcImage, blob, 1.0 / 255, cv::Size(320, 320), cv::Scalar(124, 116, 104));

    net().setInput(blob);
    cv::Mat output;

    try {
        output = net().forward(net.outputLayerName());
        // Process the output
    } catch (const cv::Exception& e) {
        std::cout << "Exception = " << e.what();
    }

    return processPredictions(output, srcImage);
}

}
