#pragma once
#ifndef READER
#define READER
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;
vector<vector<float>> Alldata;
int Cubesize;
vector<string> split(const string& str, const string& pattern)
{
	//const char* convert to char*
	char* strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str());
	vector<string> resultVec;
	char* tmpStr = strtok(strc, pattern.c_str());
	while (tmpStr != NULL)
	{
		resultVec.push_back(string(tmpStr));
		tmpStr = strtok(NULL, pattern.c_str());
	}
	delete[] strc;
	return resultVec;
}
void readData() {
	ifstream fstream("data");
	string temp;
	vector<float>data;
	while (fstream >> temp) {
		vector<string> result = split(temp, ":");
		if (result.size() == 1) {
			Cubesize = atoi(result[0].c_str())+1;
			continue;
		}
		data.clear();
		for (int i = 0; i < 5; i++) {
			data.push_back(atof(result[i].c_str()));
		}
		Alldata.push_back(data);
	}
	
}

#endif // !READER
