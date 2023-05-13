#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <BgRemoverSettings.h>

namespace backgroundRemover{

using BgRemoverSettingsPtr = std::shared_ptr<BgRemoverSettings>;

class BgRemover
{
public:
    virtual void start(BgRemoverSettingsPtr) = 0;

private:
    static cv::Mat detectNextSeg(
        cv::Mat const & src,
        cv::Mat & bgmask,
        std::shared_ptr<cv::BackgroundSubtractor> subsctructor);
    void detectNextSeg(std::filesystem::path src, std::filesystem::path dst);
//    static void myKmeans(std::string src, std::string dst);
};

}
