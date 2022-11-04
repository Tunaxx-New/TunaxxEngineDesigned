#pragma once
#include "UI.h"

class Button : public UI
{
public:
	Button(HWND* parent, std::function<void()> action) : ::UI(beforeConstruct())
	{
		rollbackBaseWindowClass();
		::CreateWindow(
			TEXT("BUTTON"),
			L"lol2",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			0,
			WindowClassTemp.y,
			120,
			30,
			handle_,
			NULL,
			NULL,
			NULL
		);

		SetParent(handle_, *parent);

		action_ = action;

		rollbackBaseWindowClass();
	}
private:
	void update() {}

	std::function<void()> action_;

	int beforeConstruct()
	{
		//WindowClassTemp.exStyle = WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
		WindowClassTemp.height = 60;
		return 0;
	}

	LRESULT process(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) override
	{
		switch (message)
		{

		case WM_COMMAND:
		{
			if (action_)
				action_();
		}

		case WM_PAINT:
			InvalidateRect(handle, NULL, TRUE);

		default:
			return ::DefWindowProc(handle, message, wparam, lparam);
		}

		return NULL;
	}
};