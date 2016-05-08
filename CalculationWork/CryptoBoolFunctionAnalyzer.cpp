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

        while (number > 0)
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

    int GetMaximum(const std::vector<int>& data)
    {
        auto result = abs(data[0]);
        for (auto& value : data)
        {
            if (abs(value) > result)
            {
                result = abs(value);
            }
        }
        return result;
    }
}

CCryptoBoolFunctionAnalyzer::CCryptoBoolFunctionAnalyzer(const CBoolFunction & function, const CBoolFunctionTable & table, const CFieldCalculationEngine & engine)
    :m_engine(engine), m_function(function), m_table(table)
{
    CalculateWalshTable();
    WriteWalshTable();
}

CCryptoBoolFunctionAnalyzer::CCryptoBoolFunctionAnalyzer(const CBoolFunction & function, const CBoolFunctionTable & table, const CFieldCalculationEngine & engine, const std::string & filename)
    :m_engine(engine), m_function(function), m_table(table)
{
    ReadWalshTable(filename);
}

void CCryptoBoolFunctionAnalyzer::ReadWalshTable(const std::string & filename)
{
    std::ifstream input(filename);

    auto size = 0;
    input >> size;


    while (size-- > 0)
    {
        std::vector<int> entry;
        auto entrySize = 0;
        input >> entrySize;
        auto tmp = 0;
        for (int valueIndex = 0; valueIndex < entrySize; ++valueIndex)
        {
            input >> tmp;
            entry.push_back(tmp);
        }
        m_walshTable.push_back(std::move(entry));
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

float CCryptoBoolFunctionAnalyzer::GetNonLinearity(int coordinateFunction) const
{
    auto nonLinearity = pow(2, m_engine.GetGeneratorDegree() - 2) - GetMaximum(m_walshTable[coordinateFunction]) / 2.;
    return nonLinearity;
}

int CCryptoBoolFunctionAnalyzer::GetCorrelationImmunity(int coordinateFunction) const
{
    auto data = m_walshTable[coordinateFunction];

    auto size = data.size();

    std::vector<bool> correlationImmunitty(m_engine.GetGeneratorDegree());
    for (auto& index : correlationImmunitty)
    {
        index = true;
    }

    for (int index = 0; index < size; ++index)
    {
        if (data[index] != 0)
        {
            auto ones = GetOnesInNumber(index);
            correlationImmunitty[ones] = false;
        }
    }

    for (int index = correlationImmunitty.size() - 1; index > -1; --index)
    {
        if (correlationImmunitty[index])
        {
            return index;
        }
    }
    return 0;
}

int CCryptoBoolFunctionAnalyzer::GetErrorDistributionCoefficient(int inputCoordinate, int coordinateFunction) const
{
    auto size = m_function.GetFunctionInputSize();

    auto errorCoef = 0;

    for (int input = 0; input < size; ++input)
    {
        auto shiftedValue = (CFieldElement)GetBinaryRepresentation(input);
        shiftedValue.AddBit(inputCoordinate, 1);
        errorCoef += (m_table.GetCoordinateFunctionValue((CFieldElement)GetBinaryRepresentation(input), coordinateFunction)
            + m_table.GetCoordinateFunctionValue(shiftedValue, coordinateFunction))%2;
    }

    return errorCoef;
}

float CCryptoBoolFunctionAnalyzer::GetErrorDispersion(int inputCoordinate, int coordinateFunction) const
{
    auto errorCoef = GetErrorDistributionCoefficient(inputCoordinate, coordinateFunction);
    return abs(errorCoef - pow(2, m_engine.GetGeneratorDegree() - 2)) / pow(2, m_engine.GetGeneratorDegree() - 2);
}

void CCryptoBoolFunctionAnalyzer::WriteWalshTable() const
{
    static int fileNumber = 0;
    ++fileNumber;
    LOG(INFO) << "Writing walsh table to file: walshTable" << fileNumber << ".txt";

    std::ofstream output("walshTable" + std::to_string(fileNumber) + ".txt");

    output << m_walshTable.size() << std::endl;

    for (auto& entry : m_walshTable)
    {
        output << entry.size() << std::endl;
        for (auto& value : entry)
        {
            output << value << std::endl;
        }
    }
}

void CCryptoBoolFunctionAnalyzer::CalculateWalshTable()
{
    auto size = m_function.GetFunctionInputSize();

    for (int coordinate = 0; coordinate < size; ++coordinate)
    {
        m_walshTable.push_back(CalculateCoordinateFunctionWalshTable(coordinate));
    }
}

std::vector<int> CCryptoBoolFunctionAnalyzer::CalculateCoordinateFunctionWalshTable(int coordinateFunction)
{
    std::vector<int> walshTable;

    auto size = m_function.GetFunctionInputSize();

    for (int input = 0; input < size; ++input)
    {
        auto functionValue = m_table.GetCoordinateFunctionValue((CFieldElement)GetBinaryRepresentation(input), coordinateFunction);
        if (functionValue == 1)
        {
            walshTable.push_back(-1);
        }
        else
        {
            walshTable.push_back(1);
        }
    }

    auto period = size / 2;

    while (period > 0)
    {
        auto startIndex = period;
        while (startIndex < size)
        {
            for (int index = startIndex; index < startIndex + period; ++index)
            {
                auto upperNewValue = 0, bottomNewValue = 0;
                upperNewValue = walshTable[index] + walshTable[index - period];
                bottomNewValue = walshTable[index - period] - walshTable[index];
                walshTable[index] = bottomNewValue;
                walshTable[index - period] = upperNewValue;
            }
            startIndex += 2 * period;
        }
        period /= 2;
    }

    return walshTable;
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
            startIndex += 2 * period;
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
            if (onesNumber > maxOnes)
            {
                maxOnes = onesNumber;
            }
        }
    }
    return maxOnes;
}
