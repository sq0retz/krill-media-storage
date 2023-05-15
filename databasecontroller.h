/*!
\file
\brief Заголовочный файл контроллера базы данных

Данный файл содержит в себе определение методов и аттрибутов
контроллера базы данных, а также структуры данных необходимые для его функционирования
*/

#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#include <sqlite3.h>
#include <iostream>
#include <QString>
#include <QFile>

/// Структура для передачи данных наборов видео файлов
struct videoDataType{
    QList<int> id;  ///< список id
    QList<QString> titles; ///< список названий
    QList<QString> paths; ///< список путей
    QList<QString> widths; ///< список ширины в пикселях
    QList<QString> heights; ///< список высоты в пикселях
    QList<int>     rating; ///< список рейтингов
    QList<QString> datesCreated; ///< список дат создания
    QList<QString> datesModified; ///< список дат модификации
    QList<QString> filetypes; ///< список типов файлов
    QList<QString> codecs; ///< список кодеков
    QList<QString> durations; ///< список продолжительностей
    QList<QString> sizes; ///< список размеров
};

/// Структура для передачи данных наборов файлов изображений
struct imageDataType{
    QList<int> id; ///< список id
    QList<QString> titles; ///< список названий
    QList<QString> paths; ///< список путей
    QList<QString> widths; ///< список ширины в пикселях
    QList<QString> heights; ///< список высоты в пикселях
    QList<int>     rating; ///< список рейтингов
    QList<QString> datesCreated; ///< список дат создания
    QList<QString> datesModified; ///< список дат модификации
    QList<QString> filetypes; ///< список типов файлов
    QList<QString> sizes; ///< список размеров
};

/// Структура для передачи данных наборов аудиофайлов
struct audioDataType{
    QList<int> id; ///< список id
    QList<QString> titles; ///< список названий
    QList<QString> paths; ///< список путей
    QList<int>     rating; ///< список рейтингов
    QList<QString> datesCreated; ///< список дат создания
    QList<QString> datesModified; ///< список дат модификации
    QList<QString> bitRates; ///< список битрейтов
    QList<QString> filetypes; ///< список типов файлов
    QList<QString> albums; ///< список альбомов
    QList<QString> bands; ///< список исполнителей
    QList<QString> titlesEncoded; ///< список закодированных названий
    QList<QString> durations; ///< список продолжительностей
    QList<QString> sizes; ///< список размеров
};

/// Структура для поисковых запросов видеофайлов
struct videoSearchType{ //структура для упращения передачи данных в контроллер НЕ ДОПИСАНО
    enum searchConditions {
        TITLE_MASK_EQUAL             = 0x01,
        TITLE_MASK_NOT_EQUAL         = 0x02,
        DATE_DAY_EQUAL               = 0x04,
        DATE_DAY_BETWEEN             = 0x08
    };
    int bitSearchConditions;

    QString titleMask;   //принимает маску
    QList<QString> paths;
    QList<QString> resolutions;
    QString dateFirst;
    QString dateSecond;
    QList<QString> filetypes;
    QList<QString> durations;
    QList<QString> sizes;

    QList<int> tags;   // для поиска по тегам добавляйте айди тегов
    QList<int> collections;   // для поиска по тегам добавляйте айди тегов
};

/// Структура для поисковых запросов изображений
struct imageSearchType{ //структура для упращения передачи данных в контроллер НЕ ДОПИСАНО
    enum searchConditions {
        TITLE_MASK_EQUAL             = 0x01,
        TITLE_MASK_NOT_EQUAL         = 0x02,
        DATE_DAY_EQUAL               = 0x04,
        DATE_DAY_BETWEEN             = 0x08
    };
    int bitSearchConditions;

    QString titleMask;   //принимает маску
    QList<QString> paths;
    QList<QString> resolutions;
    QString dateFirst;
    QString dateSecond;
    QList<QString> filetypes;
    QList<QString> durations;
    QList<QString> sizes;

    QList<int> tags;   // для поиска по тегам добавляйте айди тегов
    QList<int> collections;   // для поиска по тегам добавляйте айди тегов
};

/// Структура для поисковых запросов аудиофайлов
struct audioSearchType{ //структура для упращения передачи данных в контроллер НЕ ДОПИСАНО
    enum searchConditions {
        TITLE_MASK_EQUAL             = 0x01,
        TITLE_MASK_NOT_EQUAL         = 0x02,
        DATE_DAY_EQUAL               = 0x04,
        DATE_DAY_BETWEEN             = 0x08
    };
    int bitSearchConditions;

    QString titleMask;   //принимает маску
    QList<QString> paths;
    QList<QString> resolutions;
    QString dateFirst;
    QString dateSecond;
    QList<QString> filetypes;
    QList<QString> durations;
    QList<QString> sizes;

    QList<int> tags;   // для поиска по тегам добавляйте айди тегов
    QList<int> collections;
};

/// Структура для передачи коллекций
struct collectionData
{
    QList<int> id;
    QList<QString> name;
    QList<QString> type;

};

/// Структура для передача тегов
struct tagData
{
    QList<int> id;
    QList<QString> name;

};

/*!
    \brief Класс, отвечающий за все взаимодействия с базой данных
    \todo Методы удаления тегов и коллекций

    Данный класс обеспечивает создание или открытие базы данных sqlite,
    а также различные операции по добавлению в нее и извлечению из нее содержимого
*/
class DatabaseController
{
public:
    /// Набор видов медиафайлов
    enum mediaType{
        VIDEO_TYPE = 1, ///< Указывает, что элемент это видео
        AUDIO_TYPE = 2, ///< Указывает, что элемент это аудио
        IMAGE_TYPE = 3, ///< Указывает, что элемент это изображение
        GENERAL_TYPE = 4 ///< Указывает, что элемент не имеет типа
    };
    /*!
    Конструктор контроллера базы данных
    \param[in] fullPath Путь до базы данных
    */
    DatabaseController (QString &fullPath);
    /*!
    Деконструктор контроллера базы данных
    */
    ~DatabaseController();
    /*!
    Добавляет видеофайлы в базу данных
    \param[in] data массив данных добавляемых видеофайлов
    */
    void InsertMedia (videoDataType &data);
    /*!
    Добавляет файлы изображений в базу данных
    \param[in] data массив данных добавляемых файлов изображений
    */
    void InsertMedia (imageDataType &data);
    /*!
    Добавляет аудиофайлы в базу данных
    \param[in] data массив данных добавляемых аудиофайлов
    */
    void InsertMedia (audioDataType &data);
    /*!
    Добавляет тег в базу данных
    \param[in] tagName наименование тега
    \return Была ли успешна операция
    */
    bool InsertTag (QString tagName);
    /*!
    Добавляет коллекцию в базу данных
    \param[in] collectionName наименование коллекции
    \return Была ли успешна операция
    */
    bool InsertCollection (QString collectionName);
    /*!
    Добавляет коллекцию в базу данных, а также ее тип
    \param[in] collectionName наименование коллекции
    \param[in] type тип коллекции
    \return Была ли успешна операция
    */
    bool InsertCollection (QString collectionName, mediaType type);
    /*!
    Добавляет набор тегов к медиафайлу
    \param[in] mediaId id медиафайла
    \param[in] type тип медиафайла
    \param[in] tags набор тегов
    \return Была ли успешна операция
    */
    bool AddTagsToMedia   (int mediaId, mediaType type,  QSet<int> tags);
    /*!
    Добавляет медиафайлы в коллекцию
    \param[in] mediaIds id медиафайлов
    \param[in] type тип медиафайлов
    \param[in] CollectionId id коллекции
    \return Была ли успешна операция
    */
    bool AddMediaToCollection (QList<int> mediaIds, mediaType type, int CollectionId);
    /*!
    Изменяет рейтинг медиафайла
    \param[in] mediaId id медиафайла
    \param[in] type тип медиафайла
    \param[in] rating значение рейтинга
    \return Была ли успешна операция
    */
    bool SetMediaRating (int mediaId, mediaType type, int rating);
    /*!
    Удаляет медиафайлы
    \param[in] mediaId id медиафайлов
    \param[in] type тип медиафайлов
    \return Была ли успешна операция
    */
    bool deleteMedia(QSet<int> id, mediaType type);
    bool deleteTags(QSet<int> idTags);
    bool deleteTagsFromMedia(int mediaId, mediaType type,  QSet<int> tags);
    /*!
    Извлекает всю таблицу тегов
    \return таблица тегов
    */
    tagData getTagTable ();
    /*!
    Извлекает всю таблицу коллекций
    \return таблица коллекций
    */
    collectionData getCollectionTable();
    /*!
    Извлекает видео по их путям
    \param[in] paths пути к видеофайлам
    \return таблица видео
    */
    videoDataType  getVideoByPath(QList<QString> paths);
    /*!
    Извлекает изображения по их путям
    \param[in] paths пути к файлам изображений
    \return таблица изображений
    */
    imageDataType  getImageByPath(QList<QString> paths);
    /*!
    Извлекает аудио по их путям
    \param[in] paths пути к аудиофайлам
    \return таблица аудио
    */
    audioDataType  getAudioByPath(QList<QString> paths);
    /*!
    Извлекает видео из коллекции
    \param[in] collectionId id коллекции
    \return таблица видео
    */
    videoDataType  getVideoByCollection(int collectionId );
    /*!
    Извлекает изображения из коллекции
    \param[in] collectionId id коллекции
    \return таблица изображений
    */
    imageDataType  getImageByCollection(int collectionId );
    /*!
    Извлекает аудио из коллекции
    \param[in] collectionId id коллекции
    \return таблица аудио
    */
    audioDataType  getAudioByCollection(int collectionId );
    /*!
    Извлекает всю таблицу видео
    \param[in] searchData набор флагов и значений для поиска
    \return таблица видео
    \todo реализовать полноценный поиск
    */
    videoDataType SearchMedia     (videoSearchType &searchData);  // НА ДАННОМ ЭТАПЕ ВОЗВРАЩАЕТ ПРОСТО СПИСОК ВСЕХ ВИДЕО
    /*!
    Извлекает всю таблицу изображений
    \param[in] searchData набор флагов и значений для поиска
    \return таблица изображений
    \todo реализовать полноценный поиск
    */
    imageDataType SearchMedia     (imageSearchType &searchData);  // НА ДАННОМ ЭТАПЕ ВОЗВРАЩАЕТ ПРОСТО СПИСОК ВСЕХ картинок
    /*!
    Извлекает всю таблицу аудио
    \param[in] searchData набор флагов и значений для поиска
    \return таблица аудио
    \todo реализовать полноценный поиск
    */
    audioDataType SearchMedia     (audioSearchType &searchData);  // НА ДАННОМ ЭТАПЕ ВОЗВРАЩАЕТ ПРОСТО СПИСОК ВСЕХ аудио
    /*!
    Извлекает видео по соответствию набору тегов
    \param[in] idTags набор тегов
    \return таблица видео
    \todo переименовать
    */
    videoDataType SearchVideoTags (QList<int> idTags);
    /*!
    Извлекает изображения по соответствию набору тегов
    \param[in] idTags набор тегов
    \return таблица изображений
    \todo переименовать
    */
    imageDataType SearchImageTags (QList<int> idTags);
    /*!
    Извлекает аудио по соответствию набору тегов
    \param[in] idTags набор тегов
    \return таблица аудио
    \todo переименовать
    */
    audioDataType SearchAudioTags (QList<int> idTags);
    /*!
    Извлекает аудио из коллекции по соответствию набору тегов. Не реализована
    \param[in] searchData набор флагов и значений для поиска
    \return таблица аудио
    \todo реализовать
    */
    audioDataType SearchAudiosTagsCollections (audioSearchType &searchData); // не уверен что это нужно пока будет как пища для размышлений
private:
    sqlite3 *db;  ///< указатель на объект sqlite
    const char *path;   ///< указатель на путь к файлу базы данных

};

#endif // DATABASECONTROLLER_H
