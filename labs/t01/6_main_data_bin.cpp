#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const int N_MIN = 5;
const char * INVALID_INPUT = "Nespravny vstup.";
const char * UNSUCCESSFUL_WRITE = "Zapis se nezdaril.";

const char * INPUT_FILENAME = "data/0001_in.txt";
const char * OUTPUT_FILENAME = "sorted.bin";

struct array_t {
    size_t n;
    double * data;
};

bool array_scan ( array_t & arr, istream & in = cin );
void array_print ( const array_t & arr, ostream & out = cout );
bool array_load ( array_t & arr, const char * filename );
bool array_load_bin ( array_t & arr, const char * filename );
bool array_write ( const array_t & arr, const char * filename );
bool array_write_bin ( const array_t & arr, const char * filename );
void array_sort ( array_t & arr );
void array_free ( array_t & arr );

int main ( void ) {
//  reading an input from a file and exporting to .bin file.
//    array_t ar = { 0, nullptr };
//    if ( ! array_scan( ar ) ) {
//        cout << INVALID_INPUT << endl;
//        return 1;
//    }
//    array_sort( ar );
//    if ( ! array_write_bin( ar, OUTPUT_FILENAME ) ) {
//        cout << UNSUCCESSFUL_WRITE << endl;
//        return 2;
//    }
//    array_free( ar );
//    return 0;

//  loading input from file. sorts unsorted and sorted.
    array_t ar = { 0, nullptr };

    if ( ! array_load_bin( ar, INPUT_FILENAME ) ) {
        cout << INVALID_INPUT << endl;
        return 1;
    }

    array_print( ar ); cout << endl;
    array_sort( ar );
    array_print( ar ); cout << endl;
}

/****/
bool array_scan ( array_t & ar, istream & str )
{
    if ( ! ( str >> ar.n ) || ar.n < N_MIN ) {
        return false;
    }

    ar.data = new double [ ar.n ];
    for ( size_t i = 0; i < ar.n; ++i ) {
        if ( ! ( str >> ar.data[ i ] ) ) {
            array_free( ar );
            return false;
        }
    }

    return true;
}

void array_print ( const array_t & arr, ostream & out )
{
    for ( size_t i = 0; i < arr.n; ++i ) {
        if ( i != 0 )
            out << " ";
        out << arr.data[ i ];
    }
}

bool array_load ( array_t & arr, const char * filename )
{
    ifstream fin { filename };
    if ( ! fin ) // Does the file even exist?
        return false;
    return array_scan( arr, fin );
}

bool array_load_bin ( array_t & arr, const char * filename )
{
    // TODO: alt
}

bool array_write ( const array_t & arr, const char * filename )
{
    ofstream os { OUTPUT_FILENAME };
    if ( ! os )
        return false;

    if ( ! ( os << arr.n << endl ) )
        return false;

    array_print( arr, os );

    if ( ! ( os << endl ) )
        return false;

    return true;
}

bool array_write_bin ( const array_t & arr, const char * filename )
{
    // TODO: alt

    ofstream os { OUTPUT_FILENAME, ios::binary };
    if ( ! os )
        return false;

    if ( ! os.write( ( ( const char * ) &arr.n ), sizeof( arr.n ) ) ) {
        return false;
    }

	for ( size_t i = 0; i < arr.n; ++i )
		if ( ! os.write( ( const char * )( arr.data + i ), sizeof( arr.data[ i ] ) ) )
			return false;

    return true;
}

void array_sort ( array_t & arr )
{
    sort( arr.data, arr.data + arr.n );
}

void array_free ( array_t & arr )
{
    delete [ ] arr.data;
	arr.n = 0;
	arr.data = NULL;
}