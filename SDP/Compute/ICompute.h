#pragma once


class ICompute
{
public:
	virtual void compute(unsigned long** bitmap, int size) = 0;
};