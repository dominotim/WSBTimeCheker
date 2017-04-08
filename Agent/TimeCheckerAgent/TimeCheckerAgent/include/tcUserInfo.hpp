#pragma once
#include <string>
#include <ctime>

namespace tcUserInfoHelper
{
std::wstring GetTime(const std::time_t time);
std::wstring GetData(const std::time_t time);
}  // tcUserInfoHelper

namespace tcUserInfo
{

class UserInfo
{
public:
	UserInfo(
		const std::wstring& name,
		const std::wstring& applycationName,
		const std::wstring& url,
		std::time_t time);
	std::wstring ToString() const;
private:
	std::wstring m_name;
	std::wstring m_applycationName;
	std::wstring m_url;
	std::time_t m_time;
};

}  // tcUserInfo