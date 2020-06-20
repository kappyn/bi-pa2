#pragma once

#include "CSetQuery.hpp"
#include "../database/CDatabase.hpp"

class CUnion : public CSetQuery, public CTableQuery {
private:
	CTable * m_QueryResult = nullptr;
	string m_QuerySaveName;

public:
	explicit CUnion ( CDatabase & ref, const pair<string, string> & tableNames );
	virtual ~CUnion ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};
