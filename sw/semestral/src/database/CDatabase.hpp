#pragma once

#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <map>

#include "CTable.hpp"
#include "../console/CLog.hpp"

/**
 * This module is the upper layer of data storage, containing all the loaded data.
 */
class CDatabase {
private:
	string m_Name;
	map<string, CTable *> m_Data;

public:
	explicit CDatabase ( string name );
	~CDatabase ( );
	bool TableExists ( const string & tableName ) const;
	bool InsertTable ( const string & tableName, CTable * tableRef );
	void ListTables ( ) const;
	void PrintTables ( ) const;
};