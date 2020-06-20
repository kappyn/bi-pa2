#pragma once

#include "../database/CDatabase.hpp"

class CSetQuery {
public:
	CSetQuery( CDatabase & ref, const pair<string, string> & tableNames ) : m_Database( ref ), m_TableNames( tableNames ) { }

protected:
	CDatabase & m_Database;
	pair<CQueryOperand, CQueryOperand> m_Operands;
	pair<string, string> m_TableNames;

	bool SaveTableReferences ( );
	bool ValidateColumnTypes ( ) const;
};
