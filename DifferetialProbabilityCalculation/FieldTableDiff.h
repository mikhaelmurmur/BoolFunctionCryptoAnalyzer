#pragma once
#include <string>
#include <vector>

class CFieldTableDiff
{
public:
    explicit CFieldTableDiff(const std::string& filename);
    float CFieldTableDiff::GetMaximumDifferentialProbability() const;
private:
    int GetDifferencialValue(int argument, int direction) const;
    std::vector<std::vector<bool>> m_insideRepresentation;
};

