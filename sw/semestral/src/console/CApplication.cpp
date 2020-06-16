#include "CApplication.hpp"

CApplication::CApplication ( const string & dbName, const string & srcName ) : m_Database( CDatabase { dbName } ),
                                                                               m_FileManager( srcName, m_Database ) {
	if ( ! m_FileManager.LoadTables( ) )
		throw logic_error( "tet" );
	m_Database.PrintTables( );
}

int CApplication::Run ( ) {
	return CConsole::Start( m_Database );
}