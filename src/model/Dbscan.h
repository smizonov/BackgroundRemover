#pragma once

#include <string>

#include "BgRemover.h"

namespace backgroundRemover{

class Kmeans : public BgRemover
{
public:
//    Ohlander();

    void start(BgRemoverSettings) override;

private:
    cv::Mat performKmeans(cv::Mat src);

private:
    BgRemoverSettings settings_;
};

}
