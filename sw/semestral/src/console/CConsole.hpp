#pragma once

#include "CLog.hpp"

#include <iostream>
#include <iomanip>
#include <string>

class CConsole {
public:
	static void Start ( );
	static bool ParseQuery ( string basicString );
};