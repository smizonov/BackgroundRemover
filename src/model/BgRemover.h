#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <BgRemoverSettings.h>

namespace backgroundRemover{

class BgRemover
{
public:
    virtual static void start(BgRemoverSettings) = 0;

private:
    static cv::Mat detectNextSeg(
        cv::Mat const & src,
        cv::Mat & bgmask,
        std::shared_ptr<cv::BackgroundSubtractor> subsctructor);
    void detectNextSeg(std::filesystem::path src, std::filesystem::path dst);
//    static void myKmeans(std::string src, std::string dst);
};

}
