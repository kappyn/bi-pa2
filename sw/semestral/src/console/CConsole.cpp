#include "CConsole.hpp"

int CConsole::Start ( CDatabase & dbRef ) {
	string buffer, query;
	CQueryParser cqp { dbRef };
	CLog::Msg( CLog::CON, CLog::CON_START );

	bool quote = false;
	while ( ! cin.eof( ) ) {
		buffer.clear( );
		query.clear( );

		getline( std::cin, buffer, ';' );

//		cout << "EOF:  "  << cin.eof( ) << endl;
//		cout << "FAIL: " << cin.fail( ) << endl;

		if ( cin.eof( ) || cin.fail( ) )
			break;

		for ( const char & i : buffer ) {
			if ( i == '\n' )
				continue;
			if ( ! quote ) {
				if ( i == '"' ) {
					query += i;
					quote = true;
					continue;
				}
				if ( i != ' ' )
					query += i;
			} else {
				if ( i == '"' ) {
					query += i;
					quote = false;
					continue;
				}
				query += i;
			}
		}

		switch ( cqp.ProcessQuery( query ) ) {
			case CConsole::VALID_QUERY:
				continue;

			case CConsole::INVALID_QUERY:
				CLog::Msg( CLog::CON,
				           string( "\"\u001b[1m" )
						           .append( query )
						           .append( "\u001b[0m\"" )
						           .append( CLog::CON_INVALID_QUERY )
						           .append( "\u001b[0m" )
				);
				continue;

			case CConsole::EXIT_CONSOLE:
				CLog::Msg( CLog::CON, CLog::CON_EXIT );
				return 0;
		}
	}

	CLog::Msg( CLog::CON, CLog::CON_EOF_DETECTED );
	return CConsole::EXIT_CONSOLE;
}
