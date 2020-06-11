#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "CLog.hpp"
#include "../tool/CDataParser.hpp"
#include "../database/CDatabase.hpp"
#include "../query/CSelection.hpp"

using namespace std;

/**
 * This module's responsibility is to parse string queries entered by the user.
 */
class CQueryParser {
private:
	CDatabase & m_Database;

public:
	explicit CQueryParser( CDatabase & ref );

	static const string TABLES;

	static const string SELECTION;
	static const string PROJECTION;
	static const string JOIN;

//	static const string NATURAL_JOIN;
//	static const string RENAME;
//	static const string UNION;
//	static const string INTERSECT;
//	static const string MINUS;
//	static const string CARTESIAN_PRODUCT;

	static bool ReadQuerySave ( const string & queryDetails, const char & saveDelimiter, string & output );
	static bool ReadQueryName ( const string & fullQuery, string & output );
	static bool ReadQueryParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd, int & stringPos, string & output );
	bool ValidateQuerySyntax ( const string & queryName, const string & queryDetails );
	bool ParseQuery ( const string & basicString );
};
