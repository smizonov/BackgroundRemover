#include "Utils.h"

#include <QFileDialog>
#include <QtGui/QDesktopServices>
#include <QWindow>

namespace backgroundRemover {

QString Utils::showDirectoryDialog(const QString &directory, const QString &caption)
{
    QPointer<QWindow> focusedWindow(QGuiApplication::focusWindow());

    auto result = QFileDialog::getExistingDirectory(
        nullptr,
        caption.isEmpty() ? QCoreApplication::applicationName() : caption,
        directory,
        QFileDialog::ShowDirsOnly);

    if (!focusedWindow.isNull())
        focusedWindow->requestActivate();

    return result;
}

QString Utils::showOpenDialog(const QString &directory, const QString &caption)
{
    QPointer<QWindow> focusedWindow(QGuiApplication::focusWindow());

    auto filename = QFileDialog::getOpenFileName(
        nullptr,
        caption.isEmpty() ? QCoreApplication::applicationName() : caption,
        directory);

    if (!focusedWindow.isNull())
        focusedWindow->requestActivate();

    return filename;
}

void Utils::showInExplorer(const QString &directory)
{
    if (directory.isEmpty())
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(directory));
}

}
