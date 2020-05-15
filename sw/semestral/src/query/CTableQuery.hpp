#pragma once

#include "../database/CTable.hpp"

class CTableQuery {
private:
	string m_Query;

public:
	CTableQuery ( const string & mQuery ) : m_Query( mQuery ) { }
	virtual CTable * ResolveQuery ( );
	virtual string GetSQL( );
	virtual bool ResultEmpty( );
};



