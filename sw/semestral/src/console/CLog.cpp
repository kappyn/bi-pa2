#include "CLog.hpp"

const int    CLog::APP_PADDING        = 14;
const string CLog::APP_LINE_HEIGHT    = "\n";

const string CLog::APP_COLOR_GOOD     = "\033[0;32m";
const string CLog::APP_COLOR_RESULT   = "\033[1;36m";
const string CLog::APP_COLOR_BAD      = "\u001b[31m";
const string CLog::APP_COLOR_RESET    = "\u001b[0m";

const string CLog::FM                 = "FILE MANAGER";
const string CLog::FM_CFG_FOUND       = "Configuration file found.";
const string CLog::FM_CFG_NOT_FOUND   = string( APP_COLOR_BAD ).append( "Configuration file not found." ).append( APP_COLOR_RESET );
const string CLog::FM_CFG_FAILED      = string( APP_COLOR_BAD ).append( "Configuration file wasn't imported properly." ).append( APP_COLOR_RESET );
const string CLog::FM_LOADING         = "Loading tables from the file into database..";
const string CLog::FM_IMPORT_OK       = string( APP_COLOR_GOOD ).append( " successfully imported!" ).append( APP_COLOR_RESET );
const string CLog::FM_IMPORT_FAIL     = string( APP_COLOR_BAD ).append(" is in incorrect format or doesn't exist." ).append( APP_COLOR_RESET );

const string CLog::DP                 = "DATA PARSER";
const string CLog::DP_LINE_MISMATCH   = string( APP_COLOR_BAD ).append(" number of columns does not match at line " );
const string CLog::DP_NO_DATATYPES    = string( APP_COLOR_BAD ).append(" data types for the columns were not provided." );
const string CLog::DP_EMPTY_LINE      = string( APP_COLOR_BAD ).append(" incorrect formatting - empty line at " );

const string CLog::CON                = "CONSOLE";
const string CLog::CON_START          = "Starting console..";
const string CLog::CON_INVALID_QUERY  = string( APP_COLOR_BAD ).append(" invalid command." ).append( APP_COLOR_RESET );
const string CLog::CON_EOF_DETECTED   = "EOF detected. Exiting application.";
const string CLog::CON_EXIT           = "Exiting application.";
const char   CLog::CON_PROMPT_CHAR    = '>';
const string CLog::CON_LISTING_T      = "Listing existing tables..";
const string CLog::CON_LISTING_Q      = "Listing saved queries..";

const string CLog::TAB_NO_BODY        = "Unable to render - table body is missing.";
const string CLog::TAB_NO_DATA        = "The table doesn't have any columns.";
const string CLog::TAB_INVALID_INDEX  = "Invalid index.";

const string CLog::QP                 = "QUERY PARSER";
const string CLog::QP_NO_SUCH_TABLE   =  string( APP_COLOR_BAD ).append(" table was not found." ).append( APP_COLOR_RESET );
const string CLog::QP_NO_SUCH_COL     =  string( APP_COLOR_BAD ).append(" column was not found." ).append( APP_COLOR_RESET );
const string CLog::QP_QUERY_ADDED     =  string( APP_COLOR_GOOD ).append(" query was added!" ).append( APP_COLOR_RESET );
const string CLog::QP_TABLE_EXISTS    =  string( APP_COLOR_BAD ).append(" name already taken (please use TABLES or QUERIES to prevent this)." ).append( APP_COLOR_RESET );
const string CLog::QP_INVALID_CON     =  string( APP_COLOR_BAD ).append(" the condition clause is missing an operand." ).append( APP_COLOR_RESET );
const string CLog::QP_INVALID_REL     =  string( APP_COLOR_BAD ).append(" the condition clause is missing a relation operator." ).append( APP_COLOR_RESET );
const string CLog::QP_CON_PARSE_ERROR =  string( APP_COLOR_BAD ).append(" unable to convert constant to a correct type." ).append( APP_COLOR_RESET );


/**
 * Standart message.
 * @param[in] branch module from where the message is originated
 * @param[in] message log message string
 * @param[in, out] ost output stream to log to.
 */
void CLog::Msg ( const string & branch, const string & message, const string & extraPad, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << message << extraPad << endl;
}

/**
 * Highlighted message
 * @param[in] branch module from where the message is originated
 * @param[in] highlighted highlighted part of the message
 * @param[in] message log message string
 * @param[in, out] ost output stream to log to.
 */
void
CLog::HighlightedMsg ( const string & branch, const string & highlighted, const string & message, const string & extraPad, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << "\u001b[1m"
	<< string( "\"\u001b[1m" ).append( highlighted ).append( "\u001b[0m\"" ) << message << extraPad << endl;
}

/**
 * Bold message
 * @param[in] branch module from where the message is originated
 * @param[in] highlighted highlighted part of the message
 * @param[in] message log message string
 * @param[in, out] ost output stream to log to.
 */
void
CLog::BoldMsg ( const string & branch, const string & highlighted, const string & message, const string & extraPad, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << "\u001b[1m"
	<< string( "\u001b[1m" ).append( highlighted ).append( "\u001b[0m" ) << message << extraPad << endl;
}

/**
 * Prints a CLI prompt.
 * @param[in] promptChar prompt character to be printed
 * @param[in, out] ost output stream to log to.
 */
void CLog::CmdPromp ( const char & promptChar, ostream & ost ) {
	ost << setw( CLog::APP_PADDING + 1 ) << right << promptChar << " ";
}

