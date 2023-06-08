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
    handlers.firstResult = [this](std::filesystem::path srcIm, std::filesystem::path dstIm)
    {
        QString src = QString::fromStdString("file:///" + srcIm.generic_string());
        QString dst = QString::fromStdString("file:///" + dstIm.generic_string());
        emit previewRequested(src, dst);
    };
    return handlers;
}


}
