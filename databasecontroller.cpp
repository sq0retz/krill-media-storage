/*!
\file
\brief Файл реализации контроллера базы данных

*/
#include "databasecontroller.h"
#include <filesystem>
#include <QDebug>




DatabaseController::DatabaseController(QString &fullPath)
{
    QByteArray inUtf8 = fullPath.toUtf8(); //преобразование строки в массив char и добавление е в переменную класса
    const char *data_path = inUtf8.constData();
    path = data_path;

    if(QFile::exists(fullPath))   //если дб не существует то она будет создана и будут созданы необходимые таблицы
    {
        sqlite3_open(path, &db);
        std::string createQuery;
        sqlite3_stmt *createStmt;
        createQuery = "PRAGMA foreign_keys = ON;";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt) ;
        sqlite3_reset(createStmt);
    }
   else
   {
        sqlite3_open(path, &db);
        std::string createQuery;
        sqlite3_stmt *createStmt;
     createQuery = "PRAGMA foreign_keys = ON;";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt) ;
        sqlite3_reset(createStmt);

        createQuery = "CREATE TABLE VIDEO( " \
           "ID                        INTEGER PRIMARY KEY       ,"\
            "TITLE                    TEXT       NOT NULL ,"\
            "PATH                     TEXT       NOT NULL ,"\
            "WIDTH                    TEXT       NOT NULL,"\
            "HEIGHT                   TEXT       NOT NULL,"\
            "RATING                   INT              ,"\
            "DATE_CREATED             DATETIME   NOT NULL,"\
            "DATE_MODIFIED            DATETIME   NOT NULL,"\
            "FILETYPE                 TEXT       NOT NULL,"\
            "CODEC                    TEXT       NOT NULL,"\
            "DURATION                 TEXT       NOT NULL,"\
            "SIZE                     TEXT       NOT NULL,"\
            "UNIQUE (PATH)                     );";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE IMAGE( " \
            "ID                        INTEGER PRIMARY KEY       ,"\
            "TITLE                    TEXT       NOT NULL ,"\
            "PATH                     TEXT       NOT NULL ,"\
            "WIDTH                    TEXT       NOT NULL,"\
            "HEIGHT                   TEXT       NOT NULL,"\
            "RATING                   INT              ,"\
            "DATE_CREATED             DATETIME   NOT NULL,"\
            "DATE_MODIFIED            DATETIME   NOT NULL,"\
            "FILETYPE                 TEXT       NOT NULL,"\
            "SIZE                     TEXT       NOT NULL,"\
            "UNIQUE (PATH)                     );";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE AUDIO( " \
            "ID                       INTEGER PRIMARY KEY       ,"\
            "TITLE                    TEXT       NOT NULL ,"\
            "PATH                     TEXT       NOT NULL ,"\
            "RATING                   INT              ,"\
            "DATE_CREATED             DATETIME   NOT NULL,"\
            "DATE_MODIFIED            DATETIME   NOT NULL,"\
            "BITRATE                  TEXT       NOT NULL,"\
            "FILETYPE                 TEXT       NOT NULL,"\
            "ALBUM                    TEXT       NOT NULL,"\
            "BAND                     TEXT       NOT NULL,"\
            "TITLE_ENCODED            TEXT       NOT NULL,"\
            "DURATIONS                TEXT       NOT NULL,"\
            "SIZE                     TEXT       NOT NULL,"\
            "UNIQUE (PATH)                     );";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE TAGS ( " \
            "ID                       INTEGER PRIMARY KEY       ,"\
            "NAME                     TEXT       NOT NULL ,"\
            "UNIQUE (NAME)                     );";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE COLLECTIONS ( " \
            "ID                       INTEGER PRIMARY KEY       ,"\
            "NAME                     TEXT       NOT NULL ,"\
            "TYPE                     TEXT        ,"\
            "UNIQUE (NAME)                     );";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE VIDEO_TO_TAGS (" \
            " ID_VIDEO TEXT NOT NULL,"\
            " ID_TAG TEXT NOT NULL, "\
            " PRIMARY KEY(ID_VIDEO, ID_TAG),"\
            "FOREIGN KEY(ID_VIDEO) REFERENCES VIDEO(ID) ON DELETE CASCADE, "\
            "FOREIGN KEY(ID_TAG) REFERENCES TAGS(ID)  ON DELETE CASCADE);";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE IMAGE_TO_TAGS ( " \
            " ID_IMAGE TEXT NOT NULL,"\
            " ID_TAG TEXT NOT NULL, "\
            " PRIMARY KEY(ID_IMAGE, ID_TAG),"\
            "FOREIGN KEY(ID_IMAGE) REFERENCES IMAGE(ID) ON DELETE CASCADE, "\
            "FOREIGN KEY(ID_TAG) REFERENCES TAGS(ID) ON DELETE CASCADE );";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE AUDIO_TO_TAGS ( " \
            " ID_AUDIO TEXT NOT NULL,"\
            " ID_TAG TEXT NOT NULL, "\
            " PRIMARY KEY(ID_AUDIO, ID_TAG),"\
            "FOREIGN KEY(ID_AUDIO) REFERENCES AUDIO(ID) ON DELETE CASCADE , "\
            "FOREIGN KEY(ID_TAG) REFERENCES TAGS(ID) ON DELETE CASCADE) ;";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE VIDEO_TO_COLLECTIONS ( " \
            " ID_VIDEO TEXT NOT NULL,"\
            " ID_COLLECTION TEXT NOT NULL, "\
            " PRIMARY KEY(ID_VIDEO, ID_COLLECTION),"\
            "FOREIGN KEY(ID_VIDEO) REFERENCES VIDEO(ID) ON DELETE CASCADE, "\
            "FOREIGN KEY(ID_COLLECTION) REFERENCES COLLECTIONS(ID) ON DELETE CASCADE);";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE IMAGE_TO_COLLECTIONS ( " \
            " ID_IMAGE TEXT NOT NULL,"\
            " ID_COLLECTION TEXT NOT NULL, "\
            " PRIMARY KEY(ID_IMAGE, ID_COLLECTION),"\
            "FOREIGN KEY(ID_IMAGE) REFERENCES IMAGE(ID) ON DELETE CASCADE, "\
            "FOREIGN KEY(ID_COLLECTION) REFERENCES COLLECTIONS(ID) ON DELETE CASCADE);";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE AUDIO_TO_COLLECTIONS ( " \
            " ID_AUDIO TEXT NOT NULL,"\
            " ID_COLLECTION TEXT NOT NULL, "\
            " PRIMARY KEY(ID_AUDIO, ID_COLLECTION),"\
            "FOREIGN KEY(ID_AUDIO) REFERENCES AUDIO(ID) ON DELETE CASCADE, "\
            "FOREIGN KEY(ID_COLLECTION) REFERENCES COLLECTIONS(ID) ON DELETE CASCADE);";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
        createQuery = "CREATE TABLE TAGS_TO_COLLECTIONS ( " \
            " ID_TAG TEXT NOT NULL,"\
            " ID_COLLECTION TEXT NOT NULL, "\
            " PRIMARY KEY(ID_TAG, ID_COLLECTION),"\
            "FOREIGN KEY(ID_TAG) REFERENCES TAGS(ID) ON DELETE CASCADE, "\
            "FOREIGN KEY(ID_COLLECTION) REFERENCES COLLECTIONS(ID) ON DELETE CASCADE);";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);

  qDebug() << "aaaaaa";
        /* select * from AUDIO
        JOIN AUDIO_TO_TAGS ON AUDIO.ID = AUDIO_TO_TAGS.id_audio
         where  AUDIO_TO_TAGS.ID_TAG IN (3,5)
        GROUP BY AUDIO.id
         HAVING COUNT(DISTINCT AUDIO_TO_TAGS.id_tag) = 2;*/ // many tags first variant

        /*select * from AUDIO , AUDIO_TO_TAGS as t1,AUDIO_TO_TAGS as t2 // many tags second variant
        WHERE AUDIO.ID = t1.id_audio and t1.id_tag = 3 and
        AUDIO.id = t2.id_audio and t2.id_tag = 5
        ;*/
   }

}


void DatabaseController::InsertMedia(videoDataType &data)
{
   std::string createQuery;
   sqlite3_stmt *createStmt ;

   createQuery = "INSERT INTO VIDEO (TITLE, PATH, WIDTH , HEIGHT, DATE_CREATED ,DATE_MODIFIED , FILETYPE , CODEC , DURATION , SIZE) "\
       "VALUES ((?), (?), (?), (?), (?), (?),(?), (?), (?), (?));";
   qDebug() <<  QString::fromStdString( createQuery);
   for (int var = 0; var < data.paths.size(); var++) {
        qDebug() << sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_bind_text(createStmt, 1, data.titles[var].toUtf8()         , data.titles[var].toUtf8().length()       , SQLITE_TRANSIENT); // нужно что придумать с количеством байт SQLITE_TRANSIENT НЕОБХОДИМО
        sqlite3_bind_text(createStmt, 2, data.paths[var].toUtf8()          , data.paths[var].toUtf8().length()        , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 3, data.widths[var].toUtf8()         , data.widths[var].toUtf8().length()       , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 4, data.heights[var].toUtf8()        , data.heights[var].toUtf8().length()      , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 5, data.datesCreated[var].toUtf8()   , data.datesCreated[var].toUtf8().length() , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 6, data.datesModified[var].toUtf8()  , data.datesModified[var].toUtf8().length(), SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 7, data.filetypes[var].toUtf8()      , data.filetypes[var].toUtf8().length()    , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 8, data.codecs[var].toUtf8()         , data.codecs[var].toUtf8().length()       , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 9, data.durations[var].toUtf8()      ,data.durations[var].toUtf8().length()     , SQLITE_TRANSIENT);
        sqlite3_bind_text(createStmt, 10, data.sizes[var].toUtf8()         , data.sizes[var].toUtf8().length()        , SQLITE_TRANSIENT);
        qDebug() << sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
    }
}

void DatabaseController::InsertMedia(imageDataType &data)
{
    std::string createQuery;
    sqlite3_stmt *createStmt ;

    createQuery = "INSERT INTO IMAGE (TITLE,PATH,WIDTH ,HEIGHT, DATE_CREATED ,DATE_MODIFIED, FILETYPE, SIZE)"\
        "VALUES ((?), (?), (?), (?), (?), (?), (?), (?));";

    for (int var = 0; var < data.paths.size(); ++var) {

        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_bind_text(createStmt, 1, data.titles[var].toUtf8()         , data.titles[var].toUtf8().length()       , SQLITE_TRANSIENT); // нужно что придумать с количеством байт SQLITE_TRANSIENT НЕОБХОДИМО
        sqlite3_bind_text(createStmt, 2, data.paths[var].toUtf8()          , data.paths[var].toUtf8().length()        , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 3, data.widths[var].toUtf8()         , data.widths[var].toUtf8().length()       , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 4, data.heights[var].toUtf8()        , data.heights[var].toUtf8().length()      , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 5, data.datesCreated[var].toUtf8()   , data.datesCreated[var].toUtf8().length() , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 6, data.datesModified[var].toUtf8()  , data.datesModified[var].toUtf8().length(), SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 7, data.filetypes[var].toUtf8()      , data.filetypes[var].toUtf8().length()    , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 8, data.sizes[var].toUtf8()          , data.sizes[var].toUtf8().length()        , SQLITE_TRANSIENT);
        sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
    }
}

void DatabaseController::InsertMedia(audioDataType &data)
{
    std::string createQuery;
    sqlite3_stmt *createStmt ;

    createQuery = "INSERT INTO AUDIO (TITLE,PATH, DATE_CREATED ,DATE_MODIFIED,BITRATE, FILETYPE, ALBUM, BAND, TITLE_ENCODED,DURATIONS, SIZE)"\
        "VALUES ((?), (?), (?), (?), (?), (?), (?), (?), (?), (?),(?));";

    for (int var = 0; var < data.paths.size(); ++var) {
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_bind_text(createStmt, 1, data.titles[var].toUtf8()         , data.titles[var].toUtf8().length()        , SQLITE_TRANSIENT); // нужно что придумать с количеством байт SQLITE_TRANSIENT НЕОБХОДИМО
        sqlite3_bind_text(createStmt, 2, data.paths[var].toUtf8()          , data.paths[var].toUtf8().length()         , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 3, data.datesCreated[var].toUtf8()   , data.datesCreated[var].toUtf8().length()  , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 4, data.datesModified[var].toUtf8()  , data.datesModified[var].toUtf8().length() , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 5, data.bitRates[var].toUtf8()       , data.bitRates[var].toUtf8().length()      , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 6, data.filetypes[var].toUtf8()      , data.filetypes[var].toUtf8().length()     , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 7, data.albums[var].toUtf8()         , data.albums[var].toUtf8().length()        , SQLITE_TRANSIENT);
        sqlite3_bind_text(createStmt, 8, data.bands[var].toUtf8()          , data.bands[var].toUtf8().length()         , SQLITE_TRANSIENT);// нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 9, data.titlesEncoded[var].toUtf8()  , data.titlesEncoded[var].toUtf8().length() , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 10, data.durations[var].toUtf8()     , data.durations[var].toUtf8().length()     , SQLITE_TRANSIENT); // нужно что придумать с количеством байт
        sqlite3_bind_text(createStmt, 11, data.sizes[var].toUtf8()         , data.sizes[var].toUtf8().length()         , SQLITE_TRANSIENT);
        qDebug()<< sqlite3_step(createStmt);
        sqlite3_reset(createStmt);
    }

}

bool DatabaseController::InsertTag (QString tagName)
{
    if(tagName == "")
        return false;

    std::string createQuery;
    sqlite3_stmt *createStmt ;
    createQuery = "INSERT INTO TAGS (name) VALUES ((?));";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_text(createStmt, 1, tagName.toUtf8()     , tagName.toUtf8().length()     , SQLITE_TRANSIENT);
    if (sqlite3_step(createStmt) == 101)
    return true;
    return false;
}

bool DatabaseController::InsertCollection( QString collectionName)
{
    if(collectionName == "")
    return false;

    std::string createQuery;
    sqlite3_stmt *createStmt ;
    createQuery = "INSERT INTO COLLECTIONS (name, type) VALUES ((?),(?));";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_text(createStmt, 1, collectionName.toUtf8()     , collectionName.toUtf8().length()     , SQLITE_TRANSIENT);
    sqlite3_bind_text(createStmt, 2, "GENERAL"     , 7    , SQLITE_TRANSIENT);
    if (sqlite3_step(createStmt) == 101)
    return true;
    return false;
}

bool DatabaseController::InsertCollection( QString collectionName, mediaType type)
{
    if(collectionName == "")
    return false;

    std::string createQuery;
    qDebug() << type;
    sqlite3_stmt *createStmt ;
    createQuery = "INSERT INTO COLLECTIONS (name, type) VALUES ((?),(?));";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_text(createStmt, 1, collectionName.toUtf8() , collectionName.toUtf8().length()     , SQLITE_TRANSIENT);
    switch  (type)
    {
    case 1:
            sqlite3_bind_text(createStmt, 2, "VIDEO" , 5    , SQLITE_TRANSIENT); break;
    case 2:
            sqlite3_bind_text(createStmt, 2, "AUDIO" , 5    , SQLITE_TRANSIENT); break;
    case 3:
            sqlite3_bind_text(createStmt, 2, "IMAGE" , 5    , SQLITE_TRANSIENT); break;
    default:
            sqlite3_bind_text(createStmt, 2, "GENERAL" , 7    , SQLITE_TRANSIENT); break;
    }

    if (sqlite3_step(createStmt) == 101)
    return true;
    return false;

}

bool DatabaseController::AddTagsToMedia   (int mediaId, mediaType type, QSet<int> tags)
{
    if(tags.empty())
        return false;

    std::string createQuery ;
    sqlite3_stmt *createStmt;
    switch (type)
    {
        case 1: createQuery = "INSERT INTO VIDEO_TO_TAGS VALUES (" + std::to_string(mediaId) + ",(?));"; break;
        case 2: createQuery = "INSERT INTO AUDIO_TO_TAGS VALUES (" + std::to_string(mediaId) + ",(?));"; break;
        case 3: createQuery = "INSERT INTO IMAGE_TO_TAGS VALUES (" + std::to_string(mediaId) + ",(?));"; break;
        default:  return false;

    }
    for (int tagId : tags)
    {
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_bind_int(createStmt, 1, tagId);
        if (sqlite3_step(createStmt) != 101)
            return false;
    }
    return true;
}
bool DatabaseController::AddMediaToCollection (QList<int> mediaIds,mediaType type, int CollectionId)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    switch (type)
    {
        case 1: createQuery = "INSERT INTO VIDEO_TO_COLLECTIONS VALUES ((?), (?));"; break;
        case 2: createQuery = "INSERT INTO AUDIO_TO_COLLECTIONS VALUES ((?), (?));"; break;
        case 3: createQuery = "INSERT INTO IMAGE_TO_COLLECTIONS VALUES ((?), (?));"; break;
        default:  return false;

    }
        for (int mediaId : mediaIds) {
            sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
            sqlite3_bind_int(createStmt, 1, mediaId);
            sqlite3_bind_int(createStmt, 2, CollectionId);
            if (sqlite3_step(createStmt) != 101)
                return false;
        }
    return true;
}

videoDataType DatabaseController::SearchMedia (videoSearchType &searchData)   // пока не уверен надо ли это, в текущей реализации будет возвращать всю таблицу независимо от параметров
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    videoDataType returnTable;


    createQuery =  "SELECT * FROM VIDEO ";
  // if(searchData.bitSearchConditions & videoSearchType::TITLE_MASK_EQUAL)     createQuery +=  "title LIKE '" + searchData.titleMask.toStdString() + "'" + "AND";
   //if(searchData.bitSearchConditions & videoSearchType::TITLE_MASK_NOT_EQUAL) createQuery +=  "title NOT LIKE '" + searchData.titleMask.toStdString() + "'" + "AND" ;
   //if(searchData.bitSearchConditions & videoSearchType::DATE_DAY_EQUAL) createQuery +=  " date(date) = '" + searchData.dateFirst.toStdString() + "'" ;
    createQuery += ";";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.codecs.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));

    }
    return returnTable;

}

imageDataType DatabaseController::SearchMedia (imageSearchType &searchData)   // пока не уверен надо ли это, в текущей реализации будет возвращать всю таблицу независимо от параметров
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    imageDataType returnTable;


    createQuery =  "SELECT * FROM IMAGE ";
    // if(searchData.bitSearchConditions & videoSearchType::TITLE_MASK_EQUAL)     createQuery +=  "title LIKE '" + searchData.titleMask.toStdString() + "'" + "AND";
    //if(searchData.bitSearchConditions & videoSearchType::TITLE_MASK_NOT_EQUAL) createQuery +=  "title NOT LIKE '" + searchData.titleMask.toStdString() + "'" + "AND" ;
    //if(searchData.bitSearchConditions & videoSearchType::DATE_DAY_EQUAL) createQuery +=  " date(date) = '" + searchData.dateFirst.toStdString() + "'" ;
    createQuery += ";";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
    }
    return returnTable;

}

audioDataType DatabaseController::SearchMedia (audioSearchType &searchData)   // пока не уверен надо ли это, в текущей реализации будет возвращать всю таблицу независимо от параметров
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    audioDataType returnTable;


    createQuery =  "SELECT * FROM AUDIO ";
    // if(searchData.bitSearchConditions & videoSearchType::TITLE_MASK_EQUAL)     createQuery +=  "title LIKE '" + searchData.titleMask.toStdString() + "'" + "AND";
    //if(searchData.bitSearchConditions & videoSearchType::TITLE_MASK_NOT_EQUAL) createQuery +=  "title NOT LIKE '" + searchData.titleMask.toStdString() + "'" + "AND" ;
    //if(searchData.bitSearchConditions & videoSearchType::DATE_DAY_EQUAL) createQuery +=  " date(date) = '" + searchData.dateFirst.toStdString() + "'" ;
    createQuery += ";";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 3));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 5)));
        returnTable.bitRates.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.albums.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.bands.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.titlesEncoded.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 12)));
    }
    return returnTable;

}
audioDataType DatabaseController::SearchAudioTags (QList<int> idTags)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    audioDataType returnTable;

    if (idTags.size() == 0)  //защита на случай отсутствия тегов ERROR
        return returnTable;

    createQuery =  "select * from AUDIO";
    for (int i = 1; i < idTags.size() + 1; i++)
    {
        createQuery += ", AUDIO_TO_TAGS as t" + std::to_string(i) + " ";
    }
    createQuery += "WHERE ";
    createQuery += "AUDIO.ID = t1.id_audio and t1.id_tag = " + std::to_string(idTags[0]);
    for (int i = 2; i < idTags.size() + 1; i++)
    {
        createQuery += " and AUDIO.ID = t" + std::to_string(i) + ".id_audio and t" +  std::to_string(i) + ".id_tag = " + std::to_string(idTags[i-1]);
    }
       //          "WHERE AUDIO.ID = t1.id_audio and t1.id_tag = 3 and "
               //   "AUDIO.id = t2.id_audio and t2.id_tag = 5";

    createQuery += ";";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);

    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 3));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 5)));
        returnTable.bitRates.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.albums.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.bands.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.titlesEncoded.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 12)));

    }
    return returnTable;
}

videoDataType DatabaseController::SearchVideoTags( QList<int> idTags)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    videoDataType returnTable;

    if (idTags.size() == 0)  //защита на случай отсутствия тегов ERROR
        return returnTable;

    createQuery =  "select * from VIDEO";
    for (int i = 1; i < idTags.size() + 1; i++)
    {
        createQuery += ", VIDEO_TO_TAGS as t" + std::to_string(i) + " ";
    }
    createQuery += "WHERE ";
    createQuery += "VIDEO.ID = t1.id_video and t1.id_tag = " + std::to_string(idTags[0]);
    for (int i = 2; i < idTags.size() + 1; i++)
    {
        createQuery += " and VIDEO.ID = t" + std::to_string(i) + ".id_video and t" +  std::to_string(i) + ".id_tag = " + std::to_string(idTags[i-1]);
    }
    createQuery += ";";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);

    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.codecs.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));

    }
    return returnTable;
}

imageDataType DatabaseController::SearchImageTags( QList<int> idTags)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    imageDataType returnTable;

    if (idTags.size() == 0)  //защита на случай отсутствия тегов ERROR
        return returnTable;

    createQuery =  "select * from IMAGE";
    for (int i = 1; i < idTags.size() + 1; i++)
    {
        createQuery += ", IMAGE_TO_TAGS as t" + std::to_string(i) + " ";
    }
    createQuery += "WHERE ";
    createQuery += "IMAGE.ID = t1.id_image and t1.id_tag = " + std::to_string(idTags[0]);
    for (int i = 2; i < idTags.size() + 1; i++)
    {
        createQuery += " and IMAGE.ID = t" + std::to_string(i) + ".id_image and t" +  std::to_string(i) + ".id_tag = " + std::to_string(idTags[i-1]);
    }
    createQuery += ";";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);

    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));

    }
    return returnTable;
}

bool DatabaseController::SetMediaRating (int mediaId, mediaType type, int rating)
{
    if (rating > 10 || rating < 1)
        return false;
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    switch (type)
    {//UPDATE AUDIO SET RATING = 5 WHERE ID = 3;
    case 1: createQuery = "UPDATE VIDEO SET RATING = (?) WHERE ID = (?);"; break;
    case 2: createQuery = "UPDATE AUDIO SET RATING = (?) WHERE ID = (?);"; break;
    case 3: createQuery = "UPDATE IMAGE SET RATING = (?) WHERE ID = (?);"; break;
    default:  return false;
    }
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_int(createStmt, 1, rating);
    sqlite3_bind_int(createStmt, 2, mediaId);
    if (sqlite3_step(createStmt) != 101)
        return false;
    return true;

}

tagData DatabaseController::getTagTable ()
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    tagData returnTable;
    createQuery = "SELECT * FROM TAGS;";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append( sqlite3_column_int(createStmt, 0));
        returnTable.name.append(QString::fromUtf8( sqlite3_column_text(createStmt, 1)));

    }
    return returnTable;
}

collectionData DatabaseController::getCollectionTable ()
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    collectionData returnTable;
    createQuery = "SELECT * FROM COLLECTIONS;";
    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append( sqlite3_column_int(createStmt, 0));
        returnTable.name.append(QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.type.append(QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
    }
    return returnTable;
}
videoDataType  DatabaseController::getVideoByPath(QList<QString> paths)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    videoDataType returnTable;


    foreach (QString var, paths) {
        createQuery =  "SELECT * FROM VIDEO WHERE PATH like \"" + var.toStdString() + "\";";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.codecs.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));
        sqlite3_reset(createStmt);
    }
    return returnTable;

}

imageDataType  DatabaseController::getImageByPath(QList<QString> paths)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    imageDataType returnTable;


    foreach (QString var, paths) {
        createQuery =  "SELECT * FROM IMAGE WHERE PATH like \"" + var.toStdString() + "\";";
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        sqlite3_reset(createStmt);
    }
    return returnTable;

}


audioDataType  DatabaseController::getAudioByPath(QList<QString> paths)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    audioDataType returnTable;


    foreach (QString var, paths) {
        createQuery =  "SELECT * FROM AUDIO WHERE PATH like \"" + var.toStdString() + "\";";
        qDebug() << QString::fromStdString(createQuery);
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_step(createStmt);
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 3));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 5)));
        returnTable.bitRates.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.albums.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.bands.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.titlesEncoded.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 12)));
        sqlite3_reset(createStmt);
    }
    return returnTable;

}

videoDataType  DatabaseController::getVideoByCollection(int collectionId )
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    videoDataType returnTable;


    createQuery =  "SELECT * FROM VIDEO, VIDEO_TO_COLLECTIONS where VIDEO.id = VIDEO_TO_COLLECTIONS.id_video and VIDEO_TO_COLLECTIONS.id_collection = (?);";

    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_int(createStmt, 1, collectionId);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.codecs.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));

    }
    return returnTable;
}

imageDataType  DatabaseController::getImageByCollection(int collectionId )
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    imageDataType returnTable;


    createQuery =  "SELECT * FROM IMAGE, IMAGE_TO_COLLECTIONS where IMAGE.id = IMAGE_TO_COLLECTIONS.id_image and IMAGE_TO_COLLECTIONS.id_collection = (?);";


    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_int(createStmt, 1, collectionId);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        qDebug() << sqlite3_column_int(createStmt, 0);
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.widths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 3)));
        returnTable.heights.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 5));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));

    }
    return returnTable;
}

audioDataType  DatabaseController::getAudioByCollection(int collectionId )
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    audioDataType returnTable;


    createQuery =  "SELECT * FROM audio, AUDIO_TO_COLLECTIONS where AUDIO.id = AUDIO_TO_COLLECTIONS.id_audio and AUDIO_TO_COLLECTIONS.id_collection = (?);";


    sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_int(createStmt, 1, collectionId);
    while (sqlite3_step(createStmt) != SQLITE_DONE) { // проблема - если таблица пустая то зацикливание
        returnTable.id.append(sqlite3_column_int(createStmt, 0));
        returnTable.titles.append( QString::fromUtf8( sqlite3_column_text(createStmt, 1)));
        returnTable.paths.append( QString::fromUtf8( sqlite3_column_text(createStmt, 2)));
        returnTable.rating.append(sqlite3_column_int(createStmt, 3));
        returnTable.datesCreated.append( QString::fromUtf8( sqlite3_column_text(createStmt, 4)));
        returnTable.datesModified.append( QString::fromUtf8( sqlite3_column_text(createStmt, 5)));
        returnTable.bitRates.append( QString::fromUtf8( sqlite3_column_text(createStmt, 6)));
        returnTable.filetypes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 7)));
        returnTable.albums.append( QString::fromUtf8( sqlite3_column_text(createStmt, 8)));
        returnTable.bands.append( QString::fromUtf8( sqlite3_column_text(createStmt, 9)));
        returnTable.titlesEncoded.append( QString::fromUtf8( sqlite3_column_text(createStmt, 10)));
        returnTable.durations.append( QString::fromUtf8( sqlite3_column_text(createStmt, 11)));
        returnTable.sizes.append( QString::fromUtf8( sqlite3_column_text(createStmt, 12)));

    }
    return returnTable;
}

bool DatabaseController::deleteMedia(QSet<int> id, mediaType type)
{
    std::string createQuery ;
    sqlite3_stmt *createStmt;
    switch (type)
    {
    case 1: createQuery = "DELETE FROM VIDEO WHERE ID = (?);"; break;
    case 2: createQuery = "DELETE FROM AUDIO WHERE ID = (?);"; break;
    case 3: createQuery = "DELETE FROM IMAGE WHERE ID = (?);"; break;
    default: break;
    }
        for (int var : id) {
        sqlite3_prepare_v2( db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
        sqlite3_bind_int(createStmt, 1, var);
        sqlite3_step(createStmt);
        }
        return true;

}
DatabaseController::~DatabaseController()
{
    delete path;
    //delete db;
}
