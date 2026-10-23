cmake_minimum_required(VERSION 3.20)
project(GCU VERSION 0.2.0 LANGUAGES CXX)

option(GCU_MINIMAL "Build only the command list in profiles/minimal.txt" OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(GCU_MINIMAL)
    set(GCU_COMMANDS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/profiles/minimal.txt")
else()
    set(GCU_COMMANDS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/commands.txt")
endif()
file(STRINGS "${GCU_COMMANDS_FILE}" GCU_COMMANDS)

add_library(gcu SHARED src/gcu.cpp src/checksum.cpp)
target_include_directories(gcu PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(gcu PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(gcu PROPERTIES
    OUTPUT_NAME gcu
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    BUILD_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH "$ORIGIN/../lib"
)

set(GCU_EXECUTABLE_TARGETS)
set(GCU_INDEX 0)
foreach(command IN LISTS GCU_COMMANDS)
    set(target "gcu_command_${GCU_INDEX}")
    add_executable("${target}" "${CMAKE_CURRENT_SOURCE_DIR}/src/commands/${command}.cpp")
    target_include_directories("${target}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_link_libraries("${target}" PRIVATE gcu)
    set_target_properties("${target}" PROPERTIES
        OUTPUT_NAME "${command}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        BUILD_RPATH "$ORIGIN/../lib"
        INSTALL_RPATH "$ORIGIN/../lib"
    )
    list(APPEND GCU_EXECUTABLE_TARGETS "${target}")
    math(EXPR GCU_INDEX "${GCU_INDEX} + 1")
endforeach()

install(TARGETS gcu LIBRARY DESTINATION lib)
install(TARGETS ${GCU_EXECUTABLE_TARGETS} RUNTIME DESTINATION bin)
