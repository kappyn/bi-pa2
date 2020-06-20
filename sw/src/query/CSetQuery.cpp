#include "CSetQuery.hpp"

/*
 * Searches trough the database and saves the table references.
 * If any error occurs (table not found, invalid columns, data types), false is returned.
 */
bool CSetQuery::SaveTableReferences ( ) {
	if ( ( m_Operands.first.m_TRef = m_Database.GetTable( m_TableNames.first ) ) != nullptr ) { }
	else if ( ( m_Operands.first.m_QRef = m_Database.GetTableQ( m_TableNames.first ) ) != nullptr ) {
		m_Operands.first.m_Origin = m_Operands.first.m_QRef;
		m_Operands.first.m_TRef = m_Operands.first.m_QRef->GetQueryResult( );
	} else
		return false;

	if ( m_Operands.first.m_TRef->HasDuplicateColumns( ) ) {
		CLog::Msg( CLog::QP, CLog::QP_DUP_COL );
		return false;
	}

	if ( ( m_Operands.second.m_TRef = m_Database.GetTable( m_TableNames.second ) ) != nullptr ) { }
	else if ( ( m_Operands.second.m_QRef = m_Database.GetTableQ( m_TableNames.second ) ) != nullptr ) {
		m_Operands.second.m_Origin = m_Operands.second.m_QRef;
		m_Operands.second.m_TRef = m_Operands.second.m_QRef->GetQueryResult( );
	} else
		return false;

	if ( m_Operands.second.m_TRef->HasDuplicateColumns( ) ) {
		CLog::Msg( CLog::QP, CLog::QP_DUP_COL );
		return false;
	}

	return true;
}

/*
 * Validates individual columns types of two operands.
 * Method returns true if all columns have same data types.
 */
bool CSetQuery::ValidateColumnTypes ( ) const {
	if ( ! m_Operands.first.m_TRef || ! m_Operands.second.m_TRef )
		return false;

	size_t tmp = m_Operands.first.m_TRef->GetColumnCount( );
	for ( size_t j = 0; j < tmp; ++ j ) {
		if ( m_Operands.first.m_TRef->GetColumnType( j ) != m_Operands.second.m_TRef->GetColumnType( j ) ) {
			CLog::Msg( CLog::QP, CLog::QP_DIFF_TYPE );
			return false;
		}
	}

	return true;
}
