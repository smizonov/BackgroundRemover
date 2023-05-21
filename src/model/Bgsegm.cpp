#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
//#include <opencv2/bgsegm.hpp>
#include <string>


#include "Bgsegm.h"

using namespace std;
using namespace cv;

namespace backgroundRemover{


static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
    int niters = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat temp1, temp2;
    Mat kernel(5,5, CV_8U);
    erode(mask, temp1, kernel, Point(-1,-1));
    dilate(temp1, temp2, kernel, Point(-1,-1));
    erode(temp1, temp2, kernel, Point(-1,-1));
//    dst = temp2;
//    return;
    findContours( temp2, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    dst = Mat::zeros(img.size(), CV_8UC3);
    if( contours.size() == 0 )
        return;
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0, largestComp = 0;
    double maxArea = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > maxArea )
        {
            maxArea = area;
            largestComp = idx;
        }
    }
    Scalar color( 0, 0, 255 );
    drawContours( dst, contours, largestComp, color, FILLED, LINE_8, hierarchy );
}

void Bgsegm::start(
    std::filesystem::path srcDir,
    std::filesystem::path dstFolderPath)
{
    auto bgsubtractor=cv::createBackgroundSubtractorKNN(3, 10, true);
//    bgsubtractor->setVarThreshold(10);

    Mat bgmask;

    for (auto path : std::filesystem::directory_iterator(srcDir))
    {
        cv::Mat tmpFrame = cv::imread(path.path().string(), cv::IMREAD_GRAYSCALE);
        auto dstFrame = detectNextSeg(tmpFrame, bgmask, bgsubtractor);
        auto dstPath = dstFolderPath / path.path().filename();
        imwrite(dstPath.string(), dstFrame);
    }
}

//void Bgsegm::start(std::filesystem::path src, std::filesystem::path dst)
//{
//    bool update_bg_model = true;

//    Mat bgmask, out_frame;
//    cv::Mat tmp_frame = cv::imread(src.string(), cv::IMREAD_GRAYSCALE);

//    if(tmp_frame.empty())
//    {
//        printf("can not read data from the video source\n");
//        return ;
//    }
////    namedWindow("video", 1);
////    namedWindow("segmented", 1);
//    auto bgsubtractor=createBackgroundSubtractorMOG2();
//    bgsubtractor->setVarThreshold(10);

//    for (int i = 0; i < 100; ++i)
//    {
//    bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
//    refineSegments(tmp_frame, bgmask, out_frame);
////    imshow("video", tmp_frame);
//    imshow("segmented", out_frame);
//    cv::waitKey(0);
////        char keycode = (char)waitKey(30);
////        if( keycode == 27 )
////            break;
////        if( keycode == ' ' )
////        {
////            update_bg_model = !update_bg_model;
////            printf("Learn background is in state = %d\n",update_bg_model);
//    }

//}

cv::Mat Bgsegm::detectNextSeg(
    const cv::Mat &src,
    cv::Mat & bgmask,
    std::shared_ptr<cv::BackgroundSubtractor> subsctructor)
{
    cv::Mat dst;
    subsctructor->apply(src, bgmask, -1);
//    return bgmask;
    refineSegments(src, bgmask, dst);
    return dst;
}

}
