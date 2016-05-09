#pragma once

#include <vector>

class CFieldElement
{
public:
    explicit CFieldElement() :m_insideRepresentation(0) {}
    explicit CFieldElement(const std::string&);
    ~CFieldElement() {}
    void SetBit(int position,int value);
    void AddBit(int position, int value);
    const auto& GetRepresentation() const { return m_insideRepresentation; }
    int GetDegree() const { return m_insideRepresentation.size(); }
    void LeftShift(int position);
    void Reduce();
    operator std::string()const;
    operator int()const;
    int GetWeight()const;
private:
    std::vector<int> m_insideRepresentation;
    int m_representation=0;
};

