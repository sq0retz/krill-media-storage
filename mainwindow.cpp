
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVideoWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    centralWidgetStack = new QStackedWidget;
    videoTable = new QTableWidget;
    imageTable = new QTableWidget;
    audioTable = new QTableWidget;
    this->setWindowIcon(QIcon(":/assets/icons/assets/mainIcon"));
    this->setWindowTitle("Лучшая прога евер");
    centralWidgetStack->insertWidget(1, videoTable);
    centralWidgetStack->insertWidget(2, imageTable);
    centralWidgetStack->insertWidget(3, audioTable);
    this->setCentralWidget(centralWidgetStack);
/*
    QMediaPlayer akab;
    akab.setSource(QUrl("C:/Users/Sqoretz/Videos/video_2022-09-24_01-26-01.mp4"));

    QVideoWidget  *videoWidget= new QVideoWidget;
    akab.setVideoOutput(videoWidget);
    QVideoSink *aaa = new QVideoSink;
    akab.setVideoSink(aaa);




    connect(aaa,&QVideoSink::videoFrameChanged,this,&MainWindow::videoThumbnailCatcher);
    videoWidget->show();
   //akab.play();

    if(akab.error() == 0)
        qDebug() << "wada";
*/
    updateCache();
    constructTopToolBar();
    constructRightDock();
    constructBottomToolBar();
}
void MainWindow::constructTopToolBar()
{
    QAction *videoAction = new QAction(tr("&НАЖМИ..."), this);
    videoAction->setIcon(QIcon(":/assets/icons/assets/videoIcon"));
    videoAction->setShortcuts(QKeySequence::Open);
    videoAction->setStatusTip(tr("Открыть таблицу видео"));
    ui->topToolBar->addAction(videoAction);
    QAction *imageAction = new QAction(tr("&Open..."), this);
    imageAction->setIcon(QIcon(":/assets/icons/assets/imageIcon"));
    imageAction->setShortcuts(QKeySequence::Open);
    imageAction->setStatusTip(tr("Открыть таблицу изображений"));
    ui->topToolBar->addAction(imageAction);
    QAction *audioAction = new QAction(tr("&Open..."), this);
    audioAction->setIcon(QIcon(":/assets/icons/assets/audioIcon"));
    audioAction->setShortcuts(QKeySequence::Open);
    audioAction->setStatusTip(tr("Открыть таблицу аудио"));
    ui->topToolBar->addAction(audioAction);
    QAction *updateAction = new QAction(tr("&Выпьем..."), this);
    updateAction->setIcon(QIcon(":/assets/icons/assets/updateIcon"));
    updateAction->setShortcuts(QKeySequence::Open);
    updateAction->setStatusTip(tr("Перезагрузить базу данных"));
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred); // вау, удобно
    ui->topToolBar->addWidget(empty);
    ui->topToolBar->addAction(updateAction);
    QSize topIconSize;
    topIconSize.setHeight(100);
    topIconSize.setWidth(100);
    ui->topToolBar->setIconSize(topIconSize);


    connect(videoAction, &QAction::triggered, this ,&MainWindow::videoToolBarPressed);
    connect(imageAction, &QAction::triggered, this ,&MainWindow::imageToolBarPressed);
    connect(audioAction, &QAction::triggered, this ,&MainWindow::audioToolBarPressed);
    connect(updateAction, &QAction::triggered, this,&MainWindow::updateToolBarPressed);

}

void MainWindow::constructBottomToolBar()
{
   QAction *deleteAction = new QAction(tr("&ПОДУМОЙ..."), this);
   deleteAction->setIcon(QIcon(":/assets/icons/assets/deleteIcon"));
   deleteAction->setShortcuts(QKeySequence::Open);
   deleteAction->setStatusTip(tr("Удалить файлы"));
   ui->bottomToolBar->addAction(deleteAction);
   QAction *addFilesAction = new QAction(tr("&НЕ ДУМОЙ..."), this);
   addFilesAction->setIcon(QIcon(":/assets/icons/assets/addFileIcon"));
   addFilesAction->setShortcuts(QKeySequence::Open);
   addFilesAction->setStatusTip(tr("Добавить файлы"));
   ui->bottomToolBar->addAction(addFilesAction);
   connect(deleteAction, &QAction::triggered, this,&MainWindow::deleteButtonPressed);
   connect(addFilesAction, &QAction::triggered, this,&MainWindow::addFileButtonPressed);
}

void MainWindow::constructRightDock()
{
    QWidget* rightDockWidget = new QWidget;
    QPushButton *addTagButton = new QPushButton;
    QPushButton *resetTagsSelectionButton = new QPushButton;
    QVBoxLayout *rightDockLayout = new QVBoxLayout;
    QLabel * mainText = new QLabel;
    this->addDockWidget(Qt::RightDockWidgetArea,rightDock);
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
    rightDockWidget->setLayout(rightDockLayout);
    rightDock->setWidget(rightDockWidget);
    connect(addTagButton, &QPushButton::pressed, this, &MainWindow::addTagButtonPressed);
    connect(tagComboBox, &QComboBox::currentIndexChanged,this, &MainWindow::selectTag);
    connect(resetTagsSelectionButton, &QPushButton::pressed, this, &MainWindow::resetTagSelectionButtonPressed);
}

void MainWindow::updateVideoTable()
{

    videoSearchType SearchConstraints;
    videoTableData = db->SearchMedia(SearchConstraints);
    videoTable->clear();
    videoTable->sortItems(11,Qt::AscendingOrder);
    videoTable->setRowCount(videoTableData.id.size());
    videoTable->setColumnCount(12);
    videoTable->setColumnHidden(2,true);
    videoTable->setHorizontalHeaderLabels(QStringList() << "Название" << "Рейтинг" <<"" <<"Путь"<< "Ширина"<< "Высота"<< "Дата создания"<< "Дата изменения"<< "Тип файла" <<"Кодек"<<"Продолжительность"<<"Размер" );
    videoTable->setColumnWidth(0,500);
    videoTable->setColumnWidth(3,500);
    videoTable->verticalHeader()->hide();
    videoTable->setSortingEnabled(true);
    QString min = " минут";
    QString gb = " Гб";
    disconnect(videoTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    for (int var = 0; var < videoTableData.id.size(); ++var) {
        videoTable->setItem(var , 0,new QTableWidgetItem(videoTableData.titles[var]) );
        videoTable->setItem(var , 1,new QTableWidgetItem(QString::number(videoTableData.rating[var])) );
        videoTable->setItem(var , 2,new QTableWidgetItem(QString::number(videoTableData.id[var])  ));
        videoTable->setItem(var , 3,new QTableWidgetItem(videoTableData.paths[var]) );
        videoTable->setItem(var , 4,new QTableWidgetItem(videoTableData.widths[var]) );
        videoTable->setItem(var , 5,new QTableWidgetItem(videoTableData.heights[var]) );
        videoTable->setItem(var , 6,new QTableWidgetItem(videoTableData.datesCreated[var]) );
        videoTable->setItem(var , 7,new QTableWidgetItem(videoTableData.datesModified[var]) );
        videoTable->setItem(var , 8,new QTableWidgetItem(videoTableData.filetypes[var]) );
        videoTable->setItem(var , 9,new QTableWidgetItem(videoTableData.codecs[var]) );
        videoTable->setItem(var , 10,new QTableWidgetItem(QString::number(videoTableData.durations[var].toDouble()/(1000*60))+ min)  );
        videoTable->setItem(var , 11,new QTableWidgetItem(QString::number(videoTableData.sizes[var].toDouble()/(1024*1024*1024))+ gb) );

    }
    videoTable->sortItems(0,Qt::AscendingOrder);
    connect(videoTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
}

void MainWindow::updateImageTable()
{

    imageSearchType SearchConstraints;
    imageTableData = db->SearchMedia(SearchConstraints);
    imageTable->clear();
    imageTable->sortItems(9,Qt::AscendingOrder);
    imageTable->setRowCount(imageTableData.id.size());
    imageTable->setColumnCount(10);
    imageTable->setColumnHidden(2,true);
    imageTable->setHorizontalHeaderLabels(QStringList() << "Название" << "Рейтинг" <<"" <<"Путь"<< "Ширина"<< "Высота"<< "Дата создания"<< "Дата изменения"<< "Тип файла" <<"Размер");
    imageTable->setColumnWidth(0,500);
    imageTable->verticalHeader()->hide();
    imageTable->setSortingEnabled(true);
    QString mb = " Мб";
    disconnect(imageTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    for (int var = 0; var < imageTableData.id.size(); ++var) {
        imageTable->setItem(var , 0,new QTableWidgetItem(imageTableData.titles[var]) );
        imageTable->setItem(var , 1,new QTableWidgetItem(QString::number(imageTableData.rating[var])) );
        imageTable->setItem(var , 2,new QTableWidgetItem(QString::number(imageTableData.id[var]) ) );
        imageTable->setItem(var , 3,new QTableWidgetItem(imageTableData.paths[var]) );
        imageTable->setItem(var , 4,new QTableWidgetItem(imageTableData.widths[var]) );
        imageTable->setItem(var , 5,new QTableWidgetItem(imageTableData.heights[var]) );
        imageTable->setItem(var , 6,new QTableWidgetItem(imageTableData.datesCreated[var]) );
        imageTable->setItem(var , 7,new QTableWidgetItem(imageTableData.datesModified[var]) );
        imageTable->setItem(var , 8,new QTableWidgetItem(imageTableData.filetypes[var]) );
        imageTable->setItem(var , 9,new QTableWidgetItem(QString::number(imageTableData.sizes[var].toDouble()/(1024*1024))+ mb) );
    }
      imageTable->sortItems(0,Qt::AscendingOrder);
    connect(imageTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
}

void MainWindow::updateAudioTable()
{

    audioSearchType SearchConstraints;
    audioTableData = db->SearchMedia(SearchConstraints);
    audioTable->clear();
    audioTable->sortItems(11,Qt::AscendingOrder);
    audioTable->setRowCount(audioTableData.id.size());
    audioTable->setColumnCount(12);
    audioTable->setColumnHidden(2,true);
    audioTable->setHorizontalHeaderLabels(QStringList() << "Название" << "Рейтинг" <<"" <<"Путь"<< "Дата создания"<< "Дата изменения"<< "Тип файла" <<"Группа"<<"Альбом"<<"Название" << "Продолжительность" <<"Размер");
    audioTable->setColumnWidth(0,500);
    audioTable->verticalHeader()->hide();
    audioTable->setSortingEnabled(true);
    QString min = " минут";
    QString mb = " Мб";
    disconnect(audioTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    for (int var = 0; var < audioTableData.id.size(); ++var) {
        audioTable->setItem(var , 0,new QTableWidgetItem(audioTableData.titles[var]) );
        audioTable->setItem(var , 1,new QTableWidgetItem(QString::number(audioTableData.rating[var])) );
        audioTable->setItem(var , 2,new QTableWidgetItem(QString::number(audioTableData.id[var]) ) );
        audioTable->setItem(var , 3,new QTableWidgetItem(audioTableData.paths[var]) );
        audioTable->setItem(var , 4,new QTableWidgetItem(audioTableData.datesCreated[var]) );
        audioTable->setItem(var , 5,new QTableWidgetItem(audioTableData.datesModified[var]) );
        audioTable->setItem(var , 6,new QTableWidgetItem(audioTableData.filetypes[var]) );
        audioTable->setItem(var , 7,new QTableWidgetItem(audioTableData.bands[var]) );
        audioTable->setItem(var , 8,new QTableWidgetItem(audioTableData.albums[var]) );
        audioTable->setItem(var , 9,new QTableWidgetItem(audioTableData.titles[var]) );
        audioTable->setItem(var , 10,new QTableWidgetItem(QString::number(audioTableData.durations[var].toDouble()/(1000*60))+ min)  );
        audioTable->setItem(var , 11,new QTableWidgetItem(QString::number(audioTableData.sizes[var].toDouble()/(1024*1024))+ mb) );

    }
    audioTable->sortItems(0,Qt::AscendingOrder);
    connect(audioTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
}
void MainWindow::videoToolBarPressed()
{
   // updateVideoTable();
    selectedTable = 1;
   // videoTable->sortItems(0,Qt::AscendingOrder);
    centralWidgetStack->setCurrentIndex(0);
}

void MainWindow::imageToolBarPressed()
{
   // updateImageTable();
    selectedTable = 2;
   // imageTable->sortItems(0,Qt::AscendingOrder);
    centralWidgetStack->setCurrentIndex(1);

}

void MainWindow::audioToolBarPressed()
{
    // updateImageTable();
    selectedTable = 3;
   // audioTable->sortItems(0,Qt::AscendingOrder);
    centralWidgetStack->setCurrentIndex(2);

}

void MainWindow::deleteButtonPressed()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this," ","Вы уверены что хотите удалите эти файлы?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;
    QList<QTableWidgetItem*>  selectedItems ;
    QSet<int> idItemsForDeletion;

    switch (selectedTable)
    {
    case 1:selectedItems = videoTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            idItemsForDeletion.insert((videoTable->item(var->row(),2)->text().toInt()));
        }
            db->deleteMedia(idItemsForDeletion,DatabaseController::VIDEO_TYPE);  break;
    case 2: selectedItems = imageTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            idItemsForDeletion.insert((imageTable->item(var->row(),2)->text().toInt()));
        }
            db->deleteMedia(idItemsForDeletion,DatabaseController::IMAGE_TYPE);  break;
    case 3: selectedItems = audioTable->selectedItems();
            for (QTableWidgetItem* var : selectedItems) {
            idItemsForDeletion.insert((audioTable->item(var->row(),2)->text().toInt()));
            }
            db->deleteMedia(idItemsForDeletion,DatabaseController::AUDIO_TYPE);  break;
    }
    updateCache();
}
void MainWindow::addTagButtonPressed()
{

    QList<QTableWidgetItem*>  selectedItems ;
    switch (selectedTable)
    {
    case 1:selectedItems = videoTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            db->AddTagsToMedia(videoTable->item(var->row(),2)->text().toInt(),DatabaseController::VIDEO_TYPE,tagsToAddToMedias);
        }; break;
    case 2: selectedItems = imageTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            db->AddTagsToMedia(imageTable->item(var->row(),2)->text().toInt(),DatabaseController::IMAGE_TYPE,tagsToAddToMedias);
        }; break;
    case 3: selectedItems = audioTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            db->AddTagsToMedia(audioTable->item(var->row(),2)->text().toInt(),DatabaseController::AUDIO_TYPE,tagsToAddToMedias);
        }; break;
    }
     updateTagComboBox();
}

void MainWindow::resetTagSelectionButtonPressed()
{
    updateTagComboBox();
}

void MainWindow::addFileButtonPressed()
{
    QFileDialog fd;
    QList<QUrl> urlList = fd.getOpenFileUrls();
    qDebug() << "dw";
    extractor->getMetaData(urlList);
}

void MainWindow::updateTagsCache()
{

    tagsCache = db->getTagTable();
    int cacheSize = tagsCache.id.size();
    tagMap.clear();
    for (int var = 0; var < cacheSize; ++var) {
        tagMap.insert(tagsCache.id[var] ,tagsCache.name[var]);
    }
}

void MainWindow::updateTagComboBox()
{
    tagComboBox->clear();
    for (QString var : tagMap){
        tagComboBox->addItem(var);
    }
    tagComboBox->setCurrentIndex(-1);
    tagsToAddToMedias.clear();
    tagSelection->setText("");

}

void MainWindow::updateCache()
{

    updateVideoTable();
    updateImageTable();
    updateAudioTable();
    updateTagsCache();
    updateTagComboBox();
}
void MainWindow::updateToolBarPressed()
{
    updateCache();
}
void MainWindow::selectTag(int index)
{
    int size = tagsToAddToMedias.count();
    tagsToAddToMedias.insert(tagMap.key(tagComboBox->itemText(index)));
    bool inserted = tagsToAddToMedias.count() > size;
    if(inserted ){
    tagSelection->setText(tagSelection->text().append(" " + tagComboBox->itemText(index)));
    }
}

void MainWindow::ratingCellChanged(int row, int column)
{
    DatabaseController *db = new DatabaseController(fullPath);
    switch (selectedTable)
    {
    case 1:db->SetMediaRating(videoTable->item(row,2)->text().toInt(), DatabaseController::VIDEO_TYPE,videoTable->item(row,1)->text().toInt());break;
    case 2:db->SetMediaRating(imageTable->item(row,2)->text().toInt(), DatabaseController::IMAGE_TYPE,imageTable->item(row,1)->text().toInt());break;
    case 3:db->SetMediaRating(audioTable->item(row,2)->text().toInt(), DatabaseController::AUDIO_TYPE,audioTable->item(row,1)->text().toInt());break;
    }
}

void MainWindow::videoThumbnailCatcher(const QVideoFrame &frame)
{
     qDebug() <<"wda";
    QImage aka = frame.toImage();
    aka.save("aaa","jpg",10);
}
MainWindow::~MainWindow()
{
    delete ui;


}


