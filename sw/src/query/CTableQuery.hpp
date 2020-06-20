#pragma once

#include "../console/CLog.hpp"
#include "../database/CTable.hpp"

/*
 * This abstract class is the template for every existing query.
 */
class CTableQuery {
public:
	virtual ~CTableQuery ( ) = default;

	/** Processes the query operands. Initializes the data and creates the result. */
	virtual bool Evaluate ( ) = 0;

	/** Returns pointer to the query result. Nullptr is returned if query is not yet processed. */
	virtual CTable * GetQueryResult ( ) = 0;

	/** Generates a SQL syntax string, based on query result data. */
	virtual string GetSQL ( ) const = 0;

	/** Generates a SQL syntax string, based on query result data. */
	virtual void ArchiveQueryName ( const string & name ) = 0;

	/** Returns true, if current query is based on another saved query */
	virtual bool IsDerived ( ) const = 0;
};