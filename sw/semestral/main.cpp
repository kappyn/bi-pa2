#include "src/database/CDatabase.hpp"
#include "src/tools/CFileManager.hpp"
#include "src/console/CConsole.hpp"

using namespace std;

int main ( ) {
	// basic program configuration
	CDatabase Database { "Mock DB" };
	CFileManager FileManager { "sources.cfg", Database };

	// program itself
	FileManager.LoadTables( );
	Database.PrintTables( );
	CConsole::Start( );
	return 0;
}