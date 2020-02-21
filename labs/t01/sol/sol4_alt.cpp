#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const size_t N_MIN = 5;

const char * INVALID_INPUT = "Nespravny vstup.";
const char * UNSUCCESSFUL_WRITE = "Zapis se nezdaril.";

const char * INPUT_FILENAME = "data/0000_in.txt";

struct array_t {
	size_t n;
	double * data;
};

bool array_scan ( array_t & arr, istream & in = cin );

bool array_load ( array_t & arr, const char * filename );

void array_print ( const array_t & arr, ostream & out = cout );

bool array_save ( const array_t & arr, const char * filename );

void array_sort ( array_t & arr );

void array_free ( array_t & arr );

int main ()
{
	array_t arr { 0, nullptr };

	if ( !array_load( arr, INPUT_FILENAME ) ) {
		cout << INVALID_INPUT << endl;
		return 1;
	}

	array_print( arr ); cout << endl;

	array_sort( arr );
	array_print( arr ); cout << endl;

	array_free( arr );
	return 0;
}

/*****************************************************************************/

bool array_scan ( array_t & arr, istream & in )
{
	if ( !( in >> arr.n ) || arr.n < N_MIN ) {
		return false;
	}

	arr.data = new double [ arr.n ];
	for ( size_t i = 0; i < arr.n; ++i ) {
		if ( !( in >> arr.data[ i ] ) ) {
			array_free( arr );
			return false;
		}
	}

	return true;
}

bool array_load ( array_t & arr, const char * filename )
{
	ifstream fin { filename };
	if ( !fin )
		return false;
	return array_scan( arr, fin );
}

void array_print ( const array_t & arr, ostream & out )
{
	for ( size_t i = 0; i < arr.n; ++i ) {
		if ( i != 0 )
			out << " ";
		out << arr.data[ i ];
	}
}

bool array_save ( const array_t & arr, const char * filename )
{
	ofstream fout { filename };
	if ( !fout )
		return false;
	
	if ( !( fout << arr.n << endl ) )
		return false;
	
	array_print( arr, fout );
	if ( !fout )
		return false;

	if ( !( fout << endl ) )
		return false;

	return true;
}

void array_sort ( array_t & arr )
{
  sort( arr.data, arr.data + arr.n );
}

void array_free ( array_t & arr )
{
	delete [] arr.data;
	arr.n = 0;
	arr.data = nullptr;
}
