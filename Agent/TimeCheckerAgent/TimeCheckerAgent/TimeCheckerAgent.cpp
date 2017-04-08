#include "stdafx.h"
//// INFORMATION 
/// These inclusions should be ahead of the rest
////////////////////////////////////////////////
#include "tcNetworkUtils.hpp"
////////////////////////////////////////////////
#include "tcCurrentWindowInfoGetter.hpp"
#include "tcUserInfo.hpp"

#include <Windows.h>
#include <oleacc.h>

#include <string>
#include <iostream>
#include <chrono>

#pragma warning(disable : 4114)

int main()
{
    CoInitialize(NULL);
    const std::string HOST_NAME = "192.168.23.149";
    const std::string PORT = "3000";

    while (true)
    {
        Sleep(3000);
        HWND hwnd;
        hwnd = GetForegroundWindow();
        tcUserInfo::UserInfo info(std::wstring(L"Artem"),
            tcCurrentWindowInfoGetter::GetNormalizedCurrentApplicationName(hwnd),
            tcCurrentWindowInfoGetter::IsBrowserWindow(hwnd)
            ? tcCurrentWindowInfoGetter::GetNormalizedUrl(hwnd) : L"\"\"",
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        std::cout << tcNetworkUtils::SendPOST(HOST_NAME, PORT, info, std::chrono::seconds(5));
    }
    return 0;
}

