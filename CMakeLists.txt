project("karma-lang")

set(CMAKE_CXX_FLAGS "--std=c++11")

add_executable(karma main.cpp src/comment_remove.cpp src/diagnostic_messages.cpp src/diagnostics_report.cpp src/lex.cpp src/parse.cpp src/source_token_list.cpp src/token_analyze.cpp
	src/token.cpp src/token_keywords.cpp src/semantics.cpp)
