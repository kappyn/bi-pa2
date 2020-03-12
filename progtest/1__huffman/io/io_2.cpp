#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const char * INVALID_INPUT = "Wrong input.";
const char * WRITE_ERROR = "Error has occurred during write operation.";
const int SEQ_LEN = 22;
const int FNAME_LEN = 20;

class fl {
public:
    static bool write_binary ( char * file, char * seq ) {
        ofstream ofs { file, ios::binary };
        if ( ! ofs )
            return false;

        return !! ofs.write( seq, SEQ_LEN );
    }

    static char * read_binary ( string & file ) {
        ifstream ifs { file, ios::binary };

        if ( ! ifs )
            return nullptr;

        // find length of the loaded text.
        ifs.seekg( 0, std::ifstream::end );
        int text_len = ifs.tellg( );
        ifs.seekg( 0, std::ifstream::beg );

        // save text to the buffer
        char * buffer = new char [ text_len ];
        ifs.read( buffer, text_len );

        if ( ! ifs ) {
            delete [ ] buffer;
            return nullptr;
        }

        ifs.close( );
        return buffer;
    }
};

int main ( ) {
    char sequence[ SEQ_LEN ] = { 0 }, filename[ FNAME_LEN ] = { 0 };

    cout << "Enter a filename: \n";
    if ( ! ( cin.getline( filename, FNAME_LEN ) ) || ( strlen( filename ) < 10 ) ) {
        cout << INVALID_INPUT << endl;
        return 1;
    }
    cout << "Enter a word sequence: \n";
    if ( ! ( cin.getline( sequence, SEQ_LEN ) ) ) {
        cout << INVALID_INPUT << endl;
        return 1;
    }

    if ( ! fl::write_binary( filename, sequence ) ) {
        cout << WRITE_ERROR << endl;
        return 1;
    }

    cout << "File successfully written to " << filename << "." << endl;

    return 0;
}
