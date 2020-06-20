#include "CFileManager.hpp"

/**
 * Constructor that creates a stream based on the config file path.
 * @param[in, out] configFile path to the configuration file
 * @param[in, out] database reference to the database we're going to load to
 */
CFileManager::CFileManager ( const string & configFile, CDatabase & database ) : m_Database( database ) {
	m_ConfigStream.open( configFile, ios::in );
	if ( ! m_ConfigStream )
		throw std::logic_error( CLog::FM_CFG_NOT_FOUND );
	if ( m_ConfigStream.peek( ) == ifstream::traits_type::eof( ) )
		throw std::logic_error( CLog::FM_CFG_FAILED );
	CLog::Msg( CLog::FM, CLog::FM_CFG_FOUND );
}

/**
 * This method serves tasks to the parser module, validates correct number of imported tables.
 */
bool CFileManager::LoadTables ( ) {
	CLog::Msg( CLog::FM, CLog::FM_LOADING );
	string tableFilePath;
	ifstream tableFileStream { };
	size_t loadedTablesCnt = 0;

	while ( m_ConfigStream >> tableFilePath ) {
		tableFileStream.open( tableFilePath, ios::in );
		if ( tableFileStream && CDataParser::ParseTable( m_Database, tableFileStream, tableFilePath ) ) {
			CLog::BoldMsg( CLog::FM, tableFilePath, CLog::FM_IMPORT_OK );
			++ loadedTablesCnt;
		} else
			CLog::BoldMsg( CLog::FM, tableFilePath, CLog::FM_IMPORT_FAIL );
		tableFileStream.close( );
	}

	return loadedTablesCnt != 0;
}