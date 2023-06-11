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
    for (auto const & path : std::filesystem::directory_iterator(settings->srcFolderPath()))
    {
        if (stopped_)
            break;
        auto dstPath = settings->dstFolderPath() / path.path().filename();
        Mat srcImage = imread(path.path().string(), IMREAD_COLOR);
        auto mask{ getObjectMask(srcImage) };

//        imwrite(dstPath.string(), mask);
        MaskEditor::removeNoise(mask);
        postProcessingMask(mask);
        cv::Mat dstImage;
        cv::bitwise_and(srcImage, srcImage, dstImage, mask);
        imwrite(dstPath.string(), dstImage);

        handlers.onImageHandle(++countOfHandledImages);

        if (1 == countOfHandledImages)
        {
            paused_ = true;
            std::unique_lock lock(mutex_);
            handlers.previewImagePathsReceived(path, dstPath);
            cv_.wait(lock, [this]{ return !paused_; });
        }
    }
    handlers.onFinish(std::error_code());
}

void BgRemover::stop()
{
    stopped_ = true;
}

void BgRemover::resume()
{
    std::lock_guard lock(mutex_);
    paused_ = false;
}

}
