// TimeCheckerAgent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#define ASIO_STANDALONE
#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"
#include <chrono>
#include <time.h>
#include <ctime>

#pragma warning(disable : 4996)
struct UserData
{
	UserData(
		const std::string name,
		const const std::string applycationName,
		std::time_t time)
		: m_name(name), m_applycationName(applycationName), m_time(time)
	{}
	std::string m_name;
	std::string m_applycationName;
	std::time_t  m_time;
};

std::string GetTime(const std::time_t time)
{
	char buffer[80];
	auto timeinfo = std::localtime(&time);
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	return std::string(buffer);
}

std::string GetData(const std::time_t time)
{
	char buffer[80];
	auto timeinfo = std::localtime(&time);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo);
	return std::string(buffer);
}

std::string UserToString(const UserData& user)
{
	std::string result;
	result += "UserName=" + user.m_name + ";";
	result += "AppName=" + user.m_applycationName + ";";
	result += std::string("Data=") + GetData(user.m_time) + ";";
	result += std::string("Time=") + GetTime(user.m_time) + ";";
	return result;
}

std::string SendPOST(
	asio::ip::tcp::iostream& stream,
	const std::string& hostName,
	const std::string& port,
	const UserData& curUser)
{
	stream.connect("192.168.23.149", "3000");
	stream << "POST / HTTP/1.1\r\n";
	stream << "Host: 192.168.23.149\r\n";
	stream << "User-Agent: C/1.0";
	stream << "Accept: */*\r\n";
	stream << "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
	std::string mass = UserToString(curUser);
	stream << "Content-Length: " << mass.length() << "\r\n";
	stream << "Connection: close\r\n\r\n";
	stream << mass;
	std::cout << mass << std::endl;
	stream.flush();

	std::ostringstream oss;
	oss << stream.rdbuf();
	return oss.str();
}

int _tmain(int argc, _TCHAR* argv[])
{
	UserData user1("Vasiliy", "Google Chrome", std::time(nullptr));
	UserData user2("Artem", "Visual Studio 2015", std::time(nullptr));
	std::cout << "hello world" << std::endl;
	asio::ip::tcp::iostream stream;
	stream.expires_from_now(std::chrono::seconds(60));
	std::cout << SendPOST(stream, "192.168.23.149", "3000", user1);
	std::cout << SendPOST(stream, "192.168.23.149", "3000", user2);
	return 0;
}

