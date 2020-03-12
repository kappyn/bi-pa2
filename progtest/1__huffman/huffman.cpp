#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

using namespace std;

const char * INVALID_READ = "Error while reading.";
const char * INVALID_FILE = "File not found.";
const char * INVALID_WRITE = "Error while writing.";
const char * FNAME = "tests/test0.huf";

struct node {
    int data;
    node * left;
    node * right;

    explicit node ( char data = 0, node * left = nullptr, node * right = nullptr )
            : data( data ), left( left ), right( right ) { }
};

class btree {
public:
    node * root;

    btree ( ) {
        cout << "Binary tree was created." << endl;
        this->root = new node( );
    }

    ~btree ( ) {
        delete_nodes( root );
    }

    bool is_empty ( ) {
        return ( root == nullptr );
    }

    static void delete_nodes ( node * link ) {
        if ( link == nullptr )
            return;

        delete_nodes( link->left );
        delete_nodes( link->right );
        delete link;
    }

    static void print_nodes ( node * link ) {
        if ( link == nullptr )
            return;

        print_nodes( link->left );
        cout << link->data << " ";
        print_nodes( link->right );
    }

    static void resolve_tree ( node * link, int * ar, int & index ) {
        if ( ar[ index ] == 1 || index >= 10 ) {
            link->left = link->right = nullptr;
            link->data = 64;
            return;
        }
        resolve_tree( ( link->left = new node( ) ), ar, ++index );
        resolve_tree( ( link->right = new node( ) ), ar, ++index );
    }

private:
};

class huff {
public:
    static bool deserialize ( const char * fname ) {
        ifstream ifs{ fname, ios::binary | ios::in };
        if ( !ifs ) {
            cout << INVALID_FILE << endl;
            return false;
        }

        int bit = 0,
                abuff = 0,
                shifts = 0,
                achar = 0;

        bool amode = false;
        char c = 0;

        while ( ifs.get( c ) ) {
            // Read 8 bit sequence
            for ( int i = 7; i >= 0; i-- ) {
                bit = ( ( c >> i ) & 1 );

                // Ascii entering off
                if ( !amode )
//                    cout << bit;

                    // First 1 on the traversal sequence => Ascii entering on
                    if ( ( bit ) && ( !amode ) ) {
                        amode = true;
                        abuff = 0;
                        shifts = 7;
//                    cout << " ";
                    }

                if ( amode ) {
//                    cout << bit;
                    if ( abuff > 0 ) {
                        if ( bit )
                            // Add all powers of two, recreate character
                            achar += ( 1 << shifts );
                        shifts--;
                    }
                    if ( ++abuff == 9 ) {
                        amode = false;
                        achar = shifts = 0;
                        cout << endl;
                    }
                }
            }
        }

        if ( !ifs.eof( ) )
            return false;

        ifs.close( );
        return true;
    }
};

int main ( ) {
    btree tree;

    if ( ! huff::deserialize( FNAME ) ) {
        cout << INVALID_READ;
        return 1;
    }

    int test[ 10 ] = { 0, 1, 0, 0, 1, 1, 0, 1, 1 };
    int index = 0;
//    int data[3] = { 65, 66, 67 };

    btree::resolve_tree( tree.root, test, index );
    if ( index >= 10 ) {
        cout << "The tree sequence is incorrect.\n";
    } else {
        cout << "The tree was successfully constructed.\n";
    }

//    assert( ( char ) tree.root->right->left->left->data == '@' );

    return 0;
}