#pragma once

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"
#include "../database/CCondition.hpp"

/*
 * This query will filter rows of a table based on given condition.
 */
class CProjection : public CTableQuery {
private:
	CDatabase & m_Database;
	CCondition * m_QueryCondition;
	string m_TableName;

	CTable * m_QueryResult = nullptr;
	CTableQuery * m_Origin = nullptr;
	string m_QuerySaveName;
	bool m_Derived = false;

	string AppendWhereClause ( ) const;

public:
	explicit CProjection ( CDatabase & ref, CCondition * conditionRef, string tableName );
	virtual ~CProjection ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};