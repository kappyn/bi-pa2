#include "CJoin.hpp"

#include <utility>

CJoin::CJoin ( CDatabase & ref, string column, const pair<string, string> & tableNames )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_TableNames( std::make_pair( tableNames.first, tableNames.second ) ),
		  m_CommonCol( std::move( column ) ),
		  m_Resolved( false ) { }

CJoin::~CJoin ( ) {
	delete m_QueryResult;
}

bool CJoin::Evaluate ( ) {
	if ( ( m_Operands.first.m_TableRef = m_Database.GetTable( m_TableNames.first ) ) != nullptr ) { }
	else if ( ( m_Operands.first.m_QueryRef = m_Database.GetTableQ( m_TableNames.first ) ) != nullptr ) {
		m_Operands.first.m_Origin   = m_Operands.first.m_QueryRef;
		m_Operands.first.m_TableRef = m_Operands.first.m_QueryRef->GetQueryResult( );
	} else {
		return false;
	}
	if ( m_Operands.first.m_TableRef->HasDuplicateColumns( ) ) {
		CLog::Msg( CLog::QP, CLog::QP_DUP_COL );
		return false;
	}
	if ( ( m_Operands.second.m_TableRef = m_Database.GetTable( m_TableNames.second ) ) != nullptr ) { }
	else if ( ( m_Operands.second.m_QueryRef = m_Database.GetTableQ( m_TableNames.second ) ) != nullptr ) {
		m_Operands.second.m_Origin   = m_Operands.second.m_QueryRef;
		m_Operands.second.m_TableRef = m_Operands.second.m_QueryRef->GetQueryResult( );
	} else {
		return false;
	}
	if ( m_Operands.second.m_TableRef->HasDuplicateColumns( ) ) {
		CLog::Msg( CLog::QP, CLog::QP_DUP_COL );
		return false;
	}

	size_t tmp;
	if ( ! m_Operands.first.m_TableRef->VerifyColumn( m_CommonCol, tmp ) || ! m_Operands.second.m_TableRef->VerifyColumn( m_CommonCol, tmp ) )
		return false;

	vector<pair<string, int>> newHeaderColumns { };
	newHeaderColumns.emplace_back( pair<string, int> { m_CommonCol, 2 } );
	for ( const auto & i : m_Operands.first.m_TableRef->GetColumnNames( ) )
		if ( i != m_CommonCol )
			newHeaderColumns.emplace_back( pair<string, int> { i, 1 } );

	for ( const auto & i : m_Operands.second.m_TableRef->GetColumnNames( ) )
		if ( i != m_CommonCol )
			newHeaderColumns.emplace_back( pair<string, int> { i, 0 } );

	vector<CCell *> tmpColumn;
	if ( ! m_Operands.first.m_TableRef->GetShallowCol( newHeaderColumns.at( 0 ).first, tmpColumn ) )
		return false;
	m_QueryResult = new CTable ( newHeaderColumns );

	vector<pair<size_t, size_t>> tableIndexes = m_Operands.second.m_TableRef->FindOccurences( tmpColumn );
	if ( tableIndexes.empty( ) ) {
		CLog::Msg( CLog::QP, CLog::QP_EMPTY_RESULTS );
		return false;
	}

	vector<string> colsA, colsB;
	for ( const auto & i : newHeaderColumns )
		if ( i.second == 2 || i.second == 1 )
			colsA.emplace_back( i.first );
		else
			colsB.emplace_back( i.first );

	tmpColumn.clear( );
	vector<CCell *> aPar, bPar;
	for ( const auto & i : tableIndexes ) {
		if ( colsB.empty( ) ) {
			if ( ! m_Operands.first.m_TableRef->GetDeepRow( i.first, colsA, aPar ) ) {
				for ( const auto & y : aPar ) delete y;
				for ( const auto & y : bPar ) delete y;
			}
		} else {
			if ( ! m_Operands.first.m_TableRef->GetDeepRow( i.first, colsA, aPar ) || ! m_Operands.second.m_TableRef->GetDeepRow( i.second, colsB, bPar ) ) {
				for ( const auto & y : aPar ) delete y;
				for ( const auto & y : bPar ) delete y;
				return false;
			}
		}
		tmpColumn = CTable::MergeRows( aPar, bPar );
		if ( ! m_QueryResult->InsertShallowRow( tmpColumn ) )
			return false;
	}

	size_t ex;
	for ( const auto & i : newHeaderColumns ) {
		if ( i.second == 1 || i.second == 2 ) {
			for ( const auto & j : newHeaderColumns ) {
				if ( j.second == 0 && i.first == j.first ) {
					m_QueryResult->VerifyColumn( i.first, ex );
					m_QueryResult->ChangeColumnName( ex, string( m_TableNames.first ).append( "." ).append( i.first ) );
					m_QueryResult->VerifyColumn( j.first, ex );
					m_QueryResult->ChangeColumnName( ex, string( m_TableNames.second ).append( "." ).append( i.first ) );
				}
			}
		} else {
			break;
		}
	}

	m_Resolved = true;
	return true;
}

CTable * CJoin::GetQueryResult ( ) {
	return m_QueryResult;
}

void CJoin::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CJoin::GetSQL ( ) const {
	if ( ! m_Resolved )
		return "";
	CTableQuery * origin;
	string output;

	origin = m_Operands.first.m_Origin;
	output += string( "( SELECT " ).append( CLog::APP_COLOR_RESULT ).append( "*" ).append( CLog::APP_COLOR_RESET ).append( " FROM ");
	output += origin ? origin->GetSQL() : string( CLog::APP_COLOR_RESULT ).append( m_TableNames.first ).append( CLog::APP_COLOR_RESET );
	output += " JOIN ";
	origin = m_Operands.second.m_Origin;
	output += origin ? origin->GetSQL() : string( CLog::APP_COLOR_RESULT ).append( m_TableNames.second ).append( CLog::APP_COLOR_RESET );
	output += string( " USING ( " ).append( CLog::APP_COLOR_RESULT ).append( m_CommonCol ).append( CLog::APP_COLOR_RESET ).append( " ) )" );

	return output;
}

bool CJoin::IsDerived ( ) const {
	return false;
}
