#include "CQueryParser.hpp"

// command queries
const string CQueryParser::TABLES = "TABLES";
const string CQueryParser::QUERIES = "QUERIES";
const string CQueryParser::QUIT = "QUIT";

const string CQueryParser::SELECTION = "SEL";
const string CQueryParser::PROJECTION = "PRO";


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

	return output.length( ) > 0;
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
                                          size_t & stringPos, string & output ) {
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
			}
			if ( i == ';' )
				return false;
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
int CQueryParser::ProcessQuery ( const string & basicString ) {
	// basic query recognition
	string queryName, queryDetails;
	if ( ! ReadQueryName( basicString, queryName ) )
		return CConsole::INVALID_QUERY;
	queryDetails = basicString.substr( queryName.length( ) );

	// interface commands
	if ( queryDetails.empty( ) ) {

		if ( queryName == CQueryParser::TABLES ) {
			m_Database.ListTables( );
			return CConsole::VALID_QUERY;
		}

		if ( queryName == CQueryParser::QUERIES ) {
			m_Database.ListQueries( );
			return CConsole::VALID_QUERY;
		}

		if ( queryName == CQueryParser::QUIT )
			return CConsole::EXIT_CONSOLE;

		return CConsole::INVALID_QUERY;
	}

	size_t stringProgress = 0;
	CTableQuery * userQuery;

	// relational algebra operations

	// SELECTION
	if ( queryName == CQueryParser::SELECTION ) {
		string columns, table;
		if (
				! ReadQueryParenthesis( queryDetails, '[', ']', stringProgress, columns ) ||
				! ReadQueryParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table )
			)
			return CConsole::INVALID_QUERY;
		userQuery = new CSelection { m_Database, CDataParser::Split( columns, false, false, ',' ), table };
	}

	// PROJECTION
	else if ( queryName == CQueryParser::PROJECTION ) {
		auto * conditionQuery = new CCondition;
		string condition, table;
		if (
				! ReadQueryParenthesis( queryDetails, '[', ']', stringProgress, condition ) ||
				! ReadQueryParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table ) ||
				! ValidateConditionSyntax( condition, conditionQuery )
			) {
			delete conditionQuery;
			return CConsole::INVALID_QUERY;
		}
		CDataParser::TrimAllSpaces( conditionQuery->m_Column, '"' );
		CDataParser::TrimAllSpaces( conditionQuery->m_Constant, '"' );

		userQuery = new CProjection { m_Database, conditionQuery, table };
	} else {
		return CConsole::INVALID_QUERY;
	}

	// evaluation function for each query
	if ( ! userQuery->Evaluate( ) ) {
		delete userQuery;
		return CConsole::INVALID_QUERY;
	}

	// result to output
	cout << * userQuery->GetQueryResult( );
	CLog::Msg( CLog::QP, userQuery->GetSQL( ) );

	// save query option ?
	string querySaveName;
	if ( stringProgress == queryDetails.length( ) )
		delete userQuery;
	else {
		// if user enters tilda (~) without any query save name, the program will ignore it
		if ( ReadQuerySave( queryDetails.substr( stringProgress ), '~', querySaveName ) ) {
			if ( ! m_Database.InsertQuery( querySaveName, userQuery ) ) {
				delete userQuery;
				return CConsole::INVALID_QUERY;
			}
			userQuery->ArchiveQueryName( querySaveName );
			CLog::BoldMsg( CLog::QP, querySaveName, CLog::QP_QUERY_ADDED );
		} else {
			delete userQuery;
		}
	}

	// success
	return CConsole::VALID_QUERY;
}

/**
 * Validates condition syntax of a query.
 * @param[in] query condition expression of the query
 * @return true if condition is correct (in syntax sense only)
 */
bool CQueryParser::ValidateConditionSyntax ( const string & query, CCondition * output ) {
	string column, constant;
	size_t found = 0;
	for ( const string & i : m_Operators ) {
		if ( ( found = query.find( i ) ) != string::npos ) {
			column = query.substr( 0, found );
			if ( column.empty( ) ) {
				CLog::HighlightedMsg( CLog::QP, query, CLog::QP_INVALID_CON );
				return false;
			}
			constant = query.substr( found + i.size( ), query.size( ) );
			if ( constant.empty( ) ) {
				CLog::HighlightedMsg( CLog::QP, query, CLog::QP_INVALID_CON );
				return false;
			}
			output->m_Column   = column;
			output->m_Constant = constant;
			output->m_Operator = i;
			return true;
		}
	}
	CLog::HighlightedMsg( CLog::QP, query, CLog::QP_INVALID_REL );
	return false;
}

/**
 * Constructor with application database reference.
 */
CQueryParser::CQueryParser ( CDatabase & ref ) : m_Database( ref ) {
	m_Operators.emplace_back( "==" );
	m_Operators.emplace_back( "!=" );
	m_Operators.emplace_back( ">=" );
	m_Operators.emplace_back( "<=" );
	m_Operators.emplace_back( ">"  );
	m_Operators.emplace_back( "<"  );
}
