#pragma once

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CProjection : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	string m_QueryCondition;
	string m_TableName;

	CTableQuery * m_Origin;
	bool m_Derived;

public:
	explicit CProjection ( CDatabase & ref, string condition, string tableName );

	virtual ~CProjection ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual CTableQuery * GetOrigin ( ) override;
	virtual string GetQueryName ( ) const override;
	virtual void ArchiveQueryName ( const string & name ) override;

	virtual string GenerateSQL ( ) const override;
	virtual string CreateSQL ( ) const override;

	virtual void SetQueryAsDerived ( ) override;
	virtual bool IsDerived ( ) const override;
};
