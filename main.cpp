#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QString>
#include "worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc != 4)
    {
        qDebug() << "Usage:";
        qDebug() << "srcfinqd timestamp_from timestamp_to [out:json | out:mssql]";
        qDebug() << "\nTimestamps in seconds";
        qDebug() << "If 3rd parameter out:mssql, connection parameters must be";
        qDebug() << "present in file config.txt";
        return 1;
    }

    QStringList params = app.arguments();
    params.removeFirst();

    auto paramsOk = true;

    auto toIntOk1 = false, toIntOk2 = false;
    QString(params.at(0)).toInt(&toIntOk1);
    QString(params.at(1)).toInt(&toIntOk2);

    if (!toIntOk1 || !toIntOk2)
    {
        qDebug() << "Error: Incorrect timestamp parameter(s).";
        paramsOk = false;
    }

    QStringList thirdLst = QString(params.at(2)).split(":");
    if (!(thirdLst.at(0) == "out" && (thirdLst.at(1) == "json" || thirdLst.at(1) == "mssql")))
    {
        qDebug() << "Error: Incorrect out parameter.";
        paramsOk = false;
    }

    if (!paramsOk) return 2;

    int32_t tsFrom = QString(params.at(0)).toInt();
    int32_t tsTo = QString(params.at(1)).toInt();

    Worker worker;
    worker.initialization(app.applicationDirPath(), tsFrom, tsTo, params.at(2));
    auto configExist = false;
    worker.readConfig(configExist);
    if (!configExist)
    {
        qDebug() << "Error: Configuration file config.txt not exist.";
        return 3;
    }
    QObject::connect(&worker, SIGNAL(sig_stop_app()), qApp, SLOT(quit()));
    worker.queryThroughAPI();

    return app.exec();
}
