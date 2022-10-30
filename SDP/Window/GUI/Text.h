#pragma once
#include "UI.h"

using namespace strings;

class Text : public UI
{
public:
	Text(HWND* parent) : ::UI(beforeConstruct())
	{
		whandle_ = ::CreateWindow(
			TEXT("EDIT"),
			L"lol3",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			0,
			WindowClassTemp.y,
			640,
			200,
			handle_,
			NULL,
			NULL,
			NULL
		);
		//HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
		//SetClassLongPtr(handle_, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
		SetParent(handle_, *parent);

		rollbackBaseWindowClass();
	}

	void set(const std::string& text) {
		text_ = text;
		update();
	}

	void append(const std::string& text) {
		text_ += text + separator_;
		update();
	}

	std::string get() {
		return text_;
	}

	void update() {
		std::wstring stemp = std::wstring(text_.begin(), text_.end());
		LPCWSTR sw = stemp.c_str();
		SetWindowText(whandle_, sw);
		SendMessageA(whandle_, EM_SETSEL, 0, -1);
		SendMessageA(whandle_, EM_SETSEL, -1, -1);
		SendMessageA(whandle_, EM_SCROLLCARET, 0, 0);
	}

private:
	std::string separator_ = NEW_LINE;

	int beforeConstruct()
	{
		//WindowClassTemp.exStyle = WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
		WindowClassTemp.height = 200;
		return 0;
	}

	LRESULT process(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) override
	{
		switch (message)
		{

		case WM_PAINT:
			InvalidateRect(handle, NULL, TRUE);

		default:
			return ::DefWindowProc(handle, message, wparam, lparam);
		}

		return NULL;
	}
};