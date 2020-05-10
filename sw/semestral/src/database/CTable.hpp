#pragma once

#include "CCell.hpp"
#include "CRenderSett.hpp"
#include "../console/CLog.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

using namespace std;

class CTable {
private:
	vector<vector<CCell *>> m_Data;

public:
	explicit CTable ( const vector<CCell *> & header );
	~CTable ( );

	bool InsertRow ( const vector<CCell *> & row );
	size_t GetColumnCount ( ) const;
	vector<size_t> GetCellPadding ( ) const;

	void Render ( ostream & ost = cout ) const;
	vector<CCell *> RenderCol ( const size_t & index, ostream & ost = cout ) const;
	static void RenderSeparator ( const size_t & length, size_t & tmp, ostream & ost = cout );
	friend ostream & operator << ( ostream & ost, const CTable & table );
};
