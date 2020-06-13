#pragma once

#include "../console/CLog.hpp"
#include "../database/CTable.hpp"

class CTableQuery {
public:
	virtual ~CTableQuery ( ) = default;
	virtual bool Evaluate ( ) = 0;

	virtual CTable * GetQueryResult ( ) = 0;
	virtual CTableQuery * GetOrigin ( ) = 0;

	virtual string GenerateSQL ( ) const = 0;
	virtual string CreateSQL ( ) const = 0;

	virtual string GetQueryName ( ) const = 0;
	virtual void ArchiveQueryName ( const string & name ) = 0;
	virtual void SetQueryAsDerived ( ) = 0;
	virtual bool IsDerived ( ) const = 0;
};