#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
//#include <opencv2/bgsegm.hpp>
#include <string>


#include "Bgsegm.h"
#include "MaskEditor.h"

using namespace std;
using namespace cv;

namespace backgroundRemover{

cv::Mat tryRemoveSpot(cv::Mat image);

cv::Mat MaskEditor::removeNoise(cv::Mat srcWhiteObjCV8)
{
    Mat temp1, temp2;
    auto kernelSize{ std::max(1,
                             static_cast<int>(std::round(static_cast<double>(
                                                             std::max(srcWhiteObjCV8.size().height,
                                                                      srcWhiteObjCV8.size().width))
                                                         / 100))) };
    Mat kernel(kernelSize, kernelSize, CV_8U);
    cv::threshold(srcWhiteObjCV8, temp1, 0, 255, THRESH_BINARY | cv::THRESH_OTSU);
//    return temp1;
//    return srcWhiteObjCV8;
//    return tryRemoveSpot(temp1);
    dilate(temp1, temp2, kernel, Point(-1,-1));
    erode(temp2, temp1, kernel, Point(-1,-1));
    dilate(temp1, temp2, kernel, Point(-1,-1));
//    erode(temp2, temp1, kernel, Point(-1,-1));
//    return temp2;
    return temp2;
//return tryRemoveSpot(temp2);
}

cv::Mat tryRemoveSpot(cv::Mat image)
{
    // Apply threshold to create binary mask
    cv::Mat binaryMask;
//    cv::threshold(image, binaryMask, 200, 255, cv::THRESH_BINARY);
    binaryMask = image;

    // Find contours in the binary mask
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryMask, contours, cv::RETR_CCOMP , cv::CHAIN_APPROX_SIMPLE);

    // Filter out small contours based on area
    double minContourArea = 100.0; // Adjust this threshold as needed
    std::vector<std::vector<cv::Point>> filteredContours;
    for (const auto& contour : contours)
    {
        double contourArea = cv::contourArea(contour);
        if (contourArea >= minContourArea)
        {
            filteredContours.push_back(contour);
        }
    }

    // Create a blank mask
    cv::Mat blankMask = cv::Mat::zeros(image.size(), CV_8UC1);

    // Draw filtered contours on the blank mask
    cv::drawContours(blankMask, filteredContours, -1, cv::Scalar(100), cv::FILLED);

    return blankMask;
    // Invert the blank mask
    cv::Mat invertedMask;
    cv::bitwise_not(blankMask, invertedMask);

    // Apply the inverted mask to the input image
    cv::Mat result;
    cv::bitwise_and(image, invertedMask, result);

    return result;
    // Save the modified image
}

}
