set(CMAKE_CXX_STANDARD 20)
add_library(SearchEngine
                Indexer.h
                Indexer.cpp
                Searcher.cpp
                Searcher.h
                VarInt.h
                InfoStructures.h
                SyntaxParser.h
                SyntaxParser.cpp
                TermSearch.cpp
                TermSearch.h)
set_target_properties(SearchEngine PROPERTIES LINKER_LANGUAGE CXX)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC SearchEngine)