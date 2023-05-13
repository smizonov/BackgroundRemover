#pragma once

#include "BgRemoverSettings.h"

namespace backgroundRemover{

class SubstructionSettings : public BgRemoverSettings
{
public:
    SubstructionSettings(
        std::filesystem::path srcDir,
        std::filesystem::path dstFolderPath,
        std::filesystem::path bgImagePath)
        : BgRemoverSettings(std::move(srcDir), std::move(dstFolderPath))
        , bgImagePath_(bgImagePath)
    {}

    SubstructionSettings(SubstructionSettings &&) = default;
    std::filesystem::path const & bgImagePath()
    {
        return bgImagePath_;
    }

protected:
    std::filesystem::path bgImagePath_;
};

using SubstructionSettingsPtr = std::shared_ptr<SubstructionSettings>;

}
