#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "CLog.hpp"

using namespace std;

/**
 * This module's responsibility is to parse string queries entered by the user.
 */
class CQueryParser {
public:
	static const string HELP;
	static const string SELECTION;
	static const string PROJECTION;
	static const string JOIN;
//	static const string NATURAL_JOIN;
//	static const string RENAME;
//	static const string UNION;
//	static const string INTERSECT;
//	static const string MINUS;
//	static const string CARTESIAN_PRODUCT;

	static string ReadQueryName ( const string & fullQuery );
	static string ReadQueryParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd );
	static bool ValidateQuerySyntax ( const string & queryName, const string & queryDetails );
	static bool ParseQuery ( const string & basicString );
	bool Exists ( );
};
