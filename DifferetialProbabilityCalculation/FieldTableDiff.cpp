#include "FieldTableDiff.h"
#include <fstream>
#include <map>

CFieldTableDiff::CFieldTableDiff(const std::string& filename)
{
    std::ifstream input(filename);

    auto size = 0;
    input >> size;
    auto tmp = 0;
    while (size-- > 0)
    {
        std::string representation;
        input >> tmp >> representation;
        std::vector<bool> entry;
        for (auto it = representation.rbegin(); it != representation.rend(); ++it)
        {
            if (*it == '1')
            {
                entry.push_back(true);
            }
            else
            {
                entry.push_back(false);
            }
        }
        m_insideRepresentation.push_back(entry);
    }
}

int CFieldTableDiff::GetDifferencialValue(int argument, int direction) const
{
    int trueDirection = 0;
    {
        int tmp_arg = argument;
        int tmp_dir = direction;
        auto multiplier = 1;

        while (tmp_arg > 0 || tmp_dir > 0)
        {
            auto dir_rest = tmp_dir % 2;
            auto arg_rest = tmp_arg % 2;

            if ((dir_rest + arg_rest) % 2 == 1)
            {
                trueDirection += multiplier;
            }
            multiplier *= 2;
            tmp_arg /= 2;
            tmp_dir /= 2;
        }
    }

    auto arg_functionValue = m_insideRepresentation[argument];
    auto dir_functionValue = m_insideRepresentation[trueDirection];

    auto result = 0;
    auto multiplier = 1;
    auto it_arg = arg_functionValue.begin();
    auto it_dir = dir_functionValue.begin();
    for (;
    it_arg != arg_functionValue.end() && it_dir != dir_functionValue.end();
        ++it_arg, ++it_dir)
    {
        if (*it_arg^*it_dir)
        {
            result += multiplier;
        }
        multiplier *= 2;
    }

    if (it_arg == arg_functionValue.end())
    {
        while (it_dir != dir_functionValue.end())
        {
            if (*it_dir)
            {
                result += multiplier;
            }
            multiplier *= 2;
            ++it_dir;
        }
    }
    else if (it_dir == dir_functionValue.end())
    {
        while (it_arg != arg_functionValue.end())
        {
            if (*it_arg)
            {
                result += multiplier;
            }
            multiplier *= 2;
            ++it_arg;
        }
    }

    return result;
}


float CFieldTableDiff::GetMaximumDifferentialProbability() const
{
    auto maxCount = 0;

    auto size = m_insideRepresentation.size();

    for (int element = 1; element < size; ++element)
    {
        std::map<int, int> countDifference;
        auto direction = m_insideRepresentation[element];
        for (int input = 0; input < size; ++input)
        {
            auto result = GetDifferencialValue(input, element);
            ++countDifference[result];
        }

        for (auto& entry : countDifference)
        {
            if (entry.second > maxCount)
            {
                maxCount = entry.second;
            }
        }
    }

    return maxCount / (m_insideRepresentation.size()/2.f);
}

