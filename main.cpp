#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/model/Ohlander.h"
#include "src/model/Dbscan.h"
#include "src/model/Bgsegm.h"
#include "src/model/Substruction.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

//    std::string srcImage{ "D:\\NIR_Remove_BG\\Untitled.jpg" };
    std::string srcImage{ "D:\\NIR_Remove_BG\\cat\\20230508_151733.jpg" };
    std::string dstImage{ "D:\\NIR_Remove_BG\\results\\MayDst.jpg" };
//    backgroundRemover::Bgsegm::start(srcImage, dstImage);
    backgroundRemover::Kmeans::performKmeans(srcImage, dstImage);
//    backgroundRemover::Ohlander::start(srcImage, dstImage);
//    backgroundRemover::Bgsegm::start("D:\\NIR_Remove_BG\\cat", "D:\\NIR_Remove_BG\\catResult");
//    backgroundRemover::Substruction::start("D:\\NIR_Remove_BG\\cat", "D:\\NIR_Remove_BG\\catResult");
//    backgroundRemover::Bgsegm::start("D:\\NIR_Remove_BG\\13", "D:\\NIR_Remove_BG\\catResult");
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
