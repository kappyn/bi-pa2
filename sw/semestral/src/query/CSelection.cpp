#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_SelectedCols( std::move( cols ) ),
		  m_TableName( std::move( tableName ) ),
		  m_Origin( nullptr ),
		  m_Derived( false ) { }

CSelection::~CSelection ( ) {
	delete m_QueryResult;
}

/**
 * Evaluates the query and saves the result.
 * @return true if all query prerequisities were fulfilled and this object holds the query result.
 */
bool CSelection::Evaluate ( ) {
	// first, search trough all the tables
	CTable * tableRef;
	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) != nullptr ) {
		return tableRef->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );
	}

	// if table wasn't found, search trough the queries
	CTableQuery * queryRef;
	if ( ( queryRef = m_Database.GetTableQ( m_TableName ) ) != nullptr ) {

		// mark current query as derived from other query
		m_Derived = true;

		// save the previous saved query
		m_Origin = queryRef;

		// save the query result
		return queryRef->GetQueryResult( )->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );
	}

	CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH_TABLE );
	return false;
}

CTable * CSelection::GetQueryResult ( ) {
	return m_QueryResult;
}

CTableQuery * CSelection::GetOrigin ( ) {
	return m_Origin;
}

string CSelection::GetQueryName ( ) const {
	return m_QuerySaveName;
}

void CSelection::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CSelection::GenerateSQL ( const string & ) const {
	CTableQuery * origin = m_Origin;
	string output = CreateSQL( );
	size_t depth = 1;

	while ( origin != nullptr ) {
		output += origin->CreateSQL( );
		origin = origin->GetOrigin( );
		++ depth;
	}
	for ( ; depth > 0; -- depth )
		output += " )";

	return output;
}

string CSelection::CreateSQL ( ) const {
	string output = "( SELECT ";
	size_t max = m_SelectedCols.size( );
	for ( size_t cnt = 0; cnt < max; ++ cnt )
		if ( cnt == max - 1 )
			output += string( CLog::APP_COLOR_RESULT ).append( m_SelectedCols[ cnt ] ).append( CLog::APP_COLOR_RESET ) + "";
		else
			output += string( CLog::APP_COLOR_RESULT ).append( m_SelectedCols[ cnt ]).append( CLog::APP_COLOR_RESET ) + ", ";
	output += " FROM " + string( m_Derived ? "" : string( CLog::APP_COLOR_RESULT ).append( m_TableName ).append( CLog::APP_COLOR_RESET ) );
	return output;
}

void CSelection::SetQueryAsDerived ( ) {
	m_Derived = true;
}
bool CSelection::IsDerived ( ) const {
	return m_Derived;
}

