#include <memory>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>

#include "HandlerFactory.h"
#include "Input.h"
#include "ModelImpl.h"
#include "Output.h"

int
main(int argc, char** argv)
{
    const auto work_dir = vca::user_config_dir() / "vca";
    fs::create_directories(work_dir);

    vca::init_logging(work_dir / "logs" / "vca_desktop.log");
    vca::set_log_level(vca::Logger::Level::Debug);

    VCA_INFO << "Starting vca_desktop";
    VCA_INFO << "work_dir: " << work_dir;

    vca::SqliteUserDb user_db{work_dir / "user_data" / "user.db",
                              vca::UserDb::OpenType::ReadOnly};

    app::ModelImpl model;
    model.setHandlers(app::makeHandlers(model, user_db));

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("appModel", &model);

    qmlRegisterType<app::Input>("App", 1, 0, "AppInputBase");
    qmlRegisterType<app::Output>("App", 1, 0, "AppOutputBase");

    engine.load(QUrl(QStringLiteral("qrc:/desktop.qml")));

    return app.exec();
}
