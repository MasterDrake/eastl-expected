# Copyright (c) 2016-2022 Martin Moene.
#
# https://github.com/martinmoene/expected-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

if( NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION )
    cmake_minimum_required( VERSION 3.8 FATAL_ERROR )
endif()

project( example LANGUAGES CXX )

# unit_name provided by toplevel CMakeLists.txt
set( PACKAGE   ${unit_name}-lite )

message( STATUS "Subproject '${PROJECT_NAME}', various examples")

# Target default options and definitions:

set( OPTIONS "" )
#set( DEFINITIONS "" )

# Sources (.cpp), normal and no-exception, and their base names:

set( SOURCES_CPP11
    02-required.cpp
)

set( SOURCES_CPP14
    01-basic.cpp
)

# note: here variable must be quoted to create semicolon separated list:

string( REPLACE ".cpp" "" BASENAMES_CPP11 "${SOURCES_CPP11}" )
string( REPLACE ".cpp" "" BASENAMES_CPP14 "${SOURCES_CPP14}" )

set( TARGETS_CPP11 ${BASENAMES_CPP11} )
set( TARGETS_CPP14 ${BASENAMES_CPP14} )
set( TARGETS_ALL   ${TARGETS_CPP11} ${TARGETS_CPP14} )

# add targets:
find_package(EASTL CONFIG REQUIRED)

foreach( name ${TARGETS_ALL} )
    add_executable(        ${name} ${name}.cpp )
    target_link_libraries( ${name} PRIVATE ${PACKAGE} EASTL )
endforeach()

# set compiler options:

if( ${CMAKE_GENERATOR} MATCHES Visual )
    foreach( name ${TARGETS_ALL} )
        target_compile_options( ${name} PUBLIC -W3 -EHsc -wd4814 -Zc:implicitNoexcept- )
    endforeach()
else()
    foreach( name ${TARGETS_ALL} )
        target_compile_options( ${name} PUBLIC -Wall )
    endforeach()

    foreach( name ${TARGETS_CPP11} )
        target_compile_options( ${name} PUBLIC -std=c++11 )
    endforeach()

    foreach( name ${TARGETS_CPP14} )
        target_compile_options( ${name} PUBLIC -std=c++14 )
    endforeach()
endif()

# configure unit tests via CTest:

enable_testing()

foreach( name ${TARGETS_ALL} )
    add_test    ( NAME ${name} COMMAND ${name} )
    set_property( TEST ${name} PROPERTY LABELS example )
endforeach()

# end of file
