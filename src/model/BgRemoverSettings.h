#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>

namespace backgroundRemover{

class BgRemoverSettings
{
public:
    BgRemoverSettings(std::filesystem::path srcFolderPath, std::filesystem::path dstFolderPath)
        : srcFolderPath_(std::move(srcFolderPath))
        , dstFolderPath_(std::move(dstFolderPath))
    {}

    std::filesystem::path const & srcFolderPath() const
    {
        return srcFolderPath_;
    }

    std::filesystem::path const & dstFolderPath() const
    {
        return dstFolderPath_;
    }

    virtual ~BgRemoverSettings() = default;
private:
    std::filesystem::path srcFolderPath_;
    std::filesystem::path dstFolderPath_;
};

}
