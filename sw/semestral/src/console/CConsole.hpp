#pragma once

#include "CLog.hpp"
#include "CQueryParser.hpp"
#include "../tools/CDataParser.hpp"

#include <iostream>
#include <iomanip>
#include <string>

class CConsole {
public:
	static int Start ( ) {
		string buffer;
		char in;

		CLog::Msg( CLog::CON, CLog::CON_START );
		while ( ! cin.eof( ) ) {
			CLog::CmdPromp( CLog::CON_PROMPT_CHAR );
			buffer.clear( );
			while ( cin >> in ) {
				if ( in == ';' )
					break;
				buffer += in;
			}
			if ( cin.eof( ) ) {
				cout << endl;
				CLog::Msg( CLog::CON, CLog::CON_EOF_DETECTED );
				return -1;
			}
			if ( ! CQueryParser::ParseQuery( buffer ) )
				CLog::Msg( CLog::CON,
				           string( "\"\u001b[1m" )
						           .append( buffer )
						           .append( "\u001b[0m\"" )
						           .append( CLog::CON_INVALID_QUERY )
						           .append( "\u001b[0m" )
				);
			else {
				cout << "End of command!\nEntered command: " << buffer << endl;
			}
		}
		return 1;
	}
};