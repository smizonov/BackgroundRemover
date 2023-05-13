#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/model/Ohlander.h"
#include "src/model/KMeans.h"
#include "src/model/Bgsegm.h"
#include "src/model/Substruction.h"

#include "src/model/SubstructionSettings.h"

using namespace backgroundRemover;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

//    std::string srcImage{ "D:\\NIR_Remove_BG\\Untitled.jpg" };
    std::string srcImage{ "D:\\NIR_Remove_BG\\cat\\20230508_151733.jpg" };
    std::string dstImage{ "D:\\NIR_Remove_BG\\results\\MayDst.jpg" };
//    std::string srcFolderPath{ "D:\\NIR_Remove_BG\\cat" };
//    std::string dstFolderPath{ "D:\\NIR_Remove_BG\\catResult" };
//    std::string srcBackgroundPath{ "D:\\NIR_Remove_BG\\cat\\20230508_151730.jpg" };
    std::filesystem::path srcFolderPath{ "D:\\NIR_Remove_BG\\cat" };
    std::filesystem::path dstFolderPath{ "D:\\NIR_Remove_BG\\catResult" };
    std::filesystem::path srcBackgroundPath{ "D:\\NIR_Remove_BG\\cat\\20230508_151730.jpg" };
//    backgroundRemover::Bgsegm::start(srcImage, dstImage);
    auto settings = std::make_shared<SubstructionSettings>(srcFolderPath, dstFolderPath, srcBackgroundPath);
    Substruction remover;
    remover.start(settings);
//    backgroundRemover::Kmeans::performKmeans(srcImage, dstImage);
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
