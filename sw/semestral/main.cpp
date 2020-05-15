#include "src/console/CApplication.hpp"

int main ( ) {
	 CApplication App { "MOCK DB", "sources.cfg" };
	 return App.Run( );
}