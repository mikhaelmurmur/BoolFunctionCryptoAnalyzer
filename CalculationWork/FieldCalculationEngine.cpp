#include "FieldCalculationEngine.h"


namespace 
{
    std::vector<int> GetBinaryRepresentation(int number)
    {
        std::vector<int> result;
        while (number > 0)
        {
            result.push_back(number % 2);
            number /= 2;
        }
        return result;
    }
}

CFieldElement CFieldCalculationEngine::Addition(CFieldElement & first,CFieldElement & second) const
{
    CFieldElement result;

    auto& firstRepresentation = first.GetRepresentation();
    auto& secondRepresentation = second.GetRepresentation();

    auto size = firstRepresentation.size() > secondRepresentation.size() ?
        firstRepresentation.size() : secondRepresentation.size();

    if (firstRepresentation.size() < size)
    {
        first.SetBit(size - 1, 0);
    }

    if (secondRepresentation.size() < size)
    {
        second.SetBit(size - 1, 0);
    }

    result.SetBit(size - 1, 0);

    for (int coordinate = 0; coordinate < size; ++coordinate)
    {
        result.SetBit(coordinate, (firstRepresentation[coordinate] + secondRepresentation[coordinate]) % 2);
    }

    result.Reduce();
    return result;
}

CFieldElement CFieldCalculationEngine::Multiplication(CFieldElement & first, CFieldElement & second)
{
    auto secondRepresentation = second.GetRepresentation();

    CFieldElement result;

    for (int coordinate = 0; coordinate < secondRepresentation.size();++coordinate)
    {
        if (secondRepresentation[coordinate] == 1)
        {
            auto tmp(first);
            tmp.LeftShift(coordinate);
            result = Addition(result, tmp);
        }
    }

    result.Reduce();
    return Modulo(result);
}

CFieldElement CFieldCalculationEngine::Power(CFieldElement & element, int power)
{
    auto binaryRepresentation = GetBinaryRepresentation(power);

    CFieldElement result;
    result.SetBit(0, 1);
    
    CFieldElement multiplier(element);

    for (auto& coordinate : binaryRepresentation)
    {
        if (coordinate == 1)
        {
            result = Multiplication(result, multiplier);
        }
        multiplier = Square(multiplier);
    }
    
    if (result.GetRepresentation().size() == 0)
    {
        result.SetBit(0, 0);
    }

    return result;
}

CFieldElement CFieldCalculationEngine::Modulo(const CFieldElement & element)
{
    auto sizeElement = element.GetDegree();
    auto sizeGenerator = m_generator.GetDegree();
    auto& representation = element.GetRepresentation();

    auto result(element);

    while (sizeElement >= sizeGenerator)
    {
        auto currentSize = element.GetDegree();
        auto shiftedGenerator(m_generator);
        shiftedGenerator.LeftShift(sizeElement - sizeGenerator);
        result = Addition(result, shiftedGenerator);
        result.Reduce();
        sizeElement = result.GetDegree();
    }

    return result;
}
