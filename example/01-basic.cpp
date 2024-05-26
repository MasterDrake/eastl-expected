// Convert text to number and yield expected with number or error text.
#define nsel_CONFIG_NO_EXCEPTIONS 1
#include "nonstd/expected.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#include "EASTL/string.h"

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
    {
        return new uint8_t[size];
    }

using namespace eastl;
using namespace eastl::literals;

auto to_int( char const * const text ) -> expected<int, eastl::string>
{
    char * pos = nullptr;
    auto value = strtol( text, &pos, 0 );

    if ( pos != text ) return value;
    else               return eastl::make_unexpected("'"s + text + "' isn't a number");
}

int main( int argc, char * argv[] )
{
    auto text = argc > 1 ? argv[1] : "42";

    auto ei = to_int( text );

    if ( ei ) std::cout << "'" << text << "' is " << *ei << ", ";
    else      std::cout << "Error: " << ei.error().c_str();
}

// cl -EHsc -wd4814 -I../include 01-basic.cpp && 01-basic.exe 123 && 01-basic.exe abc
// g++ -std=c++14 -Wall -I../include -o 01-basic.exe 01-basic.cpp && 01-basic.exe 123 && 01-basic.exe abc
// '123' is 123, Error: 'abc' isn't a number
