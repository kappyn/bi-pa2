#pragma once

#include "../database/CDatabase.hpp"
#include "../tool/CFileManager.hpp"
#include "CConsole.hpp"

/**
 * This module is the main driver for the whole application.
 */
class CApplication {
private:
	CDatabase m_Database;
	CFileManager m_FileManager;

public:
	CApplication ( const string & dbName, const string & srcName );
	int Run ( );
};
