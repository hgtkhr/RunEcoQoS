#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#define NOMINMAX
#include <windows.h>
#include <processthreadsapi.h>
#include <memory>
#include <tchar.h>
#include <string>
#include <format>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Error.hpp"
#include "Path.hpp"

template < typename HandleT = HANDLE >
struct safe_handle_closer
{
	typedef HANDLE pointer;
	void operator () ( HANDLE h )
	{
		if ( h != nullptr && h != INVALID_HANDLE_VALUE )
			::CloseHandle( h );
	}
};

using char_t = TCHAR;
using string_t = std::basic_string< char_t >;
using scoped_handle = std::unique_ptr< HANDLE, safe_handle_closer< HANDLE > >;

void Run( int argc, char_t** argv )
{
	string_t appname = ToString( ComplatePath( argv[1] ) );
	string_t cmdline = std::format( _T( "\"{}\"" ), appname );

	static const string_t SwitchMemPriority = _T( "/MemPriority:" );
	static const std::unordered_map< string_t, ULONG > MemPriorities =
	{
		{ _T( "VERY_LOW" ),	MEMORY_PRIORITY_VERY_LOW },
		{ _T( "LOW" ), MEMORY_PRIORITY_LOW },
		{ _T( "MEDIUM" ), MEMORY_PRIORITY_MEDIUM },
		{ _T( "BELOW_NORMAL" ), MEMORY_PRIORITY_BELOW_NORMAL },
		{ _T( "NORMAL" ), MEMORY_PRIORITY_NORMAL },
	};

	PROCESS_POWER_THROTTLING_STATE processPowerThrottle = {};
	MEMORY_PRIORITY_INFORMATION processMemPriority = {};

	for ( int i = 2; i < argc; ++i )
	{
		string_t arg = argv[i];

		if ( arg.starts_with( SwitchMemPriority ) )
		{
			string_t sval = arg.substr( SwitchMemPriority.length() );

			if ( sval.length() == 0 )
				SystemError( ERROR_BAD_ARGUMENTS );

			::CharUpper( sval.data() );

			auto i = MemPriorities.find( sval );
			if ( i != MemPriorities.end() )
			{
				processMemPriority.MemoryPriority = i->second;
			}
			else
			{
				SystemError( ERROR_BAD_ARGUMENTS );
			}

			continue;
		}

		if ( std::ranges::any_of( arg, &::_istspace ) )
		{
			cmdline += std::format( _T( " \"{}\"" ), arg );
		}
		else
		{
			cmdline += std::format( _T( " {}" ), arg );
		}
	}

	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};

	si.cb = sizeof( STARTUPINFO );

	::GetStartupInfo( &si );

	BOOL r = ::CreateProcess( appname.c_str()
							  , cmdline.data()
							  , nullptr, nullptr
							  , FALSE, CREATE_SUSPENDED
							  , nullptr
							  , nullptr
							  , &si, &pi );
	if ( r == FALSE )
		SystemError( ::GetLastError() );

	scoped_handle process( pi.hProcess );
	scoped_handle primaryThread( pi.hThread );

	processPowerThrottle.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
	processPowerThrottle.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
	processPowerThrottle.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;

	if ( ::SetProcessInformation( process.get(), ProcessPowerThrottling, &processPowerThrottle, sizeof( PROCESS_POWER_THROTTLING_STATE ) ) == 0 )
		SystemError( ::GetLastError() );
	if ( ::SetPriorityClass( process.get(), IDLE_PRIORITY_CLASS ) == 0 )
		SystemError( ::GetLastError() );
	if ( ::SetProcessInformation( process.get(), ProcessMemoryPriority, &processMemPriority, sizeof( MEMORY_PRIORITY_INFORMATION ) ) == 0 )
		SystemError( ::GetLastError() );

	if ( ::ResumeThread( primaryThread.get() ) == DWORD( -1 ) )
		SystemError( ::GetLastError() );
}

int _tmain( int argc, char_t** argv )
{
	if ( argc < 2 )
		return EXIT_FAILURE;

	try
	{
		Run( argc, argv );
	}
	catch ( std::system_error& e )
	{
		::MessageBox( HWND_DESKTOP, MessgaeString( e.what() ).c_str(), nullptr, MB_OK | MB_ICONERROR);
	}

	return EXIT_SUCCESS;
}