#include "console/CApplication.hpp"

int main ( ) {
	CApplication App { "DATABASE", "sources.cfg" };
	return App.Run( );
}