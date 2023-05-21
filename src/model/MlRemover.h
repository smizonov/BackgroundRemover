#pragma once

#include "BgRemover.h"


namespace backgroundRemover{

class MlRemover: public BgRemover
{
protected:
    void onStartPreparation(BgRemoverSettingsPtr) override {}
    cv::Mat getObjectMask(cv::Mat const & image) override;
    void postProcessingMask(cv::Mat & image) override {}
    void startImpl(std::filesystem::path srcPath, std::filesystem::path dstPath);
};

}
