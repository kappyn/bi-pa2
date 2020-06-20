#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../database/CDatabase.hpp"
#include "CDataParser.hpp"
#include "../console/CLog.hpp"

using namespace std;

/**
 * The purpose of this module is to save loaded data into memory.
 * The procedure details are delegated to the lower level modules.
 */
class CFileManager {
private:
	CDatabase & m_Database;
	ifstream m_ConfigStream;

public:
	explicit CFileManager ( const string & configFile, CDatabase & database );
	bool LoadTables( );
};
