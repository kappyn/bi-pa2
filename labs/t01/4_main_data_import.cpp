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

bool array_scan ( array_t & arr, istream & str = cin );
bool array_load ( array_t & arr, const char * filename );
void array_print ( const array_t & arr );
void array_sort ( array_t & arr );
void array_free ( array_t & arr );

int main ( ) {
    array_t arr { 0, nullptr };

    if ( ! array_load( arr, INPUT_FILENAME ) ) {
        cout << INVALID_INPUT << endl;
        return 1;
    }

    array_print( arr ); cout << endl;

    array_sort( arr );

    array_print( arr ); cout << endl;

    array_free( arr );

    return 0;
}

/****/
bool array_scan ( array_t & ar, istream & str ) {
    if ( !( str >> ar.n ) || ar.n < N_MIN ) {
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

bool array_load ( array_t & ar, const char * filename ) {
    ifstream fin { filename };
    if ( ! fin ) // Does the file even exist?
        return false;
    return array_scan( ar, fin );
}

void array_print ( const array_t & ar ) {
    for ( size_t i = 0; i < ar.n; ++i ) {
        if ( i != 0 )
            cout << " ";
        cout << ar.data[ i ];
    }
}

void array_sort ( array_t & ar ) {
    sort ( ar.data, ar.data + ar.n );
}

void array_free ( array_t & ar ) {
    delete [ ] ar.data;
    ar.n = 0;
    ar.data = nullptr;
}