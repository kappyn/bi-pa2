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
//#include "../query/CProjection.hpp"

using namespace std;

/**
 * This module's responsibility is to parse string queries entered by the user.
 */
class CQueryParser {
private:
	CDatabase & m_Database;

	struct CCondition {
		string column;
		string relation;
		string constant;
	};
public:
	explicit CQueryParser( CDatabase & ref );

	static const string TABLES;
	static const string QUERIES;
	static const string QUIT;

	static const string SELECTION;
	static const string PROJECTION;

	static const string JOIN;

//	static const string CARTESIAN_PRODUCT;
	static bool ReadQuerySave ( const string & queryDetails, const char & saveDelimiter, string & output );
	static bool ReadQueryName ( const string & fullQuery, string & output );
	static bool ReadQueryParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd, int & stringPos, string & output );
	static bool ValidateConditionSyntax ( const string & condition, CCondition & out );
	int ProcessQuery ( const string & basicString);
};
