#include "CQueryParser.hpp"

const string CQueryParser::SELECTION = "SEL";

bool CQueryParser::Exists ( ) {
	return false;
}

bool CQueryParser::ParseQuery ( const string & basicString ) {
	string tmp;
	for ( const char & i : basicString )
		if ( i == '(' || i == '[' )
			break;
		else
			tmp += i;

	if ( tmp == CQueryParser::SELECTION && CQueryParser::ValidateSelection( basicString.substr( 3 ) )) {
		// ....
		return true;
	}
	return false;
}

bool CQueryParser::ValidateSelection( const string & input ) {
	string tmp = input;
	return true;
}