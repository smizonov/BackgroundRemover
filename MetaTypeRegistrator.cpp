#include "MetaTypeRegistrator.h"

#include <QQmlApplicationEngine>
#include <QtCore/QCoreApplication>

#include "RmBgMethods.h"
#include "PreviewImages.h"
#include "Utils.h"

namespace backgroundRemover {


void MetaTypeRegistrator::initMetaTypes()
{
    qAddPreRoutine(
            []()
            {
    qmlRegisterUncreatableMetaObject(
        RmBgMethodsNamespace::staticMetaObject,
        "bgRemover",
        1,
        0,
        "RmBgMethod",
        "Can't create RmBgMethod");

    qmlRegisterUncreatableType<PreviewImages>(
            "bgRemover",
            1,
            0,
            "PreviewImagesViewModel",
            "Cant'create PreviewImagesViewModel!");
    qRegisterMetaType<PreviewImages *>("PreviewImages *");
    });

    qmlRegisterSingletonType<Utils>("utils", 1, 0, "Utils", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new Utils();
    });
}

}
