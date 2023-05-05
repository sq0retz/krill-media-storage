// так как foreign key не включен то он и не работает
#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#include <sqlite3.h>
#include <iostream>
#include <QString>
#include <QFile>

struct videoDataType{ //структура для упращения передачи данных в контроллер
    QList<int> id;
    QList<QString> titles;
    QList<QString> paths;
    QList<QString> widths;
    QList<QString> heights;
    QList<int>     rating;
    QList<QString> datesCreated;
    QList<QString> datesModified;
    QList<QString> filetypes;
    QList<QString> codecs;
    QList<QString> durations;
    QList<QString> sizes;
};

struct imageDataType{ //структура для упращения передачи данных в контроллер
    QList<int> id;
    QList<QString> titles;
    QList<QString> paths;
    QList<QString> widths;
    QList<QString> heights;
    QList<int>     rating;
    QList<QString> datesCreated;
    QList<QString> datesModified;
    QList<QString> filetypes;
    QList<QString> sizes;
};

struct audioDataType{ //структура для упращения передачи данных в контроллер
    QList<int> id;
    QList<QString> titles;
    QList<QString> paths;
    QList<int>     rating;
    QList<QString> datesCreated;
    QList<QString> datesModified;
    QList<QString> bitRates;
    QList<QString> filetypes;
    QList<QString> albums;
    QList<QString> bands;
    QList<QString> titlesEncoded;
    QList<QString> durations;
    QList<QString> sizes;
};

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
struct collectionData
{
    QList<int> id;
    QList<QString> name;
    QList<QString> type;

};

struct tagData
{
    QList<int> id;
    QList<QString> name;

};

class DatabaseController
{
public:
    enum mediaType{
        VIDEO_TYPE = 1,
        AUDIO_TYPE = 2,
        IMAGE_TYPE = 3,
        GENERAL_TYPE = 4
    };
    DatabaseController (QString &fullPath);
    ~DatabaseController();
    void InsertMedia (videoDataType &data);
    void InsertMedia (imageDataType &data);
    void InsertMedia (audioDataType &data);
    bool InsertTag (QString tagName);
    bool InsertCollection (QString collectionName);
    bool InsertCollection (QString collectionName, mediaType type);
    bool AddTagsToMedia   (int mediaId, mediaType type,  QSet<int> tags);
    bool AddMediaToCollection (QList<int> mediaIds, mediaType type, int CollectionId);
    bool SetMediaRating (int mediaId, mediaType type, int rating);
    bool deleteMedia(QSet<int> id, mediaType type);
    tagData getTagTable ();
    collectionData getCollectionTable();
    videoDataType  getVideoByPath(QList<QString> paths);
    imageDataType  getImageByPath(QList<QString> paths);
    audioDataType  getAudioByPath(QList<QString> paths);
    videoDataType  getVideoByCollection(int collectionId );
    imageDataType  getImageByCollection(int collectionId );
    audioDataType  getAudioByCollection(int collectionId );
    videoDataType SearchMedia     (videoSearchType &searchData);  // НА ДАННОМ ЭТАПЕ ВОЗВРАЩАЕТ ПРОСТО СПИСОК ВСЕХ ВИДЕО
    imageDataType SearchMedia     (imageSearchType &searchData);  // НА ДАННОМ ЭТАПЕ ВОЗВРАЩАЕТ ПРОСТО СПИСОК ВСЕХ картинок
    audioDataType SearchMedia     (audioSearchType &searchData);  // НА ДАННОМ ЭТАПЕ ВОЗВРАЩАЕТ ПРОСТО СПИСОК ВСЕХ аудио
    videoDataType SearchVideoTags (QList<int> idTags);
    imageDataType SearchImageTags (QList<int> idTags);
    audioDataType SearchAudioTags (QList<int> idTags);
    audioDataType SearchAudiosTagsCollections (audioSearchType &searchData); // не уверен что это нужно пока будет как пища для размышлений
private:
    sqlite3 *db;
    const char *path;

};

#endif // DATABASECONTROLLER_H
