#ifndef CCELL_H
#define CCELL_H

#include <iostream>
#include <string>
#include <utility>
#include <typeinfo>
#include <math.h>
#include <float.h>

using namespace std;

/**
 * This module stores the data of the table itself.
 * Each cell is polymorphic, since we only get to know the column data types during runtime.
 *
 * However, we still need to be able to implement comparisson operators.
 * This is done trough a template - the original idea is taken from a stack overflow user leemes
 * Source: https://stackoverflow.com/a/24249968
 *
 * CString, CDouble, CInt are basically a primitive data type encapsulated objects.
 * There could also be any different (custom made) object with its own functionality.
 */
class CCell {
public:
	virtual CCell * Clone ( ) = 0;
	virtual size_t GetLength ( ) const = 0;
	virtual ostream & Print ( ostream & ost = cout ) const = 0;
	virtual string RetrieveMVal ( ) const = 0;
	virtual string GetType ( ) const = 0;
	virtual ~CCell ( ) = default;

	virtual bool operator == ( const CCell & other ) const = 0;
	virtual bool operator != ( const CCell & other ) const = 0;
	virtual bool operator >= ( const CCell & other ) const = 0;
	virtual bool operator <= ( const CCell & other ) const = 0;
	virtual bool operator  > ( const CCell & other ) const = 0;
	virtual bool operator  < ( const CCell & other ) const = 0;
};

template <typename CCell, typename Derived>
class ComparableCCell : public CCell {
private:
	bool typeCheck ( const CCell & other ) const {
		return ( typeid( other ) == typeid( Derived ) );
	}

public:
	bool operator == ( const CCell & other ) const {
		if ( ! typeCheck( other ) )
			return false;
		const Derived & a = static_cast<const Derived &>( * this );
		const Derived & b = static_cast<const Derived &>( other );
		return a == b;
	}
	bool operator != ( const CCell & other ) const {
		return ! ( * this == other );
	}
	bool operator >= ( const CCell & other ) const {
		if ( ! typeCheck( other ) )
			return false;
		const Derived & a = static_cast<const Derived &>( * this );
		const Derived & b = static_cast<const Derived &>( other );
		return a >= b;
	}
	bool operator <= ( const CCell & other ) const {
		if ( ! typeCheck( other ) )
			return false;
		const Derived & a = static_cast<const Derived &>( * this );
		const Derived & b = static_cast<const Derived &>( other );
		return a <= b;
	}
	bool operator >  ( const CCell & other ) const {
		if ( ! typeCheck( other ) )
			return false;
		const Derived & a = static_cast<const Derived &>( * this );
		const Derived & b = static_cast<const Derived &>( other );
		return a > b;
	}
	bool operator <  ( const CCell & other ) const {
		if ( ! typeCheck( other ) )
			return false;
		const Derived & a = static_cast<const Derived &>( * this );
		const Derived & b = static_cast<const Derived &>( other );
		return a < b;
	}
};

class CString : public ComparableCCell<CCell, CString> {
public:
	string m_Val;
	virtual CString * Clone ( ) override;
	explicit CString ( string data ) : m_Val( std::move( data ) ) { }
	~CString ( ) override = default;
	virtual size_t GetLength ( ) const override;
	virtual string RetrieveMVal ( ) const override;
	virtual string GetType ( ) const override;
	virtual ostream & Print ( ostream & ost = cout ) const override;

	bool operator == ( const CString & other ) const {
		return m_Val == other.m_Val;
	}
	bool operator != ( const CString & other ) const {
		return m_Val != other.m_Val;
	}
	bool operator >= ( const CString & other ) const {
		return m_Val >= other.m_Val;
	}
	bool operator <= ( const CString & other ) const {
		return m_Val <= other.m_Val;
	}
	bool operator >  ( const CString & other ) const {
		return m_Val > other.m_Val;
	}
	bool operator <  ( const CString & other ) const {
		return m_Val < other.m_Val;
	}
};

class CDouble : public ComparableCCell<CCell, CDouble> {
private:
	bool cmpEq ( double a, double b, double epsilon = DBL_EPSILON ) const {
		return fabs( a - b ) <= ( ( fabs( a ) > fabs( b ) ? fabs( b ) : fabs( a ) ) * epsilon );
	}
	bool cmpGt ( double a, double b, double epsilon = DBL_EPSILON ) const {
		return ( a - b ) > ( ( fabs( a ) < fabs( b ) ? fabs( b ) : fabs( a ) ) * epsilon );
	}
	bool cmpLt ( double a, double b, double epsilon = DBL_EPSILON ) const {
		return ( b - a ) > ( ( fabs( a ) < fabs( b ) ? fabs( b ) : fabs( a ) ) * epsilon );
	}

public:
	double m_Val;
	virtual CDouble * Clone ( ) override;
	explicit CDouble ( const double & data ) : m_Val( data ) { }
	~ CDouble ( ) override = default;
	virtual size_t GetLength ( ) const override;
	virtual string RetrieveMVal ( ) const override;
	virtual string GetType ( ) const override;
	virtual ostream & Print ( ostream & ost = cout ) const override;

	bool operator == ( const CDouble & other ) const {
		return cmpEq( m_Val, other.m_Val );
	}
	bool operator != ( const CDouble & other ) const {
		return ! cmpEq( m_Val, other.m_Val );
	}
	bool operator >= ( const CDouble & other ) const {
		return ( cmpEq( m_Val, other.m_Val ) || cmpGt( m_Val, other.m_Val ) );
	}
	bool operator <= ( const CDouble & other ) const {
		return ( cmpEq( m_Val, other.m_Val ) || cmpLt( m_Val, other.m_Val ) );
	}
	bool operator >  ( const CDouble & other ) const {
		return cmpGt( m_Val, other.m_Val );
	}
	bool operator <  ( const CDouble & other ) const {
		return cmpLt( m_Val, other.m_Val );
	}
};

class CInt    : public ComparableCCell<CCell, CInt> {
public:
	int m_Val;
	virtual CInt * Clone ( ) override;
	explicit CInt ( const int & data ) : m_Val( data ) { }
	~CInt ( ) = default;
	virtual size_t GetLength ( ) const override;
	virtual string RetrieveMVal ( ) const override;
	virtual string GetType ( ) const override;
	virtual ostream & Print ( ostream & ost = cout ) const override;

	bool operator == ( const CInt & other ) const {
		return m_Val == other.m_Val;
	}
	bool operator != ( const CInt & other ) const {
		return m_Val != other.m_Val;
	}
	bool operator >= ( const CInt & other ) const {
		return m_Val >= other.m_Val;
	}
	bool operator <= ( const CInt & other ) const {
		return m_Val <= other.m_Val;
	}
	bool operator >  ( const CInt & other ) const {
		return m_Val > other.m_Val;
	}
	bool operator <  ( const CInt & other ) const {
		return m_Val < other.m_Val;
	}
};

#endif