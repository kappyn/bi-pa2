#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../database/CDatabase.hpp"
#include "CDataParser.hpp"
#include "../console/CLog.hpp"

using namespace std;

class CFileManager {
private:
	CDatabase & m_Database;
	ifstream m_ConfigStream;

public:
	explicit CFileManager ( const string & configFile, CDatabase & database );

	void LoadTables( );
};
