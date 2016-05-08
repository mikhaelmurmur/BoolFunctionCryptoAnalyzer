#pragma once
#include "BoolFunction.h"
#include "easylogging++.h"

class CBoolFunctionTable
{
public:
    explicit CBoolFunctionTable(const CBoolFunction& function);
    explicit CBoolFunctionTable(const CBoolFunction& function, const std::string& filename);
    const CFieldElement& GetFunctionValue(const CFieldElement& argument) const;
    int GetCoordinateFunctionValue(const CFieldElement& argument, int coordinate)const;
    ~CBoolFunctionTable() {};
private:
    void ComputeTable();
    void ReadTableFromFile(const std::string& filename);
    void WriteTableToFile(const std::string& filename);
    static int ms_numberOfBoolFunctions;
    CBoolFunction m_function;
    std::vector<CFieldElement> m_table;
};

