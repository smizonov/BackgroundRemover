#pragma once

#include <QObject>
#include <QString>
#include <QDir>

#include "src/model/Ohlander.h"
#include "src/model/KMeans.h"
#include "src/model/BgRemover.h"
#include "src/model/DisModelWorker.h"
#include "src/model/U2ModelWorker.h"
#include "src/model/Substruction.h"
#include "RmBgMethods.h"
#include "Fwd.h"
#include "PreviewImages.h"

namespace backgroundRemover {

class AlgoInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString srcFolder MEMBER srcFolder_ NOTIFY srcFolderChanged);
    Q_PROPERTY(QString dstFolder MEMBER dstFolder_ NOTIFY dstFolderChanged);
    Q_PROPERTY(QString bgImagePath MEMBER bgImagePath_ NOTIFY bgImagePathChanged);
    Q_PROPERTY(float processedCount MEMBER processedCount_ NOTIFY processedCountChanged);
    Q_PROPERTY(float totalCount MEMBER totalCount_ NOTIFY totalCountChanged);
    Q_PROPERTY(backgroundRemover::RmBgMethodsNamespace::RmBgMethod method MEMBER method_ NOTIFY methodChanged);
    Q_PROPERTY(bool startEnabled READ startEnabled NOTIFY startEnabledChanged);
    Q_PROPERTY(PreviewImages* previewImages READ previewImages CONSTANT);

public:
    AlgoInterface(QObject* obj);

signals:
    void srcFolderChanged();
    void dstFolderChanged();
    void bgImagePathChanged();
    void processedCountChanged();
    void totalCountChanged();
    void methodChanged();
    void startEnabledChanged();
    void previewRequested();
    void previewCancelRequested();
    void stop();
    void elapsedTimeChanged(int time);

public:
    Q_INVOKABLE void start();

    bool startEnabled();
    PreviewImages * previewImages();
    BgRemoverHandlers bgRemoverHandlersCreator();

private:
    std::unique_ptr<BgRemover> bgRemoverCreator();
    BgRemoverSettingsPtr bgRemoverSettingsCreator();
    void completed();

private:
    QString srcFolder_;
    QString dstFolder_;
    QString bgImagePath_;
    std::shared_ptr<U2ModelWorker> u2ModelWorker_;
    std::shared_ptr<DisModelWorker> disModelWorker_;
    int processedCount_{ 0 };
    int totalCount_{ 0 };
    RmBgMethod method_;
    std::unique_ptr<QTimer> timer_;
    std::unique_ptr<BgRemoverTask> task_;
    std::unique_ptr<PreviewImages> previewImages_;
};

}
