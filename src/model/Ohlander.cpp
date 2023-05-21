#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

#include "Ohlander.h"
#include "MaskEditor.h"

namespace backgroundRemover{

using namespace std;
using namespace cv;

const int hist_size = 256;
const float ranges[] = {0, 256};
const float* hist_ranges = {ranges};
int constexpr kMaxSegmentsCount{ 2 };

const int t = 2;

cv::Mat extractCenterSquare(const cv::Mat& image, int n)
{
    int centerX = image.cols / 2;
    int centerY = image.rows / 2;

    int startX = centerX - n / 2;
    int startY = centerY - n / 2;

    cv::Rect roi(startX, startY, n, n);
    cv::Mat centerSquare = image(roi).clone();

    return centerSquare;
}

void generate_mask(Mat, int, int, std::vector<cv::Mat> & masks_stack, std::vector<cv::Mat> & channels);
Mat paintResultImage(Mat const & src);
void calculate_midpoint(Mat hist, int&, int&);
Mat convertBlackToWhite(Mat const & src);

Mat Ohlander::getObjectMask(const cv::Mat &raw_image)
{
    vector<Mat> result_masks;
    vector<Mat> mask_stack;
    vector<Mat> result_vector;
    vector<Mat> channels(3);
    vector<Mat> channels_hist(3);

    if(raw_image.empty()) {
        cerr << "Image format must be valid." << endl;
        exit(EXIT_FAILURE);
    }

    GaussianBlur(raw_image, raw_image, Size(5,5), 8);

    split(raw_image, channels);

    Mat dest_image(raw_image.size(), CV_8UC1, Scalar(0));

    Mat initial_mask(raw_image.size(), CV_8UC1, Scalar(1));

    mask_stack.push_back(initial_mask);
    auto segmentsCount{ 0 };

    OhlanderFunc(
        result_masks,
        mask_stack,
        result_vector,
        channels,
        channels_hist,
        segmentsCount);

    std::map<int, int> occupiedPixelsToResultVectorIndex;
    auto index{ 0u };

    //    std::vector<std::pair<int, int>> occupiedPixelsToResultVectorIndex;
    for (auto const & region : result_vector)
    {
        auto center = extractCenterSquare(region, 20);
        occupiedPixelsToResultVectorIndex[index] = std::count_if(
            center.begin<uchar>(),
            center.end<uchar>(),
            [&index, &occupiedPixelsToResultVectorIndex]
            (auto const & point)
            {
                return static_cast<uchar>(point) != 0;
            });
        ++index;
    }

    int dstIndex = 0;
    int maxValue = std::numeric_limits<int>::min();

    for (const auto& pair : occupiedPixelsToResultVectorIndex)
    {
        if (pair.second > maxValue)
        {
            maxValue = pair.second;
            dstIndex = pair.first;
        }
    }

    if (occupiedPixelsToResultVectorIndex.empty())
        throw std::runtime_error("occupiedPixelsToResultVectorIndex empty!");


    //    RNG rng(12345);
    //    for(int i=0; i<result_vector.size(); i++) {
    //        Vec3b color = Vec3b(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
    //        for(int x=0; x<raw_image.rows; x++) {
    //            for(int y=0; y<raw_image.cols; y++) {
    //                if(result_vector.at(i).at<uchar>(x,y) != 0) {
    //                    dest_image.at<Vec3b>(x,y) = color;
    //                }
    //            }
    //        }
    //    }

    for(int x=0; x<raw_image.rows; x++) {
        for(int y=0; y<raw_image.cols; y++) {
            if(result_vector.at(dstIndex).at<uchar>(x,y) != 0)
            {
                dest_image.at<uchar>(x,y) = 255;
            }
            else
            {
                dest_image.at<uchar>(x,y) = 0;
            }
        }
    }

    return dest_image;
}

Mat paintResultImage(Mat const & src)
{
    auto dst{ Mat() };
    auto dstGray{ Mat() };
    resize(src, dst, src.size(), 0, 0, INTER_CUBIC);
    auto centerPixel = src.at<Vec3b>(src.size().width/2, src.size().height/2);
    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            if (centerPixel == src.at<Vec3b>(i, j))
                dst.at<Vec3b>(i, j) = {255, 255, 255};
            else
                dst.at<Vec3b>(i, j) = {0, 0, 0};
        }
    }
    cvtColor(dst, dstGray, COLOR_BGR2GRAY);
    return dstGray;
}

Mat convertBlackToWhite(Mat const & src)
{
    auto dst{ Mat() };
    resize(src, dst, src.size(), 0, 0, INTER_CUBIC);
    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            if (Vec3b{0, 0, 0} == src.at<Vec3b>(i, j))
                dst.at<Vec3b>(i, j) = {255, 255, 255};
        }
    }
    return dst;

}

void Ohlander::OhlanderFunc(
    vector<Mat> & result_masks,
    vector<Mat> & mask_stack,
    vector<Mat> & result_vector,
    std::vector<cv::Mat> & channels,
    std::vector<cv::Mat> & channels_hist,
    int & segmentsCount) {

    if(mask_stack.empty() || segmentsCount >= kMaxSegmentsCount) {
        result_vector = mask_stack;
        result_vector.insert(result_vector.end(), result_masks.begin(), result_masks.end());
        return;
    }

    Mat mask = mask_stack.back();
    mask_stack.pop_back();

    int valley, peak, index, max_valley, max_peak = 0;
    bool found = false;
    for(int i=0; i<3; i++) {
        calcHist(&channels[i], 1, 0, mask, channels_hist[i], 1, &hist_size, &hist_ranges);
        calculate_midpoint(channels_hist[i], valley, peak);

        if(valley <= 0) {
            result_masks.push_back(mask);
        }
        else if(peak > max_peak) {
            found = true;
            index = i;
            max_valley = valley;
            max_peak = peak;
        }
    }

    if(found) {
        generate_mask(channels_hist[index], max_valley, index, mask_stack, channels);
        segmentsCount += 2;
    }
    else {
        result_vector = mask_stack;
        result_vector.insert(result_vector.end(), result_masks.begin(), result_masks.end());
        return;
    }

    OhlanderFunc(
        result_masks,
        mask_stack,
        result_vector,
        channels,
        channels_hist,
        segmentsCount);
}

void calculate_midpoint(Mat hist, int& valley, int& peak) {

    GaussianBlur(hist, hist, Size(55,55), 0, 0);

    map<int, int> hist_map;

    for(int i=0; i<hist.rows; i++) {
        hist_map.insert(make_pair(hist.at<float>(i,0), i));
    }

    int fmax = 0, smax = 0;

    for(int i=0; i<hist.rows; i++) {
        if(hist.at<float>(i, 0) >= fmax) {
            fmax = hist.at<float>(i, 0);
        } else break;
    }

    for(int i=hist.rows-1; i>hist_map[fmax]; i--) {
        if(hist.at<float>(i,0) >= smax) {
            smax = hist.at<float>(i, 0);
        } else break;
    }

    if(abs(hist_map[fmax] - hist_map[smax]) < t) {
        valley = -1;
        peak = -1;
    } else {
        valley = (hist_map[fmax] + hist_map[smax]) / 2;
        peak = max(fmax, smax);
    }
}

void generate_mask(Mat hist, int valley, int index, vector<Mat> & mask_stack, std::vector<cv::Mat> & channels)
{

    Mat hist_left = hist.clone();
    Mat hist_right = hist.clone();

    for(int i=valley; i<hist.rows; i++) {
        hist_left.at<float>(i,0) = 0;
    }

    for(int i=valley; i>=0; i--) {
        hist_right.at<float>(i,0) = 0;
    }

    Mat left_mask(channels[index].size(), CV_8UC1, Scalar(0));
    Mat right_mask = left_mask.clone();

    for(int i=0; i<channels[index].rows; i++) {
        for(int j=0; j<channels[index].cols; j++) {
            int intensity = channels[index].at<uchar>(i,j);
            left_mask.at<uchar>(i,j) = hist_left.at<float>(intensity, 0) > 0? 1: 0;
        }
    }

    for(int i=0; i<channels[index].rows; i++) {
        for(int j=0; j<channels[index].cols; j++) {
            int intensity = channels[index].at<uchar>(i,j);
            right_mask.at<uchar>(i,j) = hist_right.at<float>(intensity, 0) > 0? 1: 0;
        }
    }

    mask_stack.push_back(right_mask);
    mask_stack.push_back(left_mask);
}

void Ohlander::postProcessingMask(cv::Mat &mask)
{
    MaskEditor::removeNoise(mask);
}

}
