#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct CTableData {
	string m_TableName;
	string m_DataDelim;

	vector<vector<string>> m_Rows;
};

class CDataParser {
private:
	CTableData m_Data;

	vector<string> Split ( const string & s, const char & delim ) {
		stringstream sst( s );
		string token;
		vector<string> tokens;
		while ( getline( sst, token, delim ) )
			tokens.push_back( token );
		return tokens;
	}

public:
	CDataParser ( const string & fileLocation, const char & dataDelimeter ) {
		ifstream ifs { fileLocation, ios::in };
		if ( ! ifs )
			throw logic_error( "Invalid file name" );

		string line = "";
		while ( getline( ifs, line ) )
			m_Data.m_Rows.push_back( Split( line, dataDelimeter ) );
	}

	friend ostream & operator << ( ostream & ost, const CDataParser & ctp ) {
		for ( const auto & i : ctp.m_Data.m_Rows )
			for ( const string & j : i )
				cout << j << ",";
		cout << endl;
		return ost;
	}
};
