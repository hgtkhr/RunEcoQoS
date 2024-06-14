#pragma once
#include <system_error>
#include <string>

using ec_t = unsigned long;

[[noreturn]] void SystemError( ec_t errorCode );
std::wstring ToWideMessageString( const std::string& message );

template < typename StringT = std::wstring >
inline StringT MessageString( const std::string& message )
{
	return ToWideMessageString( message );
}

template <>
inline std::string MessageString< std::string >( const std::string& message )
{
	return message;
}
