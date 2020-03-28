#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>

using namespace std;

struct Transaction {
    string id;
    string debitAcc;
    string creditAcc;
};

class Customer {
private:
    string name;
    string code;
    vector<Transaction> transactions;

public:
    Customer ( string name, string code ) : name( move( name ) ), code( move( code ) ) { }

    friend ostream & operator<< ( ostream & ost, const Customer & usr ) {
        int universal_width = 20;
        if (
                !( cout << left << setw( 20 )
                        << usr.name << " " << setw( universal_width )
                        << usr.code << " " << setw( universal_width )
                        << & usr.code << " " << setw( universal_width ) << endl
                )
                )
            ost.setstate( ios_base::failbit );
        return ost;
    }
};

class Bank {
private:
    vector<Customer> customers;

public:
    bool addCustomer ( const Customer & c ) {
        customers.push_back( c );
        return true;
    }

    void printCustomers ( ) {
        for ( const auto & i : customers ) {
            cout << i;
        }
    }
};

int main ( ) {
    Customer c1( "Kevin Kroupa", "123456" );
    Customer c2( "Rangar Lothbrok", "111111" );
    Customer c3( "Dolores", "x" );
    Customer c4( "Franta Kodidek", "x20" );

    Bank b1;

    b1.addCustomer( c1 );
    b1.addCustomer( c2 );
    b1.addCustomer( c3 );
    b1.addCustomer( c4 );

    b1.printCustomers( );
}