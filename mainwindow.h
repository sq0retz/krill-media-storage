/*!
\file
\brief Заголовочный файл главного окна

Данный файл содержит в себе определение методов и аттрибутов
главного окна
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <databasecontroller.h>
#include <metadataextractor.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/*!
    \brief Класс, отвечающий за основное окно вывода
    \todo Вывод файлов по тегам, вывод коллекций, добавление коллекций

    Данный класс обеспечивате работу главного окна
*/
class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    /*!
    Конструктор главного окна
    \param[in] parent указатель на родителя
    */
    MainWindow(QWidget *parent = nullptr);
    /*!
    Деконструктор главного окна
    */
    ~MainWindow();

private:

    Ui::MainWindow *ui; ///< указатель на объект разметки
    QTableWidget *videoTable; ///< указатель на таблицу видео
    QTableWidget *imageTable; ///< указатель на таблицу изображений
    QTableWidget *audioTable; ///< указатель на таблицу аудио
    QStackedWidget *centralWidgetStack; ///< указатель на стек главных виджетов
    QDockWidget *rightDock = new QDockWidget; ///< указатель на правый док
    QComboBox *tagComboBox = new QComboBox; ///< указатель на обьект выбора тегов
    QLabel *tagSelection = new QLabel; ///< указатель на вывод выбранных для добавления тегов
    QMap<int ,QString> tagMap; ///< пары айди тега и его имени
    QString fullPath = "C:/temp/maindb.db"; ///< путь до базы данных 
    imageDataType imageTableData; ///< структура для хранения данных изображений из базы данных
    audioDataType audioTableData; ///< структура для хранения данных аудио из базы данных
    tagData tagsCache;  ///< структура для хранения тегов из базы данных
    DatabaseController *db = new DatabaseController(fullPath); ///< указатель на контроллер базы данных
    QSet<int> chosenTagsSet; ///< набор тегов для добавления к медиа
    int selectedTable = 1;   ///< номер выбранной сейчас таблицы
    /*!
    Конструирует верхнюю панель инструментов
    */
    void constructTopToolBar();
    /*!
    Конструирует нижнюю панель инструментов
    */
    void constructBottomToolBar();
    /*!
    Конструирует правый док
    */
    void constructRightDock();
    /*!
    Обновляет таблицу видео
    */
    void updateVideoTable(videoDataType videoTableData);
    /*!
    Обновляет таблицу изображений
    */
    void updateImageTable(imageDataType imageTableData);
    /*!
    Обновляет таблицу аудио
    */
    void updateAudioTable(audioDataType audioTableData);
    /*!
    Обновляет кеш тегов
    */
    void updateTagsCache();
    /*!
    Обновляет комбокс тегов
    */
    void updateTagComboBox();
    /*!
    Обновляет весь кеш
    */
    void updateCache();
    videoDataType getVideoTableData();
    videoDataType getVideoTableDataByTags();
    imageDataType getImageTableData();
    imageDataType getImageTableDataByTags();
    audioDataType getAudioTableData();
    audioDataType getAudioTableDataByTags();
private slots:

    void videoToolBarPressed();
    void imageToolBarPressed();
    void audioToolBarPressed();
    void updateToolBarPressed();
    void deleteButtonPressed();
    void addTagButtonPressed();
    void addFileButtonPressed();
    void resetTagSearchButtonPressed();
    void searchByTagsButtonPressed();
    void tableCellClicked(int row, int column);
    /*!
    Нажатие кнопки добавления нового тега
    */
    void addNewTagButtonPressed();
    void resetTagSelectionButtonPressed();
    /*!
    Слот изменения ячейки с оценкой
    \param[in] row номер ряда
    \param[in] column номер колонны
    */
    void ratingCellChanged(int row, int column);
    /*!
    Слот нажатия на тег из комбобокса
    \param[in] index тега в комбобоксе
    */
    void selectTag(int index);

};

#endif // MAINWINDOW_H
