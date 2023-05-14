#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
//#include <opencv2/bgsegm.hpp>
#include <string>


#include "BgRemover.h"

using namespace std;
using namespace cv;

namespace backgroundRemover{

void BgRemover::stop()
{
    stopped_ = true;
}

}
