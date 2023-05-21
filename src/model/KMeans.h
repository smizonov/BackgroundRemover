#pragma once

#include <string>

#include "BgRemover.h"

namespace backgroundRemover{

class KMeans : public BgRemover
{
protected:
    void onStartPreparation(BgRemoverSettingsPtr) override {}
    cv::Mat getObjectMask(cv::Mat const & image) override;
    void postProcessingMask(cv::Mat & image) override;
private:
    cv::Mat performKmeans(cv::Mat src);

};

}
