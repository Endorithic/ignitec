cmake_minimum_required(VERSION 3.20)

project(
    ignitec
    VERSION 0.0.0
    LANGUAGES CXX
)

add_executable( ignitec
    src/main.cpp
    src/get_config.cpp
    src/logging.cpp
)

set_target_properties( ignitec PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF

    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/target
)

target_include_directories( ignitec PRIVATE
    include/
)
