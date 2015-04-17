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
			const static string explicit_empty_step_size;
			const static string expected_a_point_to;
			const static string expected_an_equals;
			const static string expected_open_bracket;
			const static string expected_an_identifier;
			const static string expected_only_one_immut;
			const static string repeated_declaration_ignoring_this_one;
			const static string originally_declared_here;
			const static string incompatible_types;
			const static string unknown_symbol;
			const static string expected_at_least_one_element_in_sequence;
			const static string expected_same_type_for_all_elements_in_list_or_dictionary;
			const static string subscript_applicable_only_for_sequences_and_dictionaries;
			const static string empty_subscript_defaulting_to_whole_sequence;
			const static string expected_subscript_for_sequence_to_be_integer;
			const static string dictionaries_only_support_subscripting_not_slicing;
			const static string expected_dictionary_subscript_to_equal_its_key_type;
			const static string unequal_but_compatible_types;
			const static string expected_lvalue;
			const static string immut_value_cannot_be_modified;
			const static string unequal_but_compatible_types_list_dict;
			const static string expected_lvalue_for_increments_and_decrements;
			const static string expected_boolean;
			const static string instruction_not_supported;
			const static string tuples_cannot_be_modified;
			const static string expected_boolean_for_ternary_expression_condition;
			const static string already_a_pure_type_ignoring;
	};
}

#endif
