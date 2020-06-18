#ifndef CNATURALJOIN_H
#define CNATURALJOIN_H

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CQueryOperand {
public:
	CTableQuery * m_Origin = nullptr;
	CTable * m_QueryResult = nullptr;
	bool m_Derived         = false;
};

class CNaturalJoin : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	pair<string, string> m_TableNames;
	pair<CQueryOperand, CQueryOperand> m_Operands;

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