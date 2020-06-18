#include "console/CApplication.hpp"

int main ( ) {
	CApplication App { "DATABASE", "examples/sources.cfg" };
	return App.Run( );
}