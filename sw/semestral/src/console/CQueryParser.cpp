#include "CQueryParser.hpp"

const string CQueryParser::SELECTION = "SEL";
const string CQueryParser::PROJECTION = "PRO";
const string CQueryParser::JOIN = "JOIN";

string CQueryParser::ReadQueryName ( const string & fullQuery ) {
	string tmp;
	for ( const char & i : fullQuery )
		if ( ! isalpha( i ) )
			break;
		else
			tmp += i;
	return tmp;
}

string CQueryParser::ReadQueryParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd ) {
	bool rpm = false;
	string parContent;
	for ( const char & i : queryDetails ) {
		if ( ! rpm ) {
			if ( i == delStart ) {
				rpm = true;
			}
		} else {
			if ( i == delEnd )
				rpm = false;
			else
				parContent += i;
		}
	}
	return parContent;
}

bool CQueryParser::ValidateQuerySyntax ( const string & queryName, const string & queryDetails ) {
	if ( queryDetails.empty( ) )
		return false;

	if ( queryName == CQueryParser::SELECTION ) {
		CLog::Msg( CLog::CON, "SELECTION" );

		string columns = ReadQueryParenthesis( queryDetails, '[', ']' );
		if ( columns.empty( ) )
			return false;

		string table = ReadQueryParenthesis( queryDetails, '(', ')' );
		if ( table.empty( ) )
			return false;

		cout << "columns: "    << columns << endl;
		cout << "table_name: " << table << endl;

		return true;
	}

	if ( queryName == CQueryParser::PROJECTION ) {
		return true;
	}

	if ( queryName == CQueryParser::JOIN ) {
		return true;
	}

	return false;
}

bool CQueryParser::ParseQuery ( const string & basicString ) {
	string queryName = ReadQueryName( basicString );
	string queryDetails = basicString.substr( queryName.length( ) );

	if ( ValidateQuerySyntax( queryName, queryDetails ) )
		return true;

	return false;
}
