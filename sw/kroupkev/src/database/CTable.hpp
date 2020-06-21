#pragma once

#include "CCell.hpp"
#include "CRenderSett.hpp"
#include "../console/CLog.hpp"
#include "CCondition.hpp"

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
	explicit CTable ( const vector<pair<string, int>> & header );
	explicit CTable ( ) = default;
	~CTable ( );

	struct RowComparator {
		bool operator ( ) ( const vector<CCell *> & v1, const vector<CCell *> & v2 ) const {
			for ( size_t i = 0; i < v1.size( ); ++i ) {
				if ( * v1[ i ] == * v2[ i ] )
					continue;
				else
					return * v1[ i ] < * v2[ i ];
			}
			return false;
		};
	};

	bool VerifyColumn ( const string & col, size_t & index, const bool & msg = false ) const;
	static bool VerifyColumn ( const string & col, size_t & index, vector<string> & columnSet );

	void SortColumns ( );
	void SortColumns ( vector<pair<size_t, size_t>> & columnOrders );

	static void SortRows ( vector<vector<CCell *>> & dataRef );
	static bool CompareRows ( const vector<CCell *> & a, const vector<CCell *> & b );
	bool SwapColumns ( const size_t & a, const size_t & b );

	bool HasDuplicateColumns ( ) const;
	bool HasIdenticalHeader ( const CTable * tableRef ) const;

	bool InsertShallowRow ( const vector<CCell *> & row );
	bool InsertDeepCol ( const vector<CCell *> & col );
	bool InsertDeepRow ( const size_t & index, CTable * outPtr ) const;
	bool ChangeColumnName ( const size_t & index, const string & s ) const;
	static vector<CCell *> MergeRows ( const vector<CCell *> & rowA, const vector<CCell *> & rowB );

	vector<vector<CCell *>> Transform ( ) const;
	vector<CCell *> GetDeepHeader ( ) const;
	bool GetShallowCol ( const string & name, vector<CCell *> & outRef ) const;
	bool GetDeepTable ( CCondition * condition, CTable * outPtr ) const;
	bool GetDeepRow ( const size_t & index, vector<CCell *> & outRef ) const;
	bool GetDeepRow ( const size_t & index, vector<string> & selectedColumns, vector<CCell *> & outRef ) const;
	static vector<CCell *> GetDeepRow ( const vector<CCell *> & outRef );
	bool GetSubTable ( const vector<string> & row, CTable * outPtr ) const;
	string GetColumnType ( const size_t & index ) const;

	vector<pair<size_t, size_t>> FindOccurences ( vector<vector<CCell *>> & columnsRef ) const;
	vector<pair<size_t, size_t>> FindOccurences ( vector<CCell *> & columnRef ) const;

	size_t GetColumnCount ( ) const;
	size_t GetRowCount( ) const;
	vector<string> GetColumnNames ( ) const;
	vector<size_t> GetCellPadding ( ) const;

	void Render ( ostream & ost = cout ) const;
	void Render ( vector<string> & out ) const;
	static void RenderSeparator ( const size_t & length, size_t & tmp, ostream & ost = cout );
	friend ostream & operator << ( ostream & ost, const CTable & table );
};