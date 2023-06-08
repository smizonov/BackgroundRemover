#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <filesystem>
#include <BgRemoverSettings.h>

namespace backgroundRemover{

using BgRemoverSettingsPtr = std::shared_ptr<BgRemoverSettings>;
using OnFinish = std::function<void(std::error_code)>;
using NuberOfProcessedImages = std::function<void(int count)>;
using FirstResultImagePath = std::function<void(std::filesystem::path src, std::filesystem::path dst)>;

struct BgRemoverHandlers
{
    NuberOfProcessedImages onImageHandle;
    OnFinish onFinish;
    FirstResultImagePath firstResult;
};

class BgRemover
{
public:
    void start(BgRemoverSettingsPtr, BgRemoverHandlers);
    void stop();

protected:
    virtual void onStartPreparation(BgRemoverSettingsPtr) = 0;
    virtual cv::Mat getObjectMask(cv::Mat const & image) = 0;
    virtual void postProcessingMask(cv::Mat & image) = 0;

protected:
    bool stopped_{ false };
};

}
