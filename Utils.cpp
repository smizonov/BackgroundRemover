#include "Utils.h"

#include <QFileDialog>
#include <QtGui/QDesktopServices>
#include <QWindow>
#include <QString>

namespace backgroundRemover {

QString Utils::showDirectoryDialog(QString directory, const QString &caption)
{
    QPointer<QWindow> focusedWindow(QGuiApplication::focusWindow());

    directory.remove(0,8);
    auto result = QFileDialog::getExistingDirectory(
        nullptr,
        caption.isEmpty() ? QCoreApplication::applicationName() : caption,
        directory,
        QFileDialog::ShowDirsOnly);

    if (!focusedWindow.isNull())
        focusedWindow->requestActivate();

    return result;
}

QString Utils::showOpenDialog(QString directory, const QString &caption)
{
    QPointer<QWindow> focusedWindow(QGuiApplication::focusWindow());

    directory.remove(0,8);
    auto filename = QFileDialog::getOpenFileName(
        nullptr,
        caption.isEmpty() ? QCoreApplication::applicationName() : caption,
        directory);

    if (!focusedWindow.isNull())
        focusedWindow->requestActivate();

    return filename;
}

void Utils::showInExplorer(QString directory)
{
    if (directory.isEmpty())
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(directory));
}

}
