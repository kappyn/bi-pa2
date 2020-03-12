#include <iostream>
#include <fstream>
#include <string>

const char * INVALID_WRITE = "An error has occurred during file write.";
const char * INVALID_READ = "An error has occurred during the file reading.";

using namespace std;

class File {
public:
    static bool write_binary ( string file, string text ) {
        ofstream ofs ( file, ios::binary );
        if ( ! ofs )
            return false;

        int text_len = text.length( );
        int * chars = new int[ text_len ];

        cout << "Pre-shifted:" << endl;
        for ( int j = 0; j < text_len; ++j ) {
            cout << +text[ j ] << " ";
        }

        cout << "\nPost-shifted:" << endl;
        for ( int i = 0; i < text_len; ++i ) {
            chars[ i ] = text[ i ] + 2;
            cout << +chars[ i ] << " ";
            if ( ! ofs.write( reinterpret_cast< const char * >( & chars[ i ] ), sizeof( char ) ) )
                return false;
        }

        cout << endl;
        delete [ ] chars;
        ofs.close( );
        return true;
    }

    static bool read_binary ( string file ) {
        ifstream ifs ( file, ios::binary );

        if ( ! ifs )
            return false;

        // Find the input file length.
        ifs.seekg( 0, std::ifstream::end );
        int text_len = ifs.tellg( );
        ifs.seekg( 0, std::ifstream::beg );
        char * text = new char[ text_len + 1 ];

        ifs.read( text, text_len );
        text[ text_len ] = '\0';

        for ( int i = 0; i < text_len; ++i ) {
            text[ i ] -= 2;
            cout << text[ i ] << endl;
        }

        delete [ ] text;
        ifs.close( );
        return true;
    }
};

int main ( ) {
    string file = "a.bin";
    string text = "Kevin";

    if ( ! File::write_binary( file, text ) ) {
        cout << INVALID_WRITE << endl;
        return 1;
    }

    if ( ! File::read_binary( file ) ) {
        cout << INVALID_READ << endl;
        return 1;
    }

    return 0;
}