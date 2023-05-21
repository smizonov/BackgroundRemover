#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>

#include <BgRemover.h>

namespace backgroundRemover{

class Substruction : public BgRemover
{
protected:
    void onStartPreparation(BgRemoverSettingsPtr) override;
    cv::Mat getObjectMask(cv::Mat const & image) override;
    void postProcessingMask(cv::Mat & image) override;

private:
    cv::Mat backgroudImage_;
};

}
