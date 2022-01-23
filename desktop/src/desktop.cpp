#include <csignal>
#include <memory>

#include <QtCore/QStandardPaths>
#include <QtCore/QTimer>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <vca/config.h>
#include <vca/file_lock.h>
#include <vca/filesystem.h>
#include <vca/logging.h>

#include <qtc/CommandQueue.h>
#include <qtc/Input.h>
#include <qtc/Model.h>
#include <qtc/Output.h>
#include <qtc/ThreadPool.h>

#include <engine/DaemonHandler.h>
#include <engine/SearchHandler.h>
#include <engine/StatusHandler.h>

#include <view/View.h>

int
main(int argc, char** argv)
{
    const auto work_dir = vca::user_config_dir() / "findle";
    fs::create_directories(work_dir);

    vca::init_logging(work_dir / "logs" / "findle.log");
    vca::set_log_level(vca::Logger::Level::Debug);

    VCA_INFO << "Starting findle";
    VCA_INFO << "work_dir: " << work_dir;

    QGuiApplication app{argc, argv};
    app.setApplicationName("Findle");

    qRegisterMetaType<QList<bool>>();
    qRegisterMetaType<QList<double>>();
    qRegisterMetaType<QList<qint32>>();
    qRegisterMetaType<QList<quint32>>();
    qRegisterMetaType<QList<qint64>>();
    qRegisterMetaType<QList<quint64>>();
    qRegisterMetaType<QList<QString>>();

    qtc::Model model{"findle", "Findle"};
    qtc::ThreadPool threadPool{std::thread::hardware_concurrency()};

    qtc::CommandQueue mainQueue;

    vca::AppConfig app_config;
    const auto app_config_path = work_dir / "app.json";
    while (!fs::exists(app_config_path))
    {
        VCA_INFO << "Waiting for: " << app_config_path;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    {
        vca::FileGuard app_config_lock{app_config_path};
        app_config = vca::AppConfig::read(app_config_path);
    }

    model.addHandler(std::make_unique<vca::DaemonHandler>(
        app_config.host(), app_config.port(), model, threadPool, mainQueue));
    model.addHandler(
        std::make_unique<vca::SearchHandler>(model, threadPool, mainQueue));
    model.addHandler(
        std::make_unique<vca::StatusHandler>(model, threadPool, mainQueue));

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
