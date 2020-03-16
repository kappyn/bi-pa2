#include <iostream>
#include <fstream>

class frac {
    int nom, den;
public:
    frac ( int n = 0, int d = 1 ) : nom( n ), den( d ) {
    }

    int get_nominator ( ) const;

    int get_denominator ( ) const;

    static int gcd ( int a, int b );

    void reduce ( );

    frac operator+ ( const frac & x ) const;

    frac operator- ( const frac & x ) const;

    frac & operator+= ( const frac & x );

    frac & operator-= ( const frac & x );

    frac & operator++ ( );

    frac operator++ ( int );

    frac & operator-- ( );

    frac operator-- ( int );

    frac operator- ( );

    bool operator< ( frac x ) const;

    bool operator> ( frac x ) const;

    frac & operator~ ( );

    bool operator== ( frac x ) const;

    friend std::ostream & operator<< ( std::ostream & ofs, const frac & self );
};