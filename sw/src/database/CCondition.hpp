#pragma once

#include <string>

using namespace std;

class CCondition {
public:
	string m_Column;
	string m_Operator;
	string m_Constant;
	bool IsStringConstant = false;
};