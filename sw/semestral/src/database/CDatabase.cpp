#include "CDatabase.hpp"

/**
 * Constructor that initializes a database with a name.
 * @param[in] name name of the database.
 */
CDatabase::CDatabase ( string name ) : m_Name( std::move( name ) ) {
	transform( m_Name.begin( ), m_Name.end( ), m_Name.begin( ), ::toupper );
}

/**
 * Destructor that frees up all the tables. The tables themselves also have their own destructor.
 */
CDatabase::~CDatabase ( ) {
	for ( const auto & i : m_Data )
		delete i.second;
}

/**
 * Table insertion.
 * @param[in] tableName name of the new table
 * @param[in] tableRef reference to the table itself
 * @return true if table was inserted into database without any errors.
 */
bool CDatabase::InsertTable ( const string & tableName, CTable * tableRef ) {
	return m_Data.insert( pair<string, CTable *>( tableName, tableRef ) ).second;
}

/**
 * Table existence check.
 * @return true if table with given name is present in the database
 */
bool CDatabase::TableExists ( const string & tableName ) const {
	return m_Data.find( tableName ) != m_Data.end( );
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