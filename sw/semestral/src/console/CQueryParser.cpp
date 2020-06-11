#include "CQueryParser.hpp"

const string CQueryParser::TABLES = "TABLES";
const string CQueryParser::QUIT = "QUIT";

const string CQueryParser::SELECTION = "SEL";
const string CQueryParser::PROJECTION = "PRO";
const string CQueryParser::JOIN = "JOIN";

/**
 * Reads and determines if a user wants the query to be saved into memory.
 * @param[in] queryDetails query substring
 * @param[in] saveDelimiter delimiter that separates the characters for the query name
 * @param[in, out] output the query save name
 * @return true if user wants to save the query and the name is not used and atleast a character long
 */
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

/**
 * Separates the query name.
 * @param[in] fullQuery the query string
 * @param[in, out] output the query name.
 * @return true if name was found and is atleast a character long
 */
bool CQueryParser::ReadQueryName ( const string & fullQuery, string & output ) {
	output.clear( );
	for ( const char & i : fullQuery )
		if ( ! isalpha( i ) )
			break;
		else
			output += i;
	return ! output.empty( );
}

/**
 * Reads the content of a defined parenthesis.
 * @param[in] queryDetails the query substring
 * @param[in] delStart the parenthesis starting character
 * @param[in] delEnd parenthesis ending character
 * @param[in, out] stringPos current char position of the original query
 * @param[in, out] output the content of the parenthesis
 * @return true if any content exists and the parenthesis is correctly ended
 */
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

/**
 * Validates the syntax of a query.
 * @param[in] queryName name of the query
 * @param[in, out] queryDetails details of the query
 * @return enum value for corresponding application state
 */
int CQueryParser::ValidateQuerySyntax ( const string & queryName, const string & queryDetails ) {
	if ( queryDetails.empty( ) ) {
		if ( queryName == CQueryParser::TABLES ) {
			m_Database.ListTables( );
			return CConsole::VALID_QUERY;
		}
		else if ( queryName == CQueryParser::QUIT )
			return CConsole::EXIT_CONSOLE;
		else
			return CConsole::INVALID_QUERY;
	}

	bool saveMode = false;
	int stringProgress = 0;
	CTableQuery * userQuery;

	if ( queryName == CQueryParser::SELECTION ) {
		string columns, table;
		if ( ! ReadQueryParenthesis( queryDetails, '[', ']', stringProgress, columns )
		     || ! ReadQueryParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table ) )
			return false;
		vector<string> cols = CDataParser::Split( columns, false, false, ',' );
		userQuery = new CSelection { m_Database, cols, table };
	} else if ( false ) {
		// new queries...
	} else {
		return CConsole::INVALID_QUERY;
	}

	if ( ! userQuery->Evaluate( ) ) {
		delete userQuery;
		return CConsole::INVALID_QUERY;
	}

	cout << * userQuery->GetQueryResult( );

	delete userQuery;

//	// save query option
//	string qname;
//	if ( stringProgress == queryDetails.length( ) ) {
//		saveMode = false;
//		delete userQuery;
//	}
//	else
//		if ( ReadQuerySave( queryDetails.substr( stringProgress ), '~', qname ) )
//			saveMode = true;

	return CConsole::VALID_QUERY;
}

/**
 * Parses the query and returns the appropriate data
 * @param[in] basicString raw input query
 * @return enum value for corresponding application state
 */
int CQueryParser::ParseQuery ( const string & basicString ) {
	string queryName;
	if ( ! ReadQueryName( basicString, queryName ) )
		return CConsole::INVALID_QUERY;

	string queryDetails = basicString.substr( queryName.length( ) );
	return ValidateQuerySyntax( queryName, queryDetails );
}

/**
 * Constructor with application database reference.
 */
CQueryParser::CQueryParser ( CDatabase & ref ) : m_Database( ref ) { }
