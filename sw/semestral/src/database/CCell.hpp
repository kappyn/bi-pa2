#pragma once

#include <iostream>
#include <string>
#include <utility>

using namespace std;

class CCell {
public:
	virtual size_t GetLength ( ) const = 0;
	virtual ostream & Print ( ostream & ost = cout ) const = 0;
	virtual ~CCell ( ) = default;
};

class CString : public CCell {
public:
	string m_Val;
	explicit CString ( string data ) : m_Val( std::move( data ) ) { }
	~CString ( ) override = default;

	virtual size_t GetLength ( ) const override {
		return m_Val.length( );
	}
	virtual ostream & Print ( ostream & ost = cout ) const override {
		ost << m_Val;
		return ost;
	}
};

class CDouble : public CCell {
public:
	double m_Val;
	explicit CDouble ( const double & data ) : m_Val( data ) { }
	~ CDouble ( ) override = default;

	virtual size_t GetLength ( ) const override {
		return ::to_string( m_Val ).length( );
	}
	virtual ostream & Print ( ostream & ost = cout ) const override {
		ost.precision( 2 );
		ost << fixed << m_Val;
		return ost;
	}
};

class CInt : public CCell {
public:
	int m_Val;
	explicit CInt ( const int & data ) : m_Val( data ) { }
	~CInt ( ) = default;

	virtual size_t GetLength ( ) const override {
		return ::to_string( m_Val ).length( );
	}
	virtual ostream & Print ( ostream & ost = cout ) const override {
		ost << m_Val;
		return ost;
	}
};