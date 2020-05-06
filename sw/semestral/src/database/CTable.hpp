#pragma once

#include "CCell.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

using namespace std;

class CTable {
private:
	vector<vector<CCell *>> m_Data;
	size_t m_Columns;

public:
	explicit CTable ( vector<CCell *> header );
	~CTable ( );
	void InsertRow ( vector<CCell *> row );
	size_t GetColumnCount ( ) const;
	vector<size_t> GetCellPadding ( ) const;
	const vector<CCell *> * GetRow ( const size_t & index ) const;
	void Render ( ostream & ost = cout ) const;
	void RenderRow ( ostream & ost = cout ) const;
	static void RenderSeparator ( const size_t & length, const char & rowchar, const string & pad, size_t & tmp, ostream & ost = cout ) ;
	friend ostream & operator << ( ostream & ost, const CTable & table );
};
