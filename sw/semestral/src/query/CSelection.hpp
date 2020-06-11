#pragma once

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CSelection : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	vector<string> m_SelectedCols;
	string m_TableName;

public:
	CSelection ( CDatabase & ref, vector<string> cols, string tableName );
	virtual ~CSelection ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
};