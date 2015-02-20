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
	const string diagnostic_messages::expected_an_expression_defaulting_to_whole_sequence = "Expected an expression in a sequence slice; defaulting to whole sequence.";
	const string diagnostic_messages::explicit_empty_step_size = "Explicitly used a step size but left it empty; defaulting to '1'.";
	const string diagnostic_messages::expected_a_point_to = "Expected a point-to ('->') here.";
	const string diagnostic_messages::expected_an_equals = "Expected an equals ('=') here (or a new-line ('\\n')).";
	const string diagnostic_messages::expected_open_bracket = "Expected an open-bracket ('['), here.";
}
