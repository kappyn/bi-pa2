#pragma once

#include <utility>

#include "CBinaryQuery.hpp"
#include "../database/CDatabase.hpp"

/*
 * This query will rename a single column of a given table.
 */
class CAlias : public CTableQuery {
private:
	CDatabase & m_Database;
	pair<string, string> m_Columns;
	string m_TableName;

public:
	explicit CAlias ( CDatabase & ref, const pair<string, string> & colNames, string tableName );
	virtual ~CAlias ( ) override = default;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};