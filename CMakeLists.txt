cmake_minimum_required(VERSION 3.22)
project(FTL-Translator)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES
        src/main.cpp
        src/trie.cpp
        src/token.cpp
        src/lexical_analyzer.cpp
)

set(HEADER_FILES
        include/trie.h
        include/token.h
        include/lexical_analyzer.h
)

add_executable(FTL-Translator ${SOURCE_FILES} ${HEADER_FILES})
