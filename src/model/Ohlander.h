#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "BgRemover.h"

namespace backgroundRemover{

class Ohlander : public BgRemover
{
protected:
    void onStartPreparation(BgRemoverSettingsPtr) override {}
    cv::Mat getObjectMask(cv::Mat const & image) override;
    void postProcessingMask(cv::Mat & image) override;

private:
    void OhlanderFunc(
        std::vector<cv::Mat> & result_masks,
        std::vector<cv::Mat> & mask_stack,
        std::vector<cv::Mat> & result_vector,
        std::vector<cv::Mat> & channels,
        std::vector<cv::Mat> & channels_hist,
        int & segmentsCount);
};

}
