#pragma once

#include "CBinaryQuery.hpp"
#include "../database/CDatabase.hpp"

/*
 * This query will create a table based on cartesian product of tables A and B.
 */
class CCartesian : public CBinaryQuery, public CTableQuery {
private:
	CTable * m_QueryResult = nullptr;
	string m_QuerySaveName;

public:
	explicit CCartesian ( CDatabase & ref, const pair<string, string> & tableNames );
	virtual ~CCartesian ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};
