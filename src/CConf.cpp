#define _CRT_SECURE_NO_WARNINGS

#include "CConf.h"
#include <stack>
#include <iostream>
#include <vector>
#include "CurveFitting.h"
#include <unordered_map>
#include "CMyMenu.h"
#include "drawCoordinateSystem.h"

double a[65536];

void Conf::PrintConf() {
	CoordinateSystem cs;
	coordinateSystemInit(&cs);
	for (int i = 1; i <= this->Data.size(); ++i) {
		a[i] = this->Data[i - 1];
	}
	plotType(&cs, a, this->Data.size(), RED);
	std::stringstream filePath;
	filePath << "data\\" << this->Type
		<< "_" << std::to_string(this->CodeNumber) << ".jpg";
	saveimage(filePath.str().c_str());
	closegraph();
}

std::stack<double> st;
char StrLine[65536];

std::vector<double> Conf::LoadConf(const char* confFolder) {
	if (nullptr == confFolder) {
		return std::vector<double>();
	}
	std::string tmpStr;

	std::string confFile = confFolder;
	int posl = confFile.find_last_of("\\");
	int posr = confFile.find_last_of(".");
	int pos = posr - 1, num = 0;
	for (int i = posl + 1; i <= posr - 1; ++i) {
		if (confFile[i] >= '0' && confFile[i] <= '9') {
			pos = i - 1;
			for (int j = i; j <= posr - 1; ++j) {
				num = num * 10 + confFile[j] - '0';
			}
			break;
		}
	}
	this->Type = confFile.substr(posl + 1, pos - posl);
	this->CodeNumber = num;

	// Load data from file
	FILE* fp = fopen(confFile.c_str(), "r");
	int LineNum = 0;
	if (NULL != fp) {
		while (!feof(fp)) {
			++LineNum;
			memset(StrLine, 0, 65536);
			fgets(StrLine, 65536, fp);
			tmpStr = StrLine;
			if (tmpStr.find_last_of(",") > tmpStr.size()) {
				tmpStr.append(",");
			}
			else {
				int posl = tmpStr.find_last_of(","), posr = tmpStr.size();
				std::string tmp = tmpStr.substr(posl, posr - posl);
				tmpStr = tmpStr.substr(0, posl + 1);
				long long num = 0;
				for (std::string::iterator it = tmp.begin(); it != tmp.end(); ++it) {
					if (*it >= '0' && *it <= '9')
						num = num * 10 + *it - '0';
				}
				if (num) st.push((double)(num / 1000000.0));
			}
			while (tmpStr.find_last_of(",") <= tmpStr.size()) {
				int posr = tmpStr.find_last_of(","), posl = 0;
				tmpStr.erase(posr, 1);
				if (tmpStr.find_last_of(",") <= tmpStr.size())
					posl = tmpStr.find_last_of(",") + 1;
				std::string tmp = tmpStr.substr(posl, posr - posl);
				tmpStr = tmpStr.substr(0, posl);
				long long num = 0;
				for (std::string::iterator it = tmp.begin(); it != tmp.end(); ++it) {
					if (*it >= '0' && *it <= '9')
						num = num * 10 + *it - '0';
				}
				st.push((double)(num / 1000000.0));
			}
			while (!st.empty()) {
				this->Data.push_back(st.top());
				st.pop();
			}
		}
		fclose(fp);
	}
	return this->Data;
}