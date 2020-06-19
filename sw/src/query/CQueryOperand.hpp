#include "../database/CTable.hpp"
#include "CTableQuery.hpp"

class CQueryOperand {
public:
	CTable * m_TRef          = nullptr;
	CTableQuery * m_QRef     = nullptr;
	CTableQuery * m_Origin   = nullptr;
};