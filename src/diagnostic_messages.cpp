#include "includes/diagnostic_messages.hpp"

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
	const string diagnostic_messages::expected_same_type_for_all_elements_in_list_or_dictionary = "Expected all elements in the list or dictionary to have the same type.";
	const string diagnostic_messages::empty_subscript_defaulting_to_whole_sequence = "Empty subscript; defaulting to whole sequence.";
	const string diagnostic_messages::expected_subscript_for_sequence_to_be_integer = "Expected a sequence subscript to be an integer. (Did you mean to declare a dictionary)?";
	const string diagnostic_messages::dictionaries_only_support_subscripting_not_slicing = "Dictionaries only support subscripting, not slicing.";
	const string diagnostic_messages::expected_dictionary_subscript_to_equal_its_key_type = "Expected a dictionary subscript to equal its key type.";
	const string diagnostic_messages::unequal_but_compatible_types = "The types of this binary expression are unequal but compatible; continuing.";
	const string diagnostic_messages::expected_lvalue = "For an assignment expression, expected to find an lvalue.";
	const string diagnostic_messages::immut_value_cannot_be_modified = "An immutable variable cannot be modified.";
	const string diagnostic_messages::unequal_but_compatible_types_list_dict = "Unequal, but compatible types; warning for lists and dictionaries.";
	const string diagnostic_messages::expected_lvalue_for_increments_and_decrements = "For increments and decrements, an lvalue is expected.";
	const string diagnostic_messages::expected_boolean = "Expected a boolean expression here.";
	const string diagnostic_messages::instruction_not_supported = "Instruction not supported; terminating.";
	const string diagnostic_messages::expected_boolean_for_ternary_expression_condition = "Expected a boolean expression for a ternary expression condition.";
	const string diagnostic_messages::already_a_pure_type_ignoring = "Expression is already a pure type; continuing.";
	const string diagnostic_messages::expected_a_func_keyword_to_open_a_function = "Expected a 'func' keyword to open a function.";
	const string diagnostic_messages::expected_an_open_parenthesis = "Expected an open parenthesis '('.";
	const string diagnostic_messages::expected_an_open_brace = "Expected an open brace '{'.";
	const string diagnostic_messages::repeated_declaration_of_a_function_parameter = "Repeated declaration of a function parameter: ";
	const string diagnostic_messages::function_definitions_cannot_be_nested = "Function definitions cannot be nested.";
	const string diagnostic_messages::function_declaration_does_not_match_definition = "Function declaration does not match declaration.";
	const string diagnostic_messages::function_is_redeclared = "Function is redeclared.";
	const string diagnostic_messages::function_declared_not_defined = "Function declared but not defined.";
	const string diagnostic_messages::structure_definitions_cannot_be_nested = "Structure definitions cannot be nested.";
	const string diagnostic_messages::structure_members_cannot_be_immut = "Structure members cannot be 'immut.'";
	const string diagnostic_messages::partial_declarations_not_allowed = "Partial declarations are not allowed.";
	const string diagnostic_messages::complete_declarations_not_allowed = "Complete declarations are not allowed.";
	const string diagnostic_messages::structure_declared_not_defined = "Structure declared but not defined.";
	const string diagnostic_messages::structure_declarations_need_pure_types = "Structure declarations need to be pure types.";
	const string diagnostic_messages::builtins_cannot_be_redefined = "Builtins cannot be redefined.";
	const string diagnostic_messages::ambiguous_function_overload = "Function definition could lead to ambiguous function overload.";
	const string diagnostic_messages::modules_cannot_be_in_functions = "Modules cannot be in functions.";
	const string diagnostic_messages::return_statements_can_only_be_in_functions = "Return statements can only be present in function scopes.";
	const string diagnostic_messages::modules_functions_structures_enums_not_expected_here = "Modules, functions, structures, and enums are not expected here.";
	const string diagnostic_messages::immut_not_allowed_for_function_parameters = "'immut' not allowed for function parameters.";
	const string diagnostic_messages::enum_declared_but_not_defined = "Enum declared but not defined.";
	const string diagnostic_messages::mismatched_scopes = "Mismatched scopes.";
	const string diagnostic_messages::enum_names_must_be_unique = "Enum names must be unique.";
	const string diagnostic_messages::enums_not_allowed_in_functions = "Enums not allowed in functions.";
	const string diagnostic_messages::expected_close_brace = "Expected close brace here.";
	const string diagnostic_messages::error_parsing_statement = "Error parsing statement.";
	const string diagnostic_messages::immut_not_allowed_for_for_loop_declaration = "Immut not allowed for, for loop declarations.";
	const string diagnostic_messages::lambdas_cannot_be_partially_declared = "Lambdas cannot be partially declared.";
	const string diagnostic_messages::break_and_continue_statements_not_allowed_here = "Break and continue statements not allowed here.";
	const string diagnostic_messages::break_and_continue_statements_only_allowed_in_loops = "Break and continue statements are only allowed in loops.";
	const string diagnostic_messages::module_declared_but_not_defined = "Module declared but not defined.";
	const string diagnostic_messages::overloaded_operator_global = "Overloaded operator can only be present in the global scope.";
	const string diagnostic_messages::two_parameters_for_a_binary_operator_overload = "Two parameters are expected to overload a binary operator.";
	const string diagnostic_messages::one_parameter_for_a_unary_operator_overload = "One parameter is expected to overload a unary operator.";
	const string diagnostic_messages::overloaded_operators_must_be_immutable = "Overloaded operators must be immutable.";
	const string diagnostic_messages::print_overload_must_have_one_parameter = "Print overload must have one parameter.";
	const string diagnostic_messages::slice_overload_must_have_four_parameters = "Slice overload must have four parameters.";
	const string diagnostic_messages::import_statements_only_allowed_in_the_global_scope = "Import statements only allowed in the global scope.";
	const string diagnostic_messages::subscript_overload_must_have_two_parameters = "Subscript overload must have two parameter.";
	const string diagnostic_messages::builtin_functions_cannot_be_redefined_or_overloaded = "Builtin functions cannot be redefined or overloaded.";
}
