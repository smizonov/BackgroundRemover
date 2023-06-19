#pragma once

#include "BgRemover.h"
#include "ModelWorker.h"

namespace backgroundRemover{

class MlRemover: public BgRemover
{
public:
    MlRemover(ModelWorkerPtr const &);
protected:
    void onStartPreparation(BgRemoverSettingsPtr) override {}
    cv::Mat getObjectMask(cv::Mat const & image) override;
    void postProcessingMask(cv::Mat & image) override {}
    void startImpl(std::filesystem::path srcPath, std::filesystem::path dstPath);

private:
    ModelWorkerPtr model_;
};

}
