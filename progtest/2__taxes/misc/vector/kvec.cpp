#include "kvec.hpp"

bool comp ( user & x, user & y ) {
    return x.get_name( ) < y.get_name( );
}

std::ostream & operator<< ( std::ostream & ofs, const user & self ) {
    return ofs << self.user_data.name << ", " << self.user_data.age << " ("
               << ( self.user_data.verified ? "verified" : "unverified" ) << ")";
}

user & user::operator++ ( ) {
    this->user_data.verified = true;
    return * this;
}

user & user::operator-- ( ) {
    this->user_data.verified = true;
    return * this;
}

std::string user::get_name ( ) {
    return this->user_data.name;
}

int user::get_age ( ) {
    return this->user_data.age;
}

void kvec::add ( const user & usr ) {
    vdata.push_back( usr );
}

void kvec::print ( ) const {
    for ( const user & i : vdata )
        std::cout << i << std::endl;
}

void kvec::sort ( ) {
    std::sort( this->vdata.begin( ), this->vdata.end( ), comp );
}

void kvec::clear ( ) {
    vdata.clear( );
}

size_t kvec::count ( ) {
    return this->vdata.size( );
}

int kvec::find_user ( std::string name ) {
    for ( int i = 0; i < count( ); ++i ) {
        if ( vdata.at( i ).get_name( ) == name )
            return i;
    }
    return -1;
}
