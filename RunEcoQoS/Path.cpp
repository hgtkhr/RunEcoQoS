#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <tchar.h>
#include <windows.h>
#include "Path.hpp"
#include "Error.hpp"

path ComplatePath( const path& p )
{
	if ( p.is_absolute() )
		return p;

	if ( p.has_parent_path() )
		return std::filesystem::canonical( p );

	auto pstr = ToString< string_t >( p );

	std::vector< TCHAR > szSearchPath( MAX_PATH );
	while ( true )
	{
		DWORD r = ::SearchPath( nullptr, pstr.c_str(), nullptr, static_cast< DWORD >( szSearchPath.size() ), szSearchPath.data(), nullptr );
		if ( r == 0 )
			SystemError( ::GetLastError() );
		if ( r < szSearchPath.size() )
			break;
		szSearchPath.resize( szSearchPath.size() * 2 );
	}

	return std::filesystem::canonical( szSearchPath.data() );
}
