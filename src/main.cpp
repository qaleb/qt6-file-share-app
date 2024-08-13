#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QLocale>
#include "guibehind.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "dukto6_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QCoreApplication::setApplicationName("dukto");
    QCoreApplication::setOrganizationName("idv.coolshou");
    QCoreApplication::setOrganizationDomain("com.dukto");

    QQmlApplicationEngine engine;

    // Instantiate GuiBehind with the engine
    GuiBehind guiBehind(engine);

    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
