#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include "src/model/Ohlander.h"
#include "src/model/KMeans.h"
#include "src/model/Bgsegm.h"
#include "src/model/Substruction.h"
#include "AlgoInterface.h"

#include "src/model/SubstructionSettings.h"

using namespace backgroundRemover;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    auto interface = new AlgoInterface(&app);
    engine.rootContext()->setContextProperty("viewModel", interface);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
