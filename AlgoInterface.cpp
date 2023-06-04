#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThreadPool>
#include <functional>
#include "src/model/SubstructionSettings.h"
#include "src/model/MlRemover.h"
#include "src/model/U2ModelWorker.h"
#include "src/model/DisModelWorker.h"

#include "AlgoInterface.h"
#include "BgRemoverTask.h"

namespace backgroundRemover {

void initMetaTypes()
{
    qmlRegisterUncreatableMetaObject(
        RmBgMethodsNamespace::staticMetaObject,
        "bgRemover",
        1,
        0,
        "RmBgMethod",
        "Can't create RmBgMethod");
}

AlgoInterface::AlgoInterface(QObject *obj)
    : QObject(obj)
{
    initMetaTypes();
    connect(this, &AlgoInterface::srcFolderChanged, this, &AlgoInterface::startEnabledChanged);
    connect(this, &AlgoInterface::dstFolderChanged, this, &AlgoInterface::startEnabledChanged);
    connect(this, &AlgoInterface::methodChanged, this, &AlgoInterface::startEnabledChanged);
    connect(this, &AlgoInterface::bgImagePathChanged, this, &AlgoInterface::startEnabledChanged);
}

std::unique_ptr<BgRemover> AlgoInterface::bgRemoverCreator()
{
    switch (method_)
    {
    case RmBgMethod::KMeans:
        return std::make_unique<KMeans>();
    case RmBgMethod::Ohlander:
        return std::make_unique<Ohlander>();
    case RmBgMethod::Extruction:
        return std::make_unique<Substruction>();
    case RmBgMethod::MLU2net:
        return std::make_unique<MlRemover>(std::make_unique<U2ModelWorker>());
    case RmBgMethod::MLDis:
        return std::make_unique<MlRemover>(std::make_unique<DisModelWorker>());
    }
    return nullptr;
}

BgRemoverSettingsPtr AlgoInterface::bgRemoverSettingsCreator()
{
    switch (method_)
    {
    case RmBgMethod::KMeans:
    case RmBgMethod::Ohlander:
    case RmBgMethod::MLU2net:
    case RmBgMethod::MLDis:
        return std::make_shared<BgRemoverSettings>(std::filesystem::path(srcFolder_.toStdString()),
                                 std::filesystem::path(dstFolder_.toStdString()));
        break;
    case RmBgMethod::Extruction:
        return std::make_shared<SubstructionSettings>(std::filesystem::path(srcFolder_.toStdString()),
                                    std::filesystem::path(dstFolder_.toStdString()),
                                    std::filesystem::path(bgImagePath_.toStdString()));
    default:
        throw std::runtime_error("AlgoInterface::bgRemoverSettingsCreator: unknown method");
    }
}

void AlgoInterface::completed()
{
    progress_ = 1;
    emit progressChanged();
}

void AlgoInterface::start()
{
    auto settings = bgRemoverSettingsCreator();
    auto remover = bgRemoverCreator();
    QDir dir(srcFolder_);
    auto task = new BgRemoverTask(std::move(remover), std::move(settings), dir.count());
    task->setAutoDelete(true);
    connect(task, &BgRemoverTask::taskCompleted, [this]()
                     {
                completed();
                         qInfo() << "Task completed";
                     });
    connect(task, &BgRemoverTask::progressChanged, [this](float progress)
                     {
                         progress_ = progress;
                         emit progressChanged();
                     });

    connect(this, &AlgoInterface::stop, task, &BgRemoverTask::stop);

    QThreadPool::globalInstance()->start(task);
}

bool AlgoInterface::startEnabled()
{
    return !srcFolder_.isEmpty() &&
           !dstFolder_.isEmpty() && (RmBgMethod::Extruction != method_ || !bgImagePath_.isEmpty());
}





}
