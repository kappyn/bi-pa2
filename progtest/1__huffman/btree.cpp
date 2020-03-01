#include <iostream>
#include <inttypes.h>

using namespace std;

struct bt_node {
    char data;
    bt_node * left;
    bt_node * right;

    bt_node (
            char data = 0,
            bt_node * left = nullptr,
            bt_node * right = nullptr
                    )
                    : data( data ), left( left ), right( right )
                    {
    }
};

class bt {
public:
    bt_node * root;

    bt ( ) {
        cout << "Binary tree was created." << endl;
        this->root = nullptr;
    }

    ~bt ( ) {
        delete_nodes( root );
    }

    int is_empty ( );
    void insert_node ( char item );
    void print_nodes ( bt_node * link );
    void delete_nodes ( bt_node * link );
    int delete_node ( bt_node * link, char item );
};

int bt::is_empty ( ) {
    return ( root == nullptr );
}

void bt::insert_node ( char item ) {
    bt_node * tmp = new bt_node { item };
    if ( is_empty( ) )
        root = tmp;
    else {
        bt_node * pivot, * parent;
        pivot = root;
        parent = nullptr;

        while ( pivot != nullptr ) {
            parent = pivot;

            if ( item > pivot->data )
                pivot = parent->right;
            else
                pivot = parent->left;

        }

        if ( item > parent->data )
            parent->right = tmp;
        else
            parent->left = tmp;
    }
}

void bt::print_nodes ( bt_node * link ) {
    if ( link == nullptr )
        return;

    // a.k.a. in ordered btree traversal.
    print_nodes( link->left );
    cout << link->data << " ";
    print_nodes( link->right );
}

void bt::delete_nodes( bt_node * link ) {
    if ( link == nullptr )
        return;

    delete_nodes( link->left );
    delete_nodes( link->right );
    delete link;
}

int bt::delete_node ( bt_node * link, char item ) {
    return 0;
}

int main ( ) {
    char set[ 7 ] = { 'E', 'A', 'C', 'D', 'B', 'F', 'Z' };

    bt b;

    for ( int i = 0; i < 7; ++i ) {
        b.insert_node( set[ i ] );
    }

    b.print_nodes( b.root );

    cout << endl;

    return 0;
}