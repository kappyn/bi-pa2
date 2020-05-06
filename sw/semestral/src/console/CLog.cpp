#include "CLog.hpp"

const int    CLog::APP_PADDING       = 18;

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
const string CLog::CON_INVALID_QUERY  = "\u001b[31m command not recognized..\u001b[0m";
const string CLog::CON_EOF_DETECTED   = "EOF detected. Exiting application.";
const char   CLog::CON_PROMPT_CHAR    = '>';

void CLog::Msg ( const string & branch, const string & message, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << message << endl;
}

void
CLog::HighlightedMsg ( const string & branch, const string & highlighted, const string & message, ostream & ost ) {
	ost << setw( CLog::APP_PADDING ) << right << branch << ": " << "\u001b[1m"
	<< string( "\"\u001b[1m" ).append( highlighted ).append( "\u001b[0m\"" ) << message << endl;
}

void CLog::CmdPromp ( const char & promptChar, ostream & ost ) {
	ost << setw( CLog::APP_PADDING + 1 ) << right << promptChar << " ";
}

