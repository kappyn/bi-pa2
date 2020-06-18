#ifndef CSELECTION_H
#define CSELECTION_H

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CSelection : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	vector<string> m_SelectedCols;
	string m_TableName;

	CTableQuery * m_Origin;
	bool m_Derived;

public:
	explicit CSelection ( CDatabase & ref, vector<string> cols, string tableName );

	virtual ~CSelection ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual string GetQueryName ( ) const override;
	virtual void ArchiveQueryName ( const string & name ) override;

	virtual string GetSQL ( ) const override;

	virtual void SetQueryAsDerived ( ) override;
	virtual bool IsDerived ( ) const override;
};

#endif