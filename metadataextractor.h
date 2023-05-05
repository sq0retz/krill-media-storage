
#ifndef METADATAEXTRACTOR_H
#define METADATAEXTRACTOR_H

#include "QUrl"
#include <MediaInfoDLL.h>
#include "databasecontroller.h"
#include <qSet>
#include <QtWidgets>
class metaDataExtractor : public QObject
{
public:
    metaDataExtractor(QString &path, DatabaseController &db,QMap<int ,QString> tagMap,int selectedTable);
    ~metaDataExtractor();
    void getVideoMetaData(QList<QUrl> urlList );
    void getImageMetaData(QList<QUrl> urlList );
    void getAudioMetaData(QList<QUrl> urlList );
private:
    void constructTagSelectionDialog();
    void itemLoader(videoDataType data);
    void itemLoader(imageDataType data);
    void itemLoader(audioDataType data);
    QVBoxLayout* constructTagBar();
    QComboBox *tagComboBox = new QComboBox;
    QLabel *tagSelection = new QLabel;
    MediaInfoDLL::MediaInfo *mi = new MediaInfoDLL::MediaInfo;
    DatabaseController *daba ;
    QString fullPath;
    QSet<QString> videoTypes = {"Matroska","FLV","VOB","AVI","QuickTime", "MPEG-4","MXF","NSV","WEBM"};
    QSet<QString> imageTypes = {"PNG","JPEG","GIF","RAW"};
    QSet<QString> audioTypes = {"FLAC","MPEG Audio"};
    QSet<int> tagsToAddToMedias;
    int selectedTableLocal = 1;
    QMap<int ,QString> tagMapLocal;
    QTableWidget *mediasList = new QTableWidget;
    QDialog *selectionDialog = new QDialog;
private slots:
    void addTagButtonPressed();
    void resetTagSelectionButtonPressed();
    void selectTag(int index);
    void updateTagComboBox();
    void closeButtonPressed();
};

#endif // METADATAEXTRACTOR_H
