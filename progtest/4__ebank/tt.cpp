#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
#endif /* __PROGTEST__ */

struct CPair {
    int x, y;
    void set ( int a, int b ) {
        x = a;
        y = b;
    }
    void operator ~ ( ) {
        cout << "( d_i: " << x << " " << "; c_i: " << y << " )\n";
    }
};

class CTransaction {
public:
    char * debit;
    char * credit;
    char * sign;
    unsigned amount;

    CTransaction ( ) {
        credit = debit = sign = nullptr;
        amount = 0;
    }
    CTransaction ( char * debit, char * credit, const char * sign, unsigned amount ) : debit( debit ), credit( credit ),
                                                                                       amount( amount ) {
        this->amount = amount;
        int strl = strlen( sign );
        this->sign = new char[strl + 2];
        for ( int i = 0; i < strl; ++i ) {
            this->sign[ i ] = sign[ i ];
        }
        this->sign[ strl ] = '\0';
    }
    ~CTransaction ( ) {
        delete[] sign;
    }

    void operator ~ ( ) {
        std::cout << debit << setw( 20 ) << credit << setw( 20 ) << sign << setw( 20 ) << amount << endl;
    }
    friend ostream & operator << ( ostream & ost, const CTransaction & transaction ) {
        if (
                !( cout << transaction.debit << setw( 20 ) << transaction.credit << setw( 20 ) << transaction.sign
                        << setw( 20 ) << transaction.amount << endl
                )
                )
            ost.setstate( ios_base::failbit );
        return ost;
    }

};
typedef CTransaction * CTransactionRef;

class CAccount {
public:
    char * code;
    int64_t balance;

    CTransactionRef * transactions;
    int * refs;
    int ts_size = 4;
    int ts_cnt = 0;

    CAccount ( const char * id, int bal ) {
        int strl = strlen( id );
        code = new char[strl + 2];
        for ( int i = 0; i < strl; ++i )
            code[ i ] = id[ i ];
        code[ strl ] = '\0';

        transactions = new CTransactionRef[ts_size];
        refs = new int[ts_size];
        balance = bal;
    }
    ~CAccount ( ) {
        delete[] code;
        delete[] transactions;
        delete[] refs;
    }

    CAccount & operator = ( const CAccount & x ) {
        if ( this == & x )
            return * this;

        delete[] code;
        code = x.code;
        balance = x.balance;
        return * this;
    }
    friend ostream & operator << ( ostream & ost, const CAccount & acc ) {
        if ( !( ost << acc.code << ":\n" << "   " << acc.balance << endl ) )
            ost.setstate( ios_base::failbit );

        int64_t result = acc.balance;
        for ( int i = 0; i < acc.ts_cnt; ++i ) {
            CTransaction * curr_t = acc.transactions[ i ];
            if ( curr_t->debit == acc.code ) {
                if ( !( ost << " - " << curr_t->amount << ", to: " << curr_t->credit << ", sign: " << curr_t->sign
                            << endl ) )
                    ost.setstate( ios_base::failbit );
                result -= ( curr_t->amount );
            } else {
                if ( !( ost << " + " << curr_t->amount << ", from: " << curr_t->debit << ", sign: " << curr_t->sign
                            << endl ) )
                    ost.setstate( ios_base::failbit );
                result += ( curr_t->amount );
            }
        }
        if ( !( ost << " = " << result << endl ) )
            ost.setstate( ios_base::failbit );

        return ost;
    }

    void PushTransaction ( const CTransactionRef & transactionRef, int ref ) {
        if ( ts_cnt >= ts_size ) {
            ts_size *= 1.5;
            CTransactionRef * tmp = new CTransactionRef[ts_size];
            int * tmp_i = new int[ts_size];
            for ( int i = 0; i < ts_cnt; ++i ) {
                tmp[ i ] = transactions[ i ];
                tmp_i[ i ] = refs[ i ];
            }
            delete[] transactions;
            delete[] refs;
            transactions = tmp;
            refs = tmp_i;
        }
        refs[ ts_cnt ] = ref;
        transactions[ ( ts_cnt++ ) ] = transactionRef;
    }

    void WipeTransactions ( ) {
        balance = Balance( );
        delete[] transactions;

        ts_cnt = 0;
        ts_size = 4;
        transactions = new CTransactionRef[ts_size];
    }

    long Balance ( ) {
        int64_t result = 0;
        for ( int i = 0; i < ts_cnt; ++i ) {
            if ( transactions[ i ]->debit == code )
                result -= ( transactions[ i ]->amount );
            else
                result += ( transactions[ i ]->amount );
        }

        return balance + result;
    }
};
typedef CAccount * CAccountRef;


class CBank {
private:
    CAccountRef * accounts;
    int acc_size = 8;
    int acc_cnt = 0;

    CTransactionRef * transactions;
    int ts_size = 8;
    int ts_cnt = 0;

    int UserExists ( const char * accID ) const {
        for ( int i = 0; i < acc_cnt; ++i ) {
            const char * cp = ( * accounts[ i ] ).code;
            if ( strcmp( cp, accID ) == 0 )
                return i;
        }
        return -1;
    }

    CPair * ts_log;

public:
    // default constructor
    CBank ( ) {
        accounts = new CAccountRef[acc_size];
        transactions = new CTransactionRef[ts_size];
        ts_log = new CPair[ts_size];
    }

    // copy constructor
    CBank ( const CBank & orig ) {
        acc_size = orig.acc_size;
        acc_cnt = orig.acc_cnt;
        ts_size = orig.ts_size;
        ts_cnt = orig.ts_cnt;

        accounts = new CAccountRef[acc_size];
        transactions = new CTransactionRef[ts_size];
        ts_log = orig.ts_log;

        // rebuild accounts
        for ( int i = 0; i < acc_cnt; ++i ) {
            accounts[ i ] = new CAccount( orig.accounts[ i ]->code, orig.accounts[ i ]->balance );
        }

        // rebuild transactions
        for ( int i = 0; i < ts_cnt; ++i ) {
            int d_i = ts_log[ i ].x;
            int c_i = ts_log[ i ].y;

            transactions[ i ] = new CTransaction(
                    accounts[ d_i ]->code,
                    accounts[ c_i ]->code,
                    orig.transactions[ i ]->sign,
                    orig.transactions[ i ]->amount
            );
        }

        // allocate independent transaction log
        ts_log = new CPair[ts_size];
        for ( int j = 0; j < ts_cnt; ++j ) {
            ts_log[ j ] = orig.ts_log[ j ];
        }

        // rebuild current state of all accounts and their transactions
        for ( int k = 0; k < acc_cnt; ++k ) {
            CAccount * x = orig.accounts[ k ];
            int acc_trans_cnt = x->ts_cnt;
            if ( !acc_trans_cnt )
                continue;

            for ( int i = 0; i < acc_trans_cnt; ++i ) {
                int transaction_index = x->refs[ i ];
                CTransaction * ts = transactions[ transaction_index ];
                accounts[ k ]->PushTransaction( ts, transaction_index );
            }
        }
    }

    // destructor
    ~CBank ( ) {
        for ( int i = 0; i < acc_cnt; ++i )
            delete accounts[ i ];
        delete[] accounts;

        for ( int i = 0; i < ts_cnt; ++i ) {
            delete transactions[ i ];
        }

        delete[] transactions;
        delete[] ts_log;
    }

    // operator =
    //    CBank & operator= ( const CBank & newBank ) {
    //    }

    void operator ~ ( ) {
        for ( int i = 0; i < acc_cnt; ++i )
            std::cout << ( * accounts[ i ] ) << endl;
    }
    void operator ! ( ) {
        for ( int i = 0; i < ts_cnt; ++i )
            std::cout << ( * transactions[ i ] ) << endl;
    }

    bool NewAccount ( const char * accID, int initialBalance ) {
        if ( UserExists( accID ) >= 0 )
            return false;

        if ( acc_cnt >= acc_size ) {
            acc_size *= 1.5;
            CAccountRef * tmp = new CAccountRef[acc_size];
            for ( int i = 0; i < acc_cnt; ++i )
                tmp[ i ] = accounts[ i ];
            delete[] accounts;
            accounts = tmp;
        }

        accounts[ ( acc_cnt++ ) ] = new CAccount( accID, initialBalance );
        return true;
    }

    bool Transaction ( const char * debAccID, const char * credAccID, unsigned int amount, const char * signature ) {
        if ( strcmp( debAccID, credAccID ) == 0 || amount < 0 )
            return false;

        int d_i = 0, c_i = 0;
        if ( ( ( d_i = UserExists( debAccID ) ) == -1 ) || ( ( c_i = UserExists( credAccID ) ) == -1 ) )
            return false;

        if ( ts_cnt >= ts_size ) {
            ts_size *= 1.5;
            CTransactionRef * tmp = new CTransactionRef[ts_size];
            CPair * tmp_log = new CPair[ts_size];
            for ( int i = 0; i < ts_cnt; ++i ) {
                tmp[ i ] = transactions[ i ];
                tmp_log[ i ] = ts_log[ i ];
            }
            delete[] transactions;
            delete[] ts_log;
            transactions = tmp;
            ts_log = tmp_log;
        }

        transactions[ ts_cnt ] =
                new CTransaction(
                        ( * accounts[ d_i ] ).code,
                        ( * accounts[ c_i ] ).code,
                        signature,
                        amount
                );

        accounts[ d_i ]->PushTransaction( transactions[ ts_cnt ], ts_cnt );
        accounts[ c_i ]->PushTransaction( transactions[ ts_cnt ], ts_cnt );

        ts_log[ ts_cnt ].set( d_i, c_i );
        ts_cnt++;

        return true;
    }

    bool TrimAccount ( const char * accID ) {
        int i = -1;
        if ( ( i = UserExists( accID ) ) == -1 )
            return false;

        accounts[ i ]->WipeTransactions( );
        return true;
    }

    CAccount & Account ( const char * accID ) const {
        int i = UserExists( accID );
        if ( i == -1 )
            throw "Account( ) method: Account not found.";
        return ( * accounts[ i ] );
    }

    void PrintLog ( ) {
        for ( int i = 0; i < ts_cnt; ++i ) {
            ~ts_log[ i ];
        }
    }

    void PrintTransactions ( ) {
        for ( int i = 0; i < ts_cnt; ++i )
            cout << ( * transactions[ i ] );
    }
};

#ifndef __PROGTEST__
int main ( ) {
    ostringstream os;
    char accCpy[100], debCpy[100], credCpy[100], signCpy[100];

    CBank x0;
    assert ( x0.NewAccount( "123456", 1000 ) );
    assert ( x0.NewAccount( "987654", -500 ) );

    assert ( x0.Transaction( "123456", "987654", 300, "XAbG5uKz6E=" ) );
    assert ( x0.Transaction( "123456", "987654", 2890, "AbG5uKz6E=" ) );

    assert ( x0.NewAccount( "111111", 5000 ) );
    assert ( x0.Transaction( "111111", "987654", 290, "Okh6e+8rAiuT5=" ) );

    assert ( x0.Account( "123456" ).Balance( ) == -2190 );
    assert ( x0.Account( "987654" ).Balance( ) == 2980 );
    assert ( x0.Account( "111111" ).Balance( ) == 4710 );

    os.str( "" );
    os << x0.Account( "123456" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );

    os.str( "" );
    os << x0.Account( "987654" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 290, from: 111111, sign: Okh6e+8rAiuT5=\n = 2980\n" ) );

    os.str( "" );
    os << x0.Account( "111111" );
    assert ( !strcmp( os.str( ).c_str( ), "111111:\n   5000\n - 290, to: 987654, sign: Okh6e+8rAiuT5=\n = 4710\n" ) );

    assert ( x0.TrimAccount( "987654" ) );
    assert ( x0.Transaction( "111111", "987654", 123, "asdf78wrnASDT3W" ) );

    os.str( "" );
    os << x0.Account( "987654" );
    assert (
            !strcmp( os.str( ).c_str( ), "987654:\n   2980\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 3103\n" ) );


    CBank x2;
    strncpy( accCpy, "123456", sizeof( accCpy ) );
    assert ( x2.NewAccount( accCpy, 1000 ) );

    strncpy( accCpy, "987654", sizeof( accCpy ) );
    assert ( x2.NewAccount( accCpy, -500 ) );

    strncpy( debCpy, "123456", sizeof( debCpy ) );
    strncpy( credCpy, "987654", sizeof( credCpy ) );
    strncpy( signCpy, "XAbG5uKz6E=", sizeof( signCpy ) );
    assert ( x2.Transaction( debCpy, credCpy, 300, signCpy ) );

    strncpy( debCpy, "123456", sizeof( debCpy ) );
    strncpy( credCpy, "987654", sizeof( credCpy ) );
    strncpy( signCpy, "AbG5uKz6E=", sizeof( signCpy ) );
    assert ( x2.Transaction( debCpy, credCpy, 2890, signCpy ) );

    strncpy( accCpy, "111111", sizeof( accCpy ) );
    assert ( x2.NewAccount( accCpy, 5000 ) );

    strncpy( debCpy, "111111", sizeof( debCpy ) );
    strncpy( credCpy, "987654", sizeof( credCpy ) );
    strncpy( signCpy, "Okh6e+8rAiuT5=", sizeof( signCpy ) );
    assert ( x2.Transaction( debCpy, credCpy, 2890, signCpy ) );

    assert ( x2.Account( "123456" ).Balance( ) == -2190 );
    assert ( x2.Account( "987654" ).Balance( ) == 5580 );
    assert ( x2.Account( "111111" ).Balance( ) == 2110 );

    os.str( "" );
    os << x2.Account( "123456" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );

    os.str( "" );
    os << x2.Account( "987654" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n = 5580\n" ) );

    os.str( "" );
    os << x2.Account( "111111" );
    assert ( !strcmp( os.str( ).c_str( ), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n = 2110\n" ) );

    assert ( x2.TrimAccount( "987654" ) );
    strncpy( debCpy, "111111", sizeof( debCpy ) );
    strncpy( credCpy, "987654", sizeof( credCpy ) );
    strncpy( signCpy, "asdf78wrnASDT3W", sizeof( signCpy ) );
    assert ( x2.Transaction( debCpy, credCpy, 123, signCpy ) );
    os.str( "" );
    os << x2.Account( "987654" );
    assert (
            !strcmp( os.str( ).c_str( ), "987654:\n   5580\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );


    CBank x4;
    assert ( x4.NewAccount( "123456", 1000 ) );
    assert ( x4.NewAccount( "987654", -500 ) );
    assert ( !x4.NewAccount( "123456", 3000 ) );
    assert ( !x4.Transaction( "123456", "666", 100, "123nr6dfqkwbv5" ) );
    assert ( !x4.Transaction( "666", "123456", 100, "34dGD74JsdfKGH" ) );
    assert ( !x4.Transaction( "123456", "123456", 100, "Juaw7Jasdkjb5" ) );
    try {
        x4.Account( "666" ).Balance( );
        assert ( "Missing exception !!" == NULL );
    }
    catch ( ... ) {
    }
    try {
        os << x4.Account( "666" ).Balance( );
        assert ( "Missing exception !!" == NULL );
    }
    catch ( ... ) {
    }
    assert ( !x4.TrimAccount( "666" ) );
    // OK

    CBank x6;
    assert ( x6.NewAccount( "123456", 1000 ) );
    assert ( x6.NewAccount( "987654", -500 ) );
    assert ( x6.Transaction( "123456", "987654", 300, "XAbG5uKz6E=" ) );
    assert ( x6.Transaction( "123456", "987654", 2890, "AbG5uKz6E=" ) );
    assert ( x6.NewAccount( "111111", 5000 ) );
    assert ( x6.Transaction( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );

    CBank x7( x6 );
    assert ( x6.Transaction( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
    assert ( x7.Transaction( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
    assert ( x6.NewAccount( "99999999", 7000 ) );
    assert ( x6.Transaction( "111111", "99999999", 3789, "aher5asdVsAD" ) );
    assert ( x6.TrimAccount( "111111" ) );
    assert ( x6.Transaction( "123456", "111111", 221, "Q23wr234ER==" ) );

    os.str( "" );
    os << x6.Account( "111111" );
    assert ( !strcmp( os.str( ).c_str( ), "111111:\n   -1802\n + 221, from: 123456, sign: Q23wr234ER==\n = -1581\n" ) );

    os.str( "" );
    os << x6.Account( "99999999" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "99999999:\n   7000\n + 3789, from: 111111, sign: aher5asdVsAD\n = 10789\n" ) );

    os.str( "" );
    os << x6.Account( "987654" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );

    os.str( "" );
    os << x7.Account( "111111" );
    assert ( !strcmp( os.str( ).c_str( ),
                      "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );

    try {
        os << x7.Account( "99999999" ).Balance( );
        assert ( "Missing exception !!" == NULL );
    }
    catch ( ... ) {
    }
    os.str( "" );
    os << x7.Account( "987654" );

    assert ( !strcmp( os.str( ).c_str( ),
                      "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 789, from: 111111, sign: SGDFTYE3sdfsd3W\n = 6369\n" ) );

    
//    CBank x8;
//    CBank x9;
//    assert ( x8.NewAccount( "123456", 1000 ) );
//    assert ( x8.NewAccount( "987654", -500 ) );
//    assert ( x8.Transaction( "123456", "987654", 300, "XAbG5uKz6E=" ) );
//    assert ( x8.Transaction( "123456", "987654", 2890, "AbG5uKz6E=" ) );
//    assert ( x8.NewAccount( "111111", 5000 ) );
//    assert ( x8.Transaction( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
//    x9 = x8;
//    assert ( x8.Transaction( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
//    assert ( x9.Transaction( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
//    assert ( x8.NewAccount( "99999999", 7000 ) );
//    assert ( x8.Transaction( "111111", "99999999", 3789, "aher5asdVsAD" ) );
//    assert ( x8.TrimAccount( "111111" ) );
//    os.str( "" );
//    os << x8.Account( "111111" );
//    assert ( !strcmp( os.str( ).c_str( ), "111111:\n   -1802\n = -1802\n" ) );
//    os.str( "" );
//    os << x9.Account( "111111" );
//    assert ( !strcmp( os.str( ).c_str( ),
//                      "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );

    return 0;
}
#endif /* __PROGTEST__ */
