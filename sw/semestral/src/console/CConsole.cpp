#include "CConsole.hpp"

int CConsole::Start ( CDatabase & dbRef ) {
	string buffer;
	char in;
	CQueryParser cqp { dbRef };

	CLog::Msg( CLog::CON, CLog::CON_START );
	while ( ! cin.eof( ) ) {
//		CLog::CmdPromp( CLog::CON_PROMPT_CHAR );
		buffer.clear( );
		while ( cin >> in ) {
			if ( in == ';' )
				break;
			buffer += in;
		}
		if ( cin.eof( ) ) {
			cout << endl;
			CLog::Msg( CLog::CON, CLog::CON_EOF_DETECTED );
			return - 1;
		}
		switch ( cqp.ParseQuery( buffer ) ) {
			case CConsole::VALID_QUERY: break;
			case CConsole::INVALID_QUERY:
				CLog::Msg( CLog::CON,
				           string( "\"\u001b[1m" )
						           .append( buffer )
						           .append( "\u001b[0m\"" )
						           .append( CLog::CON_INVALID_QUERY )
						           .append( "\u001b[0m" )
				);
			case CConsole::EXIT_CONSOLE:
				CLog::Msg( CLog::CON, CLog::CON_EXIT );
				return 0;
			default: break;
		}
	}
	return 1;
}
