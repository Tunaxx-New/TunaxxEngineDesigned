#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace strings
{
    #define NEW_LINE           "\r\n"
}

namespace windows
{
    struct BaseWindowClass
    {
        DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
        LPCWSTR name = L"BaseWindow";
        LPCWSTR title = L"Base Window";
        DWORD style = WS_OVERLAPPEDWINDOW;
        int x = 0;
        int y = 0;
        int width = 640;
        int height = 480;
        HWND parent = NULL;
        HMENU menu = NULL;
        HINSTANCE hInstance = NULL;
        LPVOID lpParam = NULL;
    } BaseWindowClass;

    struct BaseWindowClass WindowClassTemp = BaseWindowClass;

    void rollbackBaseWindowClass()
    {
        WindowClassTemp = BaseWindowClass;
    }
}
#endif