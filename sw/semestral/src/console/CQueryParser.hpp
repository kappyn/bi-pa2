#pragma once

#include <string>
#include <map>
#include <iostream>

using namespace std;

class CQueryParser {
public:
	static const string SELECTION;
	static const string PROJECTION;

//	static const string JOIN;
//	static const string NATURAL_JOIN;
//	static const string RENAME;
//	static const string UNION;
//	static const string INTERSECT;
//	static const string MINUS;
//	static const string CARTESIAN_PRODUCT;

	static bool Exists ( );
	static bool ParseQuery ( const string & basicString );
	static bool ValidateSelection ( const string & input );
};
