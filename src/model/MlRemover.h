#pragma once

#include "BgRemover.h"


namespace backgroundRemover{

class MlRemover: public BgRemover
{
public:
    void start(BgRemoverSettingsPtr, BgRemoverHandlers) override;
};

}
