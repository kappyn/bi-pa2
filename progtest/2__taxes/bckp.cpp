#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>

using namespace std;
#endif /* __PROGTEST__ */

class CIterator {
private:
    // todo
public:
    bool AtEnd ( void ) const;
    void Next ( void );
    string Name ( void ) const;
    string Addr ( void ) const;
    string Account ( void ) const;
};

class CCitizen {
public:
    string name;
    string addr;
    string account;

    explicit CCitizen ( string name,
                        string addr,
                        string account )
            : name( std::move( name ) ), addr( std::move( addr ) ), account( std::move( account ) ) { };

    friend ostream & operator<< ( ostream & ost, const CCitizen & usr );

    static bool comp_func ( const CCitizen & x, const CCitizen & y ) {
        return x.account > y.account;
    }

    static bool str_comp_func ( const string & x, const string & y ) {
        return x < y;
    }
};

ostream & operator<< ( ostream & ost, const CCitizen & usr ) {
    if ( !( cout << usr.name << " " << usr.addr << " " << usr.account ) )
        ost.setstate( std::ios_base::failbit );
    return ost;
}

class CTaxRegister {
private:
    std::vector<CCitizen> db;

public:
    // helper definitions
    void print_citizens ( ) const {
        for ( const CCitizen & i : db )
            cout << i << endl;
    }

    // required part
    bool Birth ( const string & name, const string & addr, const string & account ) {
        vector<std::string> test = { "Kevin", "Pavel", "Jana", "Honza", "Jan", "Petr", "Barbora" };
        std::sort ( test.begin( ), test.end( ), CCitizen::str_comp_func );

        string lala;

        lala = "Pavel";
        auto it = lower_bound( test.begin(), test.end( ), lala, CCitizen::str_comp_func );
        if ( it == test.end() || *it != lala )
            cout << "Isn't there.";

//        cout << ( std::binary_search( test.begin( ), test.end( ), lala, CCitizen::str_comp_func ) ? "Found" : "Not found" );

//        lala = "Kokotek";
//        test.insert( lower_bound ( test.begin ( ), test.end( ), lala, CCitizen::str_comp_func ), lala );

//        lala = "Josef";
//        test.insert( lower_bound ( test.begin ( ), test.end( ), lala, CCitizen::str_comp_func ), lala );
//
//        lala = "Albert";
//        test.insert( lower_bound ( test.begin ( ), test.end( ), lala, CCitizen::str_comp_func ), lala );
//
//        lala = "Zeta";
//        test.insert( lower_bound ( test.begin ( ), test.end( ), lala, CCitizen::str_comp_func ), lala );

        for ( const string & i : test ) cout << i << endl;

//        auto x = lower_bound( db.begin( ), db.end( ), CCitizen( name, addr, account ), CCitizen::str_comp_func );
//        vector<CCitizen>::iterator lb;
//        CCitizen tmp = CCitizen ( name, addr, account );
//        lb = lower_bound ( db.begin( ), db.end( ), tmp, CCitizen::comp_func );
//        cout << ( lb - db.begin ( ) );
//        return true;
//        for ( CCitizen & i : db )
//            if ( i.account == account || ( i.name == name && i.addr == addr ) )
//                return false;
//        db.push_back( CCitizen( name, addr, account ) );

        return true;
    }
    bool Death ( const string & name, const string & addr );
    bool Income ( const string & account, int amount );
    bool Income ( const string & name, const string & addr, int amount );
    bool Expense ( const string & account, int amount );
    bool Expense ( const string & name, const string & addr, int amount );
    bool Audit ( const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense ) const;
    CIterator ListByName ( void ) const;
};
#ifndef __PROGTEST__











//    vector<string> strings = { };
//    sort( strings.begin( ), strings.end( ) );
//    for ( const string & i : strings )
//        cout << i << "\n";
//    cout << endl;
//    string constant = "Zazaza";
//    auto it = lower_bound( strings.begin( ), strings.end( ), constant );
//    if ( it == strings.end( ) || *it != constant )
//        cout << constant << " not found => inserting.\n\n";
//    strings.insert( it, constant );
//    for ( const string & i : strings )
//        cout << i << "\n";



int main ( void ) {
    string acct;
    int sumIncome, sumExpense;

    CTaxRegister b1;
    assert ( b1.Birth( "John Smith", "Oak Road 23", "123/456/789" ) );
//    assert ( b1.Birth( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
//    assert ( b1.Birth( "Peter Hacker", "Main Street 17", "634oddT" ) );
//    assert ( b1.Birth( "John Smith", "Main Street 17", "Z343rwZ" ) );

//    assert ( b1.Income( "Xuj5#94", 1000 ) );
//    assert ( b1.Income( "634oddT", 2000 ) );
//    assert ( b1.Income( "123/456/789", 3000 ) );
//    assert ( b1.Income( "634oddT", 4000 ) );
//    assert ( b1.Income( "Peter Hacker", "Main Street 17", 2000 ) );
//    assert ( b1.Expense( "Jane Hacker", "Main Street 17", 2000 ) );
//    assert ( b1.Expense( "John Smith", "Main Street 17", 500 ) );
//    assert ( b1.Expense( "Jane Hacker", "Main Street 17", 1000 ) );
//    assert ( b1.Expense( "Xuj5#94", 1300 ) );
//    assert ( b1.Audit( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
//    assert ( acct == "123/456/789" );
//    assert ( sumIncome == 3000 );
//    assert ( sumExpense == 0 );
//    assert ( b1.Audit( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
//    assert ( acct == "Xuj5#94" );
//    assert ( sumIncome == 1000 );
//    assert ( sumExpense == 4300 );
//    assert ( b1.Audit( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
//    assert ( acct == "634oddT" );
//    assert ( sumIncome == 8000 );
//    assert ( sumExpense == 0 );
//    assert ( b1.Audit( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
//    assert ( acct == "Z343rwZ" );
//    assert ( sumIncome == 0 );
//    assert ( sumExpense == 500 );
//    CIterator it = b1.ListByName( );
//    assert ( !it.AtEnd( )
//             && it.Name( ) == "Jane Hacker"
//             && it.Addr( ) == "Main Street 17"
//             && it.Account( ) == "Xuj5#94" );
//    it.Next( );
//    assert ( !it.AtEnd( )
//             && it.Name( ) == "John Smith"
//             && it.Addr( ) == "Main Street 17"
//             && it.Account( ) == "Z343rwZ" );
//    it.Next( );
//    assert ( !it.AtEnd( )
//             && it.Name( ) == "John Smith"
//             && it.Addr( ) == "Oak Road 23"
//             && it.Account( ) == "123/456/789" );
//    it.Next( );
//    assert ( !it.AtEnd( )
//             && it.Name( ) == "Peter Hacker"
//             && it.Addr( ) == "Main Street 17"
//             && it.Account( ) == "634oddT" );
//    it.Next( );
//    assert ( it.AtEnd( ) );
//
//    assert ( b1.Death( "John Smith", "Main Street 17" ) );
//
//    CTaxRegister b2;
//    assert ( b2.Birth( "John Smith", "Oak Road 23", "123/456/789" ) );
//    assert ( b2.Birth( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
//    assert ( !b2.Income( "634oddT", 4000 ) );
//    assert ( !b2.Expense( "John Smith", "Main Street 18", 500 ) );
//    assert ( !b2.Audit( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
//    assert ( !b2.Death( "Peter Nowak", "5-th Avenue" ) );
//    assert ( !b2.Birth( "Jane Hacker", "Main Street 17", "4et689A" ) );
//    assert ( !b2.Birth( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
//    assert ( b2.Death( "Jane Hacker", "Main Street 17" ) );
//    assert ( b2.Birth( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
//    assert ( b2.Audit( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
//    assert ( acct == "Xuj5#94" );
//    assert ( sumIncome == 0 );
//    assert ( sumExpense == 0 );
//    assert ( !b2.Birth( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );

    return 0;
}

#endif /* __PROGTEST__ */
