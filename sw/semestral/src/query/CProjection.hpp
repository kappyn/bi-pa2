#ifndef CPROJECTION_H
#define CPROJECTION_H

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"
#include "../database/CCondition.hpp"

class CProjection : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	CCondition * m_QueryCondition;
	string m_TableName;

	CTableQuery * m_Origin;
	bool m_Derived;
	bool m_Resolved;

	string AppendWhereClause ( ) const;

public:
	explicit CProjection ( CDatabase & ref, CCondition * conditionRef, string tableName );

	virtual ~CProjection ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual CTableQuery * GetOrigin ( ) override;
	virtual string GetQueryName ( ) const override;
	virtual void ArchiveQueryName ( const string & name ) override;

	virtual string GenerateSQL ( const string & tmp ) const override;
	virtual string CreateSQL ( ) const override;

	virtual void SetQueryAsDerived ( ) override;
	virtual bool IsDerived ( ) const override;
};

#endif