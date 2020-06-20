#include "CDatabase.hpp"

/**
 * Constructor that initializes a database with a name.
 * @param[in] name name of the database
 */
CDatabase::CDatabase ( string name ) : m_Name( std::move( name ) ) {
	transform( m_Name.begin( ), m_Name.end( ), m_Name.begin( ), ::toupper );
}

CDatabase::~CDatabase ( ) {
	for ( const auto & i : m_TableData ) delete i.second;
	for ( const auto & i : m_QueryData ) delete i.second;
}

/**
 * Table insertion.
 * @param[in] tableName name of the new table
 * @param[in] tableRef reference to the table itself
 * @return true if table was inserted into database without any errors.
 */
bool CDatabase::InsertTable ( const string & tableName, CTable * tableRef ) {
	return m_TableData.insert( pair<string, CTable *>( tableName, tableRef ) ).second;
}

/**
 * Query insertion.
 * @param[in] queryName query save name
 * @param[in] ctqRef reference to the query itself
 * @return true if query was inserted into database without any errors.
 */
bool CDatabase::InsertQuery ( const string & queryName, CTableQuery * ctqRef ) {
	if ( TableExists( queryName ) || QueryExists ( queryName ) ) {
		CLog::BoldMsg( CLog::QP, queryName, CLog::QP_TABLE_EXISTS );
		return false;
	}
	return m_QueryData.insert( pair<string, CTableQuery *>( queryName, ctqRef ) ).second;
}

/**
 * Table existence check. Returns true if table with given table is present in the database.
 */
bool CDatabase::TableExists ( const string & tableName ) const {
	return m_TableData.find( tableName ) != m_TableData.end( );
}

/**
 * Query existence check. Returns true if table with given query is present in the database.
 */
bool CDatabase::QueryExists ( const string & tableName ) const {
	return m_QueryData.find( tableName ) != m_QueryData.end( );
}

/**
 * Searches trough the tables with given name. If found, pointer is returned.
 */
CTable * CDatabase::GetTable ( const string & tableName ) const {
	auto tmp = m_TableData.find( tableName );
	return tmp == m_TableData.end( ) ? nullptr : tmp->second;
}

/**
 * Searches trough the queries with given name. If found, pointer is returned.
 */
CTableQuery * CDatabase::GetTableQ ( const string & tableName ) const {
	auto tmp = m_QueryData.find( tableName );
	return tmp == m_QueryData.end( ) ? nullptr : tmp->second;
}

void CDatabase::ListTables ( ) const {
	CLog::Msg( m_Name, CLog::CON_LISTING_T );
	vector<string> tableColumns;
	string output;
	size_t columnCounter = 0, tableCounter = 0;
	for ( const auto & i : m_TableData ) {
		tableColumns = i.second->GetColumnNames( );
		output = '(';
		for ( const string & j : tableColumns ) {
			if ( columnCounter ++ != tableColumns.size( ) - 1 )
				output.append( j + ", " );
			else
				output.append( j );
		}
		columnCounter = 0;
		output += ')';
		CLog::BoldMsg( m_Name, to_string( ++ tableCounter ).append( ". " ).append( i.first ).append( " " ).append( output ), "" );
	}
}

void CDatabase::ListQueries ( ) const {
	CLog::Msg( m_Name, CLog::CON_LISTING_Q );
	vector<string> tableColumns;
	string output;
	size_t queryCounter = 0;
	for ( const auto & i : m_QueryData )
		CLog::BoldMsg( m_Name, to_string( ++ queryCounter ).append( ". " ).append( i.first ), "" );
}

void CDatabase::PrintTables ( ) const {
	CLog::Msg( m_Name, "Printing tables...", "\n" );
	for ( const auto & i : m_TableData ) {
		CLog::Msg( i.first, "Listing contents..", "" );
		cout << * i.second << endl;
	}
}
