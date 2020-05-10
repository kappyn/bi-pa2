#pragma once

#include "CLog.hpp"
#include "../tools/CDataParser.hpp"

#include <iostream>
#include <iomanip>
#include <string>

class CConsole {
public:
	static void Start ( );
	static bool ParseQuery ( const string & basicString );
};