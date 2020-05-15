#include "CFileManager.hpp"

CFileManager::CFileManager ( const string & configFile, CDatabase & database ) : m_Database( database ) {
	m_ConfigStream.open( configFile, ios::in );
	if ( ! m_ConfigStream )
		throw std::logic_error( CLog::FM_CFG_NOT_FOUND );

	if ( m_ConfigStream.peek( ) == ifstream::traits_type::eof( ) )
		throw std::logic_error( CLog::FM_CFG_FAILED );

	CLog::Msg( CLog::FM, CLog::FM_CFG_FOUND );
}

void CFileManager::LoadTables ( ) {
	CLog::Msg( CLog::FM, CLog::FM_LOADING );
	string tableFilePath;
	ifstream tableFileStream { };

	int loadedTablesCnt = 0;
	while ( m_ConfigStream >> tableFilePath ) {
		tableFileStream.open( tableFilePath, ios::in );

		if ( tableFileStream && CDataParser::ParseTable( m_Database, tableFileStream, tableFilePath ) ) {
			CLog::HighlightedMsg( CLog::FM, tableFilePath, CLog::FM_IMPORT_OK );
			++ loadedTablesCnt;
		} else
			CLog::HighlightedMsg( CLog::FM, tableFilePath, CLog::FM_IMPORT_FAIL );
		tableFileStream.close( );
	}

	if ( ! loadedTablesCnt )
		throw std::logic_error( CLog::FM_CFG_FAILED );
}