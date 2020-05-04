#pragma once

#include "CTable.hpp"
#include <string>
#include <map>

class CDatabase {
private:
	map<string, CTable> m_Data;

public:
	void PrintTables ( ) const noexcept;

	friend ostream & operator << ( ostream & ost, const CDatabase & cdb );
};