#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <functional>
#include <QApplication>
//#include "src/model/SubstructionSettings.h"

#include <iostream>

#include "BgRemoverTask.h"

namespace backgroundRemover {

BgRemoverTask::BgRemoverTask(
    BgRemoverPtr remover,
    BgRemoverSettingsPtr settings,
    int imageCount)
    : remover_(std::move(remover))
    , settings_(std::move(settings))
    , imageCount_(imageCount)
{}

void BgRemoverTask::run()
{
    try
    {
        elapsedTimer_ = std::make_unique<QElapsedTimer>();
        elapsedTimer_->start();
        remover_->start(settings_, bgRemoverHandlersCreator());
    }
    catch(std::exception & ec)
    {
        std::cout << "BgRemoverTask::run: exception = " << ec.what() << std::endl;
    }
}

int BgRemoverTask::getElapsedTime()
{
    return (elapsedTimer_ ? elapsedTimer_->elapsed() : 0) + invocationTimeInMsec_;
}

void BgRemoverTask::stop()
{
    remover_->stop();
}

void BgRemoverTask::resume()
{
    elapsedTimer_.reset(new QElapsedTimer());
    elapsedTimer_->start();
    remover_->resume();
}

BgRemoverHandlers BgRemoverTask::bgRemoverHandlersCreator()
{
    BgRemoverHandlers handlers;
    handlers.onImageHandle = [this](int totalCount, int processedCount)
    {
        if (imageCount_ != totalCount)
        {
            imageCount_ = totalCount;
            emit totalImagesCountChanged(imageCount_);
        }

        emit processedImagesCountChanged(processedCount);
    };

    handlers.onFinish = [this](std::error_code ec)
    {
        invocationTimeInMsec_ = getElapsedTime();
        elapsedTimer_.reset();

        emit taskCompleted();
    };
    handlers.previewImagePathsReceived = [this](std::filesystem::path srcIm, std::filesystem::path dstIm)
    {
        QUrl src = QUrl::fromLocalFile(QString::fromStdString(srcIm.generic_string()));
        QUrl dst = QUrl::fromLocalFile(QString::fromStdString(dstIm.generic_string()));
        invocationTimeInMsec_ = getElapsedTime();
        elapsedTimer_.reset();
        emit previewRequested(src, dst);
    };
    return handlers;
}


}
