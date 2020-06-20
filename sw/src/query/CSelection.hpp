#pragma once

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

/*
 * This query filters the existing columns to ones that the user provide.
 */
class CSelection : public CTableQuery {
private:
	CDatabase & m_Database;
	vector<string> m_SelectedCols;
	string m_TableName;

	CTable * m_QueryResult = nullptr;
	CTableQuery * m_Origin = nullptr;
	string m_QuerySaveName;
	bool m_Derived = false;

public:
	explicit CSelection ( CDatabase & ref, vector<string> cols, string tableName );
	virtual ~CSelection ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};