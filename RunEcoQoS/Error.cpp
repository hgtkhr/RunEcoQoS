#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <system_error>
#include "Error.hpp"

[[noreturn]] void SystemError( ec_t errorCode )
{
	std::error_code ec( errorCode, std::system_category() );

	throw std::system_error( ec );
}

std::wstring ToWideMessgaeString( const std::string& message )
{
	std::size_t length = ::MultiByteToWideChar( CP_THREAD_ACP, 0, message.c_str(), -1, nullptr, 0 );

	std::vector< wchar_t > buffer( length + 1 );
	int r = ::MultiByteToWideChar( CP_THREAD_ACP, 0, message.c_str(), -1, buffer.data(), static_cast< int >( buffer.size() ) );

	if ( r == 0 )
	{
		DWORD lastError = ::GetLastError();
		if ( lastError != NO_ERROR )
			SystemError( lastError );
	}

	return buffer.data();
}
