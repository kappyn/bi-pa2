#include "csv.hpp"

int main ( ) {
	CDataParser test ( "data/sample1.csv", ',' );
	std::cout << test << std::endl;
	return 0;
}