#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
		: m_Database( ref ), m_QueryResult( nullptr ), m_SelectedCols( std::move( cols ) ),
		  m_TableName( std::move( tableName ) ) { }

CSelection::~CSelection ( ) {
	delete m_QueryResult;
}

/**
 * Evaluates the query and saves the result.
 * @return true if all query prerequisities were fulfilled and this object holds the query result.
 */
bool CSelection::Evaluate ( ) {
	CTable * tableRef;
	CTableQuery * queryRef;

	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) != nullptr ) {
		return tableRef->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );
	}

	if ( ( queryRef = m_Database.GetTableQ( m_TableName ) ) != nullptr ) {
		return queryRef->GetQueryResult( )->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );
	}

	CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH_TABLE );
	return false;
}


CTable * CSelection::GetQueryResult ( ) {
	return m_QueryResult;
}