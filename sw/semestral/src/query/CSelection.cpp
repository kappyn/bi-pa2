#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_SelectedCols( std::move( cols ) ),
		  m_TableName( std::move( tableName ) ),
		  m_Origin( nullptr ),
		  m_Derived( false )
{ }

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

		// mark current query as derived
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

string CSelection::GetQueryName ( ) {
	return m_QuerySaveName;
}

string CSelection::RetrieveSQL ( ) {
	if ( m_Origin ) {
		CTableQuery * origin = m_Origin;
		string output = "";
		while ( origin != nullptr ) {
			cout << origin->GetQueryName( );
			origin = origin->GetOrigin( );
		}
		return output;
	} else {
		return "No origin.";
	}

//	string output = string( CLog::APP_COLOR_RESULT ).append( "SELECT " );
//	for ( const auto & i : m_SelectedCols ) {
//		if ( i != * ( m_SelectedCols.end( ) - 1 ) )
//			output.append( i ).append( ", " );
//		else
//			output.append( i ).append( "\n" );
//	}

//	.append( "\n" )
//			.append( CLog::APP_COLOR_RESET );
//	return output;
}

/**
 * Flips the m_Derived flag.
 */
void CSelection::SetQueryAsDerived ( ) {
	m_Derived = true;
}
bool CSelection::IsDerived ( ) {
	return m_Derived;
}

