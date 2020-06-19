#ifndef CJOIN_H
#define CJOIN_H

#include "CTableQuery.hpp"
#include "../database/CDatabase.hpp"

class CJoin : public CTableQuery {
private:
	CDatabase & m_Database;
	CTable * m_QueryResult;
	string m_QuerySaveName;

	pair<CQueryOperand, CQueryOperand> m_Operands;
	pair<string, string> m_TableNames;
	string m_CommonCol;
	bool m_Resolved;

public:
	explicit CJoin ( CDatabase & ref, string  column, const pair<string, string> & tableNames );

	virtual ~CJoin ( ) override;

	virtual bool Evaluate ( ) override;
	virtual CTable * GetQueryResult ( ) override;
	virtual void ArchiveQueryName ( const string & name ) override;
	virtual string GetSQL ( ) const override;
	virtual bool IsDerived ( ) const override;
};

#endif