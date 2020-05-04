#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "CDatabase.hpp"

using namespace std;

class CDataParser {
public:
	static void Trim ( string & token, const char & delimiter );
	static vector<string> Split ( string & s, const bool & allowQuotes = true, const char & delim = ',' );
	static bool ParseTable ( CDatabase & db, ifstream & ifs );
};
