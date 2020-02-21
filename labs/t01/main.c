#include <stdio.h>
#include <stdlib.h>

/** Defines minimum size of array. */
#define N_MIN 5

/**
 * Struct represents array.
 */
typedef struct array {
	/** Number of elements in array. */
	size_t n;
	/** Pointer to memory with numbers. */
	double * data;
} array_t;

/**
 * Scans numbers from standard input into array.
 * @param[out] arr Pointer to array, which is used for reading numbers from input.
 * @returns 0 for error during reading standard input, 1 if everything is OK
 */
int array_scan ( array_t * arr );

/**
 * Prints numbers from array to standard output.
 * @param[in] arr Pointer to array with numbers.
 */
void array_print ( const array_t * arr );

/**
 * Sorts numbers in the passed array.
 * @param[in] arr Pointer to array (to be sorted).
 */
void array_sort ( array_t * arr );

/**
 * Free array allocated memory.
 * @param[in] arr Pointer to array (to be freed). 
 */
void array_free ( array_t * arr );

int main ( void )
{
    array_t ar;

    if ( ! array_scan( & ar ) ) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    printf("\n");
    array_sort( &ar );
    array_print( &ar );
    printf("\n");
    array_free( &ar );

	return 0;
}

/*****************************************************************************/

int array_scan ( array_t * arr ) {
    if (scanf("%zu", &(arr->n)) != 1 || arr->n < N_MIN) {
        return 0;
    }

    arr->data = (double *) malloc(arr->n * sizeof(double));
    for (size_t i = 0; i < arr->n; ++i) {
        if (scanf("%lf", arr->data + i) != 1) {
            array_free(arr);
            return 0;
        }
    }

    return 1;

}

void array_print ( const array_t * arr )
{
	for ( size_t i = 0; i < arr->n; ++i ) {
		if ( i != 0 )
			printf( " " );
		printf( "%g", arr->data[ i ] );
	}
}

int double_compare ( const double * a, const double * b )
{
	return ( *a > *b ) - ( *a < *b );
}

void array_sort ( array_t * arr )
{
  qsort( arr->data, arr->n, sizeof( double ),
		(int (*) ( const void *, const void * )) double_compare
  );
}

void array_free ( array_t * arr )
{
	free( arr->data );
	arr->n = 0;
	arr->data = NULL;
}
