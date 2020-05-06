#include "CTable.hpp"

CTable::CTable ( vector<CCell *> header ) {
	m_Columns = header.size( );
	m_Data.push_back( std::move( header ) );
}

CTable::~CTable ( ) {
	for ( auto & i : m_Data )
		for ( auto & j : i )
			delete j;
}

void CTable::InsertRow ( vector<CCell *> row ) {
	m_Data.push_back( std::move( row ) );
}

size_t CTable::GetColumnCount ( ) const {
	return m_Columns;
}

vector<size_t> CTable::GetCellPadding ( ) const {
	vector<size_t> result( GetColumnCount( ), 0 );
	int counter = 0;
	for ( const auto & j : m_Data ) {
		for ( const CCell * k : j ) {
			size_t k_len = k->GetLength( );
			if ( k_len > result[ counter ] )
				result[ counter ] = k_len;
			counter ++;
		}
		counter = 0;
	}
	return result;
}

const vector<CCell *> * CTable::GetRow ( const size_t & index ) const {
	if ( index < 0 || index >= m_Data.size( ) )
		throw std::out_of_range( "Index is out of bounds." );
	return & m_Data.at( index );
}

void CTable::Render ( ostream & ost ) const {
	vector<size_t> paddings = GetCellPadding( );

	char rowCharacter = '-';
	char columnCharacter = '|';
	string frontPadding = "  ";
	string spacePadding;
	spacePadding.append( "  " );
	spacePadding += columnCharacter;
	spacePadding.append( "  " );

	size_t rowLen = ( GetColumnCount( ) * spacePadding.size( ) ) + 1; // 5 is the length of colspace
	for ( const size_t & i : paddings )
		rowLen += i;
	size_t tmp = rowLen;

	RenderSeparator( rowLen, rowCharacter, frontPadding, tmp );

	// header
	int currentColumn = 0;
	auto j = m_Data.begin( );

	ost << spacePadding;
	for ( const CCell * columnName : * j ) {
		ost << setw( paddings[ currentColumn ++ ] ) << left;
		columnName->Print( ) << spacePadding;
	}
	ost << endl;

	// separator
	RenderSeparator( rowLen, rowCharacter, frontPadding, tmp );

	// body
	currentColumn = 0;
	j ++;
	for ( ; j != m_Data.end( ); ++ j ) {
		ost << spacePadding;
		for ( const CCell * k : * j ) {
			ost << setw( paddings[ currentColumn ++ ] ) << left;
			k->Print( ) << spacePadding;
		}
		currentColumn = 0;
		ost << endl;
	}

	// end separator
	RenderSeparator( rowLen, rowCharacter, frontPadding, tmp );
}

void CTable::RenderRow ( ostream & ost ) const {
	for ( const auto & i : m_Data[ 0 ] )
		i->Print( ) << " ";
	ost << endl;
}

void CTable::RenderSeparator ( const size_t & length, const char & rowchar, const string & pad, size_t & tmp, ostream & ost ) {
	ost << pad;
	tmp = length;
	while ( tmp -- )
		ost << rowchar;
	ost << endl;
	tmp = 0;
}

ostream & operator << ( ostream & ost, const CTable & table ) {
	table.Render( ost );
	return ost;
}
