#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "BgRemover.h"

namespace backgroundRemover{

class Ohlander : public BgRemover
{
public:
    void start(BgRemoverSettingsPtr, BgRemoverHandlers) override;

private:
    void startImpl(std::filesystem::path srcPath, std::filesystem::path dstPath);
    void prepareImage();
    void OhlanderFunc(
        std::vector<cv::Mat> & result_masks,
        std::vector<cv::Mat> & mask_stack,
        std::vector<cv::Mat> & result_vector,
        std::vector<cv::Mat> & channels,
        std::vector<cv::Mat> & channels_hist,
        int & segmentsCount);
};

}
