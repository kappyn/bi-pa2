#pragma once

#include "../console/CLog.hpp"
#include "../database/CTable.hpp"

class CTableQuery {
public:
	virtual ~CTableQuery ( ) = default;
	virtual bool Evaluate ( ) = 0;
	virtual CTable * GetQueryResult ( ) = 0;
	virtual bool SetQueryName ( const string & name ) = 0;
};