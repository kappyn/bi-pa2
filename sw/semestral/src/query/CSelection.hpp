#pragma once

#include "CTableQuery.hpp"
#include "../database/CTable.hpp"
#include "../database/CDatabase.hpp"

// Binary operator category - set operations such as cartesian product, intersection, etc.
class CSelection : public CTableQuery {
private:
	CDatabase & m_Database;
	vector<string> m_SelectedCols;
	string m_TableName;

public:
	CSelection ( CDatabase & ref, vector<string> cols, string tableName );

	virtual bool Evaluate ( ) const override;
};