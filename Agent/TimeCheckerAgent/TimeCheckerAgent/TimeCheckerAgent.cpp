// TimeCheckerAgent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#define ASIO_STANDALONE
#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"
#include <chrono>

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "hello world" << std::endl;
	asio::ip::tcp::iostream stream;
	stream.expires_from_now(std::chrono::seconds(60));
	stream.connect("www.boost.org", "http");
	stream << "GET /LICENSE_1_0.txt HTTP/1.0\r\n";
	stream << "Host: www.boost.org\r\n";
	stream << "Accept: */*\r\n";
	stream << "Connection: close\r\n\r\n";
	stream.flush();
	std::cout << stream.rdbuf();
	return 0;
}

