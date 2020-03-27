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

class CWallet {
public:
    string code;
    int32_t income;
    int32_t expense;

    explicit CWallet ( string code = "", int32_t income = 0, int32_t expenses = 0 ) : code( std::move( code ) ),
                                                                                      income( income ),
                                                                                      expense( expenses ) {
    }
};

class CCitizen {
public:
    string name;
    string addr;
    CWallet acc;
    explicit CCitizen ( string name, string addr, CWallet acc ) : name( std::move( name ) ), addr( std::move( addr ) ),
                                                                  acc( std::move( acc ) ) { };
    friend ostream & operator<< ( ostream & ost, const CCitizen & usr ) {
        int universal_width = 20;
        if (
                !( cout << left << setw( 20 )
                        << usr.name << " " << setw( universal_width )
                        << usr.addr << " " << setw( universal_width )
                        << usr.acc.code << setw( universal_width )
                        << usr.acc.income << setw( universal_width )
                        << usr.acc.expense )
                )
            ost.setstate( std::ios_base::failbit );
        return ost;
    }
    static bool cmp_acc ( const CCitizen * x, const CCitizen * y ) {
        return x->acc.code > y->acc.code;
    }
    static bool cmp_addr ( const CCitizen * x, const CCitizen * y ) {
        if ( x->name == y->name )
            return x->addr < y->addr;
        return x->name < y->name;
    }
};

class CIterator {
private:
    vector<CCitizen *>::const_iterator it;
    const vector<CCitizen *> & register_ref;
public:
    explicit CIterator( const vector<CCitizen *> & ref ) : register_ref( ref ) {
        it = register_ref.begin( );
    }

    bool AtEnd ( void ) const {
        return it == register_ref.end( );
    }
    void Next ( void ) {
        if ( ! AtEnd( ) ) it++;
    }
    string Name ( void ) const {
        if ( ! AtEnd( ) )
            return (*it)->name;
        return "";
    }
    string Addr ( void ) const {
        if ( ! AtEnd( ) )
            return (*it)->addr;
        return "";
    }
    string Account ( void ) const {
        if ( ! AtEnd( ) )
            return (*it)->acc.code;
        return "";
    }
};

class CTaxRegister {
private:
    vector<CCitizen *> db_code;
    vector<CCitizen *> db_name;
    CCitizen * tmp_cit = nullptr;
public:
    ~CTaxRegister ( ) {
        for ( const CCitizen * i : db_code )
            delete i;
    }
    void print_citizens ( bool code = false ) const {
        for ( const CCitizen * i : ( code ? db_code : db_name ) )
            cout << * i << endl;
    }
    bool validate_citizen ( CCitizen * tmp ) {
        if ( db_code.empty( ) && db_name.empty( ) ) {
            db_code.push_back( tmp );
            db_name.push_back( tmp );
            return true;
        }

        auto tmp_it_acc = lower_bound( db_code.begin( ), db_code.end( ), tmp, CCitizen::cmp_acc );
        if (
                tmp_it_acc != db_code.end( ) &&
                ( ( * tmp_it_acc )->acc.code == tmp->acc.code ||
                  ( ( * tmp_it_acc )->name == tmp->name && ( * tmp_it_acc )->addr == tmp->addr ) )
                ) {
            delete tmp;
            return false;
        }

        auto tmp_it_name = lower_bound( db_name.begin( ), db_name.end( ), tmp, CCitizen::cmp_addr );
        if (
                tmp_it_name != db_name.end( ) &&
                ( ( * tmp_it_name )->acc.code == tmp->acc.code ||
                  ( ( * tmp_it_name )->name == tmp->name && ( * tmp_it_name )->addr == tmp->addr ) )
                ) {
            delete tmp;
            return false;
        }

        // insert into code-sorted vector
        if ( tmp_it_acc != db_code.end( ) )
            db_code.insert( tmp_it_acc, tmp );
        else
            db_code.push_back( tmp );

        // insert into name-sorted vector
        if ( tmp_it_name != db_name.end( ) )
            db_name.insert( tmp_it_name, tmp );
        else
            db_name.push_back( tmp );

        return true;
    }
    bool manage_vallet ( const bool code_input, const bool expense, const string & name, const string & addr,
                         const string & code, const int & amount ) {
        if ( amount <= 0 || db_code.empty( ) ) {
            return false;
        }

        tmp_cit = new CCitizen( name, addr, CWallet( code ) );
        vector<CCitizen *>::iterator tmp_it;

        if ( code_input ) {
            tmp_it = lower_bound( db_code.begin( ), db_code.end( ), tmp_cit, CCitizen::cmp_acc );
            if ( tmp_it == db_code.end( ) || ( ( * tmp_it )->acc.code != tmp_cit->acc.code ) ) {
                delete tmp_cit;
                return false;
            }
        } else {
            tmp_it = lower_bound( db_name.begin( ), db_name.end( ), tmp_cit, CCitizen::cmp_addr );
            if ( tmp_it == db_name.end( ) || ( ( * tmp_it )->name != tmp_cit->name ) ||
                 ( ( * tmp_it )->addr != tmp_cit->addr ) ) {
                delete tmp_cit;
                return false;
            }
        }

        expense ? ( * tmp_it )->acc.expense += amount : ( * tmp_it )->acc.income += amount;
        delete tmp_cit;

        return true;
    }

    bool Birth ( const string & name, const string & addr, const string & account ) {
        if ( name.empty( ) || addr.empty( ) || account.empty( ) )
            return false;

        return validate_citizen( new CCitizen( name, addr, CWallet( account, 0 ) ) );
    }
    bool Death ( const string & name, const string & addr ) {
        if ( db_code.empty( ) || name.empty( ) || addr.empty( ) )
            return false;

        string code;
        tmp_cit = new CCitizen( name, addr, CWallet( ) );
        auto tmp_it_name = lower_bound( db_name.begin( ), db_name.end( ), tmp_cit, CCitizen::cmp_addr );
        if ( tmp_it_name == db_name.end( ) || ( ( * tmp_it_name )->name != tmp_cit->name ) ||
             ( ( * tmp_it_name )->addr != tmp_cit->addr ) ) {
            delete tmp_cit;
            return false;
        }
        code = ( * tmp_it_name )->acc.code;
        tmp_cit->acc.code = code;
        auto tmp_it_code = lower_bound( db_code.begin( ), db_code.end( ), tmp_cit, CCitizen::cmp_acc );
        delete tmp_cit;
        tmp_cit = * tmp_it_code;

        // this shouldn't happen, ever - if it does, there is a data consistency problem.
        if ( tmp_it_code == db_code.end( ) )
            return false;

        db_name.erase( tmp_it_name );
        db_code.erase( tmp_it_code );
        delete tmp_cit;

        return true;
    }
    bool Income ( const string & account, int amount ) {
        return manage_vallet( true, false, "", "", account, amount );
    }
    bool Income ( const string & name, const string & addr, int amount ) {
        return manage_vallet( false, false, name, addr, "", amount );
    }
    bool Expense ( const string & account, int amount ) {
        return manage_vallet( true, true, "", "", account, amount );
    }
    bool Expense ( const string & name, const string & addr, int amount ) {
        return manage_vallet( false, true, name, addr, "", amount );
    }
    bool Audit ( const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense ) const {
        if ( name.empty( ) || addr.empty( ) || db_code.empty( ) )
            return false;

        CCitizen * tmp = new CCitizen( name, addr, CWallet( "" ) );
        auto tmp_it_name = lower_bound( db_name.begin( ), db_name.end( ), tmp, CCitizen::cmp_addr );
        if ( tmp_it_name == db_name.end( ) || ( ( * tmp_it_name )->name != tmp->name ) || ( ( * tmp_it_name )->addr != tmp->addr ) ) {
            delete tmp;
            return false;
        }
        delete tmp;
        account = (*tmp_it_name)->acc.code;
        sumIncome = (*tmp_it_name)->acc.income;
        sumExpense = (*tmp_it_name)->acc.expense;
        return true;
    }
    CIterator ListByName ( void ) const {
        return CIterator ( db_name );
    }
};

#ifndef __PROGTEST__
int main ( void ) {
    return 0;
}
#endif /* __PROGTEST__ */
