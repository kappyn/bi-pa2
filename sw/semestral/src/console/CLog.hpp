#pragma once

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

/**
 * This module works as an application logger, storing messages to print, providing better user experience.
 */
class CLog {
public:
	static const int APP_PADDING;
	static const string APP_LINE_HEIGHT;

	static void
	Msg ( const string & branch, const string & message, const string & extraPad = CLog::APP_LINE_HEIGHT, ostream & ost = cout );

	static void
	HighlightedMsg ( const string & branch, const string & highlighted, const string & message, const string & extraPad = CLog::APP_LINE_HEIGHT, ostream & ost = cout );

	static void
	BoldMsg ( const string & branch, const string & highlighted, const string & message, const string & extraPad = CLog::APP_LINE_HEIGHT, ostream & ost = cout );

	static void
	CmdPromp ( const char & promptChar = CLog::CON_PROMPT_CHAR, ostream & ost = cout );

	static const string APP_COLOR_GOOD;
	static const string APP_COLOR_BAD;
	static const string APP_COLOR_RESULT;

	static const string APP_COLOR_RESET;
	static const string FM;
	static const string FM_CFG_FOUND;
	static const string FM_CFG_NOT_FOUND;
	static const string FM_CFG_FAILED;
	static const string FM_LOADING;
	static const string FM_IMPORT_OK;

	static const string FM_IMPORT_FAIL;
	static const string DP;
	static const string DP_LINE_MISMATCH;
	static const string DP_EMPTY_LINE;

	static const string DP_NO_DATATYPES;
	static const string CON;
	static const string CON_START;
	static const string CON_INVALID_QUERY;
	static const string CON_EOF_DETECTED;
	static const string CON_EXIT;
	static const char CON_PROMPT_CHAR;
	static const string CON_LISTING_Q;

	static const string CON_LISTING_T;
	static const string TAB_NO_BODY;
	static const string TAB_NO_DATA;

	static const string TAB_INVALID_INDEX;
	static const string QP;
	static const string QP_NO_SUCH_TABLE;
	static const string QP_NO_SUCH_COL;
	static const string QP_QUERY_ADDED;
	static const string QP_TABLE_EXISTS;
};