#include "CCell.hpp"

size_t CString::GetLength ( ) const {
	return m_Val.length( );
}

ostream & CString::Print ( ostream & ost ) const {
	ost << m_Val;
	return ost;
}

string CString::RetrieveMVal ( ) const {
	return m_Val;
}

size_t CDouble::GetLength ( ) const {
	return ::to_string( m_Val ).length( );
}

ostream & CDouble::Print ( ostream & ost ) const {
	ost.precision( 2 );
	ost << fixed << m_Val;
	return ost;
}

string CDouble::RetrieveMVal ( ) const {
	return to_string( m_Val );
}

size_t CInt::GetLength ( ) const {
	return ::to_string( m_Val ).length( );
}

ostream & CInt::Print ( ostream & ost ) const {
	ost << m_Val;
	return ost;
}

string CInt::RetrieveMVal ( ) const {
	return to_string( m_Val );
}