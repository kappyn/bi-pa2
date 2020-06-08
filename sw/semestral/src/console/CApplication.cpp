#include "CApplication.hpp"

CApplication::CApplication ( const string & dbName, const string & srcName ) : m_Database( CDatabase{ dbName } ), m_FileManager( srcName, m_Database ) {
	m_FileManager.LoadTables( );
	m_Database.PrintTables( );
}
int CApplication::Run ( ) {
	return CConsole::Start( );
}