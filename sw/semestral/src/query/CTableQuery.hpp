#pragma once

#include "../console/CLog.hpp"
#include "../database/CTable.hpp"

class CTableQuery {
public:
	virtual ~CTableQuery( ) = default;
	virtual bool Evaluate ( ) const = 0;
};