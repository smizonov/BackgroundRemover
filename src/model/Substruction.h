#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>

namespace backgroundRemover{

class Substruction
{
public:
//    Ohlander();

    static void start(std::filesystem::path srcDir, std::filesystem::path dstFolderPath);

private:
    static cv::Mat substruct(
        cv::Mat const & background,
        cv::Mat const & img);
    void detectNextSeg(std::filesystem::path src, std::filesystem::path dst);
//    static void myKmeans(std::string src, std::string dst);
};

}
