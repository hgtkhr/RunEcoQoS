#pragma once
#include <filesystem>

using std::filesystem::path;

using char_t = TCHAR;
using string_t = std::basic_string< char_t >;
using path_string_t = typename path::string_type;

template < typename StringT >
inline StringT ToString( const path& p )
{
	return p.string< StringT >();
}

template <>
inline path_string_t ToString( const path& p )
{
	return p.native();
}

template <>
inline std::string ToString< std::string >( const path& p )
{
	return p.string();
}

path ComplatePath( const path& p );
