#include "pch.h"
#include "CInfoFile.h"

CInfoFile::CInfoFile() {}

CInfoFile::~CInfoFile() {}

// 创建数据库连接
sqlite3* OpenDatabase(const char* dbFilePath) {
    sqlite3* db;

    // 打开数据库
    if (sqlite3_open(dbFilePath, &db) != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return nullptr;
    }

    return db;
}

// 创建表
void CreateTable(sqlite3* db) {
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS Users (Id INTEGER PRIMARY KEY AUTOINCREMENT, UserName TEXT, Password TEXT);";

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Error creating table." << std::endl;
    }
    else {
        std::cout << "Table created or already exists." << std::endl;
    }
}

void CInfoFile::InitLoad() {
    // 创建数据库连接
    sqlite3* db = OpenDatabase(this->dbFilePath);

    if (db) {
        CreateTable(db);
        this->db = db;
    }
    return;
}

bool CInfoFile::CheckLoad(CString UserName, CString Password) {
    CString queryDataSQL = "SELECT * FROM Users WHERE UserName=? AND Password=?;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // 绑定参数
        sqlite3_bind_text(statement, 1, UserName, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, Password, -1, SQLITE_STATIC);

        // 遍历结果集
        if (sqlite3_step(statement) == SQLITE_ROW) {
            sqlite3_finalize(statement);
            return true;
        }

        // 释放资源
        sqlite3_finalize(statement);
    }
    return false;
}


void CInfoFile::CreateLoad(CString UserName, CString Password) {
    InsertData(UserName, Password);
    return;
}

// 关闭数据库连接
void CInfoFile::CloseDatabase() {
    sqlite3_close(this->db);
}

// 插入数据
void CInfoFile::InsertData(CString UserName, CString Password) {
    const char* insertDataSQL = "INSERT INTO Users (UserName, Password) VALUES (?, ?);";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, insertDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // 绑定参数
        sqlite3_bind_text(statement, 1, UserName, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, Password, -1, SQLITE_STATIC);

        // 执行语句
        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Error inserting data." << std::endl;
        }
        else {
            std::cout << "Data inserted." << std::endl;
        }

        // 释放资源
        sqlite3_finalize(statement);
    }
}

// 查询数据
void CInfoFile::QueryData() {
    const char* queryDataSQL = "SELECT * FROM Users;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        std::cout << "Id\tUsername\tPassword" << std::endl;

        // 遍历结果集
        while (sqlite3_step(statement) == SQLITE_ROW) {
            std::cout << sqlite3_column_int(statement, 0) << "\t"
                << sqlite3_column_text(statement, 1) << "\t"
                << sqlite3_column_text(statement, 2) << std::endl;
        }

        // 释放资源
        sqlite3_finalize(statement);
    }
    return;
}

// 更新数据
void CInfoFile::UpdateData(int id, CString UserName, CString Password) {
    const char* updateDataSQL = "UPDATE Users SET Username=?, Password=? WHERE Id=?;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, updateDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // 绑定参数
        sqlite3_bind_text(statement, 1, UserName, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, Password, -1, SQLITE_STATIC);
        sqlite3_bind_int(statement, 3, id);

        // 执行语句
        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Error updating data." << std::endl;
        }
        else {
            std::cout << "Data updated." << std::endl;
        }

        // 释放资源
        sqlite3_finalize(statement);
    }
}

// 删除数据
void CInfoFile::DeleteData(int id) {
    const char* deleteDataSQL = "DELETE FROM Users WHERE Id=?;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, deleteDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // 绑定参数
        sqlite3_bind_int(statement, 1, id);

        // 执行语句
        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Error deleting data." << std::endl;
        }
        else {
            std::cout << "Data deleted." << std::endl;
        }

        // 释放资源
        sqlite3_finalize(statement);
    }
}
