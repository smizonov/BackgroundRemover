#pragma once

#include <string>

#include "BgRemover.h"

namespace backgroundRemover{

class KMeans : public BgRemover
{
public:
    void start(BgRemoverSettingsPtr, BgRemoverHandlers) override;

private:
    cv::Mat performKmeans(cv::Mat src);

};

}
