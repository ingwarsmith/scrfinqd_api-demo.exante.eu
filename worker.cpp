#include "worker.h"

#include <QDebug>
#include <QTimer>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QFile>
#include <QTextStream>

Worker::Worker(QObject *parent)
    : QObject(parent)
    , inited(false)
    , configured(false)
{
    netAccMgr = new QNetworkAccessManager(this);
    QTimer *tmr5min = new QTimer(this);
    connect(tmr5min, &QTimer::timeout, this, &Worker::reReadToken);
    tmr5min->setInterval(300000); //5min
    tmr5min->start();
}

void Worker::initialization(QString pathApp, int32_t timestampFrom, int32_t timestampTo, QString out)
{
    configPath = QString(pathApp).append("/config.txt");
    tsFrom = static_cast<int64_t>(timestampFrom)*1000LL;
    tsTo = static_cast<int64_t>(timestampTo)*1000LL;
    localJsonOut = out == "out:json";
    inited = true;
}

void Worker::readConfig(bool &configExist)
{
    QFile fl(configPath);
    configExist = fl.exists();
    if (!configExist) return;
    fl.open(QFile::ReadOnly|QFile::Text);
    QStringList strings;
    QTextStream stream(&fl);
    while (!stream.atEnd())
    {
        strings << stream.readLine();
    }

    foreach (QString str, strings)
    {
        QStringList params = str.split("=");
        if (params.size() == 2)
        {
            settings.insert(params.at(0), params.at(1));
            if (params.at(0) == "token")
            {
                token = params.at(1);
            }
            if (params.at(0) == "instrument")
            {
                instrument = params.at(1);
            }
        }
    }

    fl.close();

    configured = true;
}

void Worker::reReadToken()
{
    if (!(inited&&configured)) return;
    QFile fl(configPath);
    fl.open(QFile::ReadOnly|QFile::Text);
    QTextStream stream(&fl);
    while (!stream.atEnd())
    {
        auto string_ = stream.readLine();
        if (string_.split("=").at(0) == "token")
        {
            token = string_.split("=").at(1);
            break;
        }
    }
    fl.close();
}

void Worker::queryThroughAPI()
{
    QUrlQuery url_query;
    QString strQuery = "https://api-demo.exante.eu/md/2.0/ohlc/";
    strQuery.append(instrument).append("/60?");
    url_query.setQuery(strQuery);
    url_query.addQueryItem("size", "5000");
    url_query.addQueryItem("from", QString::number(tsFrom));
    url_query.addQueryItem("to", QString::number(tsTo));
    url_query.addQueryItem("token", token);
    auto queryString = url_query.toString(QUrl::FullyEncoded);
    QUrl url(queryString);
    QNetworkRequest request(url);
    auto *reply = netAccMgr->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(donePostMethod()));
}

void Worker::donePostMethod()
{
    auto *replySender = qobject_cast<QNetworkReply*>(sender());
    if (replySender == nullptr)
    {
        return;
    }

    QByteArray baResult = replySender->readAll();
    QString stringResult(baResult);
    auto tsLast = tsTo;
    auto tsFirst = tsFrom;

    auto aboutToExitApp = false;

    QJsonDocument documentJson = QJsonDocument::fromJson(baResult);
    QJsonArray jsonArr = documentJson.array();
    if (jsonArr.size()/*jsonObj.keys().size()*/)
    {
        tsFirst = jsonArr.first().toObject().value("timestamp").toVariant().toLongLong();
        if (jsonArr.size() == 5000)
        {
            tsLast = jsonArr.last().toObject().value("timestamp").toVariant().toLongLong();
        }
        else
        {
            aboutToExitApp = true;
        }
    }

    QString fname = instrument;
    fname.append("_ts_").append(QString::number(tsFirst)).append("-")
            .append(QString::number(tsLast)).append(".json");
    QFile fl(fname);
    fl.open(QFile::WriteOnly|QFile::Text);
    QTextStream stream(&fl);
    stream << stringResult;
    fl.close();

    if (aboutToExitApp)
    {
        emit sig_stop_app();
    }
    else
    {
        QUrlQuery url_query;
        QString strQuery = "https://api-demo.exante.eu/md/2.0/ohlc/";
        strQuery.append(instrument).append("/60?");
        url_query.setQuery(strQuery);
        url_query.addQueryItem("size", "5000");
        url_query.addQueryItem("from", QString::number(tsLast));
        url_query.addQueryItem("to", QString::number(tsTo));
        url_query.addQueryItem("token", token);
        auto queryString = url_query.toString(QUrl::FullyEncoded);
        QUrl url(queryString);
        QNetworkRequest request(url);
        auto *reply = netAccMgr->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(donePostMethod()));
    }
}
