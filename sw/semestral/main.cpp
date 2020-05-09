#include "src/database/CDatabase.hpp"
#include "src/tools/CFileManager.hpp"
#include "src/console/CConsole.hpp"

using namespace std;

int main ( ) {
	CDatabase Database { "Mock database" };
	CFileManager FileManager { "sources.cfg", Database };
	FileManager.LoadTables( );
	Database.PrintTables( );
//	CConsole::Start( );
	return 0;
}