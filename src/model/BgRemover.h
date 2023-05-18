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

struct BgRemoverHandlers
{
    NuberOfProcessedImages onImageHandle;
    OnFinish onFinish;
};

class BgRemover
{
public:
    virtual void start(BgRemoverSettingsPtr, BgRemoverHandlers) = 0;
    void stop();

protected:
    bool stopped_{ false };
};

}
