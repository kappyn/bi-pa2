#include "operators.hpp"

std::ostream & operator<< ( std::ostream & ofs, const frac & self ) {
    if ( !( ofs << "( " << self.nom << " / " << self.den << " )" ) )
        ofs.setstate( std::ios_base::failbit );
    return ofs;
}

int frac::get_nominator ( ) const {
    return nom;
}

int frac::get_denominator ( ) const {
    return den;
}


int frac::gcd ( int a, int b ) {
    if ( a == 0 ) return b;
    if ( b == 0 ) return a;
    if ( a == b ) return a;
    if ( a > b )
        return gcd( a - b, b );
    return gcd( a, b - a );
}

void frac::reduce ( ) {
    int gcd = this->gcd( abs( nom ), abs( den ) );
    nom = nom / gcd;
    den = den / gcd;
}

frac frac::operator+ ( const frac & x ) const {
    return { this->nom * x.den + this->den * x.nom, this->den * x.den };
}

frac frac::operator- ( const frac & x ) const {
    return { this->nom * x.den - this->den * x.nom, this->den * x.den };
}

frac & frac::operator+= ( const frac & x ) {
    return ( * this = * this + x );
}

frac & frac::operator-= ( const frac & x ) {
    return ( * this = * this - x );
}

frac frac::operator- ( ) {
    return { -get_nominator( ), get_denominator( ) };
}

frac & frac::operator++ ( ) {
    * this += 1;
    return * this;
}

frac frac::operator++ ( int ) {
    frac tmp = * this;
    ++* this;
    return tmp;
}

frac & frac::operator-- ( ) {
    * this -= 1;
    return * this;
}

frac frac::operator-- ( int ) {
    frac tmp = * this;
    --* this;
    return tmp;
}


bool frac::operator< ( const frac x ) const {
    int a = get_nominator( );
    int b = get_denominator( );
    int c = x.get_nominator( );
    int d = x.get_denominator( );

    int y = a * d - b * c;

    return ( y < 0 );
}

bool frac::operator> ( const frac x ) const {
    return !( * this < x );
}

frac & frac::operator~ ( ) {
    reduce( );
    return * this;
}

bool frac::operator== ( const frac x ) const {
    return ( get_nominator( ) == x.get_nominator( ) && get_denominator( ) == x.get_denominator( ) );
}