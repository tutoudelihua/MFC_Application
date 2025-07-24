#include "pch.h"
#include "CInfoFile.h"

CInfoFile::CInfoFile() {}

CInfoFile::~CInfoFile() {}

// �������ݿ�����
sqlite3* OpenDatabase(const char* dbFilePath) {
    sqlite3* db;

    // �����ݿ�
    if (sqlite3_open(dbFilePath, &db) != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return nullptr;
    }

    return db;
}

// ������
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
    // �������ݿ�����
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
        // �󶨲���
        sqlite3_bind_text(statement, 1, UserName, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, Password, -1, SQLITE_STATIC);

        // ���������
        if (sqlite3_step(statement) == SQLITE_ROW) {
            sqlite3_finalize(statement);
            return true;
        }

        // �ͷ���Դ
        sqlite3_finalize(statement);
    }
    return false;
}


void CInfoFile::CreateLoad(CString UserName, CString Password) {
    InsertData(UserName, Password);
    return;
}

// �ر����ݿ�����
void CInfoFile::CloseDatabase() {
    sqlite3_close(this->db);
}

// ��������
void CInfoFile::InsertData(CString UserName, CString Password) {
    const char* insertDataSQL = "INSERT INTO Users (UserName, Password) VALUES (?, ?);";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, insertDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // �󶨲���
        sqlite3_bind_text(statement, 1, UserName, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, Password, -1, SQLITE_STATIC);

        // ִ�����
        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Error inserting data." << std::endl;
        }
        else {
            std::cout << "Data inserted." << std::endl;
        }

        // �ͷ���Դ
        sqlite3_finalize(statement);
    }
}

// ��ѯ����
void CInfoFile::QueryData() {
    const char* queryDataSQL = "SELECT * FROM Users;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        std::cout << "Id\tUsername\tPassword" << std::endl;

        // ���������
        while (sqlite3_step(statement) == SQLITE_ROW) {
            std::cout << sqlite3_column_int(statement, 0) << "\t"
                << sqlite3_column_text(statement, 1) << "\t"
                << sqlite3_column_text(statement, 2) << std::endl;
        }

        // �ͷ���Դ
        sqlite3_finalize(statement);
    }
    return;
}

// ��������
void CInfoFile::UpdateData(int id, CString UserName, CString Password) {
    const char* updateDataSQL = "UPDATE Users SET Username=?, Password=? WHERE Id=?;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, updateDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // �󶨲���
        sqlite3_bind_text(statement, 1, UserName, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, Password, -1, SQLITE_STATIC);
        sqlite3_bind_int(statement, 3, id);

        // ִ�����
        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Error updating data." << std::endl;
        }
        else {
            std::cout << "Data updated." << std::endl;
        }

        // �ͷ���Դ
        sqlite3_finalize(statement);
    }
}

// ɾ������
void CInfoFile::DeleteData(int id) {
    const char* deleteDataSQL = "DELETE FROM Users WHERE Id=?;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(this->db, deleteDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
        // �󶨲���
        sqlite3_bind_int(statement, 1, id);

        // ִ�����
        if (sqlite3_step(statement) != SQLITE_DONE) {
            std::cerr << "Error deleting data." << std::endl;
        }
        else {
            std::cout << "Data deleted." << std::endl;
        }

        // �ͷ���Դ
        sqlite3_finalize(statement);
    }
}
