#include "CProjection.hpp"

CProjection::CProjection ( CDatabase & ref, string condition, string tableName )
		: m_Database( ref ),
		  m_QueryResult( nullptr ),
		  m_QuerySaveName( "" ),
		  m_QueryCondition( std::move( condition ) ),
		  m_TableName( std::move( tableName ) ),
		  m_Origin( nullptr ),
		  m_Derived( false ) { }

CProjection::~CProjection() {
	delete m_QueryResult;
}

bool CProjection::Evaluate ( ) {

	return false;
}