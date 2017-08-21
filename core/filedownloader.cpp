#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent) :
 QObject(parent)
{
 connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
 SLOT (fileDownloaded(QNetworkReply*)));
    url = imageUrl;
}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
 m_DownloadedData = pReply->readAll();
 //emit a signal
 pReply->deleteLater();
 emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
 return m_DownloadedData;
}

void FileDownloader::start()
{
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}
