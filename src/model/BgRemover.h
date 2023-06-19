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
using NuberOfProcessedImages = std::function<void(int totalCount, int processedCount)>;
using PreviewImagePathsReceived = std::function<void(std::filesystem::path src, std::filesystem::path dst)>;

struct BgRemoverHandlers
{
    NuberOfProcessedImages onImageHandle;
    OnFinish onFinish;
    PreviewImagePathsReceived previewImagePathsReceived;
};

class BgRemover
{
public:
    void start(BgRemoverSettingsPtr, BgRemoverHandlers);
    void stop();
    void resume();

protected:
    virtual void onStartPreparation(BgRemoverSettingsPtr) = 0;
    virtual cv::Mat getObjectMask(cv::Mat const & image) = 0;
    virtual void postProcessingMask(cv::Mat & image) = 0;

private:
    void saveTime();
protected:
    bool stopped_{ false };
    bool paused_{ false };

private:
    std::mutex mutex_;
    std::condition_variable cv_;
};

}
