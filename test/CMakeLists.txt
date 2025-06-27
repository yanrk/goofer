# find all test source files recursively
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE TEST_HEADERS CONFIGURE_DEPENDS "*.h")

# create test executable
add_executable(goofer_tests ${TEST_SOURCES} ${TEST_HEADERS})

# link test executable with main library and its dependencies
target_link_libraries(goofer_tests PRIVATE goofer ${GOOFER_DEPENDS})

# include directories for test executable
target_include_directories(goofer_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}  # current test directory
    ${PROJECT_HOME}/inc          # main library headers
)

# test framework integration (using Catch2 as example)
if(TARGET Catch2::Catch2)
    # link with Catch2 if available
    target_link_libraries(goofer_tests PRIVATE Catch2::Catch2)
    # auto-discover Catch2 tests
    catch_discover_tests(goofer_tests)
else()
    # basic test registration if no framework found
    add_test(NAME goofer_tests COMMAND goofer_tests)
endif()

# create output directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PLATFORM})

# set output directory for test executable
set_target_properties(goofer_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PLATFORM}
)

# link libraries
if(WIN32)
    if(BUILD_STATIC)
        # set static link library for Windows lib builds
        target_compile_options(goofer_tests PRIVATE
            # generator expressions for configuration-specific link selection
            "$<$<CONFIG:Debug>:/MTd>"         # debug build: static debug link library
            "$<$<CONFIG:Release>:/MT>"        # release build: static release link
            "$<$<CONFIG:RelWithDebInfo>:/MT>" # release with debug info: static link
            "$<$<CONFIG:MinSizeRel>:/MT>"     # minimum size release: static link
        )
    elseif()
        # set dynamic runtime library for Windows dll builds
        target_compile_options(goofer_tests PRIVATE
            # generator expressions for configuration-specific runtime selection
            "$<$<CONFIG:Debug>:/MDd>"         # debug build: dynamic debug runtime library
            "$<$<CONFIG:Release>:/MD>"        # release build: dynamic release runtime
            "$<$<CONFIG:RelWithDebInfo>:/MD>" # release with debug info: dynamic runtime
            "$<$<CONFIG:MinSizeRel>:/MD>"     # minimum size release: dynamic runtime
        )
    endif()
endif()

# use Windows-style dynamic library handling
if(NOT BUILD_STATIC)
    # copy main library
    add_custom_command(TARGET goofer_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:goofer>
            $<TARGET_FILE_DIR:goofer_tests>
        COMMENT "Copying main library to executable directory"
    )
    
    # for Unix-like systems, clear the RPATH setting
    if(UNIX)
        set_target_properties(goofer_tests PROPERTIES
            SKIP_BUILD_RPATH FALSE
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "$ORIGIN"
        )
    endif()
endif()
