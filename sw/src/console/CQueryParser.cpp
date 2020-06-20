#include "CQueryParser.hpp"

/**
 * Reads and determines if a user wants the query to be saved into memory.
 * @param[in] queryDetails query substring
 * @param[in] saveDelimiter delimiter that separates the characters for the query name
 * @param[in, out] output the query save name
 * @return true if user wants to save the query and the name is not used and atleast a character long
 */
bool CQueryParser::ReadQSave ( const string & queryDetails, const char & saveDelimiter, string & output ) {
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
bool CQueryParser::ReadQName ( const string & fullQuery, string & output ) {
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
bool CQueryParser::ReadQParenthesis ( const string & queryDetails, const char & delStart, const char & delEnd, size_t & stringPos, string & output ) {
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
int CQueryParser::ProcessQuery ( const string & basicString ) const {
	// basic query recognition
	string queryName, queryDetails;
	if ( ! ReadQName( basicString, queryName ) )
		return CConsole::INVALID_QUERY;
	queryDetails = basicString.substr( queryName.length( ) );

	// interface commands
	if ( queryDetails.empty( ) ) {
		if ( queryName == CLog::TABLES ) {
			m_Database.ListTables( );
			return CConsole::VALID_QUERY;
		}
		if ( queryName == CLog::QUERIES ) {
			m_Database.ListQueries( );
			return CConsole::VALID_QUERY;
		}
		if ( queryName == CLog::QUIT )
			return CConsole::EXIT_CONSOLE;
		return CConsole::INVALID_QUERY;
	}

	// relational algebra inputs
	size_t stringProgress = 0;
	CTableQuery * userQuery;

	if ( queryName == CLog::SELECTION ) {
		string columns, table;
		if (
				! ReadQParenthesis( queryDetails, '[', ']', stringProgress, columns ) ||
				! ReadQParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table )
			)
			return CConsole::INVALID_QUERY;
		userQuery = new CSelection ( m_Database, CDataParser::Split( columns, false, false, ',' ), table );
	}

	else if ( queryName == CLog::ALIAS ) {
		string table, colNames;
		if (
				! ReadQParenthesis( queryDetails, '[', ']', stringProgress, colNames ) ||
				! ReadQParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table )
			)
			return CConsole::INVALID_QUERY;
		vector<string> tableNames = CDataParser::Split( colNames, '~' );
		if ( tableNames.size( ) != 2 )
			return CConsole::INVALID_QUERY;

		userQuery = new CAlias ( m_Database, std::make_pair( tableNames.at( 0 ), tableNames.at( 1 ) ), table );
		if ( ! userQuery->Evaluate( ) ) {
			delete userQuery;
			return CConsole::INVALID_QUERY;
		}
		delete userQuery;
		return CConsole::VALID_QUERY;
	}

	else if ( queryName == CLog::PROJECTION ) {
		auto * conditionQuery = new CCondition;
		string condition, table;
		if (
				! ReadQParenthesis( queryDetails, '[', ']', stringProgress, condition ) ||
				! ReadQParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, table ) ||
				! ValidateConditionSyntax( condition, conditionQuery )
			) {
			delete conditionQuery;
			return CConsole::INVALID_QUERY;
		}
		CDataParser::TrimAllSpaces( conditionQuery->m_Column, '"' );
		CDataParser::TrimAllSpaces( conditionQuery->m_Constant, '"' );
		userQuery = new CProjection ( m_Database, conditionQuery, table );
	}

	else if ( queryName == CLog::NJOIN ) {
		string tables;
		if ( ! ReadQParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, tables ) )
			return CConsole::INVALID_QUERY;

		vector<string> tableNames = CDataParser::Split( tables, ',' );
		if ( tableNames.size( ) != 2 )
			return CConsole::INVALID_QUERY;

		userQuery = new CNaturalJoin ( m_Database, std::make_pair( tableNames.at( 0 ), tableNames.at( 1 ) ) );
	}

	else if ( queryName == CLog::JOIN ) {
		string tables, column;
		if ( ! ReadQParenthesis( queryDetails, '[', ']', stringProgress, column ) || ! ReadQParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, tables ) )
			return CConsole::INVALID_QUERY;

		vector<string> tableNames = CDataParser::Split( tables, ',' );
		if ( tableNames.size( ) != 2 )
			return CConsole::INVALID_QUERY;

		userQuery = new CJoin ( m_Database, column, std::make_pair( tableNames.at( 0 ), tableNames.at( 1 ) ) );
	}

	else if ( queryName == CLog::UNION || queryName == CLog::INTERSECT || queryName == CLog::MINUS ) {
		string tables;
		if ( ! ReadQParenthesis( queryDetails.substr( stringProgress ), '(', ')', stringProgress, tables ) )
			return CConsole::INVALID_QUERY;

		vector<string> tableNames = CDataParser::Split( tables, ',' );
		if ( tableNames.size( ) != 2 )
			return CConsole::INVALID_QUERY;

		if ( queryName == CLog::UNION )
			userQuery = new CUnion ( m_Database, std::make_pair( tableNames.at( 0 ), tableNames.at( 1 ) ) );

		else if ( queryName == CLog::INTERSECT )
			userQuery = new CIntersect ( m_Database, std::make_pair( tableNames.at( 0 ), tableNames.at( 1 ) ) );

//		else if ( queryName == CLog::MINUS )

		else
			userQuery = new CMinus ( m_Database, std::make_pair( tableNames.at( 0 ), tableNames.at( 1 ) ) );
	}

	else {
		return CConsole::INVALID_QUERY;
	}

	// evaluation process (existing tables, columns, valid conditions, ..)
	if ( ! userQuery->Evaluate( ) ) {
		delete userQuery;
		return CConsole::INVALID_QUERY;
	}

	// generating the SQL equivalent
	cout << * userQuery->GetQueryResult( );
	CLog::Msg( CLog::QP, userQuery->GetSQL( ) );

	// query save option scan
	string querySaveName;
	if ( stringProgress == queryDetails.length( ) )
		delete userQuery;
	else {
		// tilda with empty name is ignored
		if ( ReadQSave( queryDetails.substr( stringProgress ), '~', querySaveName ) ) {
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
bool CQueryParser::ValidateConditionSyntax ( const string & query, CCondition * output ) const {
	string column, constant;
	size_t found;
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
