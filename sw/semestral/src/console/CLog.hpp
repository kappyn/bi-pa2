#pragma once

#include <iostream>
#include <iomanip>
#include <string>

/**
 * This module works as an application logger, storing messages to print, providing better UI.
 */
namespace CLog {
	using namespace std;

	static constexpr int APP_PADDING ( )
	{ return 14; }
	
	const string APP_LINE_HEIGHT    = "\n";
	const string APP_COLOR_GOOD     = "\033[0;32m";
	const string APP_COLOR_RESULT   = "\033[1;36m";
	const string APP_COLOR_BAD      = "\u001b[31m";
	const string APP_COLOR_RESET    = "\u001b[0m";

	const string FM                 = "FILE MANAGER";
	const string FM_CFG_FOUND       = "Configuration file found.";
	const string FM_CFG_NOT_FOUND   = "Configuration file not found.";
	const string FM_CFG_FAILED      = "Configuration file wasn't imported properly.";
	const string FM_LOADING         = "Loading tables from the file into database..";
	const string FM_IMPORT_OK       = " successfully imported!";
	const string FM_IMPORT_FAIL     = " is in incorrect format or doesn't exist.";

	const string DP                 = "DATA PARSER";
	const string DP_LINE_MISMATCH   = " number of columns does not match at line ";
	const string DP_NO_DATATYPES    = " data types for the columns were not provided.";
	const string DP_EMPTY_LINE      = " incorrect formatting - empty line at ";

	const string CON                = "CONSOLE";
	const string CON_START          = "Starting console..";
	const string CON_INVALID_QUERY  = " invalid command.";
	const string CON_EOF_DETECTED   = "EOF detected. Exiting application.";
	const string CON_EXIT           = "Exiting application.";
	const char   CON_PROMPT_CHAR    = '>';
	const string CON_LISTING_T      = "Listing existing tables..";
	const string CON_LISTING_Q      = "Listing saved queries..";

	const string TAB_NO_BODY        = "Unable to render - table body is missing.";
	const string TAB_NO_DATA        = "The table doesn't have any columns.";
	const string TAB_INVALID_INDEX  = "Invalid index.";

	const string QP                 = "QUERY PARSER";
	const string QP_NO_SUCH_TABLE   =  " table was not found.";
	const string QP_NO_SUCH_COL     =  " column was not found.";
	const string QP_QUERY_ADDED     =  " query was added!";
	const string QP_TABLE_EXISTS    =  " name already taken (please use TABLES or QUERIES to prevent this).";
	const string QP_INVALID_CON     =  " the condition clause is missing an operand.";
	const string QP_INVALID_REL     =  " the condition clause is missing a relation operator.";
	const string QP_CON_PARSE_ERROR =  " unable to convert constant to a correct type.";

	/**
	 * Standart message.
	 * @param[in] branch module from where the message is originated
	 * @param[in] message log message string
	 * @param[in, out] ost output stream to log to.
	 */
	inline void Msg ( const string & branch, const string & message, const string & extraPad = APP_LINE_HEIGHT, ostream & ost = cout ) {
		ost << setw( APP_PADDING( ) ) << right << branch << ": " << message << extraPad << endl;
	}

	/**
	 * Highlighted message
	 * @param[in] branch module from where the message is originated
	 * @param[in] highlighted highlighted part of the message
	 * @param[in] message log message string
	 * @param[in, out] ost output stream to log to.
	 */
	inline void
	HighlightedMsg ( const string & branch, const string & highlighted, const string & message,
	                 const string & extraPad = APP_LINE_HEIGHT, ostream & ost = cout ) {
		ost << setw( APP_PADDING( ) ) << right << branch << ": " << "\u001b[1m"
		<< string( "\"\u001b[1m" ).append( highlighted ).append( "\u001b[0m\"" ) << message << extraPad << endl;
	}

	/**
	 * Bold message
	 * @param[in] branch module from where the message is originated
	 * @param[in] highlighted highlighted part of the message
	 * @param[in] message log message string
	 * @param[in, out] ost output stream to log to.
	 */
	inline void
	BoldMsg ( const string & branch, const string & highlighted, const string & message,
	          const string & extraPad = APP_LINE_HEIGHT, ostream & ost = cout ) {
		ost << setw( APP_PADDING( ) ) << right << branch << ": " << "\u001b[1m"
		<< string( "\u001b[1m" ).append( highlighted ).append( "\u001b[0m" ) << message << extraPad << endl;
	}
}