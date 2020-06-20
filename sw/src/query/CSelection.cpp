#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
: m_Database( ref ), m_SelectedCols( std::move( cols ) ), m_TableName( std::move( tableName ) ) { }

CSelection::~CSelection ( ) {
	delete m_QueryResult;
}

/**
 * Evaluates the query and saves the result.
 * @return true if all query prerequisities were fulfilled and this object holds the query result.
 */
bool CSelection::Evaluate ( ) {
	CTable * tableRef;
	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) != nullptr ) {
		return tableRef->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );
	}
	CTableQuery * queryRef;
	if ( ( queryRef = m_Database.GetTableQ( m_TableName ) ) != nullptr ) {
		m_Derived = true;
		m_Origin = queryRef;
		return queryRef->GetQueryResult( )->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );
	}
	CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH_TABLE );
	return false;
}

CTable * CSelection::GetQueryResult ( ) {
	return m_QueryResult;
}

void CSelection::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CSelection::GetSQL ( ) const {
	if ( ! m_QueryResult )
		return "";

	CTableQuery * origin = m_Origin;
	string output = "( SELECT ";
	vector<string> header = m_QueryResult->GetColumnNames( );
	size_t max = header.size( );

	for ( size_t cnt = 0; cnt < max; ++ cnt )
		if ( cnt == max - 1 )
			output += string( CLog::APP_COLOR_RESULT ).append( header[ cnt ] ).append( CLog::APP_COLOR_RESET ) + "";
		else
			output += string( CLog::APP_COLOR_RESULT ).append( header[ cnt ]).append( CLog::APP_COLOR_RESET ) + ", ";
	output += " FROM " + string( m_Derived ? "" : string( CLog::APP_COLOR_RESULT ).append( m_TableName ).append( CLog::APP_COLOR_RESET ) );

	if ( origin )
		output += origin->GetSQL( );
	output += " )";
	return output;
}

bool CSelection::IsDerived ( ) const {
	return m_Derived;
}