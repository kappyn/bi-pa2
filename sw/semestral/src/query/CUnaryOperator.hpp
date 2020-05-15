#pragma once

#include "CTableQuery.hpp"

class CUnaryOperator : public CTableQuery {
public:
};

class CSelection : public CUnaryOperator {};

class CProjection : public CUnaryOperator {};