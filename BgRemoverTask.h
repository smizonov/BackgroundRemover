#pragma once

#include <QRunnable>
#include <QObject>
#include <QString>
#include <QDir>

#include "src/model/Ohlander.h"
#include "src/model/KMeans.h"
#include "src/model/BgRemover.h"
#include "src/model/Bgsegm.h"
#include "src/model/Substruction.h"
#include "RmBgMethods.h"
#include "AlgoInterface.h"

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

public:
    BgRemoverTask(
        BgRemoverPtr,
        BgRemoverSettingsPtr,
        int imageCount);

public:
    void run() override;

public slots:
    void stop();
    void resume();

private:
    BgRemoverHandlers bgRemoverHandlersCreator();

private:
    std::unique_ptr<BgRemover> remover_;
    BgRemoverSettingsPtr settings_;
    int imageCount_;
};

}
