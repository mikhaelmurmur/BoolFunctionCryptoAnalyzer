#include "FieldElement.h"

CFieldElement::CFieldElement(const std::string & inputElement)
{
    int coordinatePosition = 0;
    for (auto coordinate = inputElement.rbegin(); coordinate != inputElement.rend();++coordinate , ++coordinatePosition)
    {
        if (*coordinate=='1')
        {
            SetBit(coordinatePosition, 1);
        }
    }
}

void CFieldElement::SetBit(int position, int value)
{
    auto size = m_insideRepresentation.size();
    while (size < position + 1)
    {
        m_insideRepresentation.push_back(0);
        ++size;
    }

    m_insideRepresentation[position] = value;
}

void CFieldElement::AddBit(int position, int value)
{
    if (m_insideRepresentation.size() <= position)
    {
        SetBit(position, 0);
    }
    m_insideRepresentation[position] += value;
    m_insideRepresentation[position] %= 2;
}

void CFieldElement::LeftShift(int position)
{
    decltype(m_insideRepresentation) newInsideRepresentation(m_insideRepresentation.size() + position);

    for (int coordinate = 0; coordinate < m_insideRepresentation.size(); ++coordinate)
    {
        newInsideRepresentation[position + coordinate] = m_insideRepresentation[coordinate];
    }

    m_insideRepresentation = newInsideRepresentation;
}

void CFieldElement::Reduce()
{
    while (m_insideRepresentation.size() > 0 && m_insideRepresentation[m_insideRepresentation.size() - 1] == 0)
    {
        m_insideRepresentation.pop_back();
    }
}

CFieldElement::operator std::string() const
{
    std::string result;

    for (int index = m_insideRepresentation.size() - 1; index > -1; --index)
    {
        if (m_insideRepresentation[index] == 1)
        {
            result += '1';
        }
        else
        {
            result += '0';
        }
    }
    if (result.size() == 0)
    {
        result += '0';
    }
    return result;
}

int CFieldElement::GetWeight() const
{
    auto result = 0;
    for (auto& coordinate : m_insideRepresentation)
    {
        if (coordinate == 1)
        {
            ++result;
        }
    }
    return result;
}
