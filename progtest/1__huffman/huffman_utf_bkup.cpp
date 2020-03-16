#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

struct node {
    __int64_t data;
    node * left;
    node * right;

    explicit node ( __int64_t data = -5, node * left = nullptr, node * right = nullptr )
            : data( data ), left( left ), right( right ) { }
};

class btree {
private:
    node * root;

public:
    btree ( ) {
        this->root = new node( );
    }

    ~btree ( ) {
        delete_nodes( root );
    }

    node * get_root ( ) {
        return this->root;
    }

    static void delete_nodes ( node * link ) {
        if ( link == nullptr )
            return;

        delete_nodes( link->left );
        delete_nodes( link->right );
        delete link;
    }

    static void print_nodes ( node * link, bool ascii_mode = false ) {
        if ( link == nullptr )
            return;

        print_nodes( link->left, ascii_mode );
        if ( link->data != -5 ) {
            if ( ascii_mode )
                cout << ( char ) link->data << " ";
            else
                cout << link->data << " ";
        }
        print_nodes( link->right, ascii_mode );
    }
};

class bitstream {
private:
    std::ifstream & input;
    const __uint32_t utf8_upper_bound = 4103061439;

    __int64_t tmp_char = -5;
    __int64_t buffer = 0;
    __int64_t decoded_char = -5;
    __int32_t chunk_chars_left = 0;
    __int8_t curr_pos = 8;

    string parsed_text;

public:
    explicit bitstream ( std::ifstream & _input ) : input( _input ) {
        buffer = read_char( );
    };

    ~bitstream ( ) {
        input.close( );
    }

    std::ifstream & get_stream ( ) {
        return input;
    }

    __int16_t read_char ( ) {
        char c;
        this->input.get( c );
        return c;
    }

    __int8_t read_bit ( ) {
        if ( input.eof( ) ) return -1;
        if ( curr_pos-- <= 0 ) {
            curr_pos = 7;
            buffer = read_char( );
            if ( input.eof( ) ) return -1;
            if ( input.fail( ) || input.bad( ) ) return -2;
        }
        return ( ( buffer >> curr_pos ) & 1 );
    }

    __int64_t read_sequence ( __int16_t bits = 8, bool print = false, __int8_t skip = 0 ) {
        __int8_t bit = 0;
        __uint32_t res = 0;

        if ( skip != 0 ) {
            __int8_t i = 0;
            while ( ++i <= skip )
                if ( read_bit( ) < 0 )
                    return -1;
        }

        for ( __int8_t i = bits - 1; i >= 0; i-- ) {
            if ( ( bit = read_bit( ) ) )
                res += ( 1 << i );
            if ( bit < 0 )
                return -1;
            if ( print ) cout << bit;
        }
        if ( print ) cout << " ";

        if ( res > utf8_upper_bound )
            return -1;

        return res;
    }

    __int64_t utf8_construct_char ( __int8_t shifts ) {
        __int64_t seq_sum = read_sequence( shifts, true );
        return !seq_sum ? -1 : seq_sum;
    }

    __int64_t utf8_read_char ( ) {
        __int64_t res = 0;
        __int8_t bit = read_bit( );
        if ( bit < 0 )
            return -1;

        // read standard byte - ascii compatibility basically.
        if ( bit == 0 )
            return read_sequence( );

        __uint8_t byte_count = 1;
        while ( ( bit = read_bit( ) ) ) {
            if ( bit < 0 )
                return -1;

            // UTF-8 only
            if ( ++byte_count >= 5 )
                return -1;
        }

        // undefined UTF8 or ASCII char. (10xxxxxx)
        if ( byte_count == 1 )
            return -1;

        __int32_t total_shifts = byte_count * 8;
        cout << total_shifts;

        if ( ( res = utf8_construct_char( total_shifts ) ) < 0 )
            return -1;

        return res;
    }

    __int8_t utf8_read_tree_bit ( ) {
        __int8_t bit = read_bit( );
        if ( bit < 0 )
            return -1;
        if ( bit == 1 )
            if ( ( tmp_char = utf8_read_char( ) ) < 0 )
                return -1;
        return bit;
    }

    void deserialize_tree ( node * link, __int8_t & index ) {
        if ( index == -1 )
            return;
        if ( index == 1 ) {
            link->data = tmp_char;
            link->left = link->right = nullptr;
            tmp_char = -5;
            return;
        }
        deserialize_tree( ( link->left = new node( ) ), ( index = utf8_read_tree_bit( ) ) );
        deserialize_tree( ( link->right = new node( ) ), ( index = utf8_read_tree_bit( ) ) );
    }

    __int64_t decode_character ( node * link ) {
        if ( !link->left && !link->right && link->data >= 0 ) {
            return link->data;
        }
        __int8_t bit = read_bit( );
        if ( bit < 0 )
            return -1;
        if ( !bit )
            return decode_character( link->left );
        return decode_character( link->right );
    }

    __int8_t decode_chunks ( node * root ) {
        __int8_t bit = read_bit( );
        chunk_chars_left = bit ? 4096 : read_sequence( 12 );
        if ( chunk_chars_left == 0 )
            if ( read_bit( ) != -1 )
                return -1;
        while ( chunk_chars_left > 0 ) {
            if ( ( decoded_char = decode_character( root ) ) < 0 )
                return -1;
            parsed_text += ( char ) decoded_char;
            chunk_chars_left--;
        }
        if ( bit ) decode_chunks( root );
        return 1;
    }

    string read_parsed_text ( ) {
        return parsed_text;
    }

    bool export_parsed_text ( const char * name ) {
        ofstream ofs{ name, ios::out };
        if ( !ofs )
            return false;

        if ( !( ofs << read_parsed_text( ) ) )
            return false;

        ofs.close( );
        return true;
    }
};


bool decompressFile ( const char * inFileName, const char * outFileName ) {
    ifstream ifs{ inFileName, ios::binary | ios::in };
    if ( !ifs )
        return false;

    bitstream bfs( ifs );
    btree huff;
    __int8_t index = bfs.utf8_read_tree_bit( );

    bfs.deserialize_tree( huff.get_root( ), index );

    if ( bfs.get_stream( ).eof( ) ) {
        return false;
    }
    cout << " hi ";

    if ( bfs.decode_chunks( huff.get_root( ) ) < 0 )
        return false;

    cout << bfs.read_parsed_text( );

    return bfs.export_parsed_text( outFileName );
}


bool compressFile ( const char * inFileName, const char * outFileName ) {
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}

#ifndef __PROGTEST__

bool identicalFiles ( const char * fileName1, const char * fileName2 ) {
    // todo
    return false;
}

#include "progtester/tests.cpp"
#include "extra.cpp"

int main ( void ) {

//    assert_extra( );
    assert_( );

    return 0;
}

#endif /* __PROGTEST__ */
