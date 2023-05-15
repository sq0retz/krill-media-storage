/*!
\file
\brief Файл реализации главного окна

*/
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"FullDungeon", "krill", this);
    if (settings->value("logic/notFirstlaunch").toBool() != true)
    {
        qDebug() << "dwada";
        MainWindow::initializeSettings();
    }
    fullPath = settings->value("fileStorage/mainDatabasePath").toString() + "/KrillMainDb.db";
    db = new DatabaseController(fullPath);
    ui->setupUi(this);
    centralWidgetStack = new QStackedWidget;
    videoTable = new QTableWidget;
    imageTable = new QTableWidget;
    audioTable = new QTableWidget;
    //this->setWindowIcon(QIcon(":/assets/icons/assets/mainIcon"));
    this->setWindowTitle("Лучшая прога евер");
    centralWidgetStack->insertWidget(1, videoTable);
    centralWidgetStack->insertWidget(2, imageTable);
    centralWidgetStack->insertWidget(3, audioTable);
    this->setCentralWidget(centralWidgetStack);
    updateCache();
    constructTopToolBar();
    constructRightDock();
    constructBottomToolBar();
}


void MainWindow::initializeSettings()
{
    settings->setValue("logic/notFirstlaunch", false);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this," ","Хотите ли вы задать свой путь для хранения базы данных?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QFileDialog fd;
        settings->setValue("fileStorage/mainDatabasePath", fd.getExistingDirectory()  ); //добавить защиту от неверного пути и закрытия файлового диалога
    }
    else
    {


        settings->setValue("fileStorage/mainDatabasePath",  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) );
    }
    settings->setValue("logic/notFirstlaunch", true);

}

void MainWindow::constructTopToolBar()
{
    QAction *videoAction = new QAction(tr("&нажать..."), this);
    videoAction->setIcon(QIcon(":/assets/icons/assets/videoIcon"));
    videoAction->setShortcuts(QKeySequence::Open);
    videoAction->setStatusTip(tr("Открыть таблицу видео"));
    ui->topToolBar->addAction(videoAction);

    QAction *imageAction = new QAction(tr("&нажать..."), this);
    imageAction->setIcon(QIcon(":/assets/icons/assets/imageIcon"));
    imageAction->setShortcuts(QKeySequence::Open);
    imageAction->setStatusTip(tr("Открыть таблицу изображений"));
    ui->topToolBar->addAction(imageAction);

    QAction *audioAction = new QAction(tr("&нажать..."), this);
    audioAction->setIcon(QIcon(":/assets/icons/assets/audioIcon"));
    audioAction->setShortcuts(QKeySequence::Open);
    audioAction->setStatusTip(tr("Открыть таблицу аудио"));
    ui->topToolBar->addAction(audioAction);

    QAction *settingsAction = new QAction(tr("&настройки..."), this);
    settingsAction->setIcon(QIcon(":/assets/icons/assets/settingsIcon"));
    settingsAction->setShortcuts(QKeySequence::Open);
    settingsAction->setStatusTip(tr("Открыть настройки"));
    ui->topToolBar->addAction(settingsAction);

    QAction *updateAction = new QAction(tr("&Обновить..."), this);
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
    connect(settingsAction, &QAction::triggered, this,&MainWindow::settingsToolBarPressed);

}

void MainWindow::constructBottomToolBar()
{
   QAction *deleteAction = new QAction(tr("&Удалить..."), this);
   deleteAction->setIcon(QIcon(":/assets/icons/assets/deleteIcon"));
   deleteAction->setShortcuts(QKeySequence::Open);
   deleteAction->setStatusTip(tr("Удалить файлы"));
   ui->bottomToolBar->addAction(deleteAction);
   QAction *addFilesAction = new QAction(tr("&Добавить..."), this);
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
    QPushButton *addNewTagButton = new QPushButton;
    QPushButton *searchByTagsButton = new QPushButton;
    QPushButton *resetTagSearchButton = new QPushButton;
    QPushButton *deleteTagsFromDbButton = new QPushButton;
    QVBoxLayout *rightDockLayout = new QVBoxLayout;
    QHBoxLayout *comboBoxAddButtonlayout = new QHBoxLayout;
    QLabel * mainText = new QLabel;

    tagComboBox->setEditable(true);
    connect(tagComboBox, &QComboBox::currentIndexChanged,this, &MainWindow::selectTag);

    tagSelection->setAlignment(Qt::AlignCenter);
    tagSelection->setWordWrap(true);

    mainText->setText("Теги");
    mainText->setAlignment(Qt::AlignTop);
    mainText->setFixedHeight(15);

    addTagButton->setText("Добавить теги к медиа");
    connect(addTagButton, &QPushButton::pressed, this, &MainWindow::addTagButtonPressed);

    resetTagsSelectionButton->setText("Очистить выборку");
    connect(resetTagsSelectionButton, &QPushButton::pressed, this, &MainWindow::resetTagSelectionButtonPressed);

    searchByTagsButton->setText("Поиск по тегам");
    connect(searchByTagsButton, &QPushButton::pressed,this, &MainWindow::searchByTagsButtonPressed);

    resetTagSearchButton->setText("Сброс поиска");
    connect(resetTagSearchButton, &QPushButton::pressed,this, &MainWindow::resetTagSearchButtonPressed);

    deleteTagsFromDbButton->setText("Удалить теги из БД");
    connect(deleteTagsFromDbButton, &QPushButton::pressed,this, &MainWindow::deleteTagsFromBdSlot);


    addNewTagButton->setText("+");
    addNewTagButton->setFixedHeight(25);
    addNewTagButton->setFixedWidth(25);
    connect(addNewTagButton, &QPushButton::pressed,this, &MainWindow::addNewTagButtonPressed);


    comboBoxAddButtonlayout->addWidget(tagComboBox);
    comboBoxAddButtonlayout->addWidget(addNewTagButton);

    rightDockLayout->addWidget(mainText);
    rightDockLayout->addLayout(comboBoxAddButtonlayout);
    rightDockLayout->addWidget(tagSelection);
    rightDockLayout->addWidget(addTagButton);
    rightDockLayout->addWidget(searchByTagsButton);
    rightDockLayout->addWidget(resetTagSearchButton);
    rightDockLayout->addWidget(resetTagsSelectionButton);
    rightDockLayout->addWidget(deleteTagsFromDbButton);

    rightDockWidget->setLayout(rightDockLayout);
    rightDock->setWidget(rightDockWidget);

    this->addDockWidget(Qt::RightDockWidgetArea,rightDock);
}


void MainWindow::addNewTagButtonPressed()
{
    QDialog *tagAddDialog = new QDialog(this);
    QLabel *labelTagAdd = new QLabel;
    QLineEdit *tagLine = new QLineEdit;
    QPushButton *addNewTagButton = new QPushButton;
    QPushButton *closeButton = new QPushButton;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    labelTagAdd->setText("Добавление новых тегов в базу даных");

    buttonsLayout->addWidget(addNewTagButton);
    buttonsLayout->addWidget(closeButton);

    mainLayout->addWidget(labelTagAdd);
    mainLayout->addWidget(tagLine);
    mainLayout->addLayout(buttonsLayout);

    tagAddDialog->setLayout(mainLayout);
    tagAddDialog->setModal(true);
    tagAddDialog->show();

    addNewTagButton->setText("Добавить");
    connect(addNewTagButton, &QPushButton::pressed,[=]() {
        db->InsertTag(tagLine->text().remove(" "));
        tagLine->clear();
    });

    closeButton->setText("Выйти");
    connect(closeButton, &QPushButton::pressed,[=]() {
        updateTagsCache();
        updateTagComboBox();
        tagAddDialog->close();
    });


}

void MainWindow::updateVideoTable(videoDataType videoTableData)
{

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
    videoTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //большая нагрузка на систему
    QString min = " минут";
    QString gb = " Гб";
    disconnect(videoTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    for (int var = 0; var < videoTableData.id.size(); ++var) {
        videoTable->setItem(var , 0,new QTableWidgetItem(videoTableData.titles[var]) );
        videoTable->setItem(var , 1,new QTableWidgetItem(QString::number(videoTableData.rating[var])) );
        videoTable->setItem(var , 2,new QTableWidgetItem(QString::number(videoTableData.id[var])  ));
        videoTable->setItem(var , 3,new QTableWidgetItem(videoTableData.paths[var].remove(0,1)) );
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
    connect(videoTable, &QTableWidget::cellDoubleClicked,this, &MainWindow::tableCellClicked);
}

void MainWindow::updateImageTable(imageDataType imageTableData)
{
    imageTable->clear();
    imageTable->sortItems(9,Qt::AscendingOrder);
    imageTable->setRowCount(imageTableData.id.size());
    imageTable->setColumnCount(10);
    imageTable->setColumnHidden(2,true);
    imageTable->setHorizontalHeaderLabels(QStringList() << "Название" << "Рейтинг" <<"" <<"Путь"<< "Ширина"<< "Высота"<< "Дата создания"<< "Дата изменения"<< "Тип файла" <<"Размер");
    imageTable->setColumnWidth(0,500);
    imageTable->verticalHeader()->hide();
    imageTable->setSortingEnabled(true);
    imageTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //большая нагрузка на систему
    QString mb = " Мб";
    disconnect(imageTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    for (int var = 0; var < imageTableData.id.size(); ++var) {
        imageTable->setItem(var , 0,new QTableWidgetItem(imageTableData.titles[var]) );
        imageTable->setItem(var , 1,new QTableWidgetItem(QString::number(imageTableData.rating[var])) );
        imageTable->setItem(var , 2,new QTableWidgetItem(QString::number(imageTableData.id[var]) ) );
        imageTable->setItem(var , 3,new QTableWidgetItem(imageTableData.paths[var].remove(0,1)) );
        imageTable->setItem(var , 4,new QTableWidgetItem(imageTableData.widths[var]) );
        imageTable->setItem(var , 5,new QTableWidgetItem(imageTableData.heights[var]) );
        imageTable->setItem(var , 6,new QTableWidgetItem(imageTableData.datesCreated[var]) );
        imageTable->setItem(var , 7,new QTableWidgetItem(imageTableData.datesModified[var]) );
        imageTable->setItem(var , 8,new QTableWidgetItem(imageTableData.filetypes[var]) );
        imageTable->setItem(var , 9,new QTableWidgetItem(QString::number(imageTableData.sizes[var].toDouble()/(1024*1024))+ mb) );
    }
      imageTable->sortItems(0,Qt::AscendingOrder);
    connect(imageTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    connect(imageTable, &QTableWidget::cellDoubleClicked,this, &MainWindow::tableCellClicked);
}

void MainWindow::updateAudioTable(audioDataType audioTableData)
{
    audioTable->clear();
    audioTable->sortItems(12,Qt::AscendingOrder);
    audioTable->setRowCount(audioTableData.id.size());
    audioTable->setColumnCount(13);
    audioTable->setColumnHidden(2,true);
    audioTable->setHorizontalHeaderLabels(QStringList() << "Название" << "Рейтинг" <<"" <<"Путь"<< "Дата создания"<< "Дата изменения"<< "Битрейт"<< "Тип файла" <<"Группа"<<"Альбом"<<"Название" << "Продолжительность" <<"Размер");
    audioTable->setColumnWidth(0,500);
    audioTable->verticalHeader()->hide();
    audioTable->setSortingEnabled(true);
    audioTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //большая нагрузка на систему
    audioTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // все же лучше задать вручную размер колонн
    QString min = " минут";
    QString mb = " Мб";
    disconnect(audioTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    for (int var = 0; var < audioTableData.id.size(); ++var) {
        audioTable->setItem(var , 0,new QTableWidgetItem(audioTableData.titles[var]) );
        audioTable->setItem(var , 1,new QTableWidgetItem(QString::number(audioTableData.rating[var])) );
        audioTable->setItem(var , 2,new QTableWidgetItem(QString::number(audioTableData.id[var]) ) );
        audioTable->setItem(var , 3,new QTableWidgetItem(audioTableData.paths[var].remove(0,1)) );
        audioTable->setItem(var , 4,new QTableWidgetItem(audioTableData.datesCreated[var]) );
        audioTable->setItem(var , 5,new QTableWidgetItem(audioTableData.datesModified[var]) );
        audioTable->setItem(var , 6,new QTableWidgetItem(audioTableData.bitRates[var]) );
        audioTable->setItem(var , 7,new QTableWidgetItem(audioTableData.filetypes[var]) );
        audioTable->setItem(var , 8,new QTableWidgetItem(audioTableData.bands[var]) );
        audioTable->setItem(var , 9,new QTableWidgetItem(audioTableData.albums[var]) );
        audioTable->setItem(var , 10,new QTableWidgetItem(audioTableData.titles[var]) );
        audioTable->setItem(var , 11,new QTableWidgetItem(QString::number(audioTableData.durations[var].toDouble()/(1000*60))+ min)  );
        audioTable->setItem(var , 12,new QTableWidgetItem(QString::number(audioTableData.sizes[var].toDouble()/(1024*1024))+ mb) );

    }
    audioTable->sortItems(0,Qt::AscendingOrder);
    connect(audioTable,&QTableWidget::cellChanged,this, &MainWindow::ratingCellChanged );
    connect(audioTable, &QTableWidget::cellDoubleClicked,this, &MainWindow::tableCellClicked);
}
void MainWindow::videoToolBarPressed()
{
    selectedTable = 1;
    centralWidgetStack->setCurrentIndex(0);
}

void MainWindow::imageToolBarPressed()
{
    selectedTable = 2;
    centralWidgetStack->setCurrentIndex(1);

}

void MainWindow::audioToolBarPressed()
{
    selectedTable = 3;
    centralWidgetStack->setCurrentIndex(2);

}

void MainWindow::settingsToolBarPressed()
{
    constructSettingsDialog();

}

void MainWindow::constructSettingsDialog()
{
    QDialog *settingsDialog = new QDialog;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *databasePathLayout = new QHBoxLayout;
    QLabel *header = new QLabel;
    QLabel *databasePathLabel = new QLabel;
    QTextEdit *databasePathTextEdit = new QTextEdit;

    header->setText("Настройки");
    header->setAlignment(Qt::AlignVCenter);
    databasePathLabel->setText("Путь до БД");
    databasePathTextEdit->setText(settings->value("fileStorage/mainDatabasePath").toString());

    settingsDialog->setLayout(mainLayout);
    mainLayout->addWidget(header);
    mainLayout->addLayout(databasePathLayout);

    databasePathLayout->addWidget(databasePathLabel);
    databasePathLayout->addWidget(databasePathTextEdit);

    settingsDialog->show();


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
            videoTable->removeRow(var->row());
        }
            db->deleteMedia(idItemsForDeletion,DatabaseController::VIDEO_TYPE);  break;
    case 2: selectedItems = imageTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            idItemsForDeletion.insert((imageTable->item(var->row(),2)->text().toInt()));
            imageTable->removeRow(var->row());
        }
            db->deleteMedia(idItemsForDeletion,DatabaseController::IMAGE_TYPE);  break;
    case 3: selectedItems = audioTable->selectedItems();
            for (QTableWidgetItem* var : selectedItems) {
            idItemsForDeletion.insert((audioTable->item(var->row(),2)->text().toInt()));
            audioTable->removeRow(var->row());
            }
            db->deleteMedia(idItemsForDeletion,DatabaseController::AUDIO_TYPE);  break;
    }

}
void MainWindow::addTagButtonPressed()
{

    QList<QTableWidgetItem*>  selectedItems ;
    switch (selectedTable)
    {
    case 1:selectedItems = videoTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            db->AddTagsToMedia(videoTable->item(var->row(),2)->text().toInt(),DatabaseController::VIDEO_TYPE,chosenTagsSet);
        }; break;
    case 2: selectedItems = imageTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            db->AddTagsToMedia(imageTable->item(var->row(),2)->text().toInt(),DatabaseController::IMAGE_TYPE,chosenTagsSet);
        }; break;
    case 3: selectedItems = audioTable->selectedItems();
        for (QTableWidgetItem* var : selectedItems) {
            db->AddTagsToMedia(audioTable->item(var->row(),2)->text().toInt(),DatabaseController::AUDIO_TYPE,chosenTagsSet);
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

    metaDataExtractor *extractor = new metaDataExtractor(fullPath, *db,tagMap,selectedTable);
    QFileDialog fd;
    QList<QUrl> urlList = fd.getOpenFileUrls();
    if(urlList.empty()){
        QMessageBox::warning(this,"Предупреждение", "Вы не выбрали файлы, вы что клоун?");
    }
    else{
    switch (selectedTable)
    {
    case 1: extractor->getVideoMetaData(urlList );break;
    case 2: extractor->getImageMetaData(urlList );break;
    case 3: extractor->getAudioMetaData(urlList );break;
    default: break;
    }
    }


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
    chosenTagsSet.clear();
    tagSelection->setText("");

}

videoDataType MainWindow::getVideoTableData()
{
    videoDataType videoTableData;
    videoSearchType SearchConstraints;
    return videoTableData = db->SearchMedia(SearchConstraints);
}
imageDataType MainWindow::getImageTableData()
{
    imageDataType imageTableData;
    imageSearchType SearchConstraints;
    return imageTableData = db->SearchMedia(SearchConstraints);
}
audioDataType MainWindow::getAudioTableData()
{
    audioDataType audioTableData;
    audioSearchType SearchConstraints;
    return audioTableData = db->SearchMedia(SearchConstraints);
}
videoDataType MainWindow::getVideoTableDataByTags()
{
    videoDataType videoTableData;
    QList<int> listOfTags;
    foreach (const int &value, chosenTagsSet)
        listOfTags.append(value);
    return videoTableData = db->SearchVideoTags(listOfTags);
}
imageDataType MainWindow::getImageTableDataByTags()
{
    imageDataType imageTableData;
    QList<int> listOfTags;
    foreach (const int &value, chosenTagsSet)
        listOfTags.append(value);
    return imageTableData = db->SearchImageTags(listOfTags);
}

audioDataType MainWindow::getAudioTableDataByTags()
{
    audioDataType audioTableData;
    QList<int> listOfTags;
    foreach (const int &value, chosenTagsSet)
        listOfTags.append(value);
    return audioTableData = db->SearchAudioTags(listOfTags);
}

void MainWindow::updateCache()
{
    updateVideoTable(getVideoTableData());
    updateImageTable(getImageTableData());
    updateAudioTable(getAudioTableData());
    updateTagsCache();
    updateTagComboBox();


}
void MainWindow::updateToolBarPressed()
{
    updateCache();
}
void MainWindow::selectTag(int index)
{
    int size = chosenTagsSet.count();
    chosenTagsSet.insert(tagMap.key(tagComboBox->itemText(index)));
    bool inserted = chosenTagsSet.count() > size;
    if(inserted ){
    tagSelection->setText(tagSelection->text().append(" " + tagComboBox->itemText(index)));
    }
     //qDebug()<< chosenTagsSet;
    //qDebug() << tagSelection->text();
}

void MainWindow::ratingCellChanged(int row, int column)
{
    if (column == 1)
    switch (selectedTable)
    {
    case 1:db->SetMediaRating(videoTable->item(row,2)->text().toInt(), DatabaseController::VIDEO_TYPE,videoTable->item(row,1)->text().toInt());break;
    case 2:db->SetMediaRating(imageTable->item(row,2)->text().toInt(), DatabaseController::IMAGE_TYPE,imageTable->item(row,1)->text().toInt());break;
    case 3:db->SetMediaRating(audioTable->item(row,2)->text().toInt(), DatabaseController::AUDIO_TYPE,audioTable->item(row,1)->text().toInt());break;
    }
}

void MainWindow::tableCellClicked(int row, int column)
{
    if (column != 1)
    switch (selectedTable)
    {
    case 1:QDesktopServices::openUrl(QUrl::fromLocalFile(videoTable->item(row,3)->text()));break;
    case 2:QDesktopServices::openUrl(QUrl::fromLocalFile(imageTable->item(row,3)->text()));break;
    case 3:QDesktopServices::openUrl(QUrl::fromLocalFile(audioTable->item(row,3)->text()));break;
    }

}

void MainWindow::searchByTagsButtonPressed()
{
    switch (selectedTable)
    {
    case 1:updateVideoTable(getVideoTableDataByTags());break;
    case 2:updateImageTable(getImageTableDataByTags());break;
    case 3:updateAudioTable(getAudioTableDataByTags());break;
    }
}

void MainWindow::resetTagSearchButtonPressed()
{

    updateSelectedTable();
}
void MainWindow::updateSelectedTable()
{
    switch (selectedTable)
    {
    case 1:updateVideoTable(getVideoTableData());break;
    case 2:updateImageTable(getImageTableData());break;
    case 3:updateAudioTable(getAudioTableData());break;
    }
}

void MainWindow::deleteTagsFromBdSlot()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this," ","Вы уверены что хотите удалите эти теги(вместе с тегами удалятся и связи файлов с ними)?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        db->deleteTags(chosenTagsSet);
        updateTagsCache();
        updateTagComboBox();
    }
}
MainWindow::~MainWindow()
{
    delete ui;


}


