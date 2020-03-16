#ifndef __PROGTEST__

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
#endif /* __PROGTEST__ */

class CIterator {
public:
    bool AtEnd ( void ) const;
    void Next ( void );
    string Name ( void ) const;
    string Addr ( void ) const;
    string Account ( void ) const;
private:
    // todo
};

class CTaxRegister {
public:
    bool Birth ( const string & name,
                 const string & addr,
                 const string & account );
    bool Death ( const string & name,
                 const string & addr );
    bool Income ( const string & account,
                  int amount );
    bool Income ( const string & name,
                  const string & addr,
                  int amount );
    bool Expense ( const string & account,
                   int amount );
    bool Expense ( const string & name,
                   const string & addr,
                   int amount );
    bool Audit ( const string & name,
                 const string & addr,
                 string & account,
                 int & sumIncome,
                 int & sumExpense ) const;
    CIterator ListByName ( void ) const;
private:
    // todo
};

#ifndef __PROGTEST__
int main ( void ) {
    string acct;
    int sumIncome, sumExpense;
    CTaxRegister b1;
    assert ( b1.Birth( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b1.Birth( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( b1.Birth( "Peter Hacker", "Main Street 17", "634oddT" ) );
    assert ( b1.Birth( "John Smith", "Main Street 17", "Z343rwZ" ) );
    assert ( b1.Income( "Xuj5#94", 1000 ) );
    assert ( b1.Income( "634oddT", 2000 ) );
    assert ( b1.Income( "123/456/789", 3000 ) );
    assert ( b1.Income( "634oddT", 4000 ) );
    assert ( b1.Income( "Peter Hacker", "Main Street 17", 2000 ) );
    assert ( b1.Expense( "Jane Hacker", "Main Street 17", 2000 ) );
    assert ( b1.Expense( "John Smith", "Main Street 17", 500 ) );
    assert ( b1.Expense( "Jane Hacker", "Main Street 17", 1000 ) );
    assert ( b1.Expense( "Xuj5#94", 1300 ) );
    assert ( b1.Audit( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "123/456/789" );
    assert ( sumIncome == 3000 );
    assert ( sumExpense == 0 );
    assert ( b1.Audit( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 1000 );
    assert ( sumExpense == 4300 );
    assert ( b1.Audit( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "634oddT" );
    assert ( sumIncome == 8000 );
    assert ( sumExpense == 0 );
    assert ( b1.Audit( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Z343rwZ" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 500 );
    CIterator it = b1.ListByName( );
    assert ( !it.AtEnd( )
             && it.Name( ) == "Jane Hacker"
             && it.Addr( ) == "Main Street 17"
             && it.Account( ) == "Xuj5#94" );
    it.Next( );
    assert ( !it.AtEnd( )
             && it.Name( ) == "John Smith"
             && it.Addr( ) == "Main Street 17"
             && it.Account( ) == "Z343rwZ" );
    it.Next( );
    assert ( !it.AtEnd( )
             && it.Name( ) == "John Smith"
             && it.Addr( ) == "Oak Road 23"
             && it.Account( ) == "123/456/789" );
    it.Next( );
    assert ( !it.AtEnd( )
             && it.Name( ) == "Peter Hacker"
             && it.Addr( ) == "Main Street 17"
             && it.Account( ) == "634oddT" );
    it.Next( );
    assert ( it.AtEnd( ) );

    assert ( b1.Death( "John Smith", "Main Street 17" ) );

    CTaxRegister b2;
    assert ( b2.Birth( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b2.Birth( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( !b2.Income( "634oddT", 4000 ) );
    assert ( !b2.Expense( "John Smith", "Main Street 18", 500 ) );
    assert ( !b2.Audit( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
    assert ( !b2.Death( "Peter Nowak", "5-th Avenue" ) );
    assert ( !b2.Birth( "Jane Hacker", "Main Street 17", "4et689A" ) );
    assert ( !b2.Birth( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b2.Death( "Jane Hacker", "Main Street 17" ) );
    assert ( b2.Birth( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b2.Audit( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 0 );
    assert ( !b2.Birth( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );

    return 0;
}
#endif /* __PROGTEST__ */
