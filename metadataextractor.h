
#ifndef METADATAEXTRACTOR_H
#define METADATAEXTRACTOR_H

#include "QUrl"
#include <MediaInfoDLL.h>
#include "databasecontroller.h"

class metaDataExtractor
{
public:
    metaDataExtractor(QString &path, DatabaseController &db);
    ~metaDataExtractor();
    void getMetaData(QList<QUrl> urlList );
private:
    MediaInfoDLL::MediaInfo *mi = new MediaInfoDLL::MediaInfo;
    DatabaseController *daba ;
    QString fullPath;
};

#endif // METADATAEXTRACTOR_H
