#pragma once
#include <Windows.h>
#include <string>

namespace tcCurrentWindowInfoGetter
{
bool IsBrowserWindow(HWND windowHendler);
std::wstring GetCurrentUrl(HWND windowHendler);
std::wstring GetNormalizedUrl(HWND windowHendler);
std::wstring GetCurrentApplicationName(HWND windowHendler);
std::wstring GetNormalizedCurrentApplicationName(HWND windowHendler);
};

