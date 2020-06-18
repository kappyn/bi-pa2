#ifndef CNATURALJOIN_H
#define CNATURALJOIN_H

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CQueryOperand {
public:
	CTable * m_TableRef      = nullptr;
	CTableQuery * m_QueryRef = nullptr;
	CTableQuery * m_Origin   = nullptr;
	bool m_Derived           = false;
};

class CNaturalJoin : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	CTableQuery * m_Origin = nullptr;
	bool m_Derived         = false;

	pair<string, string> m_TableNames;
	bool m_Resolved;

public:
	explicit CNaturalJoin ( CDatabase & ref, const pair<string, string> & tableNames );

	virtual ~CNaturalJoin ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual CTableQuery * GetOrigin ( ) override;
	virtual string GetQueryName ( ) const override;
	virtual void ArchiveQueryName ( const string & name ) override;

	virtual string GetSQL ( ) const override;

	virtual void SetQueryAsDerived ( ) override;
	virtual bool IsDerived ( ) const override;
};

#endif