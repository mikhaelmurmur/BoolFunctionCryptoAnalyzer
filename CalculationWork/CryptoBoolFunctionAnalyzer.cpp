#include "CryptoBoolFunctionAnalyzer.h"

namespace
{
    int GetOnesInNumber(int number)
    {
        auto onesNumber = 0;
        while (number > 0)
        {
            if (number % 2 == 1)
            {
                ++onesNumber;
            }
            number /= 2;
        }
        return onesNumber;
    }
    
    std::string GetBinaryRepresentation(int number)
    {
        std::string result;
        if (number == 0)
        {
            return "0";
        }

        while (number>0)
        {
            if (number % 2 == 1)
            {
                result += '1';
            }
            else
            {
                result += '0';
            }
            number /= 2;
        }
        std::reverse(result.begin(), result.end());
        return result;
    }
}

int CCryptoBoolFunctionAnalyzer::GetImbalance(int coordinateFunction) const
{
    auto size = m_function.GetFunctionInputSize();

    auto zeroNumber = 0;
    auto onesNumber = 0;

    for (int input = 0; input < size; ++input)
    {
        auto result = m_table.GetCoordinateFunctionValue((CFieldElement)GetBinaryRepresentation(input), coordinateFunction);
        if (result == 0)
        {
            ++zeroNumber;
        }
        else
        {
            ++onesNumber;
        }
    }

    return abs(zeroNumber - onesNumber);
}

int CCryptoBoolFunctionAnalyzer::GetAlgebraicDegree(int coordinateFunction) const
{
    auto fastMobiusTable = GetFastMobiusTable(coordinateFunction);

    return GetMaxCoefIndexFromTable(fastMobiusTable);
}

std::vector<int> CCryptoBoolFunctionAnalyzer::GetFastMobiusTable(int coordinateFunction) const
{
    std::vector<int> fastMobiusTable;

    auto size = m_function.GetFunctionInputSize();

    for (int input = 0; input < size; ++input)
    {
        fastMobiusTable.push_back(m_table.GetCoordinateFunctionValue((CFieldElement)GetBinaryRepresentation(input), coordinateFunction));
    }

    auto period = size / 2;

    while (period > 0)
    {
        auto startIndex = period;
        while (startIndex < size)
        {
            for (int index = startIndex; index < startIndex + period; ++index)
            {
                fastMobiusTable[index] += fastMobiusTable[index - period];
                fastMobiusTable[index] %= 2;
            }
            startIndex += 2*period;
        }
        period /= 2;
    }

    return fastMobiusTable;
}

int CCryptoBoolFunctionAnalyzer::GetMaxCoefIndexFromTable(const std::vector<int>& table) const
{
    auto size = table.size();
    auto maxOnes = 0;
    for (int entry = 0; entry < size; ++entry)
    {
        if (table[entry] == 1)
        {
            auto onesNumber = GetOnesInNumber(entry);
            if (onesNumber> maxOnes)
            {
                maxOnes = onesNumber;
            }
        }
    }
    return maxOnes;
}
