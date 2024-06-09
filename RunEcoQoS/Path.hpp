#pragma once
#include <filesystem>

using std::filesystem::path;

template < typename StringT = std::wstring >
inline StringT ToString( const path& p )
{
	return p.native();
}

template <>
inline std::string ToString< std::string >( const path& p )
{
	return p.string();
}

path ComplatePath( const path& p );
