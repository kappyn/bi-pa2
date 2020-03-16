#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>

struct data {
    std::string name;
    std::string email;
    std::string address;
    unsigned short age;
    bool verified;
};

class user {
private:
    data user_data;

public:
    user ( data userdata ) : user_data( std::move( userdata ) ) { }

    user & operator++ ( ); // verify user.
    user & operator-- ( ); // unverify user.

    std::string get_name ( );
    int get_age ( );

    friend std::ostream & operator<< ( std::ostream & ofs, const user & self );
};

class kvec {
private:
    std::vector < user > vdata;

public:
    void add ( const user & usr );
    void print ( ) const;
    void sort ( );
    void clear ( );
    size_t count ( );
    int find_user ( std::string name );
};