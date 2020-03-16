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

#include <bitset>


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

    __int64_t buffer = 0;
    __int64_t tmp_char = -5;
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

            if ( bit < 0 ) {
                return -1;
            }
            if ( print ) cout << bit;
        }
        if ( print ) cout << " ";

        if ( res > utf8_upper_bound )
            return -1;

        return res;
    }

    bool read_sequence_custom_shift (
            __uint8_t & bits,
            __int16_t & curr_shift,
            __int64_t & res,
            bool utf8_check
    ) {
        __int8_t bit = 0;

        if ( !utf8_check ) {
            for ( ; bits != 0; bits-- ) {
//                cout << (int) curr_shift << endl;
                if ( ( bit = read_bit( ) ) )
                    res += ( 1 << curr_shift );
                if ( bit < 0 ) {
                    return false;
                }
                curr_shift--;
            }
        } else {
            for ( __int8_t i = 7; i >= 0; i-- ) {
//                cout << ( int ) i << endl;
                if ( ( bit = read_bit( ) ) )
                    res += ( 1 << curr_shift );
                if ( bit < 0 || ( i == 7 && bit != 1 ) || ( i == 6 && bit != 0 ) )
                    return false;
                if ( res >= utf8_upper_bound )
                    return false;
                curr_shift--;
            }
//            cout << endl;
        }

        return true;
    }

    __int64_t generate_character ( ) {
        __int8_t bit = read_bit( );
        if ( bit < 0 )
            return -1;
        if ( bit == 0 )
            return read_sequence( 7 );

        __int16_t bytes_total = 1;
        while ( ( bit = read_bit( ) ) == 1 ) {
            if ( bit < 0 || ++bytes_total >= 5 ) {
                return -1;
            }
        }

        if ( bytes_total == 1 )
            return -1;

        __int64_t res = 0;
        __int16_t total_shifts = ( bytes_total * 8 ) - 1;
        __uint8_t first_byte_remainders = 8 - ( bytes_total + 1 );
        __int8_t index = 0;

        while ( ++index <= bytes_total )
            res += ( 1 << ( total_shifts-- ) );
        total_shifts--; // 111 -0- xxxx

        if ( !read_sequence_custom_shift( first_byte_remainders, total_shifts, res, false ) )
            return -1;

        bytes_total--;
        for ( int i = 0; i < bytes_total; ++i ) {
            if ( !read_sequence_custom_shift( first_byte_remainders, total_shifts, res, true ) )
                return -1;
        }
        return res;
    }

    __int8_t read_tree_bit ( ) {
        __int8_t bit = read_bit( );
        if ( bit < 0 )
            return -1;
        if ( bit == 1 ) {
            if ( ( tmp_char = generate_character( ) ) < 0 )
                return -1;
        }
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
        deserialize_tree( ( link->left = new node( ) ), ( index = read_tree_bit( ) ) );
        deserialize_tree( ( link->right = new node( ) ), ( index = read_tree_bit( ) ) );
    }


    __int64_t decode_character ( node * link ) {
//        cout << link->data;

        if ( !link->left && !link->right && link->data >= 0 ) {
            cout << std::bitset < 16 >( link->data );
            cout << "\n";
//            cout << (int) link->data << " ";
            return link->data;
        }
        __int8_t bit = read_bit( );
        if ( bit < 0 )
            return -1;
        if ( !bit )
            return decode_character( link->left );
        return decode_character( link->right );
    }

    __int8_t decode_chunks ( node * root, ofstream & ofs ) {
        // This section still throws segfaults somehow for the UTF8 characters. (ASCII is working flawlessly)
        // Maybe one day I'll come back and take a look at this.

        __int8_t bit = read_bit( );

        if ( bit < 0 )
            return -1;

        if ( bit ) {
            chunk_chars_left = 4096;
        } else {
            __int32_t seq = read_sequence( 12 );
            if ( seq < 0 )
                return -1;
            chunk_chars_left = seq;
            return -1;
        }

        while ( chunk_chars_left > 0 ) {
            if ( ( decoded_char = decode_character( root ) ) < 0 )
                return -1;

//            if ( chunk_chars_left == 4000 )
//                return -1;

            if ( !( ofs.write( ( char const * ) decoded_char, 8 ) ) )
                return -1;

            chunk_chars_left--;
        }

        if ( bit )
            decode_chunks( root, ofs );

        return 1;
    }

    string read_parsed_text ( ) {
        return parsed_text;
    }

    bool export_parsed_text ( const char * name, node * root ) {
        ofstream ofs{ name, ios::out };
        if ( !ofs )
            return false;

        if ( decode_chunks( root, ofs ) < 0 )
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
    __int8_t index = bfs.read_tree_bit( );

    if ( index < 0 )
        return false;

    bfs.deserialize_tree( huff.get_root( ), index );
//    huff.print_nodes( huff.get_root( ) );

    if ( bfs.get_stream( ).eof( ) ) {
        return false;
    }

    return bfs.export_parsed_text( outFileName, huff.get_root( ) );
}


bool compressFile ( const char * inFileName, const char * outFileName ) {
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}

#ifndef __PROGTEST__

template < typename InputIterator1, typename InputIterator2 >
bool
range_equal ( InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2 ) {
    while ( first1 != last1 && first2 != last2 ) {
        if ( * first1 != * first2 ) return false;
        ++first1;
        ++first2;
    }
    return ( first1 == last1 ) && ( first2 == last2 );
}

bool identicalFiles ( const char * fileName1, const char * fileName2 ) {
    std::ifstream file1( fileName1 );
    std::ifstream file2( fileName2 );
    std::istreambuf_iterator < char > begin1( file1 );
    std::istreambuf_iterator < char > begin2( file2 );
    std::istreambuf_iterator < char > end;

#endif /* __PROGTEST__ */
    return range_equal( begin1, end, begin2, end );
}

#include "progtester/tests.cpp"

int main ( void ) {

    decompressFile( "tests/extra0.huf", "tempfile" );
//    assert_( );

    return 0;
}