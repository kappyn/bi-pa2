#pragma once

#include "CTableQuery.hpp"

class CBinaryOperator : public CTableQuery {



};

class CNJoin : public CBinaryOperator {};

class CJoin : public CBinaryOperator {};

class CMinus : public CBinaryOperator {};

class CCartesianProduct : public CBinaryOperator {};

class CUnion : public CBinaryOperator {};

class CIntersect : public CBinaryOperator {};