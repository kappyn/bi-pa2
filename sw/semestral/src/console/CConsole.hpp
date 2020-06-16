#pragma once

#include "CLog.hpp"
#include "CQueryParser.hpp"
#include "../tool/CDataParser.hpp"

#include <iostream>
#include <iomanip>
#include <string>

/**
 * This module provides command line interface for the user to enter queries in relational algebra syntax.
 */
class CConsole {
public:
	static const int VALID_QUERY     = 0;
	static const int INVALID_QUERY   = 1;
	static const int EXIT_CONSOLE    = 2;
	static const int EXCEPTION_ERROR = 3;
	static int Start ( CDatabase & dbRef );
};