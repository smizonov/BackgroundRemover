#pragma once

#include <QQmlEngine>
#include <QGuiApplication>
#include <QObject>

namespace backgroundRemover {

class Utils : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE static QString showDirectoryDialog(const QString &directory, const QString &caption);
    Q_INVOKABLE static QString showOpenDialog(const QString &directory, const QString &caption);
};

}
