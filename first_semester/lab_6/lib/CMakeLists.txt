add_library(Parser
        ArgParser.cpp
        ArgParser.h
        Argument.h
        )

add_library(HamArc HamArc.cpp HamArc.h
        Hamming.cpp
        Hamming.h
        ByteAndBitOperations.cpp
        ByteAndBitOperations.h
        FileOperation.cpp
        FileOperation.h
)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC HamArc Parser)
