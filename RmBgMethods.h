#pragma once

#include <QtCore/QObject>


namespace backgroundRemover
{

namespace RmBgMethodsNamespace
{
Q_NAMESPACE
    enum RmBgMethod
{
    Ohlander,
    KMeans,
    Extruction,
    MlAlgo
};
Q_ENUM_NS(RmBgMethod)
}

using RmBgMethod = RmBgMethodsNamespace::RmBgMethod;
}
