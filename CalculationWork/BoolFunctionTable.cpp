#include "BoolFunctionTable.h"

namespace
{
    CFieldElement ConvertNumberToFieldElement(int number)
    {
        CFieldElement result;
        result.SetBit(0, 0);
        for (int coordinate = 0; number > 0; ++coordinate, number /= 2)
        {
            result.SetBit(coordinate, number % 2);
        }

        return result;
    }

    int ConvertFieldElementToNumber(const CFieldElement& argument)
    {
        auto number = 0;
        auto multiplicator = 1;
        for (auto& coordinate : argument.GetRepresentation())
        {
            if (coordinate == 1)
            {
                number += multiplicator;
            }
            multiplicator *= 2;
        }
        return number;
    }
}

int CBoolFunctionTable::ms_numberOfBoolFunctions;

CBoolFunctionTable::CBoolFunctionTable(const CBoolFunction & function):m_function(function) 
{
    ComputeTable();
    ++ms_numberOfBoolFunctions;
    WriteTableToFile("boolFunction" + std::to_string(ms_numberOfBoolFunctions) + ".txt");
}

CBoolFunctionTable::CBoolFunctionTable(const CBoolFunction & function, const std::string & filename) : m_function(function)
{
    ReadTableFromFile(filename);
    ++ms_numberOfBoolFunctions;
}

const CFieldElement & CBoolFunctionTable::GetFunctionValue(const CFieldElement & argument) const
{
    return m_table[ConvertFieldElementToNumber(argument)];
}

int CBoolFunctionTable::GetCoordinateFunctionValue(const CFieldElement & argument, int coordinate) const
{
    auto result = GetFunctionValue(argument).GetRepresentation();
    if (result.size() > coordinate)
    {
        return result[coordinate];
    }
    return 0;
}

void CBoolFunctionTable::ComputeTable()
{
    auto size = m_function.GetFunctionInputSize();

    for (int inputNumber = 0; inputNumber < size; ++inputNumber)
    {
        auto argument = ConvertNumberToFieldElement(inputNumber);

        auto result = m_function.GetFunctionValue(argument);

        m_table.push_back(result);
    }
}

void CBoolFunctionTable::ReadTableFromFile(const std::string & filename)
{
    LOG(INFO) << "Reading bool function from file: " << filename;

    std::ifstream input(filename);

    int size = 0;
    input >> size;

    auto index = 0;

    std::string element = "";

    while (size-- > 0)
    {
        input >> index;
        input >> element;
        m_table.push_back(CFieldElement(element));
    }
}

void CBoolFunctionTable::WriteTableToFile(const std::string & filename)
{
    LOG(INFO) << "BOOL FUNCTION FILENAME: " + filename;

    std::ofstream outputFile(filename);

    auto entryNumber = m_table.size();
    outputFile << entryNumber << std::endl;

    for (int entryIndex = 0; entryIndex < entryNumber; ++entryIndex)
    {
        outputFile << entryIndex << " " << (std::string)m_table[entryIndex] << std::endl;
    }
}
