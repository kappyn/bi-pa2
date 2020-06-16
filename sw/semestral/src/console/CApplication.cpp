#include "CApplication.hpp"

CApplication::CApplication ( const string & dbName, const string & srcName ) : m_Database( CDatabase { dbName } ),
                                                                               m_FileManager( srcName, m_Database )
{}

int CApplication::Run ( ) {
	try {
		if ( ! m_FileManager.LoadTables( ) )
			throw logic_error( CLog::QP_CON_PARSE_ERROR );
		m_Database.PrintTables( );
		return CConsole::Start( m_Database );
	} catch ( const logic_error & ) {
		CLog::Msg( CLog::APP, CLog::APP_EX_EXIT );
		return CConsole::EXCEPTION_ERROR;
	}
}