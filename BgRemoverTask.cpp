#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <functional>
#include "src/model/SubstructionSettings.h"

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
        remover_->start(settings_, bgRemoverHandlersCreator());
    }
    catch(std::exception & ec)
    {
        std::cout << "BgRemoverTask::run: exception = " << ec.what() << std::endl;
    }
}

void BgRemoverTask::stop()
{
    remover_->stop();
    resume();
}

void BgRemoverTask::resume()
{
    remover_->resume();
}

BgRemoverHandlers BgRemoverTask::bgRemoverHandlersCreator()
{
    BgRemoverHandlers handlers;
    handlers.onImageHandle = [this](int count)
    {
        emit progressChanged(static_cast<float>(count) / imageCount_);
    };

    handlers.onFinish = [this](std::error_code ec)
    {
        emit taskCompleted();
    };
    handlers.previewImagePathsReceived = [this](std::filesystem::path srcIm, std::filesystem::path dstIm)
    {
        QUrl src = QUrl::fromLocalFile(QString::fromStdString(srcIm.generic_string()));
        QUrl dst = QUrl::fromLocalFile(QString::fromStdString(dstIm.generic_string()));
        emit previewRequested(src, dst);
    };
    return handlers;
}


}
