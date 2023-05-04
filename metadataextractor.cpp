
#include "metadataextractor.h"

#include "qdebug.h"

metaDataExtractor::metaDataExtractor(QString &path, DatabaseController &db)
{
    fullPath = path;
    qDebug() << fullPath;
    daba = &db;
}

void metaDataExtractor::getMetaData(QList<QUrl> urlList)
{
    videoDataType data;
    int size = urlList.size();
    for (qsizetype i = 0 ; i < size; ++i)
    {
        mi->Open(urlList.at(i).path().toStdWString().erase(0,1));
        qDebug() << urlList.at(i).path();
        qDebug() << QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Modified_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name));
        data.titles.append(urlList.at(i).path().section("/",-1,-1).section(".",0,-2));
        data.paths.append(urlList.at(i).path());
        data.widths.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_Video, 0, __T("Width"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
        data.heights.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_Video, 0, __T("Height"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
        data.datesCreated.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Created_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).section(".",0,0));
        data.datesModified.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Modified_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).section(".",0,0));
        data.filetypes.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Format"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
        data.codecs.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_Video, 0, __T("CodecID"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
        data.durations.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Duration"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
        data.sizes.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("FileSize"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
    }
    daba->InsertMedia(data);

}

metaDataExtractor::~metaDataExtractor()
{

}
