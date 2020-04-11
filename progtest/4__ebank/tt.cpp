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

    CPair * ts_log;

    int UserExists ( const char * accID ) const {
        for ( int i = 0; i < acc_cnt; ++i ) {
            const char * cp = ( * accounts[ i ] ).code;
            if ( strcmp( cp, accID ) == 0 )
                return i;
        }
        return -1;
    }

    void DeepCopy ( const CBank & orig ) {
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

    void FreeResources ( ) {
        for ( int i = 0; i < acc_cnt; ++i )
            delete accounts[ i ];
        delete[] accounts;

        for ( int i = 0; i < ts_cnt; ++i ) {
            delete transactions[ i ];
        }

        delete[] transactions;
        delete[] ts_log;
    }

public:
    CBank ( ) {
        accounts = new CAccountRef[acc_size];
        transactions = new CTransactionRef[ts_size];
        ts_log = new CPair[ts_size];
    }

    CBank ( const CBank & orig ) {
        DeepCopy( orig );
    }

    ~CBank ( ) {
        FreeResources( );
    }


    CBank & operator = ( const CBank & orig ) {
        if ( this == & orig )
            return * this;

        FreeResources( );
        DeepCopy( orig );

        return * this;
    }

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
    return 0;
}
#endif /* __PROGTEST__ */
