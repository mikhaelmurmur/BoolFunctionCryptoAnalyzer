#include "BoolFunction.h"

CBoolFunction::CBoolFunction(const CFieldCalculationEngine & engine, const std::string fileName) :m_engine(engine)
{
    std::ifstream input(fileName);

    auto size = 0;
    input >> size;

    auto coordinate = 0;
    while (size-- > 0)
    {
        input >> coordinate;
        m_functionPolynomial.push_back(coordinate);
    }
}

CFieldElement CBoolFunction::GetFunctionValue(CFieldElement & argument)
{
    CFieldElement result;

    for (int coordinate = 0; coordinate < m_functionPolynomial.size(); ++coordinate)
    {
        if (m_functionPolynomial[coordinate] == 1)
        {
            result = m_engine.Addition(result, m_engine.Power(argument, coordinate));
        }
    }

    return result;
}

int CBoolFunction::GetFunctionInputSize() const //TODO: constexpr?
{
    static auto size = pow(2, m_engine.GetGeneratorDegree() - 1);
    return size;
}
