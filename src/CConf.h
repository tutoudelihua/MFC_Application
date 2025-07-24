#pragma once
#ifndef _CONF_H
#define _CONF_H
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <io.h>
#include <iostream>
#include <direct.h>
#include <afxdisp.h>
#include <Winsock2.h>

class Conf
{
public:

	CString get_confFile() {
		return this->_confFile;
	}

	void set_confFile(CString confFile) {
		this->_confFile = confFile;
	}

	int getCodeNumber() {
		return this->CodeNumber;
	}

	std::string getType() {
		return this->Type;
	}

	void setType(std::string Type) {
		this->Type = Type;
	}

	std::vector<double> getData() {
		return this->Data;
	}

	void PrintConf();

	std::vector<double> LoadConf(const char* confFolder);

private:
	std::string Type;
	int CodeNumber;
	std::vector<double> Data;
	CString _confFile;
	
};

#endif //_CONF_H

