#pragma once

#include <QObject>
#include <QUrl>

namespace backgroundRemover {

class PreviewImages:
        public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl srcImagePath MEMBER srcImagePath_ NOTIFY srcImagePathChanged);
    Q_PROPERTY(QUrl dstImagePath MEMBER dstImagePath_ NOTIFY dstImagePathChanged);

signals:
    void srcImagePathChanged();
    void dstImagePathChanged();
    void continueSelected();
    void stopSelected();

public slots:
    void updatePath(QUrl srcPath, QUrl dstPath);

public:
    PreviewImages();

private:
    QUrl srcImagePath_;
    QUrl dstImagePath_;
};

}
