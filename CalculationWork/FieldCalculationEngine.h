#pragma once

#include "FieldElement.h"

class CFieldCalculationEngine
{
public:
    explicit CFieldCalculationEngine(CFieldElement generator) :m_generator(generator) {};
    ~CFieldCalculationEngine() {};
    CFieldElement Addition( CFieldElement& first,  CFieldElement& second);
    CFieldElement Multiplication(CFieldElement& first, CFieldElement& second);
    CFieldElement Square(CFieldElement& element) { return Multiplication(element, element); };
    CFieldElement Power(CFieldElement& element, int power);
    CFieldElement Modulo(const CFieldElement& element);
    int GetGeneratorDegree()const { return m_generator.GetRepresentation().size(); };
private:
    CFieldElement m_generator;
};

