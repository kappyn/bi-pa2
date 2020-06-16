#ifndef CDATABASE_H
#define CDATABASE_H

#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <map>

#include "CTable.hpp"
#include "../console/CLog.hpp"
#include "../query/CTableQuery.hpp"

/**
 * This module is the upper layer of data storage, containing all the loaded data.
 */
class CDatabase {
private:
	string m_Name;
	map<string, CTable *> m_TableData;
	map<string, CTableQuery *> m_QueryData;

public:
	explicit CDatabase ( string name );
	~CDatabase ( );

	CTable * GetTable ( const string & tableName ) const;
	CTableQuery * GetTableQ ( const string & tableName ) const;

	bool TableExists ( const string & tableName ) const;
	bool QueryExists ( const string & tableName ) const;
	bool InsertTable ( const string & tableName, CTable * tableRef );

	bool InsertQuery ( const string & queryName, CTableQuery * ctqRef );
	void ListTables ( ) const;
	void ListQueries ( ) const;
	void PrintTables ( ) const;

	void WipeAllData( ) {
		for ( const auto & i : m_TableData )
			delete i.second;

		for ( const auto & i : m_QueryData )
			delete i.second;
	}
};

#endif