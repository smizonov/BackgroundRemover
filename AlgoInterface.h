#include <QObject>
#include <QString>

#include "src/model/Ohlander.h"
#include "src/model/Dbscan.h"
#include "src/model/Bgsegm.h"
#include "src/model/Substruction.h"

namespace backgroundRemover {

class AlgoInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString srcFolder MEMBER srcFolder_ NOTIFY srcFolderChanged);
    Q_PROPERTY(QString dstFolder MEMBER dstFolder_ NOTIFY dstFolderChanged);
    Q_PROPERTY(QString bgImagePath MEMBER bgImagePath_ NOTIFY bgImagePathChanged);

    signals:
    void srcFolderChanged();
    void dstFolderChanged();
    void bgImagePathChanged();

    public slots:
//    void start();


private:
    QString srcFolder_;
    QString dstFolder_;
    QString bgImagePath_;
};

}
