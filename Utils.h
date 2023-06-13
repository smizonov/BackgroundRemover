#pragma once

#include <QQmlEngine>
#include <QGuiApplication>
#include <QObject>

namespace backgroundRemover {

class Utils : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE static QString showDirectoryDialog(QString directory, const QString &caption);
    Q_INVOKABLE static QString showOpenDialog(QString directory, const QString &caption);
    Q_INVOKABLE static void showInExplorer(QString directory);
};

}
