#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdint>

using namespace std;

const char * OUTPUT_FILENAME = "points.bin";

struct point_t {
	float x, y;
};

bool points_compare ( const point_t & lhs, const point_t & rhs )
{
	if ( lhs.x != rhs.x )
		return lhs.x < rhs.x;
	return lhs.y < rhs.y;
}

point_t * points_load ( int points_num )
{
	point_t * points = new point_t [ points_num ];

	for ( int i = 0; i < points_num; ++i ) {
		char lb, cm, rb;
		if ( !( cin >> lb >> points[ i ].x >> cm >> points[ i ].y >> rb ) || lb != '[' || cm != ',' || rb != ']' ) {
			cerr << "'" << lb << cm << rb << "'" << endl;
			delete [] points;
			points = nullptr;
			break;
		}
	}

	return points;
}

// Místo "ofstream" muzeme pouzit i "ostream", duvod si vysvetlime u dedicnosti.
bool points_write ( int points_num, point_t * points, ostream & fout )
{
	uint32_t num32 = points_num;
	if ( (int)num32 != points_num ) {
		cerr << "Nekonzistence pretypovani." << endl;
		return false;
	}

	// Ulozime pocet nactenych prvku
	if ( !fout.write( (const char *)&num32, sizeof( num32 ) ) ) {
		cerr << "Zapis poctu prvku do souboru se nepodaril." << endl;
		return false;
	}

	num32 = 0;
	// Vlozime do souboru zatim prazdne misto pro budouci "kontrolni" hash
	if ( !fout.write( (const char *)&num32, sizeof( num32 ) ) ) {
		cerr << "Zapis hashe (0) do souboru se nepodaril." << endl;
		return false;
	}

	for ( int i = 0; i < points_num; ++i ) {
		// Ulozime postupne do souboru jednotlive body
		if ( !fout.write( (const char *)( points + i ), sizeof( points[ i ] ) ) ) {
			cerr << "Zapis bodu do souboru se nepodaril." << endl;
			return false;
		}

		// Pro kazdy bod pocitame hash z aktualni hashe jako XOR bytu a posun
		for ( size_t j = 0; j < sizeof( points[ i ] ); ++j ) {
			num32 <<= 1;
			num32 ^= ((const unsigned char *)( points + i ))[ j ];
		}
	}

	// Vratime se na rezervovane misto 4 byty od zacatku
	fout.seekp( sizeof( num32 ) );
	// Zapiseme misto 0 skutecnou hash
	if ( !fout.write( (const char *)&num32, sizeof( num32 ) ) ) {
		cerr << "Zapis hashe (" << num32 << ") do souboru se nepodaril." << endl;
		return false;
	}

	return true;
}

int main ()
{
	int points_num;
	cout << "Zadejte pocet bodu k zapsani:" << endl;
	if ( !( cin >> points_num ) || points_num <= 0 ) {
		cout << "Nespravny vstup." << endl;
		return 1;
	}

	cout << "Zadejte " << points_num << " bodu ve formatu [x, y]:" << endl;
	point_t * points = points_load( points_num );
	if ( points == nullptr ) {
		cout << "Nespravny vstup." << endl;
		return 2;
	}

	sort( points, points + points_num, points_compare );

	ofstream fout( OUTPUT_FILENAME, ios::binary );
	if ( !fout.is_open() || !fout ) {
		cout << "Nepodarilo se otevrit soubor." << endl;
		delete [] points;
		return 3;
	}

	if ( !points_write( points_num, points, fout ) ) {
		cout << "Neco se nepovedlo." << endl;
		delete [] points;
		return 4;
	}
	
	cout << "Uspesne ulozeno do souboru [" << OUTPUT_FILENAME << "]." << endl;
	delete [] points;
	return 0;
}
