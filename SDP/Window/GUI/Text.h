#pragma once
#include "UI.h"

using namespace strings;

class Text : public UI
{
public:
	Text(HWND* parent, std::string& text) : text_(text), ::UI(beforeConstruct())
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

		apply();
		update();
	}

	void apply() {
		if (!isChange_) {
			int size;
			int length = (int)text_.length() + 1;
			size = MultiByteToWideChar(CP_ACP, 0, text_.c_str(), length, 0, 0);
			wchar_t* buf = new wchar_t[size];
			MultiByteToWideChar(CP_ACP, 0, text_.c_str(), length, buf, size);
			vtext_ = buf;
		}
		isChange_ = true;
	}

	std::string get() {
		return text_;
	}

	void update() {
		if (isChange_)
			SetWindowTextW(whandle_, vtext_);
		isChange_ = false;
	}

	void scrollDown()
	{
		SendMessageA(whandle_, EM_SETSEL, 0, -1);
		SendMessageA(whandle_, EM_SETSEL, -1, -1);
		SendMessageA(whandle_, EM_SCROLLCARET, 0, 0);
	}

protected:
	std::string separator_ = NEW_LINE;

private:
	bool isChange_ = false;
	LPCWSTR vtext_;
	std::string& text_;

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
		case WM_PAINT:
			update();
			InvalidateRect(handle, NULL, TRUE);

		default:
			return ::DefWindowProc(handle, message, wparam, lparam);
		}

		return NULL;
	}
};