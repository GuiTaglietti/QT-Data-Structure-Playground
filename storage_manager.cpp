/** @author Guilherme Martinelli Taglietti
 *  @file   storage_manager.cpp
 *  @brief  SQLite storage for saving/loading structures impl
 */
#include "storage_manager.h"
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

static QString g_dir(){
    return QDir::currentPath()+"/saved_data_structures";
}

static bool open_db(QSqlDatabase& db){
    db=QSqlDatabase::addDatabase("QSQLITE","ds_conn");
    db.setDatabaseName(g_dir()+"/data.db");
    if(!db.open()) return false;
    QSqlQuery q(db);
    q.exec("CREATE TABLE IF NOT EXISTS saved(kind TEXT NOT NULL, name TEXT NOT NULL, payload BLOB NOT NULL, PRIMARY KEY(kind,name))");
    return true;
}

bool StorageManager::ensure_env(){
    QDir d(g_dir());
    if(!d.exists()) if(!d.mkpath(".")) return false;
    QSqlDatabase db;
    bool ok=open_db(db);
    db.close();
    QSqlDatabase::removeDatabase("ds_conn");
    return ok;
}

bool StorageManager::save_json(const QString& kind,const QString& name,const QJsonObject& obj){
    if(!ensure_env()) return false;
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","ds_conn_wr");
    db.setDatabaseName(g_dir()+"/data.db");
    if(!db.open()) return false;
    QSqlQuery q(db);
    q.prepare("INSERT OR REPLACE INTO saved(kind,name,payload) VALUES(?,?,?)");
    q.addBindValue(kind);
    q.addBindValue(name);
    q.addBindValue(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    bool ok=q.exec();
    db.close();
    QSqlDatabase::removeDatabase("ds_conn_wr");
    return ok;
}

QJsonObject StorageManager::load_json(const QString& kind,const QString& name){
    QJsonObject res;
    if(!ensure_env()) return res;
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","ds_conn_rd");
    db.setDatabaseName(g_dir()+"/data.db");
    if(!db.open()) return res;
    QSqlQuery q(db);
    q.prepare("SELECT payload FROM saved WHERE kind=? AND name=?");
    q.addBindValue(kind);
    q.addBindValue(name);
    if(q.exec()&&q.next()){
        QByteArray b=q.value(0).toByteArray();
        res=QJsonDocument::fromJson(b).object();
    }
    db.close();
    QSqlDatabase::removeDatabase("ds_conn_rd");
    return res;
}

QStringList StorageManager::list_names(const QString& kind){
    QStringList out;
    if(!ensure_env()) return out;
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","ds_conn_ls");
    db.setDatabaseName(g_dir()+"/data.db");
    if(!db.open()) return out;
    QSqlQuery q(db);
    q.prepare("SELECT name FROM saved WHERE kind=? ORDER BY name");
    q.addBindValue(kind);
    if(q.exec()) while(q.next()) out<<q.value(0).toString();
    db.close();
    QSqlDatabase::removeDatabase("ds_conn_ls");
    return out;
}

QString StorageManager::base_dir(){
    return g_dir();
}
