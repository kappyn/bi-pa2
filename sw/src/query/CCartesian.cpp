#include "CCartesian.hpp"

CCartesian::CCartesian ( CDatabase & ref, const pair<string, string> & tableNames ) : CBinaryQuery( ref, std::make_pair( tableNames.first, tableNames.second ) ) { }

CCartesian::~CCartesian ( ) {
	delete m_QueryResult;
}

bool CCartesian::Evaluate ( ) {
	if ( ! SaveTableReferences( ) )
		return false;

	CTable * a = m_Operands.first.m_TRef;
	CTable * b = m_Operands.second.m_TRef;
	vector<string> colsA = a->GetColumnNames( );
	vector<string> colsB = b->GetColumnNames( );

	size_t tmp = 0;
	vector<string>::iterator it;
	for ( string & i : colsA ) {
		it = find( colsB.begin( ), colsB.end( ), i );
		if ( it == colsB.end( ) )
			continue;
		i = string( m_TableNames.first ).append( "." ).append( i );
		tmp = it - colsB.begin( );
		colsB.at( tmp ) = string( m_TableNames.second ).append( "." ).append( colsB.at( tmp ) );
	}

	colsA.insert( colsA.end( ), colsB.begin( ), colsB.end( ) );
	if ( ( a->GetColumnCount( ) + b->GetColumnCount( ) ) != colsA.size( ) )
		return false;

	m_QueryResult = new CTable ( colsA );

	vector<CCell *> x, y;
	size_t amax = a->GetRowCount( );
	size_t bmax = b->GetRowCount( );
	for ( size_t i = 1; i < amax; ++i ) {
		for ( size_t j = 1; j < bmax; ++j ) {
			if ( ! a->GetDeepRow( i, x ) || ! b->GetDeepRow( j, y ) )
				return false;
			m_QueryResult->InsertShallowRow( CTable::MergeRows( x, y ) );
			x.clear( );
			y.clear( );
		}
	}

	return true;
}

CTable * CCartesian::GetQueryResult ( ) {
	return m_QueryResult;
}

void CCartesian::ArchiveQueryName ( const string & name ) {
	if ( m_QuerySaveName != name )
		m_QuerySaveName = name;
}

string CCartesian::GetSQL ( ) const {
	if ( ! m_QueryResult )
		return "";

	CTableQuery * origin;
	string output;

	origin = m_Operands.first.m_Origin;
	output += string( "( SELECT " ).append( CLog::APP_COLOR_RESULT ).append( "*" ).append( CLog::APP_COLOR_RESET ).append( " FROM ");
	output += origin ? origin->GetSQL() : string( CLog::APP_COLOR_RESULT ).append( m_TableNames.first ).append( CLog::APP_COLOR_RESET );
	output += " CROSS JOIN ";
	origin = m_Operands.second.m_Origin;
	output += origin ? origin->GetSQL() : string( CLog::APP_COLOR_RESULT ).append( m_TableNames.second ).append( CLog::APP_COLOR_RESET ).append( " )");
	return output;
}

bool CCartesian::IsDerived ( ) const {
	return false;
}
