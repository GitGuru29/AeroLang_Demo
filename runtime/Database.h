#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <jni.h>

namespace aero {

// Forward declarations
class Cursor;
class ContentValues;

/**
 * Database wrapper for Android's SQLite database.
 * Provides simple CRUD operations and raw SQL execution.
 * 
 * Usage:
 *   auto db = Database::open(activity, "myapp.db");
 *   db->execSQL("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)");
 *   
 *   ContentValues values;
 *   values.put("name", "John");
 *   db->insert("users", values);
 *   
 *   auto cursor = db->query("users", {"id", "name"}, "name = ?", {"John"});
 *   while (cursor->moveToNext()) {
 *       int id = cursor->getInt(0);
 *       std::string name = cursor->getString(1);
 *   }
 */
class Database {
public:
    ~Database();
    
    // Open or create database
    static Database* open(JNIEnv* env, jobject context, const std::string& name);
    
    // Execute raw SQL (no return value)
    void execSQL(const std::string& sql);
    void execSQL(const std::string& sql, const std::vector<std::string>& args);
    
    // Query
    Cursor* rawQuery(const std::string& sql);
    Cursor* rawQuery(const std::string& sql, const std::vector<std::string>& args);
    Cursor* query(const std::string& table, 
                  const std::vector<std::string>& columns,
                  const std::string& where = "",
                  const std::vector<std::string>& whereArgs = {},
                  const std::string& orderBy = "",
                  int limit = -1);
    
    // Insert (returns row ID or -1 on error)
    long insert(const std::string& table, const ContentValues& values);
    
    // Update (returns number of affected rows)
    int update(const std::string& table, const ContentValues& values,
               const std::string& where, const std::vector<std::string>& whereArgs);
    
    // Delete (returns number of deleted rows)
    int deleteFrom(const std::string& table, 
                   const std::string& where = "",
                   const std::vector<std::string>& whereArgs = {});
    
    // Transaction support
    void beginTransaction();
    void setTransactionSuccessful();
    void endTransaction();
    
    // Close database
    void close();
    
    // Check if database is open
    bool isOpen() const;
    
private:
    Database(JNIEnv* env, jobject dbInstance);
    
    JNIEnv* env;
    jobject dbInstance;
};

/**
 * Cursor for iterating over query results.
 */
class Cursor {
public:
    Cursor(JNIEnv* env, jobject cursorInstance);
    ~Cursor();
    
    // Navigation
    bool moveToFirst();
    bool moveToNext();
    bool moveToPrevious();
    bool moveToPosition(int position);
    bool isFirst();
    bool isLast();
    bool isBeforeFirst();
    bool isAfterLast();
    
    // Get count
    int getCount();
    int getPosition();
    int getColumnCount();
    
    // Get column info
    int getColumnIndex(const std::string& columnName);
    std::string getColumnName(int columnIndex);
    std::vector<std::string> getColumnNames();
    
    // Get values by column index
    std::string getString(int columnIndex);
    int getInt(int columnIndex);
    long getLong(int columnIndex);
    double getDouble(int columnIndex);
    std::vector<uint8_t> getBlob(int columnIndex);
    bool isNull(int columnIndex);
    
    // Close cursor
    void close();
    
private:
    JNIEnv* env;
    jobject cursorInstance;
};

/**
 * ContentValues for inserting/updating rows.
 */
class ContentValues {
public:
    ContentValues();
    ContentValues(JNIEnv* env);
    ~ContentValues();
    
    void put(const std::string& key, const std::string& value);
    void put(const std::string& key, int value);
    void put(const std::string& key, long value);
    void put(const std::string& key, double value);
    void put(const std::string& key, bool value);
    void putNull(const std::string& key);
    
    // Access JNI object
    jobject getJNIObject() const { return valuesInstance; }
    void initJNI(JNIEnv* env);
    
private:
    JNIEnv* env;
    jobject valuesInstance;
};

} // namespace aero
