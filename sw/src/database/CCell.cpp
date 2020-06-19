#include "CCell.hpp"

CString * CString::Clone ( ) {
	return new CString( * this );
}

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

string CString::GetType ( ) const {
	return typeid( m_Val ).name( );
}

bool CString::Rename ( const string & str ) {
	m_Val = str;
	return true;
}


//


CDouble * CDouble::Clone ( ) {
	return new CDouble( * this );
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

string CDouble::GetType ( ) const {
	return typeid( m_Val ).name( );
}

bool CDouble::Rename ( const string & str ) {
	return false;
}


//



CInt * CInt::Clone ( ) {
	return new CInt( * this );
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

string CInt::GetType ( ) const {
	return typeid( m_Val ).name( );
}

bool CInt::Rename ( const string & str ) {
	return false;
}