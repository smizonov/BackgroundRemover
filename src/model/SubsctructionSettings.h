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

protected:
    std::filesystem::path bgImagePath_;
};

}
