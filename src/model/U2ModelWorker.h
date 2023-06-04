#pragma once
#include <opencv2/dnn.hpp>

#include <filesystem>

#include "ModelWorker.h"

namespace backgroundRemover {

class U2ModelWorker
    : public ModelWorker
{
public:
    U2ModelWorker();

    cv::Size inputImageSize() override;
};

}
