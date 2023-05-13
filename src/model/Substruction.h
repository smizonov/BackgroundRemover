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
public:
    void start(BgRemoverSettingsPtr) override;

private:
    static cv::Mat substruct(
        cv::Mat const & background,
        cv::Mat const & img);
    void detectNextSeg(std::filesystem::path src, std::filesystem::path dst);
//    static void myKmeans(std::string src, std::string dst);
};

}
