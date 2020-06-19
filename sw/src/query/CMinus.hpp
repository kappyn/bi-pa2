#pragma once

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CMinus : public CTableQuery {
private:
	CDatabase & m_Database;
	pair<CQueryOperand, CQueryOperand> m_Operands;
	pair<string, string> m_TableNames;

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