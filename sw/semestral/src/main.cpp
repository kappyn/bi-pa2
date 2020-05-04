#include "CDatabase.hpp"
#include "CDataParser.hpp"
#include "CFileManager.hpp"

using namespace std;

int main ( ) {
	CDatabase Database;
	CFileManager FileManager { "sources.cfg", Database };
	FileManager.LoadTables( );

	return 0;
}