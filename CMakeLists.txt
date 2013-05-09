cmake_minimum_required(VERSION 2.6)

project(Shadow_Server CXX)

include(CheckCXXSourceCompiles)

######################################################################
# Flag handling
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=c++11     HAS_FLAG_STD_CXX11)
check_cxx_compiler_flag(-stdlib=libc++ HAS_FLAG_STDLIB_LIBCXX)
check_cxx_compiler_flag(-pipe          HAS_FLAG_PIPE)

if(NOT HAS_FLAG_STD_CXX11)
	check_cxx_compiler_flag(-std=c++0x     HAS_FLAG_STD_CXX0X)
endif()

if(EXTRA_WARNINGS)
	check_cxx_compiler_flag(-Wall                HAS_WARNING_ALL)
	check_cxx_compiler_flag(-Wextra              HAS_WARNING_EXTRA)
	check_cxx_compiler_flag(-Weffc++             HAS_WARNING_EFFCXX)
	check_cxx_compiler_flag(-Wconversion         HAS_WARNING_CONVERSION)
	check_cxx_compiler_flag(-Wsign-conversion    HAS_WARNING_SIGN_CONVERSION)
	check_cxx_compiler_flag(-Woverloaded-virtual HAS_WARNING_OVERLOADED_VIRTUAL)
	# check_cxx_compiler_flag(-Weverything         HAS_WARNING_EVERYTHING)

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
	# if(HAS_WARNING_EVERYTHING)
	# 	set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Weverything")
	# endif()
	if(HAS_WARNING_WPEDANTIC)
		set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wpedantic")
	endif()
	if(HAS_WARNING_PEDANTIC)
		set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -pedantic")
	endif()
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

CHECK_CXX_SOURCE_COMPILES("int main() { void* p = nullptr; }"
	HAVE_NULLPTR
	FAIL_REGEX "use of undeclared identifier 'nullptr'")
if(NOT HAVE_NULLPTR)
	message(FATAL_ERROR "Shadow World server needs nullptr")
endif()

# CHECK_CXX_SOURCE_COMPILES("int main() { const char* f = __PRETTY_FUNCTION__; }")
# CHECK_CXX_SOURCE_COMPILES("int main() { const char* f = __FUNCTION__; }")
# CHECK_CXX_SOURCE_COMPILES("int main() { const char* f = __func__; }")

CHECK_CXX_SOURCE_COMPILES("int main() { char c1[] = u8\"a\"; char16_t c2[] = u\"a\"; char32_t c3[] = U\"a\"; }"
	HAVE_UNICODE_LITERALS
	FAIL_REGEX "use of undeclared identifier 'u8'"
	FAIL_REGEX "use of undeclared identifier 'u'"
	FAIL_REGEX "use of undeclared identifier 'U'"
	FAIL_REGEX "unknown type name 'char16_t'"
	FAIL_REGEX "unknown type name 'char32_t'")
if(HAVE_UNICODE_LITERALS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_UNICODE_LITERALS=1")
endif()

CHECK_CXX_SOURCE_COMPILES("int main() { char s[] = R\"delim(string)delim\"; }"
	HAVE_RAW_STRING_LITERALS
	FAIL_REGEX "use of undeclared identifier 'R'"
	FAIL_REGEX "‘R’ was not declared in this scope")
if(HAVE_RAW_STRING_LITERALS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_RAW_STRING_LITERALS=1")
endif()

# TODO: Computed gotos?

######################################################################
# TODO: Header files

######################################################################
# TODO: Structures

######################################################################
# TODO: Libraries

# Check if the "c++abi" library is needed
CHECK_CXX_SOURCE_COMPILES("#include <iostream>\nint main() { std::cout << 'c'; }"
	LIBCXXABI_NOT_NEEDED
	FAIL_REGEX "undefined reference to symbol '__gxx_personality_v0'")
if(NOT LIBCXXABI_NOT_NEEDED)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
endif()

######################################################################
# TODO: Library functions

######################################################################
# TODO: System services (mtrace etc.)

######################################################################
# Boost

include(FindBoost)

set(BOOST_ROOT "/usr/local")

find_package(Boost 1.53.0 COMPONENTS system program_options log filesystem)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

######################################################################
# Subdirectories
add_subdirectory(src)
