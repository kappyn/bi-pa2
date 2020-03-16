//#include "kvec.hpp"
//#include <cassert>

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

bool comp ( const int x, const int y ) {
    return x > y;
}




int main ( ) {
//    kvec kv;
//    kv.add( data{ "Kevin", "Kroupa", "Prague", 19, false } );
//    kv.add( data{ "Norton", "Kel", "Stockholm", 40, false } );
//    kv.add( data{ "Xaver", "Rope", "Paris", 30, false } );
//    kv.add( data{ "Ladislav", "Vagner", "Thakurova", 55, false } );
//    kv.add( data{ "Ladislav", "Vagneros", "Thakurova", 55, false } );
//    kv.add( data{ "Viktor", "Kvetina", "Thakurova", 20, false } );
//    kv.add( data{ "Samuel", "Lenovo", "Thakurova", 13, false } );
//    kv.add( data{ "Alexander", "Cibule", "Thakurova", 12, false } );
//    kv.sort( );
//    kv.print( );
//    assert( kv.find_user ( "Alexander" ) == 0 );
//    assert( kv.find_user ( "Rozalie" ) == -1 );
//    assert( kv.find_user ( "Ladislav" ) == 2 );

    vector < int > vk{ 1, 23, 4, 30, 40, 120 };
    for ( int i : vk )
        cout << i << " ";

    sort( vk.begin( ), vk.end( ) );

    cout << endl;

    for ( int i : vk )
        cout << i << " ";

    cout << "(" << vk.size( ) << ")" << endl << endl;
    cout << lower_bound( vk.begin( ), vk.end( ), 12000 ) - vk.begin( ) << endl;
    cout << lower_bound( vk.begin( ), vk.end( ), 120 ) - vk.begin( ) << endl;
    cout << lower_bound( vk.begin( ), vk.end( ), 23 ) - vk.begin( ) << endl;

    return 0;
}