#pragma once
#include "WidgetBuilder.h"
#include <vector>

class Window : public BaseWindow
{
public:
	Window() : ::BaseWindow()
	{
		w = new WidgetBuilder();
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth = 640;
		bitmapInfo.bmiHeader.biHeight = -480;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biSizeImage = 0;
		bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
		bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
		bitmapInfo.bmiHeader.biClrUsed = 0;
		bitmapInfo.bmiHeader.biClrImportant = 0;

		SetTimer(handle_, 0, 100, (TIMERPROC)NULL);
		bitmap_ = CreateBitmap(640, 480, 1, 32, NULL);
	}

	
	void paint()
	{
		PAINTSTRUCT ps;
		HDC         hdc;
		HDC         src;

		hdc = BeginPaint(handle_, &ps);
		src = CreateCompatibleDC(hdc);
	
		/*
		SetDIBits(
			hdc,
			bitmap_,
			0,
			size_.Y,
			bmp,
			&bitmapInfo,
			DIB_RGB_COLORS
		);
		*/

		SelectObject(src, bitmap_);
		BitBlt(
			hdc,
			0,
			0,
			size_.X,
			size_.Y,
			src,
			0,
			0,
			SRCCOPY
		);

		DeleteDC(src);

		EndPaint(handle_, &ps);
	}

	void setBitmap(COLORREF** bitmap) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(handle_, &ps);
		SetDIBits(
			hdc,
			bitmap_,
			0,
			size_.Y,
			(*bitmap),
			&bitmapInfo,
			DIB_RGB_COLORS
		);
		EndPaint(handle_, &ps);
	}
	
	void setSize(UINT width, UINT height) {
		BaseWindow::setSize(width, height);
		bitmap_ = CreateBitmap(width, height, 1, 32, NULL);
	}

	LRESULT process(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) override
	{
		switch (message)
		{

		case WM_TIMER:
		{
			switch (wparam)
			{
				case 0:
					InvalidateRect(handle, NULL, TRUE);
					break;
				default:
					break;
			}
		}

		case WM_ERASEBKGND:
			break;
		case WM_PAINT:
			paint();
			break;

		case WM_SIZE:
			setSize(LOWORD(lparam), HIWORD(lparam));
			break;

		default:
			return ::DefWindowProc(handle, message, wparam, lparam);
		}
		return NULL;//w->broadcast();
	}
	int foo() {
		return 5;
	}
	void buildCommonButton() {
		w->buildButton(std::bind(&Window::paint, this), "button1", "widget1");
	}
	void buildCommonButton(std::function<void()> action, std::string name) {
		w->buildButton(action, name, "widget1");
	}

	void buildCommonText() {
		w->createWidget(&handle_, "widget1");
		w->buildText("text1", "widget1", std::bind(&Window::foo, this));
	}
	WidgetBuilder* w;

private:
	BITMAPINFO  bitmapInfo;
	std::vector<COLORREF> bmp;
	HBITMAP bitmap_;
};