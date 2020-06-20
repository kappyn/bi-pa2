#pragma once

#include "CBinaryQuery.hpp"
#include "../database/CDatabase.hpp"

/*
 * This query connects two tables A, B based on all columns, that A and B share together (values must match as well).
 */
class CNaturalJoin : public CBinaryQuery, public CTableQuery {
private:
	CTable * m_QueryResult = nullptr;
	string m_QuerySaveName;

public:
	explicit CNaturalJoin ( CDatabase & ref, const pair<string, string> & tableNames );
	virtual ~CNaturalJoin ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};