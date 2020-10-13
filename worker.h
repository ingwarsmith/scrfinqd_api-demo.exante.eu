#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMap>
#include <QUrl>

class QNetworkAccessManager;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit    Worker(QObject *parent = 0);
    void        initialization(QString pathApp, int32_t timestampFrom, int32_t timestampTo, QString out);
    void        readConfig(bool &configExist);
    void        getTokenFromConfig();
    void        queryThroughAPI();

private:
    QString     configPath;
    QString     token;
    QString     instrument;
    int64_t     tsFrom;
    int64_t     tsTo;
    bool        localJsonOut;
    bool        inited;
    bool        configured;
    QMap<QString, QString> settings;
    QNetworkAccessManager *netAccMgr;

signals:
    void        sig_stop_app();

private slots:
    void        reReadToken();
    void        donePostMethod();
};

#endif // WORKER_H
