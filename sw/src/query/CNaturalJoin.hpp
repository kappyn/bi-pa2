#ifndef CNATURALJOIN_H
#define CNATURALJOIN_H

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CNaturalJoin : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	pair<CQueryOperand, CQueryOperand> m_Operands;
	pair<string, string> m_TableNames;
	bool m_Resolved;

public:
	explicit CNaturalJoin ( CDatabase & ref, const pair<string, string> & tableNames );

	virtual ~CNaturalJoin ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};

#endif