/** @author Guilherme Martinelli Taglietti
 *  @file   storage_manager.h
 *  @brief  SQLite storage for saving/loading structures
 */
#pragma once

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

class StorageManager{
public:
    static bool ensure_env();
    static bool save_json(const QString& kind,const QString& name,const QJsonObject& obj);
    static QJsonObject load_json(const QString& kind,const QString& name);
    static QStringList list_names(const QString& kind);
    static QString base_dir();
};
