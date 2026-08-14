cmake_minimum_required(VERSION 3.10)
project(EnigmaProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(enigma_core
    src/Rotor.cpp
    src/Reflector.cpp
    src/EnigmaMachine.cpp
    src/EnigmaConfigurator.cpp
    src/Plugboard.cpp
)

add_executable(enigma_cli
    tools/enigma_cli/EnigmaCLI.cpp
)
target_link_libraries(enigma_cli enigma_core)

add_executable(enigma_setup
    tools/enigma_setup/EnigmaSetup.cpp
)
target_link_libraries(enigma_setup enigma_core)

set_target_properties(enigma_cli enigma_setup
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)