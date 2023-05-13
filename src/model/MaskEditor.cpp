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

cv::Mat MaskEditor::removeNoise(cv::Mat srcBlackObjCV8)
{
    Mat temp1, temp2;
    Mat kernel(5,5, CV_8U);
    erode(srcBlackObjCV8, temp1, kernel, Point(-1,-1));
    erode(temp1, temp2, kernel, Point(-1,-1));
    dilate(temp2, temp1, kernel, Point(-1,-1));
    return temp1;
}

}
