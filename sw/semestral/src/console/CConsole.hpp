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
	static int Start ( );
};