#include "CJoin.hpp"

#include <utility>

CJoin::CJoin ( CDatabase & ref, string column, const pair<string, string> & tableNames )
: CBinaryQuery( ref, std::make_pair( tableNames.first, tableNames.second ) ), m_CommonCol( std::move( column ) ) { }

CJoin::~CJoin ( ) {
	delete m_QueryResult;
}

bool CJoin::Evaluate ( ) {
	if ( ! SaveTableReferences( ) )
		return false;

	size_t tmp;
	if ( ! m_Operands.first.m_TRef->VerifyColumn( m_CommonCol, tmp ) || ! m_Operands.second.m_TRef->VerifyColumn( m_CommonCol, tmp ) )
		return false;

	vector<pair<string, int>> newHeaderColumns { };
	newHeaderColumns.emplace_back( m_CommonCol, 2 );
	for ( const auto & i : m_Operands.first.m_TRef->GetColumnNames( ) )
		if ( i != m_CommonCol )
			newHeaderColumns.emplace_back( i, 1 );

	for ( const auto & i : m_Operands.second.m_TRef->GetColumnNames( ) )
		if ( i != m_CommonCol )
			newHeaderColumns.emplace_back( i, 0 );

	vector<CCell *> tmpColumn;
	if ( ! m_Operands.first.m_TRef->GetShallowCol( newHeaderColumns.at( 0 ).first, tmpColumn ) )
		return false;
	m_QueryResult = new CTable ( newHeaderColumns );

	vector<pair<size_t, size_t>> tableIndexes = m_Operands.second.m_TRef->FindOccurences( tmpColumn );

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
			if ( ! m_Operands.first.m_TRef->GetDeepRow( i.first, colsA, aPar ) ) {
				for ( const auto & y : aPar ) delete y;
				for ( const auto & y : bPar ) delete y;
			}
		} else {
			if ( ! m_Operands.first.m_TRef->GetDeepRow( i.first, colsA, aPar ) || ! m_Operands.second.m_TRef->GetDeepRow( i.second, colsB, bPar ) ) {
				for ( const auto & y : aPar ) delete y;
				for ( const auto & y : bPar ) delete y;
				return false;
			}
		}
		tmpColumn = CTable::MergeRows( aPar, bPar );
		if ( ! m_QueryResult->InsertShallowRow( tmpColumn ) )
			return false;
	}

	tmp = 0;
	for ( const auto & i : newHeaderColumns ) {
		if ( i.second == 1 || i.second == 2 ) {
			for ( const auto & j : newHeaderColumns ) {
				if ( j.second == 0 && i.first == j.first ) {
					m_QueryResult->VerifyColumn( i.first, tmp );
					m_QueryResult->ChangeColumnName( tmp, string( m_TableNames.first ).append( "." ).append( i.first ) );
					m_QueryResult->VerifyColumn( j.first, tmp );
					m_QueryResult->ChangeColumnName( tmp, string( m_TableNames.second ).append( "." ).append( i.first ) );
				}
			}
		} else {
			break;
		}
	}

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
	if ( ! m_QueryResult )
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
