#include "CQueryParser.hpp"

const string CQueryParser::SELECTION = "SEL";
const string CQueryParser::PROJECTION = "PRO";
const string CQueryParser::JOIN = "JOIN";

bool CQueryParser::ReadQuerySave ( const string & queryDetails, const char & saveDelimiter, string & output ) {
	output.clear( );
	bool readMode = false;

	if ( queryDetails.empty( ) )
		return false;

	for ( const char & i : queryDetails ) {
		if ( ! readMode ) {
			if ( i == saveDelimiter )
				readMode = true;
		} else
			output += i;
	}

	return ( output.length( ) > 0 );
}

bool CQueryParser::ReadQueryName ( const string & fullQuery, string & output ) {
	output.clear( );
	for ( const char & i : fullQuery )
		if ( ! isalpha( i ) )
			break;
		else
			output += i;
	return ! output.empty( );
}

bool CQueryParser::ReadQueryParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd,
                                          int & stringPos, string & output ) {
	output.clear( );
	bool readMode = false;

	if ( queryDetails.empty( ) )
		return false;

	for ( const char & i : queryDetails ) {
		if ( ! readMode ) {
			if ( i == delStart )
				readMode = true;
		} else {
			if ( i == delEnd ) {
				++ stringPos;
				return output.length( ) > 0;
			} else
				output += i;
		}
		++ stringPos;
	}

	return false;
}

bool CQueryParser::ValidateQuerySyntax ( const string & queryName, const string & queryDetails ) {
	if ( queryDetails.empty( ) )
		return false;

	bool saveMode = false;
	int stringProgress = 0;

	if ( queryName == CQueryParser::SELECTION ) {
		CLog::Msg( CLog::CON, "SELECTION" );

		string columns;
		if ( ! ReadQueryParenthesis( queryDetails, '[', ']', stringProgress, columns ) )
			return false;

		string table;
		if ( ! ReadQueryParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table ) )
			return false;

		// at this point, we got all the information we needed - we just need to verify it actually exists
		vector<string> cols = CDataParser::Split( columns, false, false, ',' );

	} else {
		return false;
	}

	// save query option
	string qname;
	if ( stringProgress == queryDetails.length( ) ) {
		saveMode = false;
	} else {
		if ( ReadQuerySave( queryDetails.substr( stringProgress ), '~', qname ) )
			saveMode = true;
	}

	if ( saveMode )
		cout << queryName << " (" << saveMode << ") -> " << qname << endl;

	return true;
}
bool CQueryParser::ParseQuery ( const string & basicString ) {
	string queryName;
	if ( ! ReadQueryName( basicString, queryName ) )
		return false;

	string queryDetails = basicString.substr( queryName.length( ) );
	return ValidateQuerySyntax( queryName, queryDetails );
}
