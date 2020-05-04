#include "CFileManager.hpp"

CFileManager::CFileManager ( const string & configFile, CDatabase & database ) : m_Database( database ) {
	m_ConfigStream.open( configFile, ios::in );
	if ( ! m_ConfigStream )
		throw std::logic_error( "FILE MANAGER ERROR: Configuration file not found.\n" );
	cout << "FILE MANAGER: " << "Configuration file found." << endl;
}

void CFileManager::LoadTables ( ) {
	cout << "FILE MANAGER: " << "Loading tables from the file into database..." << endl;
	string tableFilePath;
	ifstream tableFileStream { };

	while ( m_ConfigStream >> tableFilePath ) {
		tableFileStream.open( tableFilePath, ios::in );

		if ( tableFileStream && CDataParser::ParseTable( m_Database, tableFileStream ) )
			cout << "FILE MANAGER: " << "Successful import: " << tableFilePath << endl;
		else
			cout << "FILE MANAGER: " << "Incorrect format or doesn't exist: " << tableFilePath << endl;

		tableFileStream.close( );
	}
}