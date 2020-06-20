#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "../database/CDatabase.hpp"
#include "../console/CLog.hpp"

using namespace std;

/**
 * This module parses raw text from the sources into logical cells which are then saved tables (and then to the database).
 * It mostly contains static members, because they might be useful for outter modules.
 * The module also provides an interface for data importing/exporting.
 */
class CDataParser {
public:
	static string & TrimSpaceLeft ( string & s );
	static string & TrimSpaceRight ( string & s );
	static string & TrimSurroundingSpace ( string & s );

	static void TrimAllSpaces ( string & token, const char & delimiter );
	static vector<string> Split ( string & s, const bool & allowQuotes, const bool & allowSpaces, const char & delim = ',' );
	static vector<string> Split ( string & s, const char & delimiter );
	static string & SplitFileName ( string & filePath );
	static string GetFileExtension ( const string & input );

	static bool ParseTable ( CDatabase & db, ifstream & ifs, string & filePath );
	static bool ParseCSV ( CDatabase & db, ifstream & ifs, string & filePath );
};
