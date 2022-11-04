#pragma once


class IEngineManipulation
{
public:
	virtual void changeCompute() = 0;
	virtual void scaleUp() = 0;
	virtual void scaleDown() = 0;
};