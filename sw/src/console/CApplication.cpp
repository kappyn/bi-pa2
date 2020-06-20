#include "CApplication.hpp"

CApplication::CApplication ( const string & dbName, const string & srcName ) : m_Database( CDatabase { dbName } ), m_FileManager( srcName, m_Database ) { }

int CApplication::Run ( ) {
	try {
		if ( ! m_FileManager.LoadTables( ) )
			throw logic_error( CLog::FM_TABLE_PARSE_ERR );
//		m_Database.ListTables( );
		m_Database.PrintTables( );
		return CConsole::Start( m_Database );
	} catch ( const logic_error & ex ) {
		CLog::Msg( CLog::APP, CLog::APP_EX_EXIT );
		CLog::Msg( CLog::APP, ex.what( ) );
		return CConsole::EXCEPTION_ERROR;
	}
}