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

class CWallet {
public:
    string code;
    int32_t bilance;
    int32_t expenses;

    explicit CWallet ( string code = "", int32_t bilance = 0, int32_t expenses = 0 ) : code( std::move( code ) ),
                                                                                       bilance( bilance ),
                                                                                       expenses( expenses ) {
    }
};

class CCitizen {
public:
    string name;
    string addr;
    CWallet account;

    explicit CCitizen ( string name, string addr, CWallet account )
            : name( std::move( name ) ), addr( std::move( addr ) ), account( std::move( account ) ) { };
    friend ostream & operator<< ( ostream & ost, const CCitizen & usr ) {
        int universal_width = 20;
        if (
                !( cout << left << setw( 20 )
                        << usr.name << " " << setw( universal_width )
                        << usr.addr << " " << setw( universal_width )
                        << usr.account.code << setw( universal_width )
                        << usr.account.bilance << setw( universal_width )
                        << usr.account.expenses )
                )
            ost.setstate( std::ios_base::failbit );
        return ost;
    }
    bool operator== ( const CCitizen & x ) const {
        return (
                account.code == x.account.code &&
                name == x.name &&
                addr == x.addr
        );
    }
    bool operator!= ( const CCitizen & x ) const {
        return !( * this == x );
    }
    static bool cmp_acc ( const CCitizen & x, const CCitizen & y ) {
        return x.account.code > y.account.code;
    }
    static bool cmp_addr ( const CCitizen & x, const CCitizen & y ) {
        if ( x.name == y.name )
            return x.addr < y.addr;
        return x.name < y.name;
    }
};

class CTaxRegister {
private:
    std::vector<CCitizen> db;
    vector<CCitizen>::iterator tmp_it;
    bool sorted_by_name = false;
public:
    // helper definitions
    void print_citizens ( ) const {
        for ( const CCitizen & i : db )
            cout << i << endl;
    }

    bool manage_wallet ( const CCitizen & tmp, bool by_name, int amount, bool expense = false ) {
        if ( amount <= 0 )
            return false;

        // re-sort only if current sorted representation is by different order
//        if ( ( !sorted_by_name && by_name ) || ( sorted_by_name && !by_name ) ) {
//            sorted_by_name = by_name;
//        }

        std::sort( db.begin( ), db.end( ), ( by_name ? CCitizen::cmp_addr : CCitizen::cmp_acc ) );

        // find element with binary search
        auto i = lower_bound( db.begin( ), db.end( ), tmp, ( by_name ? CCitizen::cmp_addr : CCitizen::cmp_acc ) );

        if ( db.empty( ) )
            return false;

        if ( i == db.end( ) )
            return false;

        // verify data of the wallet holder
        if ( by_name ) {
            if ( i->name != tmp.name || i->addr != tmp.addr )
                return false;
        } else {
            if ( i->account.code != tmp.account.code )
                return false;
        }

        expense ? db[ i - db.begin( ) ].account.expenses += amount : db[ i - db.begin( ) ].account.bilance += amount;
        return true;
    }

    void sort_by_account ( ) {
        std::sort( db.begin( ), db.end( ), CCitizen::cmp_acc );
        sorted_by_name = false;
    };

    void sort_by_name ( ) {
        std::sort( db.begin( ), db.end( ), CCitizen::cmp_addr );
        sorted_by_name = true;
    };

    bool validate_citizen ( const CCitizen & tmp ) {
        // empty database - nothing search for
        if ( db.empty( ) ) {
            db.push_back( tmp );
            return true;
        }

        // sort by accounts, search for duplicates
        sort_by_account( );
        tmp_it = lower_bound( db.begin( ), db.end( ), tmp, CCitizen::cmp_acc );
        if ( tmp_it != db.end( ) &&
             ( ( tmp_it->account.code == tmp.account.code ) ||
               ( tmp_it->name == tmp.name && tmp_it->addr == tmp.addr ) ) )
            return false;

        // sort by names/addresses, search for duplicates
        sort_by_name( );
        tmp_it = lower_bound( db.begin( ), db.end( ), tmp, CCitizen::cmp_addr );
        if ( tmp_it != db.end( ) &&
             ( ( tmp_it->account.code == tmp.account.code ) ||
               ( tmp_it->name == tmp.name && tmp_it->addr == tmp.addr ) ) )
            return false;

        if ( tmp_it == db.end( ) )
            db.push_back( tmp );
        else
            db.insert( tmp_it, tmp );

        return true;
    }

    bool get_sorted_state ( ) const {
        return sorted_by_name;
    }

    // required part
    bool Birth ( const string & name, const string & addr, const string & account ) {
        CCitizen tmp( name, addr, CWallet( account, 0 ) );

        if ( name.empty( ) || addr.empty( ) || account.empty( ) )
            return false;

        return validate_citizen( tmp );
    }

    bool Death ( const string & name, const string & addr ) {
        return false;
    }

    bool Income ( const string & account, int amount ) {
        return manage_wallet(
                CCitizen( "", "", CWallet( account ) ),
                false,
                amount,
                false
        );
    }

    bool Income ( const string & name, const string & addr, int amount ) {
        return manage_wallet(
                CCitizen( name, addr, CWallet( ) ),
                true,
                amount,
                false
        );
    }

    bool Expense ( const string & account, int amount ) {
        return manage_wallet(
                CCitizen( "", "", CWallet( account ) ),
                false,
                amount,
                true
        );
    }

    bool Expense ( const string & name, const string & addr, int amount ) {
        return manage_wallet(
                CCitizen( name, addr, CWallet( ) ),
                true,
                amount,
                true
        );
    }

    bool Audit ( const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense ) const {
//        // find element with binary search
//
//        auto i = lower_bound( db.begin( ), db.end( ), CCitizen( name, addr, CWallet( ) ), CCitizen::cmp_addr );
//
//        if ( db.empty( ) )
//            return false;
//
//        if ( i == db.end( ) )
//            return false;
//
//        // verify data of the wallet holder
//        if ( by_name ) {
//            if ( i->name != tmp.name || i->addr != tmp.addr )
//                return false;
//        } else {
//            if ( i->account.code != tmp.account.code )
//                return false;
//        }
//
//        expense ? db[ i - db.begin( ) ].account.expenses += amount : db[ i - db.begin( ) ].account.bilance += amount;
//        return true;
    }

//    CIterator ListByName ( void ) const;
};
#ifndef __PROGTEST__

int main ( void ) {
    string acct;
    int sumIncome, sumExpense;

    CTaxRegister b1;

    assert ( b1.Birth( "AAAA", "Main Street 17", "634oddzzz" ) );
    assert ( b1.Birth( "John Smith", "Main Street 17", "Z343rwZ" ) );
    assert ( b1.Birth( "John Smith", "Oak Road 23", "123/456/789" ) );


    assert ( b1.Birth( "John Smitho", "Main Street 17", "Z343rwZy" ) );
    assert ( b1.Birth( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( b1.Birth( "Riemann Zeta", "Main Street 17", "634oddTx" ) );
    assert ( b1.Birth( "Peter Hacker", "Main Street 17", "634oddT" ) );
    assert ( b1.Birth( "John Smith", "Main Streety 17", "Z343rwZwww" ) );
    assert ( b1.Birth( "John Smith", "Oak Road", "123/456/789y" ) );

    // unique bank account tests
    assert ( !b1.Birth( "John Smith", "Main Street 17", "Z343rwZ" ) );
    assert ( !b1.Birth( "John Smithos", "Main Street 17", "Z343rwZy" ) );
    assert ( !b1.Birth( "John Smithos", "Main Street 17", "Xuj5#94" ) );
    assert ( !b1.Birth( "John Smith", "Main Street 17", "Z343rwZy" ) );
    assert ( !b1.Birth( "John Smith", "Main Street 17", "Z343rwZ" ) );

    assert ( b1.Birth( "Arnost Hlavacek", "Main Street 17", "XFX" ) );
    assert ( b1.Birth( "Arnost Hlavka", "Main Street 17", "Z343rwZye2" ) );
    assert ( b1.Birth( "Arnost Blavka", "Main Street 17", "Z343rwZye2q" ) );
    assert ( b1.Birth( "Arnost Blavka", "Main Street 18", "ahoj" ) );
    assert ( b1.Birth( "Zandej Zle", "Main Street 17", "Z343rwZye2qy" ) );

//    cout << (b1.get_sorted_state( ) ? "Sorted by name" : "Sorted by code") << endl;

    b1.print_citizens( );

    //    assert ( b1.Income( "Xuj5#94", 1000 ) );
//    assert ( b1.Income( "634oddT", 2000 ) );
//    assert ( b1.Income( "123/456/789", 3000 ) );
//    assert ( b1.Income( "634oddT", 4000 ) );
//    assert ( !b1.Income( "634oddT___", -300 ) );
//    assert ( !b1.Income( "634oddT___", 4000 ) );
//
//    assert ( b1.Income( "Peter Hacker", "Main Street 17", 20000 ) );
//    assert ( b1.Expense( "Peter Hacker", "Main Street 17", 2000 ) );
//    assert ( b1.Expense( "John Smith", "Main Street 17", 500 ) );
//    assert ( b1.Expense( "Jane Hacker", "Main Street 17", 1000 ) );
//    assert ( b1.Expense( "Xuj5#94", 1300 ) );
//    assert ( ! b1.Expense( "__Xuj5#94", 1300 ) );

//    cout << endl;
//    b1.print_citizens( );



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
