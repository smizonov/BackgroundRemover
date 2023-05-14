#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThreadPool>
#include <functional>
#include "src/model/SubstructionSettings.h"

#include "AlgoInterface.h"
#include "BgRemoverTask.h"

namespace backgroundRemover {

namespace {

}

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
    case RmBgMethod::Substruction:
        return std::make_unique<Substruction>();
    }
    return nullptr;
}

BgRemoverSettingsPtr AlgoInterface::bgRemoverSettingsCreator()
{
    switch (method_)
    {
    case RmBgMethod::KMeans:
    case RmBgMethod::Ohlander:
        return std::make_shared<BgRemoverSettings>(std::filesystem::path(srcFolder_.toStdString()),
                                 std::filesystem::path(dstFolder_.toStdString()));
        break;
    case RmBgMethod::Substruction:
        return std::make_shared<SubstructionSettings>(std::filesystem::path(srcFolder_.toStdString()),
                                    std::filesystem::path(dstFolder_.toStdString()),
                                    std::filesystem::path(bgImagePath_.toStdString()));
    }
}

void AlgoInterface::completed()
{
    progress_ = 1;
    emit progressChanged();
}

void AlgoInterface::start()
{
    srcFolder_ = "D:\\NIR_Remove_BG\\cat";
    dstFolder_ = "D:\\NIR_Remove_BG\\catResult";
    bgImagePath_ = "D:\\NIR_Remove_BG\\cat\\20230508_151730.jpg";
    method_ = RmBgMethod::Substruction;
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

//    QThreadPool::globalInstance()->setMaxThreadCount(3);
    QThreadPool::globalInstance()->start(task);
}

bool AlgoInterface::startEnabled()
{
    return !srcFolder_.isEmpty() &&
           !dstFolder_.isEmpty() && (RmBgMethod::Substruction != method_ || !bgImagePath_.isEmpty());
}





}
