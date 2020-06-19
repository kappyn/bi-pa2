#include "../database/CTable.hpp"
#include "CTableQuery.hpp"

class CQueryOperand {
public:
	CTable * m_TableRef      = nullptr;
	CTableQuery * m_QueryRef = nullptr;
	CTableQuery * m_Origin   = nullptr;
};