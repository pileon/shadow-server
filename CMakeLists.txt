cmake_minimum_required(VERSION 2.6)

project(Shadow_Server CXX)

######################################################################
# Flag handling
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=c++11     HAS_FLAG_STD_CXX11)
check_cxx_compiler_flag(-stdlib=libc++ HAS_FLAG_STDLIB_LIBCXX)
check_cxx_compiler_flag(-pipe          HAS_FLAG_PIPE)

if(NOT HAS_FLAG_STD_CXX11)
	check_cxx_compiler_flag(-std=c++0x     HAS_FLAG_STD_CXX0X)
endif()

# TODO: Only check for these flags if requested by the user
check_cxx_compiler_flag(-Wall                HAS_WARNING_ALL)
check_cxx_compiler_flag(-Wextra              HAS_WARNING_EXTRA)
check_cxx_compiler_flag(-Weffc++             HAS_WARNING_EFFCXX)
check_cxx_compiler_flag(-Wconversion         HAS_WARNING_CONVERSION)
check_cxx_compiler_flag(-Wsign-conversion    HAS_WARNING_SIGN_CONVERSION)
check_cxx_compiler_flag(-Woverloaded-virtual HAS_WARNING_OVERLOADED_VIRTUAL)
check_cxx_compiler_flag(-Weverything         HAS_WARNING_EVERYTHING)

check_cxx_compiler_flag(-Wpedantic HAS_WARNING_WPEDANTIC)
if(NOT HAS_WARNING_WPEDANTIC)
	check_cxx_compiler_flag(-pedantic HAS_WARNING_PEDANTIC)
endif()

if(HAS_WARNING_ALL)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wall")
endif()
if(HAS_WARNING_EXTRA)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wextra")
endif()
if(HAS_WARNING_EFFCXX)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Weffc++")
endif()
if(HAS_WARNING_CONVERSION)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wconversion")
endif()
if(HAS_WARNING_SIGN_CONVERSION)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wsign-conversion")
endif()
if(HAS_WARNING_OVERLOADED_VIRTUAL)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Woverloaded-virtual")
endif()
if(HAS_WARNING_EVERYTHING)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Weverything")
endif()
if(HAS_WARNING_WPEDANTIC)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wpedantic")
endif()
if(HAS_WARNING_PEDANTIC)
	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -pedantic")
endif()

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -DDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DDEBUG")

if(HAS_FLAG_STD_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
if(HAS_FLAG_STDLIB_LIBCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
if(HAS_FLAG_PIPE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
endif()
if(HAS_FLAG_STD_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_FLAGS}")

######################################################################
# TODO: Other compiler specifics (`nullptr` etc.)

######################################################################
# TODO: Header files

######################################################################
# TODO: Structures

######################################################################
# TODO: Libraries

######################################################################
# TODO: Library functions

######################################################################
# TODO: System services (mtrace etc.)

######################################################################
# TODO: Boost

######################################################################
# Subdirectories
add_subdirectory(src)
