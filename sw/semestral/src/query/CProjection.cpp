#include "CProjection.hpp"

CProjection::CProjection ( CDatabase & ref, CCondition * conditionRef, string tableName )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_QueryCondition( conditionRef ),
		  m_TableName( std::move( tableName ) ),
		  m_Origin( nullptr ),
		  m_Derived( false ),
		  m_Resolved( false ) { }

CProjection::~CProjection ( ) {
	delete m_QueryResult;
	delete m_QueryCondition;
}

bool CProjection::Evaluate ( ) {
	CTable * tableRef;
	CTableQuery * queryRef;

	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) != nullptr ) {
		return
		( m_Resolved = tableRef->GetDeepTableCopy( m_QueryCondition, ( m_QueryResult = new CTable { tableRef->GetHeader( ) } ) ) );
	}
	else if ( ( queryRef = m_Database.GetTableQ( m_TableName ) ) != nullptr ) {
		m_Derived = true;
		m_Origin = queryRef;
		return
		( m_Resolved = queryRef->GetQueryResult( )->GetDeepTableCopy( m_QueryCondition, ( m_QueryResult = new CTable { queryRef->GetQueryResult( )->GetHeader( ) } ) ) );
	}
	else {
		return false;
	}
}

CTable * CProjection::GetQueryResult ( ) {
	return m_QueryResult;
}

CTableQuery * CProjection::GetOrigin ( ) {
	return m_Origin;
}

string CProjection::GetQueryName ( ) const {
	return std::__cxx11::string( );
}

void CProjection::ArchiveQueryName ( const string & name ) {

}

string CProjection::GenerateSQL ( const string & tmp ) const {
	if ( ! m_Resolved )
		return "";

	CTableQuery * origin = m_Origin;
	string output = CreateSQL( );
	size_t depth = 1;

	while ( origin != nullptr ) {
		output += origin->CreateSQL( );
		origin = origin->GetOrigin( );
		++ depth;
	}

	for ( ; ( depth - 1 ) > 0; -- depth )
		output += " )";
	return output;
}

string CProjection::CreateSQL ( ) const {
	string output = "( SELECT ";
	vector<string> header = m_QueryResult->GetColumnNames( );
	size_t max = header.size( );
	for ( size_t cnt = 0; cnt < max; ++ cnt )
		if ( cnt == max - 1 )
			output += string( CLog::APP_COLOR_RESULT ).append( header[ cnt ] ).append( CLog::APP_COLOR_RESET ) + "";
		else
			output += string( CLog::APP_COLOR_RESULT ).append( header[ cnt ]).append( CLog::APP_COLOR_RESET ) + ", ";
	output += " FROM " + string( m_Derived ? "" : string( CLog::APP_COLOR_RESULT ).append( m_TableName ).append( CLog::APP_COLOR_RESET ) );
	output += AppendWhereClause( );
	return output;
}

void CProjection::SetQueryAsDerived ( ) {
}

bool CProjection::IsDerived ( ) const {
	return false;
}
string CProjection::AppendWhereClause ( ) const {
	return string( " WHERE " )
	.append( m_QueryCondition->m_Column )
	.append( " " )
	.append( m_QueryCondition->m_Operator )
	.append( " " )
	.append( m_QueryCondition->m_Constant )
	.append( " )");
}
