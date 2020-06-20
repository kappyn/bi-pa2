#pragma once

#include "CBinaryQuery.hpp"
#include "../database/CDatabase.hpp"

/*
 * This query will subtract all rows from the table A which are also contained in table B.
 */
class CMinus : public CBinaryQuery, public CTableQuery {
private:
	CTable * m_QueryResult = nullptr;
	string m_QuerySaveName;

public:
	explicit CMinus ( CDatabase & ref, const pair<string, string> & tableNames );
	virtual ~CMinus ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};
