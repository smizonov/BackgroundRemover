#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "MlRemover.h"
#include <iostream>
#include <fstream>
#include <iomanip>
//#define _SECURE_SCL 0
//#define _CONTAINER_DEBUG_LEVEL 0

namespace backgroundRemover{


cv::Mat readCSV(const std::string& filename, int rows, int cols)
{
    cv::Mat mat(rows, cols, CV_32FC3); // Create a floating-point BGR image matrix

    std::ifstream file(filename);
    std::string line;
    int row = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;

        while (std::getline(ss, cell, ','))
        {
            float pixelValue = std::stof(cell);
            mat.at<cv::Vec3f>(row, col)[0] = pixelValue;

            col++;
        }

        row++;
    }

    return mat;
}

//void normalizeImage(cv::Mat& image) {
//    cv::Scalar mean(0.485, 0.456, 0.406);
//    cv::Scalar stdDev(0.229, 0.224, 0.225);
//    cv::Size targetSize(320, 320);

//    // Convert image to floating point representation
//    image.convertTo(image, CV_32F);

//    // Split the image into separate channels
//    std::vector<cv::Mat> channels;
//    cv::split(image, channels);

////    for (auto & channel : channels)
////    {
////        for (auto & pix : channel) {

////        }
////    }

//    // Normalize each channel
//    for (int i = 0; i < channels.size(); ++i) {
//        channels[i] = (channels[i] - mean[i]) / stdDev[i];
//    }

//    // Merge the normalized channels
//    cv::merge(channels, image);

//    // Resize the image to the target size
//    cv::resize(image, image, targetSize);
//}

cv::Mat normalizeImage(const cv::Mat& img) {

    cv::Scalar mean(0.485, 0.456, 0.406);
    cv::Scalar std(0.229, 0.224, 0.225);
    cv::Size size(320, 320);
    cv::Mat im;
    cv::cvtColor(img, im, cv::COLOR_BGR2RGB);
    cv::resize(im, im, size, cv::INTER_LANCZOS4);

    cv::Mat im_ary;
    im.convertTo(im_ary, CV_32F);
    im_ary /= 255.0;

//    return im_ary;
    std::cout << "im_ary size = " << im_ary.size << std::endl;

    cv::Mat tmpImg(im_ary.size(), CV_32FC3);
    cv::subtract(im_ary, mean, tmpImg);
    cv::divide(tmpImg, std, tmpImg);

//    cv::Mat normalizedImg;
//    tmpImg.convertTo(normalizedImg, CV_32F);
//    normalizedImg = normalizedImg.t();

//    tmpImg = tmpImg.t();
//    cv::imshow("Result", tmpImg);
//    cv::waitKey(0);
    return tmpImg;
}

cv::Mat predict(cv::Mat& pred) {

//    pred = pred.reshape(1, size.height);

    double ma, mi;
    cv::minMaxLoc(pred, &mi, &ma);

    cv::Mat normalizedPred = (pred - mi) / (ma - mi);

    cv::Mat mask;
    normalizedPred.convertTo(mask, CV_8UC1, 255.0);

    return mask;
}

cv::Mat processPredictions(cv::Mat& ort_outs, const cv::Mat& inputImage)
{
    std::vector<cv::Mat> channels;
    cv::split(ort_outs, channels);

//    std::cout << ort_outs;

    std::cout << "size = " << ort_outs.size << std::endl;
    std::cout << "size = " << channels.size() << std::endl;
//    cv::Mat pred = channels[0];
    cv::Mat pred(ort_outs.size[2], ort_outs.size[3], CV_32F, ort_outs.ptr<float>());


    std::ofstream file("D:\\qt\\mlArrayResult\\cppDst.txt");
    for(int i = 0; i < pred.cols; ++i)
    {
        for(int j = 0; j < pred.rows; ++j)
        {
            file << pred.at<float>(i, j) << " ";
        }
        file << "\n";
    }

    std::cout << "size = " << pred.size << std::endl;
    double ma, mi;
    cv::minMaxLoc(pred, &mi, &ma);
    std::cout << "minVal = " << mi << "maxVal = " << ma << std::endl;

    cv::Mat normalizedPred = pred;// (pred - mi) / (ma - mi);

//    cv::imshow((cv::Mat(320, 320, CV_32FC1, normalizedPred));
    cv::waitKey(0);
    cv::Mat mask;
    normalizedPred.convertTo(mask, CV_8UC1, 255.0);

    double minValue = 0.0;
    double maxValue = 255.0;

    // Convert the floatMat to CV_8UC1 format
    cv::Mat uint8Mat;
    cv::normalize(pred, uint8Mat, minValue, maxValue, cv::NORM_MINMAX, CV_8UC1);

    mask = uint8Mat;
    std::ofstream file1("D:\\qt\\mlArrayResult\\maskCppDst.txt");
    for(int i = 0; i < mask.cols; ++i)
    {
        for(int j = 0; j < mask.rows; ++j)
        {
            file1 << static_cast<int>(mask.at<uchar>(i, j)) << " ";
        }
        file1 << "\n";
    }

//    cv::minMaxLoc(pred, &mi, &ma);
//    cv::Mat resizedMask;
//    cv::resize(mask, resizedMask, inputImage.size(), cv::INTER_LANCZOS4);

    cv::Mat dstMask;
//    cv::threshold(mask, dstMask, 1, 255, cv::THRESH_BINARY);
    return mask;
}


void MlRemover::start(BgRemoverSettingsPtr, BgRemoverHandlers)
{
    cv::dnn::Net net = cv::dnn::readNetFromONNX("D:\\qt\\BackgroundRemover\\model\\u2net.onnx");
    cv::Mat image = cv::imread("D:\\NIR_Remove_BG\\photo_2.jpg");
    cv::Mat blob;
//    normalizeImage(image);
//    cv::Mat normalizedImage = normalizeImage(image);
    cv::Mat normalizedImage = readCSV("D:\\qt\\mlArrayResult\\pythonSrc.csv", 320, 320);
    cv::dnn::blobFromImage(normalizedImage, blob, 1, cv::Size(320, 320), cv::Scalar(0, 0, 0));

    std::ofstream outputFile("D:\\qt\\mlArrayResult\\cppSrc.txt");

    // Iterate through each element and write it to the file
    std::cout << "blob total = " << blob.total() << std::endl;
//    for (int i = 0; i < blob.total(); i++) {
//        auto value = blob.at<float>(i);
//        outputFile << value << " ";
//    }
    std::cout << "blob total = " << blob.total() << std::endl;

    std::cout << "Blob size = " << blob.size << std::endl;
//    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0, cv::Size(100, 100), cv::Scalar(), false, false);

    net.setInput(blob);
    cv::Mat output;

    try {
        output = net.forward();
        // Process the output
    } catch (const cv::Exception& e) {
        std::cout << "Exception = " << e.what();
//        return;
        // Handle the exception (e.g., print error message, log, etc.)
    }

    std::cout << "1, size = " << output.size << std::endl;
//    cv::Mat mask(output.size[2], output.size[3], CV_32F, output.ptr<float>());
    std::cout << "2"<< std::endl;
//    cv::threshold(mask, mask, 0.7, 255, cv::THRESH_BINARY);
    std::cout << "3"<< std::endl;
//    mask.convertTo(mask, CV_8U);
    std::cout << "4"<< std::endl;

    cv::Mat result = processPredictions(output, image);
//    cv::Mat result = predict(output);
//    image.copyTo(result, mask);

    cv::imshow("Result", result);
    cv::waitKey(0);
}

}
