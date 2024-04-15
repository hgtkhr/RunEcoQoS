#pragma once
#include <system_error>
#include <string>

using ec_t = unsigned long;

[[noreturn]] void SystemError( ec_t errorCode );
std::wstring ToWideMessgaeString( const std::string& message );

template < typename StringT = std::wstring, bool IsUnicode = true >
inline StringT MessgaeString( const std::string& message )
{
	return ToWideMessgaeString( message );
}

template <>
inline std::string MessgaeString< std::string, false >( const std::string& message )
{
	return message;
}
