#pragma once

#include <iostream>
#include <string>
#include <utility>

using namespace std;

/**
 * This module stores the data of the table itself.
 * Each cell is polymorphic, since we only get to know the column data types during runtime.
 * CString, CDouble, CInt are basically a primitive data type encapsulated objects.
 * There could also be any different (custom made) object with its own functionality.
 */
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
	virtual size_t GetLength ( ) const override;
	virtual ostream & Print ( ostream & ost = cout ) const override;
};

class CDouble : public CCell {
public:
	double m_Val;
	explicit CDouble ( const double & data ) : m_Val( data ) { }
	~ CDouble ( ) override = default;
	virtual size_t GetLength ( ) const override;
	virtual ostream & Print ( ostream & ost = cout ) const override;
};

class CInt : public CCell {
public:
	int m_Val;
	explicit CInt ( const int & data ) : m_Val( data ) { }
	~CInt ( ) = default;
	virtual size_t GetLength ( ) const override;
	virtual ostream & Print ( ostream & ost = cout ) const override;
};