#ifndef __PROGTEST__

#include <cctype>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <set>
#include <list>
#include <forward_list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <deque>
#include <optional>
#include <variant>
#include <any>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template <typename _T, typename _TComp = std::equal_to<typename _T::value_type> >
class CSearch {
private:
	typedef typename _T::value_type T;
	map<int, _T> m_Data;
	_TComp Comparator;

public:
	// default constructor
	CSearch ( ) { }

	// constructor with comparator parameter
	CSearch ( _TComp comparator ) : Comparator( comparator ) { }

	// destructor (if needed)
	CSearch ( const CSearch & src ) = delete;
	CSearch & operator = ( const CSearch & src ) = delete;

	bool NeedlePresent ( const _T & hayHeap, const _T & needle ) const {
		return std::search( hayHeap.begin( ), hayHeap.end( ), needle.begin( ), needle.end( ), Comparator ) !=
		       hayHeap.end( );
	}

	// do NOT edit the function interface
	void Add ( int id, const _T & needle ) {
		if ( ! m_Data.insert( pair<int, _T>( id, needle ) ).second )
			throw std::invalid_argument( "Key already exists." );
	}

	// do NOT edit the function interface
	std::set<int> Search ( const _T & hayHeap ) const {
		std::set<int> result;
		for ( const auto & i : m_Data )
			if ( NeedlePresent( hayHeap, i.second ) )
				result.insert( i.first );
		return result;
	}

	// debug
	friend std::ostream & operator << ( std::ostream & ost, const CSearch<_T, _TComp> & cs ) {
		for ( const auto & i : cs.m_Data ) {
			for ( const auto & j : i.second )
				std::cout << j << " ";
			std::cout << endl;
		}
		return ost;
	}
};
//-------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
class CharComparator {
private:
	bool m_CaseSensitive;
public:
	CharComparator ( bool caseSensitive = true ) : m_CaseSensitive( caseSensitive ) { }
	bool operator () ( const char & a, const char & b ) const {
		return m_CaseSensitive ? a == b : toupper( a ) == toupper( b );
	};
};
//-------------------------------------------------------------------------------------------------
bool upperCaseCompare ( const char & a, const char & b ) {
	return toupper( a ) == toupper( b );
}
//-------------------------------------------------------------------------------------------------
int main ( void ) { }
#endif /* __PROGTEST__ */
