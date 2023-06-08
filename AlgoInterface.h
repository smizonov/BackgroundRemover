#pragma once

#include <QObject>
#include <QString>
#include <QDir>

#include "src/model/Ohlander.h"
#include "src/model/KMeans.h"
#include "src/model/BgRemover.h"
#include "src/model/Bgsegm.h"
#include "src/model/Substruction.h"
#include "RmBgMethods.h"

namespace backgroundRemover {

using BgRemoverPtr = std::unique_ptr<BgRemover>;
class AlgoInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString srcFolder MEMBER srcFolder_ NOTIFY srcFolderChanged);
    Q_PROPERTY(QString dstFolder MEMBER dstFolder_ NOTIFY dstFolderChanged);
    Q_PROPERTY(QString bgImagePath MEMBER bgImagePath_ NOTIFY bgImagePathChanged);
    Q_PROPERTY(float progress MEMBER progress_ NOTIFY progressChanged);
    Q_PROPERTY(backgroundRemover::RmBgMethodsNamespace::RmBgMethod method MEMBER method_ NOTIFY methodChanged);
    Q_PROPERTY(bool startEnabled READ startEnabled NOTIFY startEnabledChanged);

public:
    AlgoInterface(QObject* obj);

signals:
    void srcFolderChanged();
    void dstFolderChanged();
    void bgImagePathChanged();
    void progressChanged();
    void methodChanged();
    void startEnabledChanged();
    void stop();
    void priviewRequested(QString srcPath, QString dstPath);

public:
    Q_INVOKABLE void start();
    Q_INVOKABLE QString showDirectoryDialog(const QString &directory, const QString &caption) const;
    Q_INVOKABLE QString showOpenDialog(const QString &directory, const QString &caption) const;

    bool startEnabled();
    BgRemoverHandlers bgRemoverHandlersCreator();

private:
    std::unique_ptr<BgRemover> bgRemoverCreator();
    BgRemoverSettingsPtr bgRemoverSettingsCreator();
    void completed();

private:
    QString srcFolder_;
    QString dstFolder_;
    QString bgImagePath_;
    float progress_{ 0.0 };
    RmBgMethod method_;
};

}
