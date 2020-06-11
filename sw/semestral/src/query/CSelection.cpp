#include "CSelection.hpp"

CSelection::CSelection ( CDatabase & ref, vector<string> cols, string tableName )
		: m_Database( ref ), m_QueryResult( nullptr ), m_SelectedCols( std::move( cols ) ),
		  m_TableName( std::move( tableName ) ) { }

/**
 * Evaluates the query and saves the result.
 * @return true if query was correct and result generated without errors.
 */
bool CSelection::Evaluate ( ) const {
	CTable * tableRef;

	// 1. check, if table exists
	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) == nullptr ) {
		CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH_TABLE );
		return false;
	}

	// 2. check, if columns exist
	if ( ! tableRef->VerifyColumns( m_SelectedCols ) )
		return false;

	// 3. save the result
//	m_QueryResult = new CTable ( m_SelectedCols );



	return true;
}