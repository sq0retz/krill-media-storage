/*!
\file
\brief Заголовочный файл извлекателя метаданных

Данный файл содержит в себе определение методов и аттрибутов
извлекателя метаданных, а также методы для добавления к файлам тегов и
их последующей загрузки в базу данных
*/
#ifndef METADATAEXTRACTOR_H
#define METADATAEXTRACTOR_H

#include "QUrl"
#include <MediaInfoDLL.h>
#include "databasecontroller.h"
#include <qSet>
#include <QtWidgets>
/*!
    \brief Класс, отвечающий за извлечение метаданных
    \todo Разделить методы, возможно даже на классы

    Данный класс обеспечивает извлечение метаданных и добавление их
    в базу данных
*/
class metaDataExtractor : public QObject
{
      Q_OBJECT
public:
    /*!
    Конструктор объекта
    \param[in] path путь к базе данных
    \param[in] db указатель на объект контроллера базы данных
    \param[in] tagMap таблица всех тегов
    \param[in] selectedTable тип коллекции
    */
    metaDataExtractor(QString &path, DatabaseController &db,QMap<int ,QString> tagMap,int selectedTable);
    /*!
    Деконструктор объекта
    */
    ~metaDataExtractor();
    /*!
    Получение метаданных видео
    \param[in] urlList список путей до видеофайлов
    */
    void getVideoMetaData(QList<QUrl> urlList );
    /*!
    Получение метаданных изображений
    \param[in] urlList список путей до файлов изображений
    */
    void getImageMetaData(QList<QUrl> urlList );
    /*!
    Получение метаданных аудио
    \param[in] urlList список путей до аудиофайлов
    */
    void getAudioMetaData(QList<QUrl> urlList );
private:
    /*!
    Конструирование диалога добавления тегов
    */
    void constructTagSelectionDialog();
    /*!
    Получение данных видео из базы данных
    \param[in] data данные видео
    */
    void itemLoader(videoDataType data);
    /*!
    Получение данных изображений из базы данных
    \param[in] data данные изображений
    */
    void itemLoader(imageDataType data);
    /*!
    Получение данных аудио из базы данных
    \param[in] data данные аудио
    */
    void itemLoader(audioDataType data);
    /*!
    Конструирование ячейки выбора тегов
    \return лейаут
    */
    QVBoxLayout* constructTagBar();
    QComboBox  *tagComboBox = new QComboBox;///< указатель на комбобокс тегов
    QLabel *tagSelection = new QLabel; ///< указатель на поле текста с выбранными тегами
    std::unique_ptr<MediaInfoDLL::MediaInfo> mi = std::make_unique<MediaInfoDLL::MediaInfo>();

    //MediaInfoDLL::MediaInfo *mi = new MediaInfoDLL::MediaInfo; ///< указатель на обьект mediainfo

    DatabaseController *daba ; ///< указатель на контроллер баз даных
    QString fullPath;  ///< путь до базы данных
    QSet<QString> videoTypes = {"Matroska","FLV","VOB","AVI","QuickTime", "MPEG-4","MXF","NSV","WEBM"}; ///< набор поддерживаемых видов видео
    QSet<QString> imageTypes = {"PNG","JPEG","GIF","RAW"};///< набор поддерживаемых видов изображений
    QSet<QString> audioTypes = {"FLAC","MPEG Audio"};///< набор поддерживаемых видов аудио
    QSet<int> tagsToAddToMedias; ///< список тегов которые предстоит добавить к медиафайлам
    int selectedTableLocal = 1; ///< для какой таблицы меди осуществляется добавление
    QMap<int ,QString> tagMapLocal; ///< пары айди тега и его имени
    QTableWidget *mediasList = new QTableWidget; ///< таблица с добавленными медиафайлами
    QDialog *selectionDialog = new QDialog; ///< диалог выбора тегов
private slots:
    /*!
    Слот нажатия на кнопку добавление тега
    */
    void addTagButtonPressed();
    /*!
    Слот нажатия на кнопку очистки набора выбранных тегов
    */
    void resetTagSelectionButtonPressed();
    /*!
    Слот нажатия на тег из комбобокса
    \param[in] index тега в комбобоксе
    */
    void selectTag(int index);
    /*!
    Слот обновления комбокса
    */
    void updateTagComboBox();
    /*!
    Слот закрытия диалога
    */
    void closeButtonPressed();
};

#endif // METADATAEXTRACTOR_H
