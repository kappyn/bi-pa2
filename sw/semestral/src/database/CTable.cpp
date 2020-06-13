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
 * Table row insertion. References are copied, not reallocated!
 * @param[in] row Row to be inserted
 * @return true if row was inserted without errors
 */
bool CTable::InsertShallowRow ( const vector<CCell *> & row ) {
	if ( m_Data.size( ) != row.size( ) )
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
 * @param[in] col Column to be inserted
 * @return true if col was inserted without errors and column name is not taken.
 */
bool CTable::InsertDeepCol ( const vector<CCell *> & col ) {
	size_t elementCount = 0;
	if ( ! m_Data.empty( ) ) {
		for ( const auto & i : m_Data.at( 0 ) )
			++ elementCount;
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

size_t CTable::GetColumnCount ( ) const {
	return m_Data.size( );
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
