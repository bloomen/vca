#include <memory>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "HandlerFactory.h"
#include "Input.h"
#include "ModelImpl.h"
#include "Output.h"

int
main(int argc, char** argv)
{
    app::ModelImpl model;
    model.setHandlers(app::makeHandlers(model));

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("appModel", &model);

    qmlRegisterType<app::Input>("App", 1, 0, "AppInputBase");
    qmlRegisterType<app::Output>("App", 1, 0, "AppOutputBase");

    engine.load(QUrl(QStringLiteral("qrc:/desktop.qml")));

    return app.exec();
}
