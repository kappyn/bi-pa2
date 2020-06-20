#include "CUnion.hpp"

CUnion::CUnion ( CDatabase & ref, const pair<string, string> & tableNames ) : CBinaryQuery( ref, std::make_pair( tableNames.first, tableNames.second ) ) { }

CUnion::~CUnion ( ) {
	delete m_QueryResult;
}

bool CUnion::Evaluate ( ) {
	if ( ! SaveTableReferences( ) )
		return false;

	vector<string> tmpHeader = m_Operands.first.m_TRef->GetColumnNames( );

	// save current indexes and their column values
	vector<pair<size_t, size_t>> columnOrders;
	vector<string> headerUnsorted;
	size_t max = tmpHeader.size( );
	for ( size_t i = 0; i < max; ++i ) {
		columnOrders.emplace_back( i, 0 );
		headerUnsorted.emplace_back( tmpHeader.at( i ) );
	}
	tmpHeader.clear( );

	// sort both table headers
	m_Operands.first.m_TRef->SortColumns( );
	m_Operands.second.m_TRef->SortColumns( );
	tmpHeader = m_Operands.first.m_TRef->GetColumnNames( );

	if ( ! m_Operands.first.m_TRef->HasIdenticalHeader( m_Operands.second.m_TRef ) ) {
		CLog::Msg( CLog::QP, CLog::QP_DIFF_HEADER );
		return false;
	}

	// correct data type check
	if ( ! ValidateColumnTypes( ) )
		return false;

	// save current indexes and their column values IN the new order
	size_t index = 0, tmp = 0;
	for ( const auto & i : headerUnsorted ) {
		tmp = find( tmpHeader.begin(), tmpHeader.end(), i ) - tmpHeader.begin( );
		if ( tmp >= headerUnsorted.size( ) )
			return false;
		columnOrders.at( index++ ).second = tmp;
	}

	m_QueryResult = new CTable ( m_Operands.first.m_TRef->GetColumnNames( ) );

	// transformation for easier row access
	vector<vector<CCell *>> tA, tB;
	CTable::SortRows( ( tA = m_Operands.first.m_TRef->Transform( ) ) );
	CTable::SortRows( ( tB = m_Operands.second.m_TRef->Transform( ) ) );
	vector<vector<CCell *>> res ( tA.size( ) + tB.size( ) );
	set_union( tA.begin( ), tA.end( ), tB.begin( ), tB.end( ), res.begin( ), CTable::RowComparator( ) );
	for ( const auto & i : res ) {
		if ( i.empty( ) )
			continue;
		if ( ! m_QueryResult->InsertShallowRow( CTable::GetDeepRow( i ) ) )
			return false;
	}

	m_QueryResult->SortColumns( columnOrders );
	m_Operands.first.m_TRef->SortColumns( columnOrders );
	m_Operands.second.m_TRef->SortColumns( columnOrders );

	return true;
}

CTable * CUnion::GetQueryResult ( ) {
	return m_QueryResult;
}

void CUnion::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CUnion::GetSQL ( ) const {
	if ( ! m_QueryResult )
		return "";

	CTableQuery * origin;
	string output;
	string tmp = string( "( SELECT " ).append( CLog::APP_COLOR_RESULT ).append( "*" ).append( CLog::APP_COLOR_RESET ).append( " FROM ");
	origin = m_Operands.first.m_Origin;
	output += "( ";
	output += origin ? origin->GetSQL( ) : string( tmp ).append( CLog::APP_COLOR_RESULT ).append( m_TableNames.first ).append( CLog::APP_COLOR_RESET ).append( " )");
	output += " UNION ";
	origin = m_Operands.second.m_Origin;
	output += origin ? origin->GetSQL( ) : string( tmp ).append( CLog::APP_COLOR_RESULT ).append( m_TableNames.second ).append( CLog::APP_COLOR_RESET ).append( " )");
	output += " )";
	return output;
}

bool CUnion::IsDerived ( ) const {
	return false;
}
