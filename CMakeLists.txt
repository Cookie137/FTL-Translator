# cmake_minimum_required(VERSION 3.22)
# project(FTL-Translator)

# set(CMAKE_CXX_STANDARD 23)

# set(SOURCE_FILES
#         src/main.cpp
#         src/trie.cpp
#         src/token.cpp
#         src/lexical_analyzer.cpp
# )

# set(HEADER_FILES
#         include/trie.h
#         include/token.h
#         include/lexical_analyzer.h
# )

# add_executable(FTL-Translator ${SOURCE_FILES} ${HEADER_FILES})
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)

cmake_minimum_required(VERSION 3.18)
project(Elang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/lib"
)

file(
    GLOB
    SOURCES
    "src/**/*.cpp"
    "src/*.cpp"
)

set(
    PROJECT_LINK_LIBS
)

add_executable("${PROJECT_NAME}" ${SOURCES})
target_link_libraries("${PROJECT_NAME}" ${PROJECT_LINK_LIBS})
