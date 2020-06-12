#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
		: m_Database( ref ), m_QueryResult( nullptr ), m_SelectedCols( std::move( cols ) ),
		  m_TableName( std::move( tableName ) ) { }

CSelection::~CSelection ( ) {
	delete m_QueryResult;
}

/**
 * Evaluates the query and saves the result.
 * @return true if query was correct and result generated without errors.
 */
bool CSelection::Evaluate ( ) {
	CTable * tableRef;

	// check, if table exists
	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) == nullptr ) {
		CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH_TABLE );
		return false;
	}

	// check if columns exist => save the result
	return tableRef->GetSubTable( m_SelectedCols, ( m_QueryResult = new CTable { } ) );

}

CTable * CSelection::GetQueryResult ( ) {
	return m_QueryResult;
}