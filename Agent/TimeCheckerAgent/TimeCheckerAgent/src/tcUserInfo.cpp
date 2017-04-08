#include "stdafx.h"
#include "tcUserInfo.hpp"

#pragma warning(disable : 4996)

namespace tcUserInfoHelper
{
std::wstring GetTime(const std::time_t time)
{
    char buffer[80];
    auto timeinfo = std::localtime(&time);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    std::string tmp(buffer);
    return std::wstring(tmp.begin(), tmp.end());
}

std::wstring GetData(const std::time_t time)
{
    char buffer[80];
    auto timeinfo = std::localtime(&time);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo);
    std::string tmp(buffer);
    return std::wstring(tmp.begin(), tmp.end());
}
}  // tcUserInfoHelper

namespace tcUserInfo
{

UserInfo::UserInfo(
    const std::wstring& name,
    const std::wstring& applycationName,
    const std::wstring& url,
    std::time_t time)
    : m_name(name), m_applycationName(applycationName), m_url(url), m_time(time)
{
}

std::wstring UserInfo::ToString() const
{
    std::wstring result;
    result += L"UserName=" + this->m_name + L"&";
    result += L"AppName=" + this->m_applycationName + L"&";
    result += L"URL=" + this->m_url + L"&";
    result += std::wstring(L"Date=") + tcUserInfoHelper::GetData(this->m_time) + L"&";
    result += std::wstring(L"Time=") + tcUserInfoHelper::GetTime(this->m_time);
    return result;
}
}  // tcUserInfo
