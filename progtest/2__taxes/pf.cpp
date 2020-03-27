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

/**
 * Poznamky k formatovani:
 * 1. Nektere if statementy, konstruktory, rozhrani metod rozdeluji na vice radek v zajmu lepsi citelnosti.
 * 2. Funkce jsou camelcase a "nafouknute mezerami"; jejich volani nema mezeru pred (), narozdil od deklaraci.
 * 3. Atributy objektu jsou malym pismenem.
**/

struct CWallet {
    string code;
    u_int16_t income;
    u_int16_t expense;

    explicit CWallet ( string code = "", int32_t income = 0, int32_t expenses = 0 ) : code( std::move( code ) ),
                                                                                      income( income ),
                                                                                      expense( expenses ) {}
};

class CCitizen {
public:
    string name;
    string addr;
    CWallet acc;
    explicit CCitizen ( string name, string addr, CWallet acc ) : name( std::move( name ) ), addr( std::move( addr ) ),
                                                                  acc( std::move( acc ) ) { };

    /**
    * Metoda porovna dva obcany x,y na zaklade kodu jejich bankovniho uctu.
    * @return true pokud x je abecedne pred y.
    **/
    static bool CmpAcc ( const CCitizen * x, const CCitizen * y ) {
        return x->acc.code > y->acc.code;
    }

    /**
    * Metoda porovna dva obcany x,y na zaklade jmena/prijmeni, popr. adresy.
    * @return true pokud x je abecedne pred y.
    **/
    static bool CmpNameAddr ( const CCitizen * x, const CCitizen * y ) {
        if ( x->name == y->name )
            return x->addr < y->addr;
        return x->name < y->name;
    }

    /**
    * Pretizeny operator pro snadnejsi debugovani.
    * @param[in] usr: Reference na obcana, ktery bude printovan.
    * @param[in,out] ost: Reference na vystupni stream.
    **/
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
};

class CIterator {
private:
    vector<CCitizen *>::const_iterator it;
    const vector<CCitizen *> & origin_ref;
public:
    /**
    * Konstruktor kopiruje odkaz na existujici vektor pointeru a na zaklade neho vytvari iterator.
    **/
    explicit CIterator ( const vector<CCitizen *> & ref ) : origin_ref( ref ) {
        it = origin_ref.begin( );
    }

    /**
    * Funkcionalita nasledujicich metod plyne ze zadani domaci ulohy.
    **/
    bool AtEnd ( ) const {
        return it == origin_ref.end( );
    }
    void Next ( ) {
        if ( !AtEnd( ) ) it++;
    }
    string Name ( ) const {
        return ( !AtEnd( ) ? ( * it )->name : "" );
    }
    string Addr ( ) const {
        return ( !AtEnd( ) ? ( * it )->addr : "" );
    }
    string Account ( ) const {
        return ( !AtEnd( ) ? ( * it )->acc.code : "" );
    }
};

class CTaxRegister {
/**
 * Kazda instance tohoto objektu obsahuje nasledujici vektory ukazujici na stejnou mnozinu registrovanych obcanu.
 * db_code - udrzuje ukazatele na obcany, serazeny podle bankovnich uctu.
 * db_name - udrzuje ukazatele na obcany, serazeny podle jmena/prijmeni, popr. adresy.
 * Diky temto konzistentne udrzovanym vektorum muzeme obsah registru prochazet efektivneji (binarnim vyhledavanim).
**/
private:
    vector<CCitizen *> db_code;
    vector<CCitizen *> db_name;
    CCitizen * tmp_cit = nullptr;
public:
    ~CTaxRegister ( ) {
        // Staci uvolnit pouze z jednoho vektoru - kontejnery odkazuji na stejne objekty.
        for ( const CCitizen * i : db_code )
            delete i;
    }

    /**
     * Metoda vypise vsechny registrovane obcany.
     * @param[in] code_order: Serazeni. Implicitne vypise serazene podle jmen/prijmeni (false).
    **/
    void PrintCitizens ( bool code_order = false ) const {
        for ( const CCitizen * i : ( code_order ? db_code : db_name ) )
            cout << * i << endl;
    }

    /**
     * Metoda zkontroluje uzivateluv vstup a prida jej do patricnych kontejneru.
     * @param[in] tmp: Serazeni. Defaultne vypise serazene podle jmen/prijmeni, popr. adres.
     * @return true, pokud byl vstup korektni a pridani obcana probehla uspesne.
    **/
    bool ValidateCitizen ( CCitizen * tmp ) {
        if ( db_code.empty( ) && db_name.empty( ) ) {
            db_code.push_back( tmp );
            db_name.push_back( tmp );
            return true;
        }

        // Kontrola duplikatu bankovniho uctu.
        auto tmp_it_acc = lower_bound( db_code.begin( ), db_code.end( ), tmp, CCitizen::CmpAcc );
        if (
            tmp_it_acc != db_code.end( ) &&
            ( ( * tmp_it_acc )->acc.code == tmp->acc.code ||
            ( ( * tmp_it_acc )->name == tmp->name && ( * tmp_it_acc )->addr == tmp->addr ) )
        ) {
            delete tmp;
            return false;
        }

        // Kontrola duplikatu jmena + adresy:
        auto tmp_it_name = lower_bound( db_name.begin( ), db_name.end( ), tmp, CCitizen::CmpNameAddr );
        if (
            tmp_it_name != db_name.end( ) &&
            ( ( * tmp_it_name )->acc.code == tmp->acc.code ||
            ( ( * tmp_it_name )->name == tmp->name && ( * tmp_it_name )->addr == tmp->addr ) )
        ) {
            delete tmp;
            return false;
        }

        // Zjisteni pozic na vlozeni do jednotliych vektoru.
        if ( tmp_it_acc != db_code.end( ) )
            db_code.insert( tmp_it_acc, tmp );
        else
            db_code.push_back( tmp );

        if ( tmp_it_name != db_name.end( ) )
            db_name.insert( tmp_it_name, tmp );
        else
            db_name.push_back( tmp );

        return true;
    }

    /**
     * Metoda se stara o zapis prijmu/vydaju na ucet obcana.
     * Tuto metodu vyuzivaji dalsi metody (Income, Expense), kazda z nich v trosku jine konfiguraci, proto ma vice parametru.
     * Konfigurace:
     * @param[in] code_input: Rozhoduje, zda pripisujeme pomoci kodu bankovniho uctu (true), nebo jmena/adresy (false).
     * @param[in] expense: Rozhoduje, zda pripisujeme vydaj (true), nebo prijem (false).
     * Vstupy:
     * @param[in] name, addr: Jmeno/adresa obcana.
     * @param[in] code: Kod bankovniho uctu obcana.
     * @param[in] amount: Castka.
     * @return true, pokud byl vstup korektni a zapis probehl uspesne.
    **/
    bool ManageWallet ( const bool code_input, const bool expense,
                        const string & name, const string & addr,
                        const string & code,
                        const int & amount
    ) {
        if ( amount <= 0 || db_code.empty( ) )
            return false;

        // Hledani obcana v registru.
        tmp_cit = new CCitizen( name, addr, CWallet( code ) );
        vector<CCitizen *>::iterator tmp_it;
        if ( code_input ) {
            tmp_it = lower_bound( db_code.begin( ), db_code.end( ), tmp_cit, CCitizen::CmpAcc );

            // Jmeno a kod se musi shodovat.
            if ( tmp_it == db_code.end( ) || ( ( * tmp_it )->acc.code != tmp_cit->acc.code ) ) {
                delete tmp_cit;
                return false;
            }
        } else {
            tmp_it = lower_bound( db_name.begin( ), db_name.end( ), tmp_cit, CCitizen::CmpNameAddr );
            if ( tmp_it == db_name.end( ) ||
                ( ( * tmp_it )->name != tmp_cit->name ) ||
                ( ( * tmp_it )->addr != tmp_cit->addr ) )
            {
                delete tmp_cit;
                return false;
            }
        }

        expense ? ( * tmp_it )->acc.expense += amount : ( * tmp_it )->acc.income += amount;
        delete tmp_cit;
        return true;
    }

    /**
     * Funkcionalita nasledujicich metod plyne ze zadani domaci ulohy.
     * Strucne poznamky:
     * Birth( ) - vyuziva ValidateCitizen(), popsano vyse.
     * Income( name,.. ) , Income( account ), Expense( name,..), .. - vyuziva ManageWallet() popsano vyse.
     * Audit( ) - vyhledavani ve vektoru db_name - serazeny podle jmen.
     * ListByName - Vytvori novy objekt na zaklade reference db_name. Popsano dale v CIterator.
    **/
    bool Birth ( const string & name, const string & addr, const string & account ) {
        if ( name.empty( ) || addr.empty( ) || account.empty( ) )
            return false;
        return ValidateCitizen( new CCitizen( name, addr, CWallet( account, 0 ) ) );
    }
    bool Death ( const string & name, const string & addr ) {
        if ( db_code.empty( ) || name.empty( ) || addr.empty( ) )
            return false;
        string code;
        tmp_cit = new CCitizen( name, addr, CWallet( ) );
        auto tmp_it_name = lower_bound( db_name.begin( ), db_name.end( ), tmp_cit, CCitizen::CmpNameAddr );
        if ( tmp_it_name == db_name.end( ) || ( ( * tmp_it_name )->name != tmp_cit->name ) ||
             ( ( * tmp_it_name )->addr != tmp_cit->addr ) ) {
            delete tmp_cit;
            return false;
        }
        code = ( * tmp_it_name )->acc.code;
        tmp_cit->acc.code = code;
        auto tmp_it_code = lower_bound( db_code.begin( ), db_code.end( ), tmp_cit, CCitizen::CmpAcc );
        delete tmp_cit;
        tmp_cit = * tmp_it_code;

        if ( tmp_it_code == db_code.end( ) )
            return false;

        db_name.erase( tmp_it_name );
        db_code.erase( tmp_it_code );
        delete tmp_cit;
        return true;
    }
    bool Income ( const string & account, int amount ) {
        return ManageWallet( true, false, "", "", account, amount );
    }
    bool Income ( const string & name, const string & addr, int amount ) {
        return ManageWallet( false, false, name, addr, "", amount );
    }
    bool Expense ( const string & account, int amount ) {
        return ManageWallet( true, true, "", "", account, amount );
    }
    bool Expense ( const string & name, const string & addr, int amount ) {
        return ManageWallet( false, true, name, addr, "", amount );
    }
    bool Audit ( const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense ) const {
        if ( name.empty( ) || addr.empty( ) || db_code.empty( ) )
            return false;

        CCitizen * tmp = new CCitizen( name, addr, CWallet( "" ) );
        auto tmp_it_name = lower_bound( db_name.begin( ), db_name.end( ), tmp, CCitizen::CmpNameAddr );
        if ( tmp_it_name == db_name.end( ) || ( ( * tmp_it_name )->name != tmp->name ) ||
             ( ( * tmp_it_name )->addr != tmp->addr ) ) {
            delete tmp;
            return false;
        }
        delete tmp;
        account = ( * tmp_it_name )->acc.code;
        sumIncome = ( * tmp_it_name )->acc.income;
        sumExpense = ( * tmp_it_name )->acc.expense;
        return true;
    }
    CIterator ListByName ( ) const {
        return CIterator( db_name );
    }
};

#ifndef __PROGTEST__
int main ( ) {
    return 0;
}
#endif /* __PROGTEST__ */
