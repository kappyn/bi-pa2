#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Person {
    string name;
    int age;

public:
    explicit Person ( string name, int age ) : name (name), age( age ) { }
    void print ( ) {
        cout << name << ", " << age << endl;
    }

    static bool cmp_str( Person * x, Person * y ) {
        return ((x->name) < (y->name));
    }

    static bool cmp_int( Person * x, Person * y ) {
        return ((x->age) < (y->age));
    }
};

class Register {
private:
    vector<Person *> db_names;
    vector<Person *> db_addr;

public:
    bool add_person( string name, int age ) {
        if ( name.empty( ) || age == 0 )
            return false;

        Person * p = new Person( name, age );

        auto it = std::lower_bound( db_names.begin( ), db_names.end( ), p, Person::cmp_str );
        if ( it == db_names.end( ) ) {
            db_names.push_back( p );
        } else {
            db_names.insert( it, p );
        }

        auto bt = lower_bound( db_addr.begin( ), db_addr.end( ), p, Person::cmp_int );
        if ( bt == db_addr.end( ) ) {
            db_addr.push_back( p );
        } else {
            db_addr.insert( bt, p );
        }

        return true;
    }

    void sorted_addr ( ) {
        for ( const auto i : db_names )
            i->print();
    }

    void sorted_age ( ) {
        for ( const auto i : db_addr )
            i->print();
    }
};

int main ( void ) {
    Register reg;

    reg.add_person( "Kevin", 10 );
    reg.add_person( "Pepa", 24 );
    reg.add_person( "Zandal", 1 );
    reg.add_person( "Smaženku", 2 );

    reg.sorted_addr( );

    cout << endl << endl;

    reg.sorted_age( );
}