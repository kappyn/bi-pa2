#include "CLog.hpp"

const int    CLog::APP_PADDING       = 17;

const string CLog::FM                = "FILE MANAGER";
const string CLog::FM_CFG_FOUND      = "Configuration file found.";
const string CLog::FM_CFG_NOT_FOUND  = "\u001b[31mConfiguration file not found.\u001b[0m";
const string CLog::FM_CFG_FAILED     = "\u001b[31mConfiguration file wasn't imported properly.\u001b[0m";
const string CLog::FM_LOADING        = "Loading tables from the file into database..";
const string CLog::FM_IMPORT_OK      = "\u001b[32m successfully imported!\u001b[0m";
const string CLog::FM_IMPORT_FAIL    = "\u001b[31m is in incorrect format or doesn't exist.\u001b[0m";

const string CLog::DP                = "DATA PARSER";
const string CLog::DP_LINE_MISMATCH  = "\u001b[31m number of columns does not match at line ";
const string CLog::DP_NO_DATATYPES   = "\u001b[31m data types for the columns were not provided.";

const string CLog::CON                = "CONSOLE";
const string CLog::CON_START          = "Starting console..";
const string CLog::CON_INVALID_QUERY  = "\u001b[31m an error had occurred during command execution.\u001b[0m";
const string CLog::CON_EOF_DETECTED   = "EOF detected. Exiting application.";
const char   CLog::CON_PROMPT_CHAR    = '>';

const string CLog::TAB_NO_BODY        = "Unable to render - table body is missing.";
const string CLog::TAB_INVALID_INDEX  = "Invalid index.";

const string CLog::QP                 = "QUERY PARSER";
const string CLog::QP_NO_SUCH_TABLE   =  "\u001b[31m table was not found.\u001b[0m";
const string CLog::QP_NO_SUCH_COL     =  "\u001b[31m column was not found.\u001b[0m";


/**
 * Standart message.
 * @param[in] branch module from where the message is originated
 * @param[in] message log message string
 * @param[in, out] ost output stream to log to.
 */
void CLog::Msg ( const string & branch, const string & message, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << message << endl;
}

/**
 * Highlighted message
 * @param[in] branch module from where the message is originated
 * @param[in] highlighted highlighted part of the message
 * @param[in] message log message string
 * @param[in, out] ost output stream to log to.
 */
void
CLog::HighlightedMsg ( const string & branch, const string & highlighted, const string & message, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << "\u001b[1m"
	<< string( "\"\u001b[1m" ).append( highlighted ).append( "\u001b[0m\"" ) << message << endl;
}

/**
 * Bold message
 * @param[in] branch module from where the message is originated
 * @param[in] highlighted highlighted part of the message
 * @param[in] message log message string
 * @param[in, out] ost output stream to log to.
 */
void
CLog::BoldMsg ( const string & branch, const string & highlighted, const string & message, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << "\u001b[1m"
	<< string( "\u001b[1m" ).append( highlighted ).append( "\u001b[0m" ) << message << endl;
}

/**
 * Prints a CLI prompt.
 * @param[in] promptChar prompt character to be printed
 * @param[in, out] ost output stream to log to.
 */
void CLog::CmdPromp ( const char & promptChar, ostream & ost ) {
	ost << setw( CLog::APP_PADDING + 1 ) << right << promptChar << " ";
}

