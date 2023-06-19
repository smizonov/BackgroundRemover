#pragma once

#include <QRunnable>
#include <QObject>
#include <QString>
#include <QElapsedTimer>
#include <QTimer>
#include <QDir>

#include "src/model/BgRemover.h"
#include "Fwd.h"
//#include "AlgoInterface.h"

#include <QUrl>

namespace backgroundRemover {

class BgRemoverTask : public QObject, public QRunnable
{
    Q_OBJECT

signals:
    void taskCompleted();
    void previewRequested(QUrl srcPath, QUrl dstPath);
    void processedImagesCountChanged(int count);
    void totalImagesCountChanged(int count);
    void elapsedTimeChanged(int count);

public:
    BgRemoverTask(
        BgRemoverPtr,
        BgRemoverSettingsPtr,
        int imageCount);

public:
    void run() override;
    int getElapsedTime();

public slots:
    void stop();
    void resume();

private:
    BgRemoverHandlers bgRemoverHandlersCreator();

private:
    std::unique_ptr<BgRemover> remover_;
    BgRemoverSettingsPtr settings_;
    int imageCount_;
    int invocationTimeInMsec_{ 0 };
//    std::unique_ptr<QTimer> timer_;
    std::unique_ptr<QElapsedTimer> elapsedTimer_;
};

}
