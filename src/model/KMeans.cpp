#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <vector>

#include "KMeans.h"
#include "MaskEditor.h"

namespace backgroundRemover{

double euclidean_distance(const std::vector<double>& point1, const std::vector<double>& point2)
{
    double sum = 0.0;
    for (size_t i = 0; i < point1.size(); ++i)
    {
        double diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

// Function to find the neighboring points of a given point
std::vector<unsigned int> find_neighbors(const std::vector<std::vector<double>>& feature_points, unsigned int point_index, double eps)
{
    std::vector<unsigned int> neighbors;
    for (unsigned int i = 0; i < feature_points.size(); ++i)
    {
        if (i != point_index && euclidean_distance(feature_points[i], feature_points[point_index]) <= eps)
        {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

// Function to perform DBSCAN clustering
void dbscan(const std::vector<std::vector<double>>& feature_points, double eps, unsigned int min_pts, std::vector<unsigned int>& labels)
{
    const unsigned int num_points = feature_points.size();
    labels.assign(num_points, 0);  // Assign an initial label of 0 to all points

    unsigned int current_label = 0;

    for (unsigned int i = 0; i < num_points; ++i)
    {
        if (labels[i] != 0)  // Skip points that have already been assigned a label
            continue;

        std::vector<unsigned int> neighbors = find_neighbors(feature_points, i, eps);

        if (neighbors.size() < min_pts)  // Assign noise label to points with insufficient neighbors
        {
            labels[i] = -1;
        }
        else  // Start a new cluster and expand it by visiting neighbors
        {
            ++current_label;
            labels[i] = current_label;

            unsigned int neighbor_index = 0;
            while (neighbor_index < neighbors.size())
            {
                unsigned int neighbor = neighbors[neighbor_index];

                if (labels[neighbor] == -1)  // Change noise points to border points of the current cluster
                {
                    labels[neighbor] = current_label;
                }
                else if (labels[neighbor] == 0)  // Expand the cluster to unvisited neighbors
                {
                    labels[neighbor] = current_label;
                    std::vector<unsigned int> neighbor_neighbors = find_neighbors(feature_points, neighbor, eps);
                    if (neighbor_neighbors.size() >= min_pts)
                    {
                        neighbors.insert(neighbors.end(), neighbor_neighbors.begin(), neighbor_neighbors.end());
                    }
                }

                ++neighbor_index;
            }
        }
    }
}

//void Dbscan::start(std::string src, std::string dst)
//{
//    // Load color image and convert to 2D vector of features
//    cv::Mat image = cv::imread(src);
//    std::vector<std::vector<double>> data;
//    for (int i = 0; i < image.rows; i++)
//    {
//        for (int j = 0; j < image.cols; j++)
//        {
//            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
//            data.push_back({static_cast<double>(pixel[0]), static_cast<double>(pixel[1]), static_cast<double>(pixel[2])});
//        }
//    }

//    // Set DBSCAN parameters and perform clustering
//    double eps = 10.0;
//    unsigned int min_pts = 10;
//    std::vector<unsigned long> labels;
//    dlib::find_clusters_dbscan(data, labels, squared_euclidean_distance(), eps, min_pts);
//    dlib::cluster(data, labels, centroids, squared_euclidean_distance(), eps, min_pts);

//    // Create output image with cluster labels
//    cv::Mat output = cv::Mat::zeros(image.size(), CV_8UC3);
//    for (int i = 0; i < labels.size(); i++)
//    {
//        int row = i / image.cols;
//        int col = i % image.cols;
//        int label = static_cast<int>(labels[i]);
//        output.at<cv::Vec3b>(row, col) = cv::Vec3b{static_cast<uchar>(data[i][0]), static_cast<uchar>(data[i][1]), static_cast<uchar>(data[i][2])};
//    }

//    // Show output image
//    cv::imshow("Output", output);
//    cv::waitKey(0);

//    return 0;
//}

void KMeans::start(BgRemoverSettingsPtr settings, BgRemoverHandlers handlers)
{
    auto countOfHandledImages{ 0 };
    for (auto path : std::filesystem::directory_iterator(settings->srcFolderPath()))
    {
        if (stopped_)
            break;
        cv::Mat tmpFrame = cv::imread(path.path().string(), cv::IMREAD_GRAYSCALE);
        auto preDst = performKmeans(std::move(tmpFrame));
//        auto dst = MaskEditor::removeNoise(std::move(preDst));
        auto dstPath = settings->dstFolderPath() / path.path().filename();
        cv::imwrite(dstPath.string(), preDst);
        handlers.onImageHandle(++countOfHandledImages);
    }
    handlers.onFinish(std::error_code());
}

cv::Mat KMeans::performKmeans(cv::Mat src)
{
    cv::Mat image = src;

    // Reshape the image to a 2D matrix of pixels
    auto reshapedImage = image.reshape(1, image.rows * image.cols);
    reshapedImage.convertTo(reshapedImage, CV_32F);

    // Set the parameters for k-means clustering
    int clusterCount = 2; // Number of clusters
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 20.0); // Termination criteria
    int attempts = 3; // Number of times the algorithm is executed using different initial labellings

    // Perform k-means clustering
    cv::Mat labels, centers;
    cv::kmeans(reshapedImage, clusterCount, labels, criteria, attempts, cv::KmeansFlags::KMEANS_PP_CENTERS, centers);

    // Reshape the clustered image
    cv::Mat clusteredImage = labels.reshape(0, image.rows);

    // Convert the clustered image to 8-bit for visualization
    clusteredImage.convertTo(clusteredImage, CV_8U, 255.0 / (clusterCount - 1));

    return clusteredImage;
    // Display the original and clustered images
//    cv::imshow("Original Image", image);


////    cv.erode(src, dst, M, anchor, 1, cv.BORDER_CONSTANT, cv.morphologyDefaultBorderValue());

//    cv::imshow("Clustered Image", clusteredImage);
//    cv::imwrite(dst, clusteredImage);
//    cv::waitKey(0);

}


}
