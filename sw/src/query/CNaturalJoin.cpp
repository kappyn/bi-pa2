#include "CNaturalJoin.hpp"

CNaturalJoin::CNaturalJoin ( CDatabase & ref, const pair<string, string> & tableNames )
: CBinaryQuery( ref, std::make_pair( tableNames.first, tableNames.second ) ) { }

CNaturalJoin::~CNaturalJoin ( ) {
	delete m_QueryResult;
}

bool CNaturalJoin::Evaluate ( ) {
	if ( ! SaveTableReferences( ) )
		return false;

	vector<string> colsA = m_Operands.first.m_TRef->GetColumnNames( );
	vector<string> colsB = m_Operands.second.m_TRef->GetColumnNames( );

	// search for common columns
	// 0 for independent right columns
	// 1 for independent left columns
	// 2 for common columns
	vector<pair<string, int>> newHeaderColumns;
	size_t commonColCnt = 0;
	bool foundCommon = false;

	for ( const string & i : colsA )
		for ( const string & j : colsB )
			if ( i == j ) {
				newHeaderColumns.emplace_back( pair<string, int> { j, 2 } );
				++ commonColCnt;
			}
	if ( commonColCnt == 0 ) {
		CLog::Msg( CLog::QP, CLog::QP_NO_COMMON_COL );
		return false;
	}
	for ( const string & i : colsA ) {
		for ( const auto & j : newHeaderColumns ) {
			if ( j.first == i ) {
				foundCommon = true;
				break;
			}
		}
		if ( ! foundCommon )
			newHeaderColumns.emplace_back( pair<string, int> { i, 1 } );
		foundCommon = false;
	}
	for ( const string & i : colsB ) {
		for ( const auto & j : newHeaderColumns ) {
			if ( j.first == i ) {
				foundCommon = true;
				break;
			}
		}
		if ( ! foundCommon )
			newHeaderColumns.emplace_back( pair<string, int> { i, 0 } );
		foundCommon = false;
	}
	m_QueryResult = new CTable ( newHeaderColumns );

	vector<vector<CCell *>> requiredColumns;
	vector<CCell *> tmp;
	for ( const auto & i : newHeaderColumns ) {
		if ( i.second == 2 ) {
			if ( ! m_Operands.first.m_TRef->GetShallowCol( i.first, tmp ) )
				return false;
			requiredColumns.emplace_back( tmp );
		}
	}

	tmp.clear( );
	colsA.clear( );
	colsB.clear( );

	vector<pair<size_t, size_t>> tableIndexes = m_Operands.second.m_TRef->FindOccurences( requiredColumns );
	if ( tableIndexes.empty( ) ) {
		CLog::Msg( CLog::QP, CLog::QP_EMPTY_RESULTS );
		return false;
	}

	for ( const auto & i : newHeaderColumns ) {
		if ( i.second == 2 || i.second == 1 )
			colsA.emplace_back( i.first );
		else
			colsB.emplace_back( i.first );
	}

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
		tmp = CTable::MergeRows( aPar, bPar );
		if ( ! m_QueryResult->InsertShallowRow( tmp ) )
			return false;
	}

	return true;
}

CTable * CNaturalJoin::GetQueryResult ( ) {
	return m_QueryResult;
}

void CNaturalJoin::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CNaturalJoin::GetSQL ( ) const {
	if ( ! m_QueryResult )
		return "";

	CTableQuery * origin;
	string output;

	origin = m_Operands.first.m_Origin;
	output += string( "( SELECT " ).append( CLog::APP_COLOR_RESULT ).append( "*" ).append( CLog::APP_COLOR_RESET ).append( " FROM ");
	output += origin ? origin->GetSQL() : string( CLog::APP_COLOR_RESULT ).append( m_TableNames.first ).append( CLog::APP_COLOR_RESET );
	output += " NATURAL JOIN ";
	origin = m_Operands.second.m_Origin;
	output += origin ? origin->GetSQL() : string( CLog::APP_COLOR_RESULT ).append( m_TableNames.second ).append( CLog::APP_COLOR_RESET );
	output += " )";
	return output;
}

bool CNaturalJoin::IsDerived ( ) const {
	return false;
}