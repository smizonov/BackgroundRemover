#include <string>

#include "BgRemover.h"

namespace backgroundRemover{

class Ohlander : public BgRemover
{
public:
    static void start(BgRemoverSettings) override;

private:
    void startImpl(BgRemoverSettings);
    void prepareImage();
    void OhlanderFunc();
};

}
