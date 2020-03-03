#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "camerasdk.h"
#include "brqttcamera.h"

static QObject *camera_sdk_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new CameraSDK();
}

static QObject *brqtt_camera_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new BrqttCamera();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Iktwo Corp.");
    QCoreApplication::setOrganizationDomain("iktwo.com");
    QCoreApplication::setApplicationName("Brqtt");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/qml/qml");

    qmlRegisterSingletonType<CameraSDK>("com.iktwo.brqtt", 1, 0, "CameraSDK", camera_sdk_provider);
    qmlRegisterSingletonType<BrqttCamera>("com.iktwo.brqtt", 1, 0, "BrqttCamera", brqtt_camera_provider);

    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    const QUrl url(QStringLiteral("qrc:/qml/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
