#include "CDataParser.hpp"

void CDataParser::Trim ( string & token, const char & delimiter ) {
	token.erase( remove( token.begin( ), token.end( ), delimiter ), token.end( ) );
}

vector<string> CDataParser::Split ( string & s, const bool & allowQuotes, const char & delim ) {
	Trim( s, ' ' );
	stringstream sst( s );
	string token;
	vector<string> tokens;
	while ( getline( sst, token, delim ) ) {
		if ( ! allowQuotes )
			Trim( token, '\"' );
		tokens.push_back( token );
	}
	return tokens;
}

bool CDataParser::ParseTable ( CDatabase & db, ifstream & ifs ) {
	string tmp;

	// table column names
	getline( ifs, tmp );
	if ( * ( tmp.end( ) - 1 ) == ',' )
		return false;

	vector<string> columnNames = Split( tmp, false );
	for ( const auto & i : columnNames )
		if ( i.empty( ) )
			return false;
	size_t requiredColumns = columnNames.size( );

	// table rows
	tmp.clear( );
	while ( getline( ifs, tmp ) ) {
		// checks for incomplete rows
		if ( * ( tmp.end( ) - 1 ) == ',' || tmp.empty( ) )
			return false;

		vector<string> newRow = Split ( tmp );

		// checks for number of columns to insert
		if ( newRow.size( ) != requiredColumns ) {
			for ( const auto & i : newRow )
				cout << i << " ";
			cout << endl;
			return false;
		}
	}

	return true;
}
