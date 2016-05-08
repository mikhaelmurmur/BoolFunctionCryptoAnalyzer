#pragma once
#include "BoolFunctionTable.h"

class CCryptoBoolFunctionAnalyzer
{
public:
    explicit CCryptoBoolFunctionAnalyzer(const CBoolFunction& function, const CBoolFunctionTable& table, const CFieldCalculationEngine& engine)
        :m_engine(engine), m_function(function), m_table(table) {};

    int GetImbalance(int coordinateFunction)const;
    int GetAlgebraicDegree(int coordinateFunction)const;
private:
    std::vector<int> GetFastMobiusTable(int coordinateFunction) const;
    int GetMaxCoefIndexFromTable(const std::vector<int>&)const;
    CBoolFunctionTable m_table;
    CBoolFunction m_function;
    CFieldCalculationEngine m_engine;
};

