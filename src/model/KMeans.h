#pragma once

#include <string>

#include "BgRemover.h"

namespace backgroundRemover{

class Kmeans : public BgRemover
{
public:
//    Ohlander();

    void start(BgRemoverSettingsPtr) override;

private:
    cv::Mat performKmeans(cv::Mat src);

private:
    BgRemoverSettingsPtr settings_;
};

}
