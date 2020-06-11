#pragma once

#include "CCell.hpp"
#include "CRenderSett.hpp"
#include "../console/CLog.hpp"

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * This module is the most essential part for the database. Forms tables with their respective cells.
 */
class CTable {
private:
	vector<vector<CCell *>> m_Data;

public:
	explicit CTable ( const vector<CCell *> & header );
	explicit CTable ( const vector<string> & header );
	explicit CTable ( ) = default;

	~CTable ( );

	bool VerifyColumns ( const vector<string> & cols ) const;
	bool InsertShallowRow ( const vector<CCell *> & row );
	bool InsertDeepCol ( const vector<CCell *> & col );

	bool GetSubTable ( const vector<string> & row, CTable * outPtr ) const;
	size_t GetColumnCount ( ) const;
	vector<string> GetColumnNames ( ) const;
	vector<size_t> GetCellPadding ( ) const;
	void DeleteData( );

	void Render ( ostream & ost = cout ) const;
	vector<CCell *> RenderCol ( const size_t & index, ostream & ost = cout ) const;
	static void RenderSeparator ( const size_t & length, size_t & tmp, ostream & ost = cout );
	friend ostream & operator << ( ostream & ost, const CTable & table );
};
