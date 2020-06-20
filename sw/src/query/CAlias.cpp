#include "CAlias.hpp"

CAlias::CAlias ( CDatabase & ref, const pair<string, string> & colNames, string tableName )
: m_Database( ref ), m_Columns( std::move( colNames ) ), m_TableName( std::move( tableName ) ) { }

/**
 * Evaluates the query.
 * @return true column was found, renamed and saved.
 */
bool CAlias::Evaluate ( ) {
	CTable * tableRef = nullptr;
	CTableQuery * queryRef = nullptr;
	size_t colIndex = 0;

	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) != nullptr ) {
		//
	}
	else if ( ( queryRef = m_Database.GetTableQ( m_TableName ) ) != nullptr ) {
		tableRef = queryRef->GetQueryResult( );
	}
	else {
		CLog::HighlightedMsg( CLog::QP, m_TableName, CLog::QP_NO_SUCH_TABLE );
		return false;
	}

	if ( ! tableRef->VerifyColumn( m_Columns.first, colIndex, false ) ) {
		CLog::HighlightedMsg( CLog::QP, m_Columns.first, CLog::QP_NO_SUCH_COL );
		return false;
	}

	if ( tableRef->VerifyColumn( m_Columns.second, colIndex, false ) ) {
		CLog::HighlightedMsg( CLog::QP, m_Columns.second, CLog::QP_COL_EXISTS );
		return false;
	}

	if ( ! tableRef->ChangeColumnName( colIndex, m_Columns.second ) )
		return false;

	string output = "( ALTER TABLE ";
	if ( queryRef != nullptr )
		output += queryRef->GetSQL( );
	else
		output += string( CLog::APP_COLOR_RESULT ).append( m_TableName ).append( CLog::APP_COLOR_RESET );

	output += string( " RENAME COLUMN " )
	.append( CLog::APP_COLOR_RESULT ).append( m_Columns.first ).append( CLog::APP_COLOR_RESET )
	.append( " TO ")
	.append( CLog::APP_COLOR_RESULT ).append( m_Columns.second ).append( CLog::APP_COLOR_RESET )
	.append( " )");

	cout << * tableRef;
	CLog::Msg( CLog::QP, output );
	return true;
}

CTable * CAlias::GetQueryResult ( ) {
	return nullptr;
}

void CAlias::ArchiveQueryName ( const string & name ) { }

string CAlias::GetSQL ( ) const {
	return "";
}

bool CAlias::IsDerived ( ) const {
	return false;
}