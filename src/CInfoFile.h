#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();
	
	void InitLoad();

	void CloseDatabase();
	
	void InsertData(CString UserName, CString Password);

	void QueryData();

	void UpdateData(int id, CString UserName, CString Password);

	void DeleteData(int id);

	void CreateLoad(CString UserName, CString Password);

	bool CheckLoad(CString UserName, CString Password);


private:
	const char* dbFilePath = "test.db";
	
	sqlite3* db;
};

