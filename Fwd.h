#pragma once

#include <memory>

namespace backgroundRemover {

class BgRemoverTask;

class BgRemover;
using BgRemoverPtr = std::unique_ptr<BgRemover>;

}
