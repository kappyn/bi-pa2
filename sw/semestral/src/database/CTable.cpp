#include "CTable.hpp"

/**
 * Constructor which initializes the component with pre-defined header. This header contains column names.
 */
CTable::CTable ( const vector<CCell *> & header ) {
	m_Data.reserve( header.size( ) );
	for ( const auto & i : header ) {
		m_Data.push_back( vector<CCell *> { i } );
	}
}

CTable::CTable ( const vector<string> & header ) {
	m_Data.reserve( header.size( ) );
	for ( const string & i : header ) {
		m_Data.push_back( vector<CCell *> { new CString( i ) } );
	}
}

/**
 * Destructor for each cell. Each cell also has its own destructor.
 */
CTable::~CTable ( ) {
	for ( auto & i : m_Data )
		for ( auto & j : i )
			delete j;
}

/**
 * Verifies if given columns are present in the table
 */
bool CTable::VerifyColumns ( const vector<string> & cols ) const {
	vector<string> correctColumns = GetColumnNames( );
	for ( const auto & i : cols ) {
		if ( find( correctColumns.begin( ), correctColumns.end( ), i ) == correctColumns.end( ) ) {
			CLog::HighlightedMsg( CLog::QP, i, CLog::QP_NO_SUCH_COL );
			return false;
		}
	}
	return true;
}

/**
 * Verifies if given column is present in the table
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
 * Verifies if table has any duplicate columns.
 */
bool CTable::HasDuplicateColumns ( ) const {
	std::map<std::string, int> countMap;
	for ( const auto & elem : m_Data ) {
	    auto result = countMap.insert( pair<string, int> ( elem.at( 0 )->RetrieveMVal( ), 1 ) );
	    if ( ! result.second )
	    	return true;
	}
	return false;
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
 * Creates a deep copy of a table row.
 * @param[in, out] outPtr out row save pointer
 * @return true if the row was exported without errors
 */
bool CTable::GetDeepRow ( const size_t & index, vector<CCell *> & outPtr ) const {
	if ( ! outPtr.empty( ) )
		outPtr.clear( );
	for ( const auto & i : m_Data )
		outPtr.push_back( i.at( index )->Clone( ) );
	return true;
}

/**
 * Returns a table clone with origin data, but only with some columns.
 * @param[in] cols columns to create new table with
 * @param[in] outPtr pointer to a new table to save
 * @return true if columns provided were correct, and table was generated
 */
bool CTable::GetSubTable ( const vector<string> & cols, CTable * outPtr ) const {
	if ( cols.size( ) == 1 && ( * cols.begin( ) == "*" ) ) {
		for ( const auto & i : m_Data )
			if ( ! outPtr->InsertDeepCol( i ) )
				return false;
		return true;
	}

	vector<int> columnIndexes;
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
	for ( const int & i : columnIndexes )
		if ( ! outPtr->InsertDeepCol( m_Data.at( i ) ) )
			return false;

	return true;
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
		}

		else if ( colType == ( typeid( int ).name( ) ) )
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
	}
	catch ( std::logic_error const & e ) {
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
				// extra spaces for readability

				if ( condition->m_Operator == "==" ) {
					if ( ( * m_Data.at( j ).at( i ) == * criterionCell ) )
						rowMatchFound = true;
				}

				else if ( condition->m_Operator == "!=" ) {
					if ( ( * m_Data.at( j ).at( i ) != * criterionCell ) )
						rowMatchFound = true;
				}

				else if ( condition->m_Operator == ">=" ) {
					if ( ( * m_Data.at( j ).at( i ) >= * criterionCell ) )
						rowMatchFound = true;
				}

				else if ( condition->m_Operator == "<=" ) {
					if ( ( * m_Data.at( j ).at( i ) <= * criterionCell ) )
						rowMatchFound = true;
				}

				else if ( condition->m_Operator == ">" ) {
					if ( ( * m_Data.at( j ).at( i ) > * criterionCell ) )
						rowMatchFound = true;
				}

				else if ( condition->m_Operator == "<" ) {
					if ( ( * m_Data.at( j ).at( i ) < * criterionCell ) )
						rowMatchFound = true;
				}

				else {
					CLog::Msg( CLog::QP, CLog::QP_INVALID_OPER );
					return false;
				}

				//

				if ( rowMatchFound ) {
					++ rcnt;
					if ( ! InsertDeepRow( i, outPtr) ) {
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
 * Column count getter.
 */
size_t CTable::GetColumnCount ( ) const {
	return m_Data.size( );
}

/**
 * Row count getter.
 */
size_t CTable::GetRowCount ( ) const {
	if ( m_Data.empty( ) )
		return 0;
	return m_Data.begin( )->size( );
}

/**
 * Duplicates a table header with duplicated CCells.
 */
vector<CCell *> CTable::GetDeepHeader ( ) const {
	if ( m_Data.empty( ) )
		return vector<CCell *> { };

	vector<CCell *> out;
	for ( const auto & i : m_Data ) {
		out.push_back( i.at( 0 )->Clone( ) );
	}
	return out;
}

/**
 * Returns all column names.
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
	int counter = 0;
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
 * Wipes table's content. Use with caution.
 */
void CTable::DeleteData ( ) {
	m_Data.clear( );
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

	RenderSeparator( rowLen, tmp );

	// header
	int currentColumn = 0;
	ost << CRenderSett::m_SpacePad;
	for ( const auto & columnName : m_Data ) {
		ost << setw( paddings.at( currentColumn ++ ) ) << left;
		columnName.at( 0 )->Print( ) << CRenderSett::m_SpacePad;
	}
	ost << endl;

	RenderSeparator( rowLen, tmp );

	// body
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

/**
 * Renders a specified column of the table.
 * @param[in] index index of the column in the table
 * @param[in,out] ost output stream.
 * TODO: not fully functional yet.
 */
vector<CCell *> CTable::RenderCol ( const size_t & index, ostream & ost ) const {
	if ( index < 0 || index > m_Data.size( ) )
		throw std::out_of_range( CLog::TAB_INVALID_INDEX );
	return m_Data.at( index );
}
ostream & operator << ( ostream & ost, const CTable & table ) {
	table.Render( ost );
	return ost;
}
