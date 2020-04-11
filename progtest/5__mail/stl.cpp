#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
class CInt
 {
   public:
          CInt ( int x ) : m_X ( new int ( x ) ) {}
          CInt ( const CInt & src ) { m_X = src . m_X; }
    void  operator = ( int x ) { *m_X = x; }
    void  print ( void ) const { cout << *m_X; }
   private:
    int * m_X;
 };
int main ( void )
 {
   CInt a ( 40 ), b ( 44 ), c = a;
   a = 29;
   c . print ();
   return 0;
 }