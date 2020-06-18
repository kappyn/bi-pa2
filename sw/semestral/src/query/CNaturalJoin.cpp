#include "CNaturalJoin.hpp"

#include <utility>

CNaturalJoin::CNaturalJoin ( CDatabase & ref, const pair<string, string> & tableNames )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_TableNames( std::make_pair( tableNames.first, tableNames.second ) ),
		  m_Operands( ),
		  m_Resolved( false ) { }

CNaturalJoin::~CNaturalJoin ( ) {
	delete m_Operands.first.m_QueryResult;
	delete m_Operands.second.m_QueryResult;
}

bool CNaturalJoin::Evaluate ( ) {
	vector<string> columnsA, columnsB;

	CTable * tableRef = nullptr;
	if ( ( tableRef = m_Database.GetTable( m_TableNames.first ) ) != nullptr ) {
//		m_Operands.first.m_QueryResult = new CTable { tableRef->GetDeepHeader( ) };
		cout << "table A found!" << endl;
			
		if ( tableRef->HasDuplicateColumns( ) ) {
			CLog::Msg( CLog::QP, CLog::QP_DUP_COL );
			return false;
		}
		columnsA = tableRef->GetColumnNames( );
	} else {
		return false;
	}

	if ( ( tableRef = m_Database.GetTable( m_TableNames.second ) ) != nullptr ) {
		columnsB = tableRef->GetColumnNames( );
		cout << "table B found!" << endl;

		if ( tableRef->HasDuplicateColumns( ) ) {
			CLog::Msg( CLog::QP, CLog::QP_DUP_COL );
			return false;
		}
	} else {
		return false;
	}

	// 0 for independent right columns
	// 1 for independent left columns
	// 2 for common columns
	vector<pair<string, int>> newHeaderColumns;
	size_t commonColCnt = 0;
	bool foundCommon = false;

	for ( const string & i : columnsA )
		for ( const string & j : columnsB )
			if ( i == j ) {
				newHeaderColumns.emplace_back( pair<string, int> { j, 2 } );
				++ commonColCnt;
			}
	if ( commonColCnt == 0 ) {
		CLog::Msg( CLog::QP, CLog::QP_NO_COMMON_COL );
		return false;
	}

	for ( const string & i : columnsA  ) {
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
	for ( const string & i : columnsB  ) {
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

	for ( const auto & i : newHeaderColumns )
		cout << i.first <<  " " << i.second << endl;

//	vector<string> newTableCols;
//	pair<vector<string>, vector<string>> independentColumns;
//	vector<string> commonColumns;
//
//	for ( const string & i : columnsA ) {
//		for ( const string & j : columnsB ) {
//			if ( i == j ) {
//				newTableCols.push_back( i );
//				break;
//			}
//		}
//	}
//
//	bool commonFound = false;
//	for ( const string & i : columnsA ) {
//		for ( const string & j : newTableCols )
//			commonFound = true;
//
//		if ( ! commonFound ) {
//			independentColumns
//		}
//	}

	return true;
}

CTable * CNaturalJoin::GetQueryResult ( ) {
	return nullptr;
}

CTableQuery * CNaturalJoin::GetOrigin ( ) {
	return nullptr;
}

string CNaturalJoin::GetQueryName ( ) const {
	return m_QuerySaveName;
}

void CNaturalJoin::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CNaturalJoin::GetSQL ( ) const {
	return "";
}

void CNaturalJoin::SetQueryAsDerived ( ) {
}

bool CNaturalJoin::IsDerived ( ) const {
	return false;
}