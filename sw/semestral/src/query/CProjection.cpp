#include "CProjection.hpp"

CProjection::CProjection ( CDatabase & ref, CCondition * conditionRef, string tableName )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_QueryCondition( conditionRef ),
		  m_TableName( std::move( tableName ) ),
		  m_Origin( nullptr ),
		  m_Derived( false ) { }

CProjection::~CProjection ( ) {
	delete m_QueryResult;
	delete m_QueryCondition;
}

bool CProjection::Evaluate ( ) {
	CTable * tableRef;
	CTableQuery * queryRef;

	if ( ( tableRef = m_Database.GetTable( m_TableName ) ) != nullptr ) {
		return tableRef
		->GetDeepTableCopy( m_QueryCondition, ( m_QueryResult = new CTable { tableRef->GetHeader( ) } ) );
	}
	else if ( ( queryRef = m_Database.GetTableQ( m_TableName ) ) != nullptr ) {
		m_Derived = true;
		m_Origin = queryRef;

		return queryRef
		->GetQueryResult( )
		->GetDeepTableCopy( m_QueryCondition, ( m_QueryResult = new CTable { tableRef->GetHeader( ) } ) );
	}
	else {
		return false;
	}
}

CTable * CProjection::GetQueryResult ( ) {
	return m_QueryResult;
}

CTableQuery * CProjection::GetOrigin ( ) {
	return nullptr;
}

string CProjection::GetQueryName ( ) const {
	return std::__cxx11::string( );
}

void CProjection::ArchiveQueryName ( const string & name ) {

}

string CProjection::GenerateSQL ( ) const {
	return std::__cxx11::string( );
}

string CProjection::CreateSQL ( ) const {
	return std::__cxx11::string( );
}

void CProjection::SetQueryAsDerived ( ) {
}

bool CProjection::IsDerived ( ) const {
	return false;
}