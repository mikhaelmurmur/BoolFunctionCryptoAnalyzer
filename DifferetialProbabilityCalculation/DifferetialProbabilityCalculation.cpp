// DifferetialProbabilityCalculation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FieldTableDiff.h"
#include <iostream>
#include <iomanip>



int main()
{
    CFieldTableDiff analyzer("boolFunction1.txt");

    std::cout<<std::setprecision(3)<<analyzer.GetMaximumDifferentialProbability();
    std::cin.get();
    std::cin.get();
    std::cin.get();
    return 0;
}

