#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

using namespace std;

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
        this->root = new node( );
        cout << "Binary tree root was created." << endl;
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
};

class bitstream {
private:
    std::ifstream & input;
    int buffer = 0;
    int curr_pos = 8;
public:
    explicit bitstream ( std::ifstream & _input ) : input( _input ) {
        buffer = read_char( );
    };

    int read_curr_bit ( ) {
        return ( ( buffer >> curr_pos ) & 1 );
    }

    int read_char ( ) {
        char c;
        this->input.get( c );
        return c;
    }

    int read_bit ( ) {
        if ( curr_pos-- <= 0 ) {
            curr_pos = 7;
            buffer = read_char( );
            if ( input.eof( ) ) return -1;
            if ( input.fail( ) || input.bad( ) ) return -2;
        }
        return ( ( buffer >> curr_pos ) & 1 );
    }

    int read_sequence ( ) {
        int bit = 0, res = 0;
        for ( int i = 7; i >= 0; i-- ) {
            if ( ( bit = read_bit( ) ) )
                res += ( 1 << i );
            if ( bit < 0 )
                return bit;
        }
        return res;
    }

    int read_tree_bit ( ) {
        int bit = read_bit( );
        if ( bit < 0 )
            return -1;

        if ( bit == 1 )
            if ( read_sequence( ) < 0 )
                return -1;

        cout << bit;
        return bit;
    }

    int treedata[ 12 ] = { 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0 };
    void resolve_tree ( node * link, int & index ) {
        if ( treedata[ index ] == 1 ) {
            link->left = link->right = nullptr;
            return;
        }
        resolve_tree( ( link->left = new node( ) ),  ++index );
        resolve_tree( ( link->right = new node( ) ), ++index );
    }

    void print_bits ( ) {
        int bit;
        while ( ( bit = this->read_bit( ) ) >= 0 )
            std::cout << bit;
        std::cout << endl;
    }
};

int main ( ) {
    ifstream ifs( FNAME, ios::binary | ios::in );
    bitstream bfs{ ifs };
    btree huff;

    int index = 0;
    bfs.resolve_tree( huff.root, index );
    
    cout << "\nRecursion done.\n";
    cout << index;

    assert ( huff.root->left );
    assert ( !huff.root->left->left );
    assert ( !huff.root->left->right );

    assert( huff.root->right );
    assert( huff.root->right->left );
    assert( huff.root->right->right );

    assert ( huff.root->right->left );
    assert ( huff.root->right->left->left );
    assert ( huff.root->right->left->right );

    assert ( huff.root->right->right );
    assert ( huff.root->right->right->left );
    assert ( huff.root->right->right->right );

    return 0;
}
