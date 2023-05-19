#pragma once
#include <opencv2/dnn.hpp>
#include <filesystem>

class ModelWorker
{
private:
    ModelWorker(std::filesystem::path);

public:
    static ModelWorker& getInstance(std::filesystem::path);

private:
    cv::dnn::Net net_;
};

#endif // MODELWORKER_H
