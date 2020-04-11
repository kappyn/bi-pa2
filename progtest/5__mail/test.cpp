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
#include <set>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>

#endif /* __PROGTEST */
using namespace std;

#ifndef __PROGTEST__
class CTimeStamp {
private:
	int y, m, d, h, min, sec;
public:
	CTimeStamp ( int year, int month, int day, int hour, int minute, int sec ) :
			y( year ), m( month ), d( day ), h( hour ), min( minute ), sec( sec ) { }
	int Compare ( const CTimeStamp & x ) const {
		if ( y == x.y )
			if ( m == x.m )
				if ( d == x.d )
					if ( h == x.h )
						if ( min == x.min )
							if ( sec == x.sec ) return 0;
							else return ( sec > x.sec ) ? 1 : -1;
						else return ( min > x.min ) ? 1 : -1;
					else return ( h > x.h ) ? 1 : -1;
				else return ( d > x.d ) ? 1 : -1;
			else return ( m > x.m ) ? 1 : -1;
		else return ( y > x.y ) ? 1 : -1;
	}

	friend ostream & operator << ( ostream & os, const CTimeStamp & x ) {
		if ( !
				( os << x.y << "-"
				     << ( x.m < 10 ? "0" : "" ) << x.m << "-"
				     << ( x.d < 10 ? "0" : "" ) << x.d << " "
				     << ( x.h < 10 ? "0" : "" ) << x.h << ":"
				     << ( x.min < 10 ? "0" : "" ) << x.min << ":"
				     << ( x.sec < 10 ? "0" : "" ) << x.sec )
				)
			os.setstate( ios_base::failbit );
		return os;
	}
};
//=================================================================================================
class CMailBody {
private:
	int m_Size;
	char * m_Data;

public:
	CMailBody ( ) {
		m_Data = nullptr;
		m_Size = 0;
	}

	CMailBody ( int size, const char * data ) {
		m_Size = size + 1;
		m_Data = new char[m_Size];
		for ( int i = 0; i < m_Size - 1; ++i )
			m_Data[ i ] = data[ i ];
		m_Data[ m_Size - 1 ] = '\0';
	}

	CMailBody ( const CMailBody & ref ) {
		if ( !ref.m_Data ) {
			m_Data = nullptr;
			m_Size = 0;
			return;
		}
		m_Size = ref.m_Size;
		m_Data = new char[m_Size];
		for ( int i = 0; i < m_Size; ++i )
			m_Data[ i ] = ref.m_Data[ i ];
	}

	CMailBody & operator = ( const CMailBody & ref ) {
		if ( this == & ref )
			return * this;

		if ( !ref.m_Data ) {
			m_Size = 0;
			m_Data = nullptr;
			return * this;
		}

		delete[] m_Data;
		m_Size = ref.m_Size;
		m_Data = new char[m_Size];
		for ( int i = 0; i < m_Size; ++i )
			m_Data[ i ] = ref.m_Data[ i ];
		return * this;
	}

	~CMailBody ( ) {
		delete[] m_Data;
	}

	// copy cons/op=/destructor is correctly implemented in the testing environment
	friend ostream & operator << ( ostream & os, const CMailBody & x ) {
		return os << "mail body: " << (x.m_Size - 1) << " B";
	}
};
//=================================================================================================
class CAttach {
public:
	CAttach ( int x )
			: m_X( x ),
			  m_RefCnt( 1 ) {
	}
	void AddRef ( void ) const {
		m_RefCnt++;
	}
	void Release ( void ) const {
		if ( !--m_RefCnt )
			delete this;
	}
private:
	int m_X;
	mutable int m_RefCnt;
	CAttach ( const CAttach & x );
	CAttach & operator = ( const CAttach & x );
	~CAttach ( void ) = default;
	friend ostream & operator << ( ostream & os,
	                               const CAttach & x ) {
		return os << "+ attachment: " << x.m_X << " B";
	}
};
//=================================================================================================
#endif /* __PROGTEST__, DO NOT remove */


class CMail {
private:
	CTimeStamp timeStamp;
	string from;
	CMailBody body;
	const CAttach * attach;

	string bckup;
public:
	CMail ( const CTimeStamp & timeStamp,
	        string from,
	        const CMailBody & body,
	        const CAttach * attach ) :
			timeStamp( timeStamp ), from( std::move( from ) ), body( body ), attach( attach ) {
		if ( attach ) {
            std::ostringstream oss;
            oss << * attach;
            bckup = oss.str();
		}
	}

	const string & From ( ) const {
		return from;
	}
	const CMailBody & Body ( ) const {
		return body;
	}
	const CTimeStamp & TimeStamp ( ) const {
		return timeStamp;
	}
	const CAttach * Attachment ( ) const {
		return attach;
	}
	friend ostream & operator << ( ostream & os, const CMail & x ) {
		if ( !( os << x.TimeStamp( ) << " " << x.From( ) << " " << x.Body( ) ) )
			os.setstate( ios::failbit );

		if ( ! x.bckup.empty( ) ) {
			if ( !( os << " " << x.bckup ) )
				os.setstate( ios::failbit );
		}
		return os;
	}
};
//=================================================================================================
class CMailBox {
public:
	struct CMailCmp {
		bool operator ( ) ( const CMail & a, const CMail & b ) const {
			return a.TimeStamp().Compare(b.TimeStamp()) < 0;
		}
	};

	typedef multiset<CMail, CMailCmp> CMailMultiset;
	map<string, CMailMultiset> mb;

	CMailBox ( ) {
		mb.insert( pair<string, CMailMultiset>( "inbox", CMailMultiset( ) ) );
	}
	bool Delivery ( const CMail & mail ) {
		auto i = mb.find( "inbox" );
		if ( i == mb.end( ) )
			return false;
		i->second.insert( mail );
		return true;
	}
	bool NewFolder ( const string & folderName ) {
		auto i = mb.find( folderName );
		if ( i != mb.end( ) )
			return false;
		mb.insert( pair<string, CMailMultiset>( folderName, CMailMultiset( ) ) );
		return true;
	}
	bool MoveMail ( const string & fromFolder, const string & toFolder ) {
		auto i = mb.find( fromFolder );
		if ( i == mb.end( ) )
			return false;

		auto j = mb.find( toFolder );
		if ( j == mb.end( ) )
			return false;

		for ( auto x : i->second ) {
			j->second.insert( x );
		}
		i->second.clear();

		return true;
	}

	list<CMail> ListMail ( const string & folderName, const CTimeStamp & from, const CTimeStamp & to ) const {
		auto f = mb.find( folderName );

		list<CMail> res;
		for ( const auto & i : f->second ) {
			if ( ( i.TimeStamp().Compare( from ) > 0 && i.TimeStamp().Compare( to ) < 0 ) ||
			       i.TimeStamp().Compare( from ) == 0 || i.TimeStamp().Compare( to ) == 0 )
			{
				res.push_back( i );
			}
		}

		return res;
	}
	set<string> ListAddr ( const CTimeStamp & from, const CTimeStamp & to ) const {
		set<string> res;
		for ( auto i : mb ) {
			for ( const auto & j : i.second ) {
				if ( ( j.TimeStamp().Compare( from ) > 0 && j.TimeStamp().Compare( to ) < 0 ) ||
				       j.TimeStamp().Compare( from ) == 0 || j.TimeStamp().Compare( to ) == 0 )
					res.insert( j.From() );
			}
		}
		return res;
	}
	void printMails ( string inbox ) {
		auto i = mb.find( inbox );
		if ( i == mb.end( ) )
			throw "Wrong name!\n";

		for ( const auto & j : i->second ) {
			cout << j << endl;
		}
	}
};
//=================================================================================================
#ifndef __PROGTEST__
static string showMail ( const list<CMail> & l ) {
	ostringstream oss;
	for ( const auto & x : l ) {
		oss << x << endl;
	}
	return oss.str( );
}
static string showUsers ( const set<string> & s ) {
	ostringstream oss;
	for ( const auto & x : s ) {
		oss << x << endl;
	}
	return oss.str( );
}

int main ( ) {
  list<CMail> mailList;
  set<string> users;
  CAttach   * att;

  CMailBox m0;
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), nullptr ) ) );
  att = new CAttach ( 200 );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), att ) ) );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
  att -> Release ();
  att = new CAttach ( 97 );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), att ) ) );
  att -> Release ();
  assert ( showMail ( m0 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n" );
  assert ( showMail ( m0 . ListMail ( "inbox",
                      CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
                      CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) ) == "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n" );
  assert ( showUsers ( m0 . ListAddr ( CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                       CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "boss1@fit.cvut.cz\n"
                        "user1@fit.cvut.cz\n"
                        "user2@fit.cvut.cz\n" );
  assert ( showUsers ( m0 . ListAddr ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
                       CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) ) == "boss1@fit.cvut.cz\n"
                        "user2@fit.cvut.cz\n" );

  CMailBox m1;
  assert ( m1 . NewFolder ( "work" ) );
  assert ( m1 . NewFolder ( "spam" ) );
  assert ( !m1 . NewFolder ( "spam" ) );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
  att = new CAttach ( 500 );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), att ) ) );
  att -> Release ();
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), nullptr ) ) );
  att = new CAttach ( 468 );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
  att -> Release ();
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), nullptr ) ) );
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( m1 . MoveMail ( "inbox", "work" ) );
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 19, 24, 13 ), "user2@fit.cvut.cz", CMailBody ( 14, "mail content 4" ), nullptr ) ) );
  att = new CAttach ( 234 );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 13, 26, 23 ), "user3@fit.cvut.cz", CMailBody ( 9, "complains" ), att ) ) );
  att -> Release ();
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                        "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( m1 . MoveMail ( "inbox", "work" ) );
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
                        "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );

  return 0;
}
#endif /* __PROGTEST__ */
