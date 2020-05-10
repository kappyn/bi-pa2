#include "CRenderSett.hpp"

const char CRenderSett::m_RowChar = '-';
const char CRenderSett::m_ColCharacter = '|';
const string CRenderSett::m_FrontPad = "  ";
const string CRenderSett::m_SpacePad = ( string( "  " ) += CRenderSett::m_ColCharacter ).append( "  " );
