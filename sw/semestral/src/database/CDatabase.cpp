#include "CDatabase.hpp"

CDatabase::CDatabase ( const string & name ) : m_Name( name ) {
	transform( m_Name.begin( ), m_Name.end( ), m_Name.begin( ), ::toupper );
}

CDatabase::~CDatabase ( ) {
	for ( const auto & i : m_Data )
		delete i.second;
}

bool CDatabase::TableExists ( const string & tableName ) const {
	return m_Data.find( tableName ) != m_Data.end( );
}

bool CDatabase::InsertTable ( const string & tableName, CTable * tableRef ) {
	return m_Data.insert( pair<string, CTable *>( tableName, tableRef ) ).second;
}

void CDatabase::ListTables ( ) const {
	CLog::Msg( m_Name, "Listing tables..." );
	for ( const auto & i : m_Data )
		CLog::HighlightedMsg( m_Name, i.first, "" );
}

void CDatabase::PrintTables ( ) const {
	CLog::Msg( m_Name, "Printing tables..." );
	for ( const auto & i : m_Data )
		cout << * i.second;
}