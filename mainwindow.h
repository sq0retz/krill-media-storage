
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <databasecontroller.h>
#include <metadataextractor.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QTableWidget *videoTable;
    QTableWidget *imageTable;
    QTableWidget *audioTable;
    QStackedWidget *centralWidgetStack;
    QDockWidget *rightDock = new QDockWidget;
    QComboBox *tagComboBox = new QComboBox;
    QLabel *tagSelection = new QLabel;
    QMap<int ,QString> tagMap;
    QString fullPath = "C:/temp/maindb.db";
    videoDataType videoTableData;
    imageDataType imageTableData;
    audioDataType audioTableData;
    tagData tagsCache;
    DatabaseController *db = new DatabaseController(fullPath);

    QSet<int> tagsToAddToMedias;
    int selectedTable = 1;
    void constructTopToolBar();
    void constructBottomToolBar();
    void constructRightDock();
    void updateVideoTable();
    void updateImageTable();
    void updateAudioTable();
    void updateTagsCache();
    void updateTagComboBox();
    void updateCache();
private slots:
    void videoToolBarPressed();
    void imageToolBarPressed();
    void audioToolBarPressed();
    void updateToolBarPressed();
    void deleteButtonPressed();
    void addTagButtonPressed();
    void addFileButtonPressed();
    void resetTagSelectionButtonPressed();
    void ratingCellChanged(int row, int column);
    void selectTag(int index);

};

#endif // MAINWINDOW_H
