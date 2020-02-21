#include <iostream>
#include <algorithm>

/** Defines minimum size of array. */
const int N_MIN = 5;

using namespace std;

/**
 * Struct represents array.
 */
struct array_t {
    /** Number of elements in array. */
    size_t n;
    /** Pointer to memory with numbers. */
    double * data;
};

/**
 * Scans numbers from standard input into array.
 * @param[out] arr Pointer to array, which is used for reading numbers from input.
 * @returns 0 for error during reading standard input, 1 if everything is OK
 */
bool array_scan ( array_t & arr );

/**
 * Prints numbers from array to standard output.
 * @param[in] arr Pointer to array with numbers.
 */
void array_print ( const array_t & arr );

/**
 * Sorts numbers in the passed array.
 * @param[in] arr Pointer to array (to be sorted).
 */
void array_sort ( array_t & arr );

/**
 * Free array allocated memory.
 * @param[in] arr Pointer to array (to be freed).
 */
void array_free ( array_t & arr );

int main ( void )
{
    array_t ar = { 0, nullptr };

    if ( ! array_scan( ar ) ) {
        cout << "Nespravny vstup." << endl;
        return 1;
    }

    cout << endl;
    array_sort( ar );
    array_print( ar );
    cout << endl;
    array_free( ar );

	return 0;
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

void array_print ( const array_t & arr )
{
    for ( size_t i = 0; i < arr.n; ++i ) {
        if ( i != 0 )
            cout << " ";
        cout << arr.data[ i ];
    }
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