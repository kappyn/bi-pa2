#pragma once

#include <string>

using namespace std;

/**
 * This module stores the rendering settings for the database tables.
 * Each application instance may only have one since the members are all static.
 */
class CRenderSett {
public:
	static const char m_RowChar;
	static const char m_ColCharacter;
	static const string m_FrontPad;
	static const string m_SpacePad;
};