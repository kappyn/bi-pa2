#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

/** Defines minimum size of array. */
const int N_MIN = 5;

/** Program messages */
const char * INVALID_INPUT = "Nespravny vstup.";
const char * UNSUCCESSFUL_WRITE = "Zapis se nezdaril.";

const char * OUTPUT_FILENAME = "sorted.txt";

/**
 * Struct represents array.
 */
struct array_t {
    /** Number of elements in array. */
    size_t n;
    /** Pointer to memory with numbers. */
    double * data;
};

bool array_scan ( array_t & arr );

void array_print ( const array_t & arr, ostream & out = cout );

bool array_save ( const array_t & arr, const char * filename );

void array_sort ( array_t & arr );

void array_free ( array_t & arr );

int main ( void ) {
    array_t ar = { 0, nullptr };

    if ( ! array_scan( ar ) ) {
        cout << INVALID_INPUT << endl;
        return 1;
    }

    array_sort( ar );
    if ( ! array_save( ar, OUTPUT_FILENAME ) ) {
        cout << UNSUCCESSFUL_WRITE << endl;
        return 2;
    }
}

/*****************************************************************************/

bool array_scan ( array_t & arr ) {
    if ( !( cin >> arr.n ) || arr.n < N_MIN ) {
        return false;
    }

    arr.data = new double [ arr.n ];
    for ( size_t i = 0; i < arr.n; ++i ) {
        if ( !( cin >> arr.data[ i ] ) ) {
            array_free( arr );
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

bool array_save ( const array_t & arr, const char * filename )
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