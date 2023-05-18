#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <BgRemoverSettings.h>

namespace backgroundRemover{

class MaskEditor
{
public:
    static void removeNoise(cv::Mat & srcBlackObjCV8);
};

}
