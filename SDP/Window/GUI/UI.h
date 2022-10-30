#pragma once
#include "../BaseWindow.h"

class UI : public BaseWindow
{
public:
	UI(int) : ::BaseWindow() {}
protected:
	std::string text_;
	HWND whandle_;
};