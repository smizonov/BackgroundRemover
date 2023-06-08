#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
//#include <opencv2/bgsegm.hpp>
#include <string>


#include "BgRemover.h"
#include "MaskEditor.h"

using namespace std;
using namespace cv;

namespace backgroundRemover{

void BgRemover::start(BgRemoverSettingsPtr settings, BgRemoverHandlers handlers)
{
    onStartPreparation(settings);
    auto countOfHandledImages{ 0 };
    for (auto path : std::filesystem::directory_iterator(settings->srcFolderPath()))
    {
        if (stopped_)
            break;
        auto dstPath = settings->dstFolderPath() / path.path().filename();
        Mat srcImage = imread(path.path().string(), IMREAD_COLOR);
        auto mask{ getObjectMask(srcImage) };

        MaskEditor::removeNoise(mask);
        postProcessingMask(mask);
        cv::Mat dstImage;
        cv::bitwise_and(srcImage, srcImage, dstImage, mask);
        imwrite(dstPath.string(), dstImage);
        if (0 == countOfHandledImages)
            handlers.firstResult(path, dstPath);

        handlers.onImageHandle(++countOfHandledImages);
    }
    handlers.onFinish(std::error_code());
}

void BgRemover::stop()
{
    stopped_ = true;
}

}
