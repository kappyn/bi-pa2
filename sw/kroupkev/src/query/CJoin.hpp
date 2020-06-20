#pragma once

#include <utility>

#include "CBinaryQuery.hpp"
#include "../database/CDatabase.hpp"

/*
 * This query connect two tables A, B based on matching values in the column C.
 * Each table must have column C.
 */
class CJoin : public CBinaryQuery, public CTableQuery {
private:
	CTable * m_QueryResult = nullptr;
	string m_QuerySaveName;
	string m_CommonCol;

public:
	explicit CJoin ( CDatabase & ref, string column, const pair<string, string> & tableNames );
	virtual ~CJoin ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};