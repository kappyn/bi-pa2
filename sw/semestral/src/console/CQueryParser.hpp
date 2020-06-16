#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "CLog.hpp"
#include "CConsole.hpp"
#include "../tool/CDataParser.hpp"
#include "../database/CDatabase.hpp"

#include "../query/CSelection.hpp"
#include "../query/CProjection.hpp"

using namespace std;

/**
 * This module's responsibility is to parse string queries entered by the user.
 */
class CQueryParser {
private:
	CDatabase & m_Database;
	vector<string> m_Operators;

public:
	explicit CQueryParser( CDatabase & ref );

	static const string TABLES;
	static const string QUERIES;
	static const string QUIT;

	static const string SELECTION;
	static const string PROJECTION;

	static bool ReadQuerySave ( const string & queryDetails, const char & saveDelimiter, string & output );
	static bool ReadQueryName ( const string & fullQuery, string & output );
	static bool ReadQueryParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd, size_t & stringPos, string & output );

	int ProcessQuery ( const string & basicString);
	bool ValidateConditionSyntax ( const string & query, CCondition * output );
};
