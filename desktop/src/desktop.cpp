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
#include <vca/sqlite_userdb.h>

#include <qtc/CommandQueue.h>
#include <qtc/Input.h>
#include <qtc/Model.h>
#include <qtc/Output.h>
#include <qtc/ThreadPool.h>

#include <engine/SearchHandler.h>

#include <view/View.h>

#define VCA_REGISTER_QT_METATYPE(typeName)                                     \
    qRegisterMetaType<typeName>();                                             \
    qRegisterMetaTypeStreamOperators<typeName>(#typeName)

int
main(int argc, char** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    const auto work_dir = vca::user_config_dir() / "vca";
    fs::create_directories(work_dir);

    vca::init_logging(work_dir / "logs" / "vca_desktop.log");
    vca::set_log_level(vca::Logger::Level::Debug);

    VCA_INFO << "Starting vca_desktop";
    VCA_INFO << "work_dir: " << work_dir;

    vca::SqliteUserDb user_db{work_dir / "user_data" / "user.db",
                              vca::UserDb::OpenType::ReadOnly};

    QGuiApplication app{argc, argv};

    VCA_REGISTER_QT_METATYPE(QList<bool>);
    VCA_REGISTER_QT_METATYPE(QList<double>);
    VCA_REGISTER_QT_METATYPE(QList<qint32>);
    VCA_REGISTER_QT_METATYPE(QList<quint32>);
    VCA_REGISTER_QT_METATYPE(QList<qint64>);
    VCA_REGISTER_QT_METATYPE(QList<quint64>);
    VCA_REGISTER_QT_METATYPE(QList<QString>);

    qtc::Model model{"Vca", "Findle"};
    qtc::ThreadPool threadPool{std::thread::hardware_concurrency()};

    qtc::CommandQueue mainQueue;

    model.addHandler(std::make_unique<vca::SearchHandler>(
        model, threadPool, mainQueue, user_db));

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
