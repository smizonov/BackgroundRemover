#include "PreviewImages.h"

namespace backgroundRemover {

void PreviewImages::updatePath(QUrl srcPath, QUrl dstPath)
{
    if (srcPath != srcImagePath_)
    {
        srcImagePath_ = srcPath;
        emit srcImagePathChanged();
    }

    if (dstPath != dstImagePath_)
    {
        dstImagePath_ = dstPath;
        emit dstImagePathChanged();
    }
}

PreviewImages::PreviewImages()
{

}

}
