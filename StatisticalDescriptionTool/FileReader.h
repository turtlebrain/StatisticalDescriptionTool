#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CFileReader
{
public:
	CFileReader();
	~CFileReader();

	bool ReadTableOfIntegers(string FileName);
	bool ReadTableOfDoubles(string FileName);
	vector<vector<int>> GetIntegersDataSet() const { return m_iDataSet; }
	vector<vector<double>> GetDoublesDataSet() const { return m_dDataSet; }

protected:
	vector<vector<int>> m_iDataSet;
	vector<vector<double>> m_dDataSet;
};

