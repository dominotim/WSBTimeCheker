#pragma once
#define ASIO_STANDALONE
#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include <chrono>
#include <string>

namespace tcNetworkUtils
{
namespace
{
struct toStringDetector
{
    static auto check(void*) ->std::wstring;
    template<class T>
    static auto check(T* objPtr) -> decltype(objPtr->ToString())*;
    typedef std::wstring* match_type;
};
template<class T> struct hasToString
    : std::is_same<toStringDetector::match_type,
                    decltype(toStringDetector::check(static_cast<T*>(nullptr)))> 
{
};
}  // namespace

template <typename ObjectType>
std::string SendPOST(
    const std::string& hostName,
    const std::string& port,
    const ObjectType& toSendObj,
    const std::chrono::seconds expiresTime)
{
    asio::ip::tcp::iostream stream;
    stream.expires_from_now(expiresTime);
    stream.connect(hostName, port);
    stream << "POST / HTTP/1.1\r\n";
    stream << "Host: " + hostName + "\r\n";
    stream << "User-Agent: C/1.0";
    stream << "Accept: */*\r\n";
    stream << "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
    std::wstring mass = (hasToString<ObjectType>::value) ? toSendObj.ToString() : L"";
    stream << "Content-Length: " << mass.length() << "\r\n";
    stream << "Connection: close\r\n\r\n";
    std::wcout << mass << std::endl;
    stream << mass.c_str();
    stream.flush();

    std::ostringstream oss;
    oss << stream.rdbuf();
    stream.close();
    return oss.str();
}
} //  tcNetworkUtils

