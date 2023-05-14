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
    remover_->start(settings_, bgRemoverHandlersCreator());
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
    return handlers;
}


}
