#include "CTable.hpp"

CTable::CTable ( const vector<CCell *> & header ) {
	m_Data.reserve( header.size( ) );
	for ( const auto & i : header )
		m_Data.push_back( vector<CCell *> { i } );
}

CTable::CTable ( const vector<string> & header ) {
	m_Data.reserve( header.size( ) );
	for ( const string & i : header )
		m_Data.push_back( vector<CCell *> { new CString( i ) } );
}

CTable::CTable ( const vector<pair<string, int>> & header ) {
	m_Data.reserve( header.size( ) );
	for ( const auto & i : header )
		m_Data.push_back( vector<CCell *> { new CString( i.first ) } );
}

CTable::~CTable ( ) {
	for ( auto & i : m_Data )
		for ( auto & j : i )
			delete j;
}

/**
 * Verifies if given column is present in the table
 * @param[in] col name of the column
 * @param[in, out] index index of the column in the table
 */
bool CTable::VerifyColumn ( const string & col, size_t & index ) const {
	vector<string> correctColumns = GetColumnNames( );
	vector<string>::iterator i;
	if ( ( i = find( correctColumns.begin( ), correctColumns.end( ), col ) ) == correctColumns.end( ) ) {
		CLog::HighlightedMsg( CLog::QP, col, CLog::QP_NO_SUCH_COL );
		return false;
	}
	index = i - correctColumns.begin( );
	return true;
}

/**
 * This method will sort columns based on their header.
 */
void CTable::SortColumns ( ) {
	std::sort( m_Data.begin(), m_Data.end(), [ ] ( const vector<CCell *> & a, const std::vector<CCell *> & b) { return ( * a.at( 0 ) ) < ( * b.at( 0 ) ); } );
}

/**
 * This method will sort its columns based on the pairs provided.
 * First of the two represents which index, second the position in the new order.
 * The columns must have equal number of rows, if not, exception is thrown.
 */
void CTable::SortColumns ( vector<pair<size_t, size_t>> & columnOrders ) {
	vector<vector<CCell *>> newColumnOrder ( m_Data.size( ), vector<CCell *>( m_Data.at( 0 ).size( ) ) );
	for ( const auto & i : columnOrders )
		newColumnOrder.at( i.first ) = std::move( m_Data.at( i.second ) );
	m_Data.clear( );
	m_Data = std::move( newColumnOrder );
}

/**
 * Row sorting. Works only on transformed tables (they're normally stored by columns).
 * @param[in, out] dataRef reference to transformed table rows
 */
void CTable::SortRows ( vector<vector<CCell *>> & dataRef ) {
	sort( dataRef.begin(), dataRef.end(), RowComparator( ) );
}

/**
 * This method will compare two rows. If all cells are equal, true is returned.
 */
bool CTable::CompareRows ( const vector<CCell *> & a, const vector<CCell *> & b ) {
	if ( a.empty( ) || b.empty( ) || a.size( ) != b.size( ) )
		return false;
	size_t rowLen = a.size( );
	for ( size_t i = 0; i < rowLen; ++i )
		if ( * a[ i ] != * b[ i ] )
			return false;
	return true;
}

bool CTable::SwapColumns ( const size_t & a, const size_t & b ) {
	if ( a >= m_Data.size( ) || b >= m_Data.size( ) || a == b )
		return false;
	std::swap( m_Data.at( a ), m_Data.at( b ) );
	return true;
}

/**
 * Verifies if table has any duplicate columns.
 */
bool CTable::HasDuplicateColumns ( ) const {
	std::map<std::string, int> countMap;
	for ( const auto & elem : m_Data ) {
		auto result = countMap.insert( pair<string, int>( elem.at( 0 )->RetrieveMVal( ), 1 ) );
		if ( ! result.second )
			return true;
	}
	return false;
}

/**
 * Verifies if two tables have identical headers.
 * @param[in]: tableRef table to compare header with
 */
bool CTable::HasIdenticalHeader ( const CTable * tableRef ) const {
	if ( ! tableRef )
		return false;
	auto j = tableRef->m_Data.begin( );
	for ( const auto & i : m_Data ) {
		if ( (* i.at( 0 )) != ( * j->at( 0 )) )
			return false;
		if ( j != tableRef->m_Data.end( ) )
			++j;
		else
			break;
	}
	return true;
}

/**
 * Table row insertion. References are copied, not reallocated!
 * @param[in] row Row to be inserted
 * @return true if row was inserted without errors
 */
bool CTable::InsertShallowRow ( const vector<CCell *> & row ) {
	if ( ( m_Data.begin( )->empty( ) ) || m_Data.size( ) != row.size( ) )
		return false;
	auto d = m_Data.begin( );
	auto r = row.begin( );
	while ( d != m_Data.end( ) ) {
		d->push_back( * r );
		++ d;
		++ r;
	}
	return true;
}

/**
 * Table column insertion ~ deep copy is made (for queries).
 * @param[in] col Column reference to be inserted
 * @return true if col was inserted without errors and column name is not taken.
 */
bool CTable::InsertDeepCol ( const vector<CCell *> & col ) {
	size_t elementCount = 0;
	if ( ! m_Data.empty( ) ) {
		elementCount = m_Data.at( 0 ).size( );
		if ( elementCount != col.size( ) || * col.begin( ) == * m_Data.at( 0 ).begin( ) )
			return false;
	}
	vector<CCell *> newColumn;
	newColumn.reserve( elementCount );
	for ( const auto & i : col )
		newColumn.push_back( i->Clone( ) );
	m_Data.push_back( std::move( newColumn ) );
	return true;
}

/**
 * Table row insertion ~ deep copy is made (for queries).
 * @param[in] index index of a row to be duplicated
 * @param[in, out] outPtr table pointer to save to
 * @return true if the deep copy of a row was created and inserted correctly
 */
bool CTable::InsertDeepRow ( const size_t & index, CTable * outPtr ) const {
	vector<CCell *> newRow;
	GetDeepRow( index, newRow );
	if ( ! outPtr || ! outPtr->InsertShallowRow( newRow ) )
		return false;
	newRow.clear( );
	return true;
}

/**
 * Changes column header element's name.
 * @param[in] index index of a column to be edited
 * @param[in] str new name.
 */
bool CTable::ChangeColumnName ( const size_t & index, const string & s ) const {
	return ( index < m_Data.size( ) && ! s.empty( ) && ( ! m_Data.at( index ).at( 0 )->Rename( s ) ) );
}

/**
 * Merges two rows together. Shallow copy is made.
 */
vector<CCell *> CTable::MergeRows ( const vector<CCell *> & rowA, const vector<CCell *> & rowB ) {
	vector<CCell *> output;
	output.reserve( rowA.size( ) + rowB.size( ) );
	for ( const auto & i : rowA )
		output.push_back( i );
	for ( const auto & i : rowB )
		output.push_back( i );
	return output;
}

bool CTable::GetShallowCol ( const string & name, vector<CCell *> & outRef ) const {
	for ( const auto & i : m_Data ) {
		if ( i.at( 0 )->RetrieveMVal( ) == name )
			outRef = i;
	}
	return true;
}

/**
 * Creates a deep copy of a table row.
 * @param[in] index index of a row
 * @param[in, out] outRef out row save pointer
 * @return true if the row was exported without errors
 */
bool CTable::GetDeepRow ( const size_t & index, vector<CCell *> & outRef ) const {
	if ( ! outRef.empty( ) )
		outRef.clear( );
	for ( const auto & i : m_Data )
		outRef.push_back( i.at( index )->Clone( ) );
	return true;
}

/**
 * Creates a deep copy of a table row with specified columns.
 * @param[in] index index of a row
 * @param[in] selectedColumns sequence of colums to include
 * @param[in, out] outRef out row save pointer
 * @return true if the row was exported without errors
 */
bool CTable::GetDeepRow ( const size_t & index, vector<string> & selectedColumns, vector<CCell *> & outRef ) const {
	if ( selectedColumns.empty( ) )
		return false;

	vector<size_t> columnSequence;
	size_t tmp;
	for ( const string & i : selectedColumns ) {
		if ( ! VerifyColumn( i, tmp ) )
			return false;
		columnSequence.push_back( tmp );
	}

	outRef.clear( );
	for ( const size_t & i : columnSequence )
		outRef.push_back( m_Data.at( i ).at( index )->Clone( ) );
	return true;
}

/**
 * Creates a deep copy of a provided row.
 * @param[in, out] outRef row to copy from
 * @return duplicated row with new cells.
 */
vector<CCell *> CTable::GetDeepRow ( const vector<CCell *> & outRef ) {
	vector<CCell *> out;
	for ( const auto & i : outRef )
		out.push_back( i->Clone( ) );
	return out;
}

/**
 * Returns a table clone with origin data, but only with some columns.
 * @param[in] cols columns to create new table with
 * @param[in] outPtr pointer to a new table to save
 * @return true if columns provided were correct, and subtable was generated
 */
bool CTable::GetSubTable ( const vector<string> & cols, CTable * outPtr ) const {
	if ( cols.size( ) == 1 && ( * cols.begin( ) == "*" ) ) {
		for ( const auto & i : m_Data )
			if ( ! outPtr->InsertDeepCol( i ) )
				return false;
		return true;
	}

	vector<size_t> columnIndexes;
	vector<string> correctColumns = GetColumnNames( );

	// get the column indexes to copy them
	for ( const auto & i : cols ) {
		auto el = find( correctColumns.begin( ), correctColumns.end( ), i );
		if ( el == correctColumns.end( ) ) {
			CLog::HighlightedMsg( CLog::QP, i, CLog::QP_NO_SUCH_COL );
			return false;
		} else {
			columnIndexes.push_back( el - correctColumns.begin( ) );
		}
	}

	// create a deep copy of all those columns
	for ( const size_t & i : columnIndexes )
		if ( ! outPtr->InsertDeepCol( m_Data.at( i ) ) )
			return false;
	return true;
}

/*
 * Returns the data type of a column for a given index.
 */
string CTable::GetColumnType ( const size_t & index ) const {
	if ( m_Data.empty( ) || index >= m_Data.size( ) || m_Data.at( index ).empty( ) )
		return "";
	return m_Data.at( index ).at( 1 )->GetType( );
}

/**
 * Creates a deep copy of current object. The rows must meet the condition to be copied.
 * @param[in] condition the condition object
 * @param[in] outPtr pointer to a new table to save
 * @return true if table was successfully created
 */
bool CTable::GetDeepTable ( CCondition * condition, CTable * outPtr ) const {
	if ( ! outPtr || outPtr->GetColumnCount( ) == 0 || m_Data.empty( ) )
		return false;

	// column projection verification
	size_t index;
	if ( ! VerifyColumn( condition->m_Column, index ) )
		return false;

	// constant type conversion
	string colType = m_Data.at( index ).at( 1 )->GetType( );
	CCell * criterionCell;
	try {
		if ( colType == typeid( string ).name( ) ) {
			criterionCell = new CString( condition->m_Constant );
			condition->IsStringConstant = true;
		} else if ( colType == ( typeid( int ).name( ) ) )
			criterionCell = new CInt( std::stoi( condition->m_Constant ) );
		else {
			char * c;
			double output = std::strtod( condition->m_Constant.c_str( ), & c );
			criterionCell = new CDouble( output );
			if ( c == condition->m_Constant.c_str( ) ) {
				CLog::BoldMsg( CLog::QP, condition->m_Constant, CLog::QP_CON_PARSE_ERROR );
				delete criterionCell;
				return false;
			}
		}
	} catch ( std::logic_error const & e ) {
		CLog::BoldMsg( CLog::QP, condition->m_Constant, CLog::QP_CON_PARSE_ERROR );
		return false;
	};

	// filtering data
	size_t rcnt = 0;
	size_t rows = m_Data.begin( )->size( );
	size_t cols = m_Data.size( );
	bool rowMatchFound = false;
	for ( size_t i = 1; i < rows; ++ i ) {
		for ( size_t j = 0; j < cols; ++ j ) {
			if ( j == index ) {
				if ( condition->m_Operator == "==" ) {
					if ( ( * m_Data.at( j ).at( i ) == * criterionCell ) )
						rowMatchFound = true;
				} else if ( condition->m_Operator == "!=" ) {
					if ( ( * m_Data.at( j ).at( i ) != * criterionCell ) )
						rowMatchFound = true;
				} else if ( condition->m_Operator == ">=" ) {
					if ( ( * m_Data.at( j ).at( i ) >= * criterionCell ) )
						rowMatchFound = true;
				} else if ( condition->m_Operator == "<=" ) {
					if ( ( * m_Data.at( j ).at( i ) <= * criterionCell ) )
						rowMatchFound = true;
				} else if ( condition->m_Operator == ">" ) {
					if ( ( * m_Data.at( j ).at( i ) > * criterionCell ) )
						rowMatchFound = true;
				} else if ( condition->m_Operator == "<" ) {
					if ( ( * m_Data.at( j ).at( i ) < * criterionCell ) )
						rowMatchFound = true;
				} else {
					CLog::Msg( CLog::QP, CLog::QP_INVALID_OPER );
					return false;
				}
				//
				//
				if ( rowMatchFound ) {
					++ rcnt;
					if ( ! InsertDeepRow( i, outPtr ) ) {
						delete criterionCell;
						return false;
					}
					rowMatchFound = false;
				}
			}
		}
	}

	delete criterionCell;
	if ( rcnt < 1 ) {
		CLog::Msg( CLog::QP, CLog::QP_EMPTY_RESULTS );
		return false;
	}
	return true;
}

/**
 * This method will find occurrences of given rows in given columns.
 * It will scan trough all rows in the table, searching for a match.
 * If it succeeds, corresponding row indexes of each objects are copied to the output.
 * @param[in] columnsRef source for data filtering
 * @return vector of matched row indexes (source + current table)
 */
vector<pair<size_t, size_t>> CTable::FindOccurences ( vector<vector<CCell *>> & columnsRef ) const {
	vector<pair<size_t, size_t>> matches;

	// find equivalent indexes of the columns in current table
	vector<size_t> columnIndexes;
	size_t currentIndex = 0;
	size_t cnt = 0;
	for ( const auto & item : columnsRef )
		if ( ! VerifyColumn( item.at( 0 )->RetrieveMVal( ), currentIndex ) )
			return vector<pair<size_t, size_t>>( );
		else {
			columnIndexes.push_back( currentIndex );
			++ cnt;
		}

	// scan for for occurrences
	bool equal;
	size_t columnCnt = columnsRef.size( );
	size_t rowRefCount = columnsRef.at( 0 ).size( );
	size_t rowDataCount = m_Data.at( 0 ).size( );
	for ( size_t i = 1; i < rowRefCount; ++ i ) {
		for ( size_t j = 1; j < rowDataCount; ++ j ) {
			equal = true;
			for ( size_t k = 0; k < columnCnt; ++ k )
				if ( * columnsRef.at( k ).at( i ) != * m_Data.at( columnIndexes.at( k ) ).at( j ) )
					equal = false;
			if ( equal )
				matches.emplace_back( i, j );
		}
	}
	return matches;
}

/**
 * Small alternative for occurence searching. Forked from identical method but accepts single column.
 * @param[in] columnRef source for data filtering
 * @return vector of matched row indexes (source + current table)
 */
vector<pair<size_t, size_t>> CTable::FindOccurences ( vector<CCell *> & columnRef ) const {
	// find equivalent index of the column in current table
	size_t tableColIndex = 0;
	if ( ! VerifyColumn( columnRef.at( 0 )->RetrieveMVal( ), tableColIndex ) )
		return vector<pair<size_t, size_t>>( );
	size_t rowRefCount  = columnRef.size( );
	size_t rowDataCount = m_Data.at( 0 ).size( );

	// scan for for occurrences
	vector<pair<size_t, size_t>> matches;
	for ( size_t i = 1; i < rowRefCount; ++ i )
		for ( size_t j = 1; j < rowDataCount; ++ j )
			if ( * columnRef.at( i ) == * m_Data.at( tableColIndex ).at( j ) )
				matches.emplace_back( i, j );

	return matches;
}

/**
 * Column count getter.
 */
size_t CTable::GetColumnCount ( ) const {
	return m_Data.size( );
}

/**
 * This method will transform existing data into row format.
 * Note: Only pointers are copied.
 * @return vector of rows
 */
vector<vector<CCell *>> CTable::Transform ( ) const {
	vector<vector<CCell *>> out;
	size_t rowCnt = m_Data.at( 0 ).size( );
	size_t colCnt = m_Data.size( );
	vector<CCell *> tmp;
	for ( size_t i = 1; i < rowCnt; ++i ) {
		for ( size_t j = 0; j < colCnt; ++j )
			tmp.push_back( m_Data.at( j ).at( i ) );
		out.push_back( std::move( tmp ) );
	}
	return out;
}

/**
 * Duplicates a table header with duplicated CCells.
 */
vector<CCell *> CTable::GetDeepHeader ( ) const {
	if ( m_Data.empty( ) )
		return vector<CCell *> { };
	vector<CCell *> out;
	for ( const auto & i : m_Data )
		out.push_back( i.at( 0 )->Clone( ) );
	return out;
}

/**
 * Returns all column names. Copy is made.
 */
vector<string> CTable::GetColumnNames ( ) const {
	vector<string> res;
	if ( m_Data.empty( ) )
		throw logic_error( CLog::TAB_NO_DATA );
	for ( const auto & i : m_Data )
		res.push_back( i.at( 0 )->RetrieveMVal( ) );
	return res;
}

/**
 * Calculates the widest cell for each column (needed for rendering)
 * @return vector of cell sizes for each column
 */
vector<size_t> CTable::GetCellPadding ( ) const {
	vector<size_t> result( m_Data.size( ), 0 );
	size_t counter = 0;
	for ( const auto & j : m_Data ) {
		for ( const CCell * k : j ) {
			size_t k_len = k->GetLength( );
			if ( k_len > result.at( counter ) )
				result.at( counter ) = k_len;
		}
		counter ++;
	}
	return result;
}

/**
 * Renders the table data into output stream.
 * @param[in,out] ost output stream.
 */
void CTable::Render ( ostream & ost ) const {
	if ( m_Data.empty( ) || m_Data.begin( )->size( ) == 1 )
		throw logic_error( CLog::TAB_NO_BODY );

	vector<size_t> paddings = GetCellPadding( );
	size_t rowLen = ( GetColumnCount( ) * CRenderSett::m_SpacePad.size( ) ) + 1;
	for ( const size_t & i : paddings )
		rowLen += i;
	size_t tmp = rowLen;

	// header
	RenderSeparator( rowLen, tmp );
	int currentColumn = 0;
	ost << CRenderSett::m_SpacePad;
	for ( const auto & columnName : m_Data ) {
		ost << setw( paddings.at( currentColumn ++ ) ) << left;
		columnName.at( 0 )->Print( ) << CRenderSett::m_SpacePad;
	}
	ost << endl;

	// body
	RenderSeparator( rowLen, tmp );
	currentColumn = 0;
	size_t colSize = m_Data.begin( )->size( );
	for ( size_t i = 1; i < colSize; ++ i ) {
		ost << CRenderSett::m_SpacePad;
		for ( const auto & j : m_Data ) {
			ost << setw( paddings[ currentColumn ++ ] ) << left;
			j.at( i )->Print( ) << CRenderSett::m_SpacePad;
		}
		currentColumn = 0;
		ost << endl;
	}

	RenderSeparator( rowLen, tmp );
}

/**
 * Creates character separators for the table rendering.
 */
void CTable::RenderSeparator ( const size_t & length, size_t & tmp, ostream & ost ) {
	ost << CRenderSett::m_FrontPad;
	tmp = length;
	while ( tmp -- )
		ost << CRenderSett::m_RowChar;
	ost << endl;
	tmp = 0;
}

ostream & operator << ( ostream & ost, const CTable & table ) {
	table.Render( ost );
	return ost;
}