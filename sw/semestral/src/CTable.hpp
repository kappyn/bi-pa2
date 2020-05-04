#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

class CTable {
private:
	string m_Name;
	map<string, vector<string>> m_Header;
	map<string, vector<string>> m_Data;
	size_t m_Columns;

public:
	bool ColumnExists ( const string & name ) const;

	bool InsertHeader ( );
	bool InsertRow ( );
	size_t ColumnCount ( ) const;
};
