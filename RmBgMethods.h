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
    MLU2net,
    MLDis
};
Q_ENUM_NS(RmBgMethod)
}

using RmBgMethod = RmBgMethodsNamespace::RmBgMethod;
}
