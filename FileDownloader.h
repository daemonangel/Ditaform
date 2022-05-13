#pragma once

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    FileDownloader(QObject* parent = nullptr);
    ~FileDownloader();

public slots:
    void finished();
    void error();
    void sslError();
    void progress();

private:
    void startDownload();
};

