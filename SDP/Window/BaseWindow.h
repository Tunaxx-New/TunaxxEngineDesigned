#pragma once
#include <Windows.h>
#include <map>
#include <string>
#include "Constants.h"

using namespace windows;

class BaseWindow {
public:

    BaseWindow() {
        HWND window;
        HINSTANCE hInstance = ::GetModuleHandle(NULL);
        WNDCLASSEX window_class = { 0 };
        window_class.hInstance = hInstance;
        window_class.cbSize = sizeof(WNDCLASSEX);
        window_class.lpszClassName = L"BaseWindow";
        window_class.lpfnWndProc = (WNDPROC)internal_process;

        RegisterClassEx(&window_class);

        window = ::CreateWindowEx(
            WindowClassTemp.exStyle,
            WindowClassTemp.name,
            WindowClassTemp.title,
            WindowClassTemp.style,
            WindowClassTemp.x,
            WindowClassTemp.y,
            WindowClassTemp.width,
            WindowClassTemp.height,
            WindowClassTemp.parent,
            WindowClassTemp.menu,
            WindowClassTemp.hInstance,
            WindowClassTemp.lpParam
        );
       
        ::SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)this);
        handle_ = window;

        ::ShowWindow(window, SW_SHOW);
        ::UpdateWindow(window);

        setSize(WindowClassTemp.width, WindowClassTemp.height);
        active_ = true;
    }

    BaseWindow(int) : BaseWindow() {}

    bool broadcast()
    {
        MSG msg;
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(0);
        return true;
    }

    HWND* getHandle()
    {
        return &handle_;
    }

    void setSize(UINT width, UINT height) {
        size_.X = width;
        size_.Y = height;
        std::cout << width << ' ' << height << '\n';
    }

protected:
    virtual LRESULT process(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) = 0;

    HWND handle_;
    COORD size_ = { 0, 0 };

private:

    static LRESULT CALLBACK internal_process(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) {
        BaseWindow* c = (BaseWindow*)::GetWindowLongPtr(handle, GWLP_USERDATA);

        if (c != NULL && c->active_) {
            return c->process(handle, message, wparam, lparam);
        }

        return DefWindowProc(handle, message, wparam, lparam);
    }

    bool active_ = false;

};