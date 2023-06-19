#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThreadPool>
#include <QFileDialog>
#include <QWindow>

#include <functional>
#include "src/model/SubstructionSettings.h"
#include "src/model/MlRemover.h"
#include "src/model/U2ModelWorker.h"
#include "src/model/DisModelWorker.h"

#include "AlgoInterface.h"
#include "BgRemoverTask.h"

namespace backgroundRemover {

AlgoInterface::AlgoInterface(QObject *obj)
    : QObject(obj)
    , previewImages_(std::make_unique<PreviewImages>())
{
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
        if (!u2ModelWorker_)
            u2ModelWorker_ = std::make_shared<U2ModelWorker>();
        return std::make_unique<MlRemover>(u2ModelWorker_);
    case RmBgMethod::MLDis:
        if (!disModelWorker_)
            disModelWorker_ = std::make_shared<DisModelWorker>();
        return std::make_unique<MlRemover>(disModelWorker_);
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
//    processedCount_ = 1;
    //    emit processedCountChanged();
}

auto invokeInContextThread(QObject * obj)
{
    return [obj](std::function<void()> fun)
    {
        QMetaObject::invokeMethod(
                obj,
                [fun { std::move(fun) }]() mutable
                {
                    fun();
                },
                Qt::QueuedConnection);
    };
}

void AlgoInterface::start()
{
    auto settings = bgRemoverSettingsCreator();
    auto remover = bgRemoverCreator();
    QDir dir(srcFolder_);
    task_ = std::make_unique<BgRemoverTask>(std::move(remover), std::move(settings), dir.count());
    task_->setAutoDelete(false);
    connect(task_.get(), &BgRemoverTask::taskCompleted, [this]()
            {
                completed();
                task_.reset();
                qInfo() << "Task completed";
            });
    connect(task_.get(), &BgRemoverTask::processedImagesCountChanged, [this](int processedCount)
            {
                processedCount_ = processedCount;
                emit processedCountChanged();
            });
    connect(task_.get(), &BgRemoverTask::totalImagesCountChanged, [this](int processedCount)
            {
                totalCount_ = processedCount;
                emit processedCountChanged();
            });

    connect(task_.get(), &BgRemoverTask::previewRequested, [this](...){ emit previewRequested(); });
    connect(task_.get(), &BgRemoverTask::previewRequested, previewImages_.get(), &PreviewImages::updatePath);

    connect(task_.get(), &BgRemoverTask::elapsedTimeChanged, this, &AlgoInterface::elapsedTimeChanged);
    connect(previewImages_.get(), &PreviewImages::continueSelected, task_.get(), &BgRemoverTask::resume);
    connect(previewImages_.get(), &PreviewImages::stopSelected, task_.get(), &BgRemoverTask::stop);

    connect(previewImages_.get(), &PreviewImages::continueSelected, [this](...){ emit previewCancelRequested(); });
    connect(previewImages_.get(), &PreviewImages::stopSelected, [this](...){ emit previewCancelRequested(); });

    connect(this, &AlgoInterface::stop, task_.get(), &BgRemoverTask::stop);

    timer_ = std::make_unique<QTimer>();
    connect(timer_.get(), &QTimer::timeout, this, [this]
    {
        if (task_)
        emit elapsedTimeChanged(task_->getElapsedTime());
    });
    timer_->start(1000);

    QThreadPool::globalInstance()->start(task_.get());
}

bool AlgoInterface::startEnabled()
{
    return !srcFolder_.isEmpty() &&
            !dstFolder_.isEmpty() && (RmBgMethod::Extruction != method_ || !bgImagePath_.isEmpty());
}

PreviewImages *AlgoInterface::previewImages()
{
    qInfo() << "previewImages_.get()" << previewImages_.get();
    return previewImages_.get();
}


}
