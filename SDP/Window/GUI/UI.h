#pragma once
#include "../BaseWindow.h"

class UI : public BaseWindow
{
public:
	UI(int) : ::BaseWindow() {}
	virtual void update() = 0;
protected:
	HWND whandle_;
};