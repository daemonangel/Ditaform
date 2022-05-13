#include "FileDownloader.h"
#include "RegulatoryTemplate.h"
#include "Urlmon.h"

QNetworkAccessManager* manager;

FileDownloader::FileDownloader(QObject* parent)
{
	
}

void FileDownloader::startDownload()
{
    manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    //request.setUrl(QUrl("https://zebra.sharepoint.com/:u:/s/ETP/EWQ-s0wzN-xAjIdYv70pPyUBdAn0wg5Wmsixpxjv2ANbZQ?e=Ktpc9J"));
    //HRESULT hr = URLDownloadToFile(NULL, L"https://zebra.sharepoint.com/sites/ETP/Shared%20Documents/Forms/AllItems.aspx?id=%2Fsites%2FETP%2FShared%20Documents%2FDITA%20Repository%2Fdita%2Dfiles%2Fbarcode%2Dtest%2Edita&parent=%2Fsites%2FETP%2FShared%20Documents%2FDITA%20Repository%2Fdita%2Dfiles&p=true&ga=1", L".\\PRAYFORDOWNLOAD.dita", 0, 0);
    //sharepoint with webview2
    //sharepoint with https://docs.microsoft.com/en-us/azure/active-directory/develop/msal-overview
    //public git repo
    //sharepoint using REST API and OAUTH
    //request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:99.0) Gecko/20100101 Firefox/99.0");
    request.setTransferTimeout(5000);

    QNetworkReply* reply = manager->get(request);
    auto e1 = reply->errorString();
    auto e2 = reply->error();
    connect(reply, &QNetworkReply::errorOccurred,
        this, &FileDownloader::error);
    connect(reply, &QNetworkReply::sslErrors,
        this, &FileDownloader::sslError);
    connect(reply, &QNetworkReply::downloadProgress,
        this, &FileDownloader::progress);
    connect(reply, &QNetworkReply::finished,
        this, &FileDownloader::finished);
    connect(manager, &QNetworkAccessManager::sslErrors,
        this, &FileDownloader::sslError);
}

void FileDownloader::finished()
{
    QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
    QByteArray b = reply->readAll();
    QFile file(RegulatoryTemplate::sourceBookmapFile);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    std::cout << b;
}

void FileDownloader::progress()
{
    std::cout << "ERROR :-(";
}

void FileDownloader::error()
{
    auto reply = dynamic_cast<QNetworkReply*>(sender());
    std::cout << reply->errorString().toStdString();
}

void FileDownloader::sslError()
{
    std::cout << "ERROR :-(";
}

FileDownloader::~FileDownloader()
{
}