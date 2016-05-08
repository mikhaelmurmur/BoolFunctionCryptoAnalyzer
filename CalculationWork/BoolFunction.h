#pragma once

#include "FieldCalculationEngine.h"
#include <fstream>
#include <string>

class CBoolFunction
{
public:
    explicit CBoolFunction(const CFieldCalculationEngine& engine, const std::string);
    ~CBoolFunction() {};
    CFieldElement GetFunctionValue(CFieldElement& argument);
    int GetFunctionInputSize()const;
private:
    std::vector<int> m_functionPolynomial;
    CFieldCalculationEngine m_engine;
};

