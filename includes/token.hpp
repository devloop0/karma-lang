#ifndef KARMA_LANG_TOKEN_HPP
#define KARMA_LANG_TOKEN_HPP

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

namespace karma_lang {
	
	enum token_kind {
		TOKEN_EMPTY, TOKEN_IDENTIFIER, TOKEN_EQUALS, TOKEN_STRING, TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
		TOKEN_EXPONENT, TOKEN_SHIFT_LEFT, TOKEN_BITWISE_AND, TOKEN_BITWISE_OR, TOKEN_COMPLEMENT, TOKEN_BITWISE_EXCLUSIVE_OR,
		TOKEN_PLUS_EQUALS, TOKEN_MINUS_EQUALS, TOKEN_MULTIPLY_EQUALS, TOKEN_DIVIDE_EQUALS, TOKEN_EXPONENT_EQUALS, TOKEN_MODULUS, 
		TOKEN_MODULUS_EQUALS, TOKEN_SHIFT_LEFT_EQUALS, TOKEN_BITWISE_AND_EQUALS, TOKEN_BITWISE_OR_EQUALS, TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS,
		TOKEN_SHIFT_RIGHT, TOKEN_SHIFT_RIGHT_EQUALS, TOKEN_LOGICAL_AND, TOKEN_LOGICAL_OR, TOKEN_GREATER_THAN, TOKEN_LESS_THAN, 
		TOKEN_GREATER_THAN_OR_EQUAL_TO, TOKEN_LESS_THAN_OR_EQUAL_TO, TOKEN_NOT_EQUALS, TOKEN_DOUBLE_EQUALS, TOKEN_EXCLAMATION, 
		TOKEN_OPEN_PARENTHESIS, TOKEN_CLOSE_PARENTHESIS, TOKEN_INCREMENT, TOKEN_DECREMENT, TOKEN_QUESTION_MARK, TOKEN_COLON, TOKEN_COMMA, TOKEN_DOT,
		TOKEN_TRUE, TOKEN_FALSE, TOKEN_OPEN_TUPLE, TOKEN_CLOSE_TUPLE, TOKEN_OPEN_LIST, TOKEN_CLOSE_LIST, TOKEN_OPEN_DICT, TOKEN_CLOSE_DICT,
		TOKEN_OPEN_BRACKET, TOKEN_CLOSE_BRACKET, TOKEN_OPEN_BRACE, TOKEN_CLOSE_BRACE, TOKEN_VAR, TOKEN_IMMUT, TOKEN_NIL, TOKEN_POINT,

		/* number kinds*/
		TOKEN_HEX_NUMBER, TOKEN_REGULAR_NUMBER, TOKEN_DECIMAL_NUMBER, 
		
		TOKEN_NEW_LINE, TOKEN_ERROR
	};

	class token {
		int tab_count;
		int column_start;
		int column_end;
		string raw_string;
		token_kind kind;
		string file_name;
		int line_number;
	public:
		token(int tc, int cs, int ce, string rs, string f, int l, token_kind k = token_kind::TOKEN_EMPTY);
		~token();
		const int get_tab_count();
		const int get_column_start();
		const int get_column_end();
		const string get_raw_string();
		const token_kind get_token_kind();
		token_kind set_token_kind(token_kind k);
		const string get_file_name();
		const int get_line_number();
	};	
}

#endif
