/*!
\file
\brief Файл реализации класса извлечения метаданных

*/
#include "metadataextractor.h"

#include "qdebug.h"

metaDataExtractor::metaDataExtractor(QString &path, DatabaseController &db,QMap<int ,QString> tagMap, int selectedTable)
{
    tagMapLocal = tagMap;
    fullPath = path;
    qDebug() << fullPath;
    daba = &db;
    selectedTableLocal = selectedTable;
}

void metaDataExtractor::getVideoMetaData(QList<QUrl> urlList)
{

    videoDataType data;
    int size = urlList.size();
    bool wrongMediaType = false;
    bool noVideos = true;
    for (qsizetype i = 0 ; i < size; ++i)
    {

        mi->Open(urlList.at(i).path().toStdWString().erase(0,1));
        qDebug() << QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("StreamKind"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name));

        if (videoTypes.contains(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Format"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name))))
        {
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
            noVideos = false;
        }
        else wrongMediaType = true;
    }
    if(wrongMediaType == true)
        QMessageBox::warning(nullptr,"Предупреждение","Не видео файлы были отброшены");
    if(noVideos)
        return ;
    daba->InsertMedia(data);
    constructTagSelectionDialog();
    itemLoader(data);
}

void metaDataExtractor::getImageMetaData(QList<QUrl> urlList)
{

    imageDataType data;
    int size = urlList.size();
    bool wrongMediaType = false;
    bool noAudios = true;
    for (qsizetype i = 0 ; i < size; ++i)
    {

        mi->Open(urlList.at(i).path().toStdWString().erase(0,1));
        qDebug() << QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("StreamKind"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name));

        if (imageTypes.contains(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Format"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name))))
        {
            qDebug() << urlList.at(i).path();
            qDebug() << QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Modified_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name));
            data.titles.append(urlList.at(i).path().section("/",-1,-1).section(".",0,-2));
            data.paths.append(urlList.at(i).path());
            data.widths.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_Image, 0, __T("Width"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.heights.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_Image, 0, __T("Height"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.datesCreated.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Created_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).section(".",0,0));
            data.datesModified.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Modified_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).section(".",0,0));
            data.filetypes.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Format"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.sizes.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("FileSize"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            noAudios = false;
        }
        else wrongMediaType = true;
    }
    if(wrongMediaType == true)
        QMessageBox::warning(nullptr,"Предупреждение","Не изображения были отброшены");
    if(noAudios)
        return ;
    daba->InsertMedia(data);
    constructTagSelectionDialog();
    itemLoader(data);
}

void metaDataExtractor::getAudioMetaData(QList<QUrl> urlList)
{

    audioDataType data;
    int size = urlList.size();
    bool wrongMediaType = false;
    bool noAudios = true;
    for (qsizetype i = 0 ; i < size; ++i)
    {

        mi->Open(urlList.at(i).path().toStdWString().erase(0,1));
        qDebug() << QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("StreamKind"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name));

        if (audioTypes.contains(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Format"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name))))
        {
            qDebug() << urlList.at(i).path();
            qDebug() << QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Modified_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name));
            data.titles.append(urlList.at(i).path().section("/",-1,-1).section(".",0,-2));
            data.paths.append(urlList.at(i).path());
            data.datesCreated.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Created_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).section(".",0,0));
            data.datesModified.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("File_Modified_Date"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).section(".",0,0));
            data.bitRates.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_Audio, 0, __T("BitRate"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.filetypes.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Format"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.albums.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Album"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.bands.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Performer"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.titlesEncoded.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Title"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.durations.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("Duration"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            data.sizes.append(QString::fromStdWString(mi->Get(MediaInfoDLL::Stream_General, 0, __T("FileSize"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)));
            noAudios = false;
        }
        else wrongMediaType = true;
    }
    if(wrongMediaType == true)
        QMessageBox::warning(nullptr,"Предупреждение","Не аудио файлы были отброшены");
    if(noAudios)
        return ;
    daba->InsertMedia(data);
    constructTagSelectionDialog();
    itemLoader(data);
}

void metaDataExtractor::constructTagSelectionDialog()
{

    QVBoxLayout *dialogMainLayout = new QVBoxLayout;
    QHBoxLayout *dialogCenterLayout = new QHBoxLayout;
    QHBoxLayout *dialogBotttomLayout = new QHBoxLayout;
    QLabel *header = new QLabel;
    QPushButton *closeButton = new QPushButton;

    // getVideoByTitle
    header->setText("При необходимости, добавьте теги к файлам");
    closeButton->setText("Закончить с добавлением");
    selectionDialog->setLayout(dialogMainLayout);
    dialogMainLayout->addWidget(header);
    dialogMainLayout->addLayout(dialogCenterLayout);
    dialogMainLayout->addLayout(dialogBotttomLayout);
    dialogBotttomLayout->addWidget(closeButton);
    selectionDialog->setFixedHeight(500);
    selectionDialog->setFixedWidth(500);
    dialogCenterLayout->addWidget(mediasList);
    selectionDialog->setModal(true);
    selectionDialog->show();
    mediasList->setColumnCount(2);
    mediasList->setColumnHidden(1,true);
    mediasList->verticalHeader()->hide();
    mediasList->setHorizontalHeaderLabels(QStringList() << "Название" );

    mediasList->setColumnWidth(0, 300);

    dialogCenterLayout->addLayout(constructTagBar());
    connect(closeButton, &QPushButton::pressed, this, &metaDataExtractor::closeButtonPressed);

}

void metaDataExtractor::itemLoader(videoDataType data)
{
    videoDataType videosToAddTagsTo;
    videosToAddTagsTo = daba->getVideoByPath(data.paths);
    mediasList->setRowCount(videosToAddTagsTo.id.size());
    for (int var = 0; var < videosToAddTagsTo.titles.size(); ++var) {
        mediasList->setItem(var , 0,new QTableWidgetItem(videosToAddTagsTo.titles[var]) );
        mediasList->setItem(var , 1,new QTableWidgetItem(QString::number(videosToAddTagsTo.id[var])) );
    }
}

void metaDataExtractor::itemLoader(imageDataType data)
{
    imageDataType mediasToAddTagsTo;
    mediasToAddTagsTo = daba->getImageByPath(data.paths);
    mediasList->setRowCount(mediasToAddTagsTo.id.size());
    for (int var = 0; var < mediasToAddTagsTo.titles.size(); ++var) {
        mediasList->setItem(var , 0,new QTableWidgetItem(mediasToAddTagsTo.titles[var]) );
        mediasList->setItem(var , 1,new QTableWidgetItem(QString::number(mediasToAddTagsTo.id[var])) );
    }
}

void metaDataExtractor::itemLoader(audioDataType data)
{
    audioDataType mediasToAddTagsTo;
    mediasToAddTagsTo = daba->getAudioByPath(data.paths);
    mediasList->setRowCount(mediasToAddTagsTo.id.size());
    for (int var = 0; var < mediasToAddTagsTo.titles.size(); ++var) {
        mediasList->setItem(var , 0,new QTableWidgetItem(mediasToAddTagsTo.titles[var]) );

        mediasList->setItem(var , 1,new QTableWidgetItem(QString::number(mediasToAddTagsTo.id[var])) );
    }
}

QVBoxLayout* metaDataExtractor::constructTagBar()
{
    QPushButton *addTagButton = new QPushButton;
    QPushButton *resetTagsSelectionButton = new QPushButton;
    QVBoxLayout *rightDockLayout = new QVBoxLayout;
    QLabel * mainText = new QLabel;

    tagComboBox->setEditable(true);
    tagSelection->setAlignment(Qt::AlignCenter);
    tagSelection->setWordWrap(true);
    addTagButton->setText("Добавить теги");
    resetTagsSelectionButton->setText("Очистить выборку");
    mainText->setText("Добавление тегов");
    mainText->setAlignment(Qt::AlignTop);
    mainText->setFixedHeight(15);
    rightDockLayout->addWidget(mainText);
    rightDockLayout->addWidget(tagComboBox);
    rightDockLayout->addWidget(tagSelection);
    rightDockLayout->addWidget(addTagButton);
    rightDockLayout->addWidget(resetTagsSelectionButton);
    updateTagComboBox();
    connect (addTagButton, &QPushButton::pressed, this, &metaDataExtractor::addTagButtonPressed);
    connect (tagComboBox, &QComboBox::currentIndexChanged,this, &metaDataExtractor::selectTag);
    connect (resetTagsSelectionButton, &QPushButton::pressed, this, &metaDataExtractor::resetTagSelectionButtonPressed);

    return rightDockLayout;
}

void metaDataExtractor::selectTag(int index)
{
    int size = tagsToAddToMedias.count();
    tagsToAddToMedias.insert(tagMapLocal.key(tagComboBox->itemText(index)));
    bool inserted = tagsToAddToMedias.count() > size;
    if(inserted ){
        tagSelection->setText(tagSelection->text().append(" " + tagComboBox->itemText(index)));
    }
   // qDebug()<< tagsToAddToMedias;
    //qDebug() << tagSelection->text();
}
void metaDataExtractor::updateTagComboBox()
{
    tagComboBox->clear();

    for (QString var : tagMapLocal){
        tagComboBox->addItem(var);
    }
    tagComboBox->setCurrentIndex(-1);
    tagsToAddToMedias.clear();
    tagSelection->setText("");

}

void metaDataExtractor::resetTagSelectionButtonPressed()
{
    updateTagComboBox();
}

void metaDataExtractor::addTagButtonPressed()
{

    QList<QTableWidgetItem*>  selectedItems ;
    switch (selectedTableLocal)
    {
    case 1:selectedItems = mediasList->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            daba->AddTagsToMedia(mediasList->item(var->row(),1)->text().toInt(),DatabaseController::VIDEO_TYPE,tagsToAddToMedias);
        }; break;
    case 2: selectedItems = mediasList->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            daba->AddTagsToMedia(mediasList->item(var->row(),1)->text().toInt(),DatabaseController::IMAGE_TYPE,tagsToAddToMedias);
        }; break;
    case 3: selectedItems = mediasList->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            daba->AddTagsToMedia(mediasList->item(var->row(),1)->text().toInt(),DatabaseController::AUDIO_TYPE,tagsToAddToMedias);
        }; break;
    }
    updateTagComboBox();
}

void metaDataExtractor::closeButtonPressed()
{
    selectionDialog->close();
}
metaDataExtractor::~metaDataExtractor()
{

}
