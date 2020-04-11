#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

using namespace std;

template <typename _T>
class CBST;
template <typename _T>
ostream & operator << ( ostream & os, const CBST<_T> & x );

template <typename _T>
class CBST {
public:
	CBST ( );
	CBST ( const CBST<_T> & src );
	~CBST ( );
	CBST<_T> & operator = ( const CBST<_T> & src );

	bool Insert ( const _T & x );
	bool IsSet ( const _T & x ) const;
	bool Delete ( const _T & x );
	size_t Size ( ) const;
	friend ostream & operator <<< > ( ostream & os, const CBST<_T> & x );

private:
	class CNode {
	public:
		CNode ( const _T x ) : m_Val( x ), m_L( NULL ), m_R( NULL ) { }
		CNode ( const CNode & src ) :
				m_Val( src.m_Val ),
				m_L( src.m_L ? src.m_L->Clone( ) : NULL ),
				m_R( src.m_R ? src.m_R->Clone( ) : NULL ) {
		}
		CNode * Clone ( ) const {
			return new CNode( * this );
		}
		~CNode ( ) {
			delete m_L;
			delete m_R;
		}
		void Print ( ostream & os, bool isFirst ) const {
			if ( m_L ) {
				m_L->Print( os, isFirst );
				isFirst = false;
			}
			if ( !isFirst )
				os << ", ";
			os << m_Val;

			if ( m_R )
				m_R->Print( os, false );
		}
		_T m_Val;
		CNode * m_L;
		CNode * m_R;
	};

	size_t m_Size;
	CNode * m_Root;
};

template <typename _T>
CBST<_T>::CBST ( ) : m_Size( 0 ), m_Root( NULL ) { }

template <typename _T>
CBST<_T>::CBST ( const CBST & src ) : m_Size( src.m_Size ), m_Root( src.m_Root ? src.m_Root->Clone( ) : NULL ) { }

template <typename _T>
CBST<_T> & CBST<_T>::operator = ( const CBST<_T> & src ) {
	if ( this == & src )
		return * this;

	delete m_Root;
	m_Size = src.m_Size;
	m_Root = src.m_Root ? src.m_Root->Clone( ) : NULL;
	return * this;
}

template <typename _T>
CBST<_T>::~CBST ( ) {
	delete m_Root;
}

template <typename _T>
bool CBST<_T>::Insert ( const _T & x ) {
	CNode * tmp = m_Root;
	CNode * p = NULL;

	while ( tmp ) {
		if ( tmp->m_Val == x )
			return false;
		p = tmp;
		if ( x < tmp->m_Val )
			tmp = tmp->m_L;
		else
			tmp = tmp->m_R;
	}

	CNode * n = new CNode( x );
	if ( !p )
		m_Root = n;
	else if ( x < p->m_Val )
		p->m_L = n;
	else
		p->m_R = n;

	++m_Size;
	return true;
}

template <typename _T>
bool CBST<_T>::IsSet ( const _T & x ) const {
	CNode * tmp = m_Root;
	while ( tmp ) {
		if ( tmp->m_Val == x )
			return true;
		if ( x < tmp->m_Val )
			tmp = tmp->m_L;
		else
			tmp = tmp->m_R;
	}
	return false;
}

template <typename _T>
bool CBST<_T>::Delete ( const _T & x ) {
	CNode * tmp = m_Root;
	CNode * p = NULL;
	while ( tmp ) {
		if ( tmp->m_Val == x ) {
			if ( tmp->m_L && tmp->m_R ) {
				CNode * toReplace = tmp;
				p = tmp;
				tmp = tmp->m_L;
				while ( tmp->m_R ) {
					p = tmp;
					tmp = tmp->m_R;
				}
				toReplace->m_Val = tmp->m_Val;
			}

			CNode * toSave;
			if ( tmp->m_L ) toSave = tmp->m_L;
			else toSave = tmp->m_R;

			if ( !p ) m_Root = toSave;
			else if ( tmp->m_Val < p->m_Val ) p->m_L = toSave;
			else p->m_R = toSave;

			tmp->m_L = tmp->m_R = NULL;
			delete tmp;
			--m_Size;
			return true;
		}
		p = tmp;
		if ( x < tmp->m_Val )
			tmp = tmp->m_L;
		else
			tmp = tmp->m_R;
	}
	return false;
}

template <typename _T>
size_t CBST<_T>::Size ( void ) const {
	return m_Size;
}

template <typename _T>
ostream & operator << ( ostream & os, const CBST<_T> & x ) {
	os << '{';
	if ( x.m_Root )
		x.m_Root->Print( os, true );
	return os << '}';
}

int main ( ) {
	CBST<int> x;
	assert( x.Insert( 10 ) );
	assert( x.Insert( 20 ) );
	assert( x.Insert( 5 ) );
	assert( x.Insert( 7 ) );
	assert( !x.Insert( 5 ) );
	cout << x << endl;

	assert( x.IsSet( 5 ) );
	assert( x.IsSet( 20 ) );
	assert( !x.IsSet( 15 ) );
	assert( x.Delete( 10 ) );
	cout << x << endl;
	assert( !x.IsSet( 10 ) );

	CBST<string> y;
	assert( y.Insert( "test" ) );
	assert( y.Insert( "Progtest" ) );
	cout << y << endl;

	return 0;
}