#ifndef KARMA_LANG_DIAGNOSTIC_MESSAGES_HPP
#define KARMA_LANG_DIAGNOSTIC_MESSAGES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <memory>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::shared_ptr;

namespace karma_lang {

	class diagnostic_messages {
		public:
			const static string malformed_token;
			const static string malformed_primary_expression; 
			const static string malformed_binary_expression; 
			const static string malformed_ternary_expression;
			const static string malformed_unary_expression;
			const static string expected_close_parenthesis;
			const static string unreachable;
			const static string expected_new_line;
			const static string unexpected_end_of_file;
			const static string malformed_expression;
			const static string expected_close_tuple;
			const static string expected_close_list;
			const static string expected_close_dict;
			const static string expected_a_colon_to_separate_key_value_pairs;
			const static string expected_close_bracket;
			const static string expected_an_expression_defaulting_to_whole_sequence;
			const static string explicit_empty_step_size;
			const static string expected_a_point_to;
			const static string expected_an_equals;
			const static string expected_open_bracket;
	};
}

#endif
