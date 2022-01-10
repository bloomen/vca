#include <csignal>
#include <memory>

#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QTimer>
#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <vca/filesystem.h>
#include <vca/logging.h>

#include <qtc/CommandQueue.h>
#include <qtc/Input.h>
#include <qtc/Model.h>
#include <qtc/Output.h>
#include <qtc/ThreadPool.h>

#include <engine/DaemonHandler.h>
#include <engine/SearchHandler.h>

#include <view/View.h>

#include "uds_client.h"

#define VCA_REGISTER_QT_METATYPE(typeName)                                     \
    qRegisterMetaType<typeName>()

int
main(int argc, char** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    const auto work_dir = vca::user_config_dir() / "findle";
    fs::create_directories(work_dir);

    vca::init_logging(work_dir / "logs" / "findle.log");
    vca::set_log_level(vca::Logger::Level::Debug);

    VCA_INFO << "Starting findle";
    VCA_INFO << "work_dir: " << work_dir;

    QGuiApplication app{argc, argv};
    app.setApplicationName("Findle");

    VCA_REGISTER_QT_METATYPE(QList<bool>);
    VCA_REGISTER_QT_METATYPE(QList<double>);
    VCA_REGISTER_QT_METATYPE(QList<qint32>);
    VCA_REGISTER_QT_METATYPE(QList<quint32>);
    VCA_REGISTER_QT_METATYPE(QList<qint64>);
    VCA_REGISTER_QT_METATYPE(QList<quint64>);
    VCA_REGISTER_QT_METATYPE(QList<QString>);

    qtc::Model model{"findle", "Findle"};
    qtc::ThreadPool threadPool{std::thread::hardware_concurrency()};

    qtc::CommandQueue mainQueue;

    vca::UdsClient uds_client{work_dir / "findle.sock",
                              work_dir / "findled.sock"};

    model.addHandler(std::make_unique<vca::DaemonHandler>(
        uds_client.host(), uds_client.port(), model, threadPool, mainQueue));
    model.addHandler(
        std::make_unique<vca::SearchHandler>(model, threadPool, mainQueue));

    model.connectHandlers();

    mainQueue.sync();

    vca::View view;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("vcaModel", &model);
    engine.rootContext()->setContextProperty("vcaView", &view);

    qmlRegisterType<qtc::Input>("Vca", 1, 0, "VcaInputBase");
    qmlRegisterType<qtc::Output>("Vca", 1, 0, "VcaOutputBase");

    engine.load("qrc:/view/qml/Main.qml");

    QTimer updateTimer;
    QObject::connect(
        &updateTimer, &QTimer::timeout, [&mainQueue] { mainQueue.sync(); });
    updateTimer.start(15);

    return app.exec();
}
