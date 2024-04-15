#pragma once
#include <filesystem>

using std::filesystem::path;

template < typename StringT = std::wstring, bool IsUnicode = true >
inline StringT ToString( const path& p )
{
	return p.native();
}

template <>
inline std::string ToString< std::string, false >( const path& p )
{
	return p.string();
}

path ComplatePath( const path& p );
