cmake_minimum_required(VERSION 4.0)

# naming the project
project(manalyze)

# options for building
option(TESTS "Generate unit tests" OFF)
option(USE_YARA "Include Yara functionality into the program (confirm what this does)" ON)
option(USE_HASHLIB "Include a hash library into the program (confirm what this does)" ON)
option(USE_OPENSSL "Adds support for OpenSSL integration into the program" ON)

# setting output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# win32 shit
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME ON)
endif()

# apple shit
if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
endif()

# pulling boost
if(TESTS)
    message("Building unit tests for the program")
    find_package(Boost REQUIRED COMPONENTS regex system filesystem program_options unit_test_framework)
    add_subdirectory(test)
else()
    find_package(Boost REQUIRED COMPONENTS regex system filesystem program_options)
endif()

# optionally compiling with OpenSSL
if(USE_OPENSSL)
    # get the package
    find_package(OpenSSL REQUIRED)
    add_definitions(-DWITH_OPENSSL)
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message("Building without OpenSSL.")
endif()

# download/update Yara
if(USE_YARA)
    if(EXISTS external/yara)
        message("Updating yara...")
        execute_process(COMMAND ${GIT_EXECUTABLE} --git-dir=external/yara/.git fetch)
        execute_process(COMMAND ${GIT_EXECUTABLE} --git-dir=external/yara/.git --work-tree=external/yara merge origin/master)
    else()
        message("Checking out yara...")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/JusticeRage/yara.git external/yara)
    endif()
else()
    message("Not building yara")
endif()

# download/update hashlib
if(USE_HASHLIB)
    if(EXISTS external/yara)
        message("Updating hash-library...")
        execute_process(COMMAND ${GIT_EXECUTABLE} --git-dir=external/hash-library/.git fetch)
        execute_process(COMMAND ${GIT_EXECUTABLE} --git-dir=external/hash-library/.git --work-tree=external/hash-library merge origin/master)
    else()
        message("Checking out hash-library...")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/JusticeRage/hash-library.git external/hash-library)
    endif()
else()
    message("Not building hash library")
endif()

add_definitions(-DWITH_MANACOMMONS) # Use functions from manacommons.
add_library(manape SHARED manape/pe.cpp manape/nt_values.cpp manape/utils.cpp manape/imports.cpp manape/resources.cpp manape/section.cpp manape/imported_library.cpp manape/ordinals.cpp)

add_library(manacommons SHARED manacommons/color.cpp manacommons/output_tree_node.cpp manacommons/escape.cpp manacommons/base64.cpp manacommons/plugin_framework/result.cpp)

# the main executable
add_executable(manalyze src/main.cpp src/config_parser.cpp src/output_formatter.cpp src/dump.cpp src/import_hash.cpp
    src/plugin_framework/dynamic_library.cpp src/plugin_framework/plugin_manager.cpp # Plugin system
    plugins/plugins_yara.cpp plugins/plugin_packer_detection.cpp plugins/plugin_imports.cpp plugins/plugin_resources.cpp plugins/plugin_mitigation.cpp plugins/plugin_overlay.cpp) # Bundled plugins

if(WIN32)
    add_definitions(/D_CRT_SECURE_NO_WARNINGS) # Please don't complain about fopen()
    add_definitions(/D_WINSOCK_DEPRECATED_NO_WARNINGS) # Don't complain about ASIO's WSASocket functions either.
    add_definitions(/D_SCL_SECURE_NO_WARNINGS) # And finally don't complain about std::_Copy_impl used by Karma.
    add_definitions(/bigobj) # Needed by JSON Spirit Reader with VS2019
    set_target_properties(manape PROPERTIES COMPILE_DEFINITIONS "MANAPE_EXPORT") # Export the PE parsing functions
    set_target_properties(manacommons PROPERTIES COMPILE_DEFINITIONS "MANACOMMONS_EXPORT") # Export core functions
    set(BUILD_SHARED_LIBS FALSE)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MT")

    add_definitions(-DBOOST_ALL_NO_LIB -DNOMINMAX) # Problems with autolink and MSVC + don't hide std::min and std::max.

    # Windows only plugins:
    add_library(plugin_authenticode SHARED plugins/plugin_authenticode/plugin_authenticode.cpp
        plugins/plugin_authenticode/commons.cpp)
    target_link_libraries(plugin_authenticode manape manacommons ${Boost_LIBRARIES})
else()
    string(REGEX MATCH "BSD" IS_BSD ${CMAKE_SYSTEM_NAME}) # Detect if we are compiling on a BSD system.

    if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
        add_definitions("/D_DEBUG")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        if(TESTS) # Add coverage option if unit TESTS were requested.
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
        endif()
    endif()

    if(NOT IS_BSD) # No need to link against dl on BSD.
        target_link_libraries(manalyze dl)
    endif()

    # setting compilation flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")

    # Compile the *nix authenticode plugin if OpenSSL was found.
    # this also required Yara
    if(USE_OPENSSL AND USE_YARA AND USE_HASHLIB)
        add_library(plugin_authenticode SHARED plugins/plugin_authenticode/plugin_authenticode_openssl.cpp
            plugins/plugin_authenticode/commons.cpp
            plugins/plugin_authenticode/asn1.cpp
            plugins/plugin_authenticode/pe_authenticode_digest.cpp)
        target_link_libraries(plugin_authenticode manacommons manape hash-library yara ${OPENSSL_LIBRARIES})
    endif()
endif()

# plugin for VirusTotal, requires hashlib and yara
if(USE_YARA AND USE_HASHLIB)
    add_library(plugin_virustotal SHARED plugins/plugin_virustotal/plugin_virustotal.cpp
        plugins/plugin_virustotal/json_spirit/json_spirit_reader.cpp
        plugins/plugin_virustotal/json_spirit/json_spirit_value.cpp)
    target_link_libraries(plugin_virustotal manape manacommons hash-library yara ${Boost_LIBRARIES})

    # optional linking
    if(USE_OPENSSL)
        target_link_libraries(plugin_virustotal ${OPENSSL_LIBRARIES})
    endif()
endif()

# link libraries
target_link_libraries(manape manacommons ${Boost_LIBRARIES})
target_link_libraries(
    manalyze
    manacommons
    manape
    ${Boost_LIBRARIES}
)

# optionally linking libraries
if(USE_YARA)
    target_link_libraries(manalyze yara)
endif()
if(USE_HASHLIB)
    target_link_libraries(manalyze hash-library)
endif()

# make install command for linux machines:
if("${CMAKE_SYSTEM}" MATCHES "Linux")
    # Copy binaries
    install(TARGETS manalyze manacommons manape
        RUNTIME DESTINATION /usr/local/bin
        LIBRARY DESTINATION /usr/local/lib
        ARCHIVE DESTINATION /usr/local/lib/static)

    if(USE_YARA)
        # Copy the yara_rules folder to /etc/manalyze.
        install(DIRECTORY bin/yara_rules
            DESTINATION /etc/manalyze)
    endif()

    # Copy the plugins to /etc/manalyze.
    install(DIRECTORY bin/
        DESTINATION /etc/manalyze
        FILES_MATCHING PATTERN "libplugin_*.so")

    # Copy the configuration file template.
    install(FILES bin/manalyze.conf
        DESTINATION /etc/manalyze)

    # Run ldconfig.
    install(CODE "execute_process(COMMAND ldconfig)")
endif()
