#pragma once
#include <vector>


class ComputeMatrix
{
public:
	virtual void multiply(std::vector<std::vector<double>>& matrix, std::vector<std::vector<double>>& vertices, char mode) = 0;
};