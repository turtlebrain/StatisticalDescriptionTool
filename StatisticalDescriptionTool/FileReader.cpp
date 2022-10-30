#include "FileReader.h"


CFileReader::CFileReader()
{
    //Default Constructor
}

CFileReader::~CFileReader()
{
}

bool CFileReader::ReadTableOfIntegers(string FileName)
{
    m_iDataSet.clear();
    vector<int> row;
    string line, word;
    bool FileIsRead = false;

    fstream file(FileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(stoi(word));
            m_iDataSet.push_back(row);
        }
        FileIsRead = true;
    }
    else {
        cout << "Could not open the file\n";
        FileIsRead = false;
    }
 
    return FileIsRead;
}

bool CFileReader::ReadTableOfDoubles(string FileName)
{
    m_dDataSet.clear();
    vector<double> row;
    string line, word;
    bool FileIsRead = false;

    fstream file(FileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(stod(word));
            m_dDataSet.push_back(row);
        }
        FileIsRead = true;
    }
    else {
        cout << "Could not open the file\n";
        FileIsRead = false;
    }

    return FileIsRead;
}

    
