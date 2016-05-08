#pragma once
#include "BoolFunctionTable.h"

class CCryptoBoolFunctionAnalyzer
{
public:
    explicit CCryptoBoolFunctionAnalyzer(const CBoolFunction& function, const CBoolFunctionTable& table, const CFieldCalculationEngine& engine);
    explicit CCryptoBoolFunctionAnalyzer(const CBoolFunction& function, const CBoolFunctionTable& table, const CFieldCalculationEngine& engine, const std::string& filename);

    void ReadWalshTable(const std::string& filename);
    int GetImbalance(int coordinateFunction)const;
    int GetAlgebraicDegree(int coordinateFunction)const;
    int GetAlgebraicDegree()const;
    float GetNonLinearity(int coordinateFunction)const;
    int GetCorrelationImmunity(int coordinateFunction)const;
    int GetErrorDistributionCoefficient(int inputCoordinate, int coordinateFunction)const;
    float GetErrorDispersion(int inputCoordinate, int coordinateFunction)const;
private:
    void WriteWalshTable()const;
    void CalculateWalshTable();
    std::vector<int> CalculateCoordinateFunctionWalshTable(int coordinateFunction);
    std::vector<std::vector<int>> m_walshTable;
    std::vector<int> GetFastMobiusTable(int coordinateFunction) const;
    int GetMaxCoefIndexFromTable(const std::vector<int>&)const;
    CBoolFunctionTable m_table;
    CBoolFunction m_function;
    CFieldCalculationEngine m_engine;
};

