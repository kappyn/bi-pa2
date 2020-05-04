#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "CDatabase.hpp"
#include "CDataParser.hpp"

using namespace std;

class CFileManager {
private:
	CDatabase & m_Database;
	ifstream m_ConfigStream;

public:
	explicit CFileManager ( const string & configFile, CDatabase & database );

	void LoadTables( );
};
