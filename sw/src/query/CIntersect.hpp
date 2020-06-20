#pragma once

#include "CSetQuery.hpp"
#include "../database/CDatabase.hpp"

class CIntersect : public CSetQuery, public CTableQuery {
private:
	CTable * m_QueryResult = nullptr;
	string m_QuerySaveName;

public:
	explicit CIntersect ( CDatabase & ref, const pair<string, string> & tableNames );
	virtual ~CIntersect ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};
