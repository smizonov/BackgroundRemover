#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>

namespace backgroundRemover{

class Bgsegm
{
public:
//    Ohlander();

    static void start(std::filesystem::path srcDir, std::filesystem::path dstFolderPath);

private:
    static cv::Mat detectNextSeg(
        cv::Mat const & src,
        cv::Mat & bgmask,
        std::shared_ptr<cv::BackgroundSubtractor> subsctructor);
    void detectNextSeg(std::filesystem::path src, std::filesystem::path dst);
//    static void myKmeans(std::string src, std::string dst);
};

}
