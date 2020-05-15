#pragma once

#include "../database/CDatabase.hpp"
#include "../tools/CFileManager.hpp"
#include "CConsole.hpp"

class CApplication {
private:
	CDatabase m_Database;
	CFileManager m_FileManager;

public:
	CApplication ( const string & dbName, const string & srcName ) : m_Database( CDatabase{ dbName } ), m_FileManager( srcName, m_Database ) {
		m_FileManager.LoadTables( );
		m_Database.PrintTables( );
	}

	int Run ( ) {
		return CConsole::Start( );
	}
};
