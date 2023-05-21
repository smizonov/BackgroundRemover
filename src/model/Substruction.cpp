#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
#include <iostream>
//#include <opencv2/bgsegm.hpp>
#include <string>

#include "SubstructionSettings.h"

#include "Substruction.h"
#include "MaskEditor.h"

using namespace std;
using namespace cv;

namespace backgroundRemover{

void Substruction::onStartPreparation(BgRemoverSettingsPtr settings)
{
    auto subscriptionSettings = std::dynamic_pointer_cast<SubstructionSettings>(settings);
    backgroudImage_ = cv::imread(subscriptionSettings->bgImagePath().string(), cv::IMREAD_GRAYSCALE);
}

Mat Substruction::getObjectMask(const cv::Mat &image)
{
    cv::Mat grayFrame;
    cv::cvtColor(image, grayFrame, COLOR_BGR2GRAY);
    cv::Mat dst;
    cv::subtract(backgroudImage_, grayFrame, dst);
    return dst;
}

void Substruction::postProcessingMask(cv::Mat &mask)
{
    MaskEditor::removeNoise(mask);
}

}
