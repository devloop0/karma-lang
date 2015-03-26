#include "../includes/diagnostic_messages.hpp"

using namespace karma_lang;

namespace karma_lang {

	const string diagnostic_messages::malformed_token = "Malformed token.";
	const string diagnostic_messages::malformed_primary_expression = "Unwinding stack: Expected primary-expression (string, number, identifier, or parenthesized expression).";
	const string diagnostic_messages::malformed_binary_expression = "Unwinding stack: Expected a binary-expression, with a binary operator ('*', '/', '+', '-', etc).";
	const string diagnostic_messages::malformed_ternary_expression = "Unwinding stack: Expected a ternary-expression, with the ternary operators ('?', ':').";
	const string diagnostic_messages::malformed_unary_expression = "Unwinding stack: Expected a primary-expression with a unary operator (one of '~', '-', '+', or '!').";
	const string diagnostic_messages::expected_close_parenthesis = "Expected a close parenthesis ( ')' ), at the end of the expression.";
	const string diagnostic_messages::unreachable = "Internal compiler error. This point should be unreachable.";
	const string diagnostic_messages::expected_new_line = "Expected a new line character ('\\n') after this token-sequence.";
	const string diagnostic_messages::unexpected_end_of_file = "Unexpected end of file; terminating.";
	const string diagnostic_messages::malformed_expression = "Error with this expression; (did you forget a '\\n', new-line character at the end)?";
	const string diagnostic_messages::expected_close_tuple = "Expected a close tuple ('>>>'), at the end of the expression.";
	const string diagnostic_messages::expected_close_list = "Expected a close list (']]]'), at the end of the expression.";
	const string diagnostic_messages::expected_close_dict = "Expected a close dict ('}}}'), at the end of the expression.";
	const string diagnostic_messages::expected_a_colon_to_separate_key_value_pairs = "Expected a colon (':') to separate dictionary key-value pairs.";
	const string diagnostic_messages::expected_close_bracket = "Expected a close bracket (']'), at the the end of the subscript expression.";
	const string diagnostic_messages::explicit_empty_step_size = "Explicitly used a step size but left it empty; defaulting to '1'.";
	const string diagnostic_messages::expected_a_point_to = "Expected a point-to ('->') here.";
	const string diagnostic_messages::expected_an_equals = "Expected an equals ('=') here (or a new-line ('\\n')).";
	const string diagnostic_messages::expected_open_bracket = "Expected an open-bracket ('['), here.";
	const string diagnostic_messages::expected_an_identifier = "Expected an identifier.";
	const string diagnostic_messages::expected_only_one_immut = "Expected only one 'immut' qualifier; defaulting to one 'immut'.";
	const string diagnostic_messages::repeated_declaration_ignoring_this_one = "Repeated variable declaration here.";
	const string diagnostic_messages::originally_declared_here =  "Originally declared here.";
	const string diagnostic_messages::incompatible_types = "Incompatible types with this expression; expected compatible types.";
	const string diagnostic_messages::unknown_symbol = "Unknown symbol found here. (Did you forget to declare it somewhere)?: ";
	const string diagnostic_messages::expected_at_least_one_element_in_sequence = "Expected at least one element in the list, tuple, or dictionary to its deduce type; defaulting to 'int'.";
	const string diagnostic_messages::expected_same_type_for_all_elements_in_list_or_dictionary = "Expected all elements in the list or dictionary to have the same type.";
	const string diagnostic_messages::subscript_applicable_only_for_sequences_and_dictionaries = "Subscripts are only applicable for lists, dictionaries, strings, and tuples.";
	const string diagnostic_messages::empty_subscript_defaulting_to_whole_sequence = "Empty subscript; defaulting to whole sequence.";
	const string diagnostic_messages::expected_subscript_for_sequence_to_be_integer = "Expected a sequence subscript to be an integer. (Did you mean to declare a dictionary)?";
	const string diagnostic_messages::dictionaries_only_support_subscripting_not_slicing = "Dictionaries only support subscripting, not slicing.";
	const string diagnostic_messages::expected_dictionary_subscript_to_equal_its_key_type = "Expected a dictionary subscript to equal its key type.";
	const string diagnostic_messages::unequal_but_compatible_types = "The types of this binary expression are unequal but compatible; continuing.";
	const string diagnostic_messages::expected_lvalue = "For an assignment expression, expected to find an lvalue.";
	const string diagnostic_messages::immut_value_cannot_be_modified = "An immutable variable cannot be modified.";
	const string diagnostic_messages::unequal_but_compatible_types_list_dict = "Unequal, but compatible types; warning for lists or dictionaries.";
	const string diagnostic_messages::expected_lvalue_for_increments_and_decrements = "For increments and decrements, an lvalue is expected.";
	const string diagnostic_messages::expected_boolean = "Expected a boolean expression here.";
	const string diagnostic_messages::instruction_not_supported = "Instruction not supported; terminating.";
	const string diagnostic_messages::tuples_cannot_be_modified = "Tuples can be read from, but cannot be modified or written to.";
}
