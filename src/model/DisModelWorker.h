#pragma once
#include <opencv2/dnn.hpp>

#include <filesystem>

#include "ModelWorker.h"

namespace backgroundRemover {

class DisModelWorker
    : public ModelWorker
{
public:
    DisModelWorker();

    cv::Size inputImageSize() override;
};

}
