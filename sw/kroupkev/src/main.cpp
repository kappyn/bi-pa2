#include "console/CApplication.hpp"
#include "console/CLog.hpp"

int main ( ) {
	CApplication App { CLog::APP_DB_NAME, CLog::APP_CFG_PATH };
	return App.Run( );
}