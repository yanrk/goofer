cmake_minimum_required(VERSION 3.15)

project(goofer
    LANGUAGES C CXX
    VERSION 1.0.0
)

# configuration build options
option(BUILD_STATIC "Build static library instead of shared" ON)
set(PLATFORM "" CACHE STRING "Target platform (windows, mac, linux)")

# platform auto-detection
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(IS_WINDOWS TRUE)
    set(PLATFORM "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(IS_MACOS TRUE)
    set(PLATFORM "mac")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(IS_LINUX TRUE)
    set(PLATFORM "linux")
endif()

# set path variables
set(PROJECT_HOME ${CMAKE_SOURCE_DIR})
set(GNU_HOME ${PROJECT_HOME}/gnu)
set(BIN_DIR ${PROJECT_HOME}/lib/${PLATFORM})

# create output directory if it doesn't exist
file(MAKE_DIRECTORY ${BIN_DIR})

# declare target build type
if(BUILD_STATIC)
    add_library(goofer STATIC)
else()
    add_library(goofer SHARED)
endif()

# collect source files recursively
file(GLOB_RECURSE GOOFER_SRC 
    ${PROJECT_HOME}/src/*.cpp
    ${PROJECT_HOME}/src/*.c
)

file(GLOB_RECURSE CMARKUP_SRC 
    ${GNU_HOME}/cmarkup/src/*.cpp
    ${GNU_HOME}/cmarkup/src/*.c
)

file(GLOB_RECURSE JSONCPP_SRC 
    ${GNU_HOME}/jsoncpp/src/*.cpp
    ${GNU_HOME}/jsoncpp/src/*.c
)

file(GLOB_RECURSE SQLITE_SRC 
    ${GNU_HOME}/sqlite/src/*.c
)

# add sources to target
target_sources(goofer PRIVATE
    ${GOOFER_SRC}
    ${CMARKUP_SRC}
    ${JSONCPP_SRC}
    ${SQLITE_SRC}
)

# include directories
target_include_directories(goofer PRIVATE
    ${PROJECT_HOME}/inc
    ${GNU_HOME}/cmarkup/inc
    ${GNU_HOME}/jsoncpp/inc
    ${GNU_HOME}/sqlite/inc
)

# set C++11 standard
target_compile_features(goofer PRIVATE cxx_std_11)

# add Windows platform DLL export macro
if(WIN32 AND NOT BUILD_STATIC)
    target_compile_definitions(goofer PRIVATE
        EXPORT_GOOFER_DLL
    )
endif()

# macOS use @executable_path
if(APPLE AND NOT BUILD_STATIC)
    set_target_properties(goofer PROPERTIES
        INSTALL_NAME_DIR "@executable_path"
    )
endif()

# platform-independent compile options with proper Debug/Release separation
target_compile_options(goofer PRIVATE
    # MSVC options (Visual Studio)
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:
        /Od      # disable optimizations for Debug
        /RTC1    # enable runtime checks
        /Zi      # generate debug information
        /W3      # warning level 3
        /sdl-    # disable SDL checks (optional)
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:
        /O2      # maximize speed optimizations
        /Zi      # generate debug information
        /W3      # warning level 3
        /GL      # whole program optimization
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>>:
        /O1      # favor size optimizations
        /Zi      # generate debug information
        /W3      # warning level 3
    >
    
    # GCC/Clang options
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:
        -g       # generate debug information
        -O0      # disable optimizations
        -pipe    # use pipes instead of temp files
      # -Wall    # enable all warnings
      # -Wextra  # extra warnings
    >
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:
        -g       # generate debug information
        -O2      # optimize for speed
        -pipe    # use pipes instead of temp files
      # -Wall    # enable all warnings
      # -Wextra  # extra warnings
    >
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>>:
        -g       # generate debug information
        -O1      # balanced optimization
        -pipe    # use pipes instead of temp files
      # -Wall    # enable all warnings
      # -Wextra  # extra warnings
    >
)

# platform-specific compile options
if(MSVC)
    # MSVC specific warnings suppression
    target_compile_options(goofer PRIVATE
        /wd4996  # Disable 'deprecated declarations' warning
        /wd4090  # Disable 'different const qualifiers' warning
        /wd4091  # Disable 'about typedef not rigorous' warning
        /wd4251  # Disable 'about dll-interface Not rigorous' warning
    )
else()
    # GCC/Clang common options
    target_compile_options(goofer PRIVATE
        -Wall
        -Wno-error=deprecated-declarations
        -Wno-deprecated-declarations
        -Wno-misleading-indentation
        -Wno-stringop-truncation
        -Wno-return-local-addr
        -Wno-unused-result
    )
    
    # Linux-specific options
    if(IS_LINUX)
        target_compile_options(goofer PRIVATE
             $<$<COMPILE_LANGUAGE:CXX>:-Wno-class-memaccess>
        )
    endif()
    
    # position independent code for shared libraries (PIC)
    if(NOT BUILD_STATIC)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif()
endif()

# library type configuration
if(BUILD_STATIC)
    if(WIN32)
        set_target_properties(goofer PROPERTIES
            OUTPUT_NAME "goofer"
            ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
            SUFFIX ".lib"
        )
    else()
        set_target_properties(goofer PROPERTIES
            OUTPUT_NAME "goofer"
            ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
            SUFFIX ".a"
            PREFIX "lib"
        )
    endif()
else()
    if(WIN32)
        set_target_properties(goofer PROPERTIES
            OUTPUT_NAME "goofer"
            RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
            SUFFIX ".dll"
        )
    elseif(APPLE)
        set_target_properties(goofer PROPERTIES
            OUTPUT_NAME "goofer"
            LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}
            SUFFIX ".dylib"
            PREFIX "lib"
        )
    else()
        set_target_properties(goofer PROPERTIES
            OUTPUT_NAME "goofer"
            LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}
            SUFFIX ".so"
            PREFIX "lib"
        )
    endif()
endif()

# link libraries
if(WIN32)
    if(BUILD_STATIC)
        # set static link library for Windows lib builds
        target_compile_options(goofer PRIVATE
            # generator expressions for configuration-specific link selection
            "$<$<CONFIG:Debug>:/MTd>"         # debug build: static debug link library
            "$<$<CONFIG:Release>:/MT>"        # release build: static release link
            "$<$<CONFIG:RelWithDebInfo>:/MT>" # release with debug info: static link
            "$<$<CONFIG:MinSizeRel>:/MT>"     # minimum size release: static link
        )
    elseif()
        # set dynamic runtime library for Windows dll builds
        target_compile_options(goofer PRIVATE
            # generator expressions for configuration-specific runtime selection
            "$<$<CONFIG:Debug>:/MDd>"         # debug build: dynamic debug runtime library
            "$<$<CONFIG:Release>:/MD>"        # release build: dynamic release runtime
            "$<$<CONFIG:RelWithDebInfo>:/MD>" # release with debug info: dynamic runtime
            "$<$<CONFIG:MinSizeRel>:/MD>"     # minimum size release: dynamic runtime
        )
        
        # explicitly ignore static runtime libraries to prevent conflicts
        target_link_options(goofer PRIVATE
            "/NODEFAULTLIB:libcmt.lib"     # Ignore release static runtime library
            "/NODEFAULTLIB:libcmtd.lib"    # Ignore debug static runtime library
        )
    endif()

    target_link_libraries(goofer PRIVATE
        ws2_32    # Windows Sockets 2 API for network communication
        dbghelp   # Debug Help Library for error handling and diagnostics
        iphlpapi  # IP Helper API for network configuration
    )
else()
    target_link_libraries(goofer PRIVATE
        pthread   # POSIX threads
        dl        # Dynamic loading (dlopen/dlsym)
    )
endif()

# install rules for packaging
install(TARGETS goofer
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# enable folder organization in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# configuration test options
option(BUILD_TESTS "Build test programs" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
