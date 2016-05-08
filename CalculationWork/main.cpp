#include <iostream>
#include "easylogging++.h"
#include "main.h"
#include "CryptoBoolFunctionAnalyzer.h"

void RunAllTests();
INITIALIZE_EASYLOGGINGPP

int main()
{
    SetUpLogger();
    RunAllTests();
    return 0;
}

void SetUpLogger()
{
    el::Configurations logConfig("loggerConfig.ini");
    el::Loggers::reconfigureLogger("default", logConfig);

}

bool Test1()
{
    CFieldElement element;
    CFieldCalculationEngine engine(element);

    CFieldElement first;
    CFieldElement second;

    first.SetBit(3, 1);
    second.SetBit(3, 1);
    second.SetBit(5, 1);

    auto result = engine.Addition(first, second);
    
    auto resultRepresentation = result.GetRepresentation();

    for (auto& coordinate : resultRepresentation)
    {
        std::cout << coordinate;
    }
    std::cout << std::endl;

    return true;
}

bool Test2()
{
    CFieldElement generator;
    generator.SetBit(16, 1);
    generator.SetBit(2, 1);
    generator.SetBit(0, 1);
    CFieldCalculationEngine engine(generator);

    CFieldElement element;
    element.SetBit(12, 1);
    auto result = engine.Modulo(element);


    auto resultRepresentation = result.GetRepresentation();

    for (auto& coordinate : resultRepresentation)
    {
        std::cout << coordinate;
    }
    std::cout << std::endl;
    return true;
}

bool Test3()
{
    CFieldElement generator;
    generator.SetBit(3, 1);
    generator.SetBit(1, 1);
    generator.SetBit(0, 1);
    CFieldCalculationEngine engine(generator);

    CFieldElement firstElement;
    firstElement.SetBit(1, 1);
    firstElement.SetBit(0, 1);

    CFieldElement secondElement;
    secondElement.SetBit(2, 1);
    secondElement.SetBit(0, 1);

    auto result = engine.Multiplication(firstElement, secondElement);

    auto resultRepresentation = result.GetRepresentation();

    for (auto& coordinate : resultRepresentation)
    {
        std::cout << coordinate;
    }
    std::cout << std::endl;
    return true;
}

bool Test4()
{
    CFieldElement generator;
    generator.SetBit(3, 1);
    generator.SetBit(1, 1);
    generator.SetBit(0, 1);
    CFieldCalculationEngine engine(generator);


    CFieldElement element;
    element.SetBit(2, 1);
    element.SetBit(0, 1);

    auto result = engine.Power(element, 3);

    auto straightResult = engine.Multiplication(element, engine.Multiplication(element, element));

    auto resultRepresentation = result.GetRepresentation();

    for (auto& coordinate : resultRepresentation)
    {
        std::cout << coordinate;
    }
    std::cout << std::endl;

    auto straightResultRepresentation = straightResult.GetRepresentation();

    for (auto& coordinate : straightResultRepresentation)
    {
        std::cout << coordinate;
    }
    std::cout << std::endl;



    return true;
}

bool Test5()
{
    CFieldElement generator;
    generator.SetBit(17, 1);
    generator.SetBit(3, 1);
    generator.SetBit(0, 1);

    CFieldCalculationEngine engine(generator);

    CBoolFunction function(engine, "polynomialFirst.txt");

    CFieldElement element;
    element.SetBit(0, 1);

    auto result = function.GetFunctionValue(element);

    auto resultRepresentation = result.GetRepresentation();

    for (auto& coordinate : resultRepresentation)
    {
        std::cout << coordinate;
    }
    std::cout << std::endl;

    return true;
}

bool Test6()
{
    CFieldElement generator;
    generator.SetBit(17, 1);
    generator.SetBit(3, 1);
    generator.SetBit(0, 1);

    CFieldCalculationEngine engine(generator);

    CBoolFunction function(engine, "polynomialFirst.txt");

    CBoolFunctionTable table(function);

    return true;
}

bool Test7()
{
    CFieldElement generator;
    generator.SetBit(17, 1);
    generator.SetBit(3, 1);
    generator.SetBit(0, 1);

    CFieldCalculationEngine engine(generator);

    CBoolFunction function(engine, "polynomialFirst.txt");

    CFieldElement element;
    element.SetBit(0, 1);

    auto result = function.GetFunctionValue(element);

    auto resultRepresentation = result.GetRepresentation();

    std::cout << (std::string)generator;
    std::cout << std::endl;

    return true;
}

bool Test8() 
{
    CFieldElement generator;
    generator.SetBit(17, 1);
    generator.SetBit(3, 1);
    generator.SetBit(0, 1);

    CFieldCalculationEngine engine(generator);

    CBoolFunction function(engine, "polynomialFirst.txt");

    CBoolFunctionTable table(function, "boolFunction1.txt");

    return true;
}

bool Test9()
{
    CFieldElement generator;
    generator.SetBit(17, 1);
    generator.SetBit(3, 1);
    generator.SetBit(0, 1);

    CFieldCalculationEngine engine(generator);

    CBoolFunction function(engine, "polynomialFirst.txt");

    CBoolFunctionTable table(function, "boolFunction1.txt");

    CCryptoBoolFunctionAnalyzer analyzer(function, table, engine);
   
    std::ofstream imbalanceInfo("imbalanceinfo1.txt");

    for (int coordinate = 0; coordinate < generator.GetDegree(); ++coordinate)
    {
        imbalanceInfo << coordinate << " " << analyzer.GetImbalance(coordinate) << std::endl;
    }

    return true;
}

bool Test10()
{
    CFieldElement generator;
    generator.SetBit(17, 1);
    generator.SetBit(3, 1);
    generator.SetBit(0, 1);

    CFieldCalculationEngine engine(generator);

    CBoolFunction function(engine, "polynomialFirst.txt");

    CBoolFunctionTable table(function, "boolFunction1.txt");

    CCryptoBoolFunctionAnalyzer analyzer(function, table, engine);

    std::ofstream algebraicDegree("algebraicDegreeinfo1.txt");

    for (int coordinate = 0; coordinate < generator.GetDegree(); ++coordinate)
    {
        algebraicDegree << coordinate << " " << analyzer.GetAlgebraicDegree(coordinate) << std::endl;
    }

    return true;
}

void RunAllTests()
{
    Test9();
    Test10();
}


