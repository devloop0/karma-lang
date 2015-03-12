#ifndef KARMA_LANG_PARSE_HPP
#define KARMA_LANG_PARSE_HPP

#include "../includes/token.hpp"
#include "../includes/diagnostic_messages.hpp"
#include "../includes/diagnostics_report.hpp"
#include "../includes/lex.hpp"
#include "../includes/source_token_list.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::shared_ptr;
using std::make_shared;

using namespace karma_lang;

namespace karma_lang {

	class binary_expression;
	class root_node;
	class sequence;
	class dictionary;
	class subscript;
	class function_argument_list;
	class declaration;
	class statement;

	class parser {
		protected:
			shared_ptr<root_node> root;
		public:
			parser(shared_ptr<root_node> r);
			~parser();
			shared_ptr<root_node> get_root_node();
	};

	class root_node : public parser {
		protected:
			shared_ptr<lexer> lex;
			shared_ptr<diagnostics_reporter> dreporter;
			source_token_list::iterator pos;
			vector<shared_ptr<statement>> statement_list;
		public:
			root_node(shared_ptr<lexer> l, shared_ptr<diagnostics_reporter> dr);
			~root_node();
			source_token_list::iterator get_position();
			vector<shared_ptr<statement>> get_statement_list();
			vector<shared_ptr<statement>> parse_program(shared_ptr<parser> p);
			source_token_list::iterator set_position(source_token_list::iterator it);
			shared_ptr<diagnostics_reporter> get_diagnostics_reporter();
			shared_ptr<lexer> get_lexer();
	};

	enum literal_kind {
		LITERAL_IDENTIFIER, LITERAL_REGULAR_HEX_NUMBER, LITERAL_DECIMAL_NUMBER, LITERAL_STRING, LITERAL_BOOLEAN, LITERAL_NIL,
		LITERAL_NONE
	};

	class literal : public root_node {
		shared_ptr<token> raw_literal;
		bool valid;
		literal_kind kind;
		source_token_list::iterator literal_pos;
		public:
			literal(shared_ptr<root_node> r);
			~literal();
			shared_ptr<literal> parse_literal();
			const bool get_valid();
			const literal_kind get_literal_kind();
			shared_ptr<token> get_raw_literal();
			source_token_list::iterator get_position();
	};

	enum primary_expression_kind {
		PRIMARY_EXPRESSION_LITERAL, PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION, PRIMARY_EXPRESSION_BRACKETED_EXPRESSION, PRIMARY_EXPRESSION_SEQUENCE,
		PRIMARY_EXPRESSION_DICTIONARY, PRIMARY_EXPRESSION_NONE
	};

	class primary_expression : public root_node {
		shared_ptr<literal> raw_literal;
		bool valid;
		primary_expression_kind kind;
		shared_ptr<binary_expression> raw_parenthesized_expression;
		shared_ptr<sequence> seq;
		shared_ptr<dictionary> dict;
		source_token_list::iterator primary_expression_pos;
		public:
			primary_expression(shared_ptr<root_node> r);
			~primary_expression();
			shared_ptr<primary_expression> parse_primary_expression();
			const bool get_valid();
			const primary_expression_kind get_primary_expression_kind();
			shared_ptr<literal> get_raw_literal();
			source_token_list::iterator get_position();
			shared_ptr<binary_expression> get_raw_parenthesized_expression();
			shared_ptr<sequence> get_sequence();
			shared_ptr<dictionary> get_dictionary();
	};

	enum postfix_operation_kind {
		POSTFIX_OPERATION_INCREMENT, POSTFIX_OPERATION_DECREMENT, POSTFIX_FUNCTION_CALL, POSTFIX_DOT_OPERATOR, POSTFIX_SUBSCRIPT, POSTFIX_OPERATION_NONE
	};

	enum postfix_expression_kind {
		POSTFIX_EXPRESSION_PRIMARY_EXPRESSION, POSTFIX_EXPRESSION_POSTFIX_EXPRESSION, POSTFIX_EXPRESSION_NONE
	};

	class postfix_expression : public root_node {
		shared_ptr<primary_expression> raw_primary_expression;
		shared_ptr<function_argument_list> argument_list;
		shared_ptr<subscript> subscr;
		bool valid;
		postfix_operation_kind kind;
		postfix_expression_kind expression_kind;
		shared_ptr<postfix_expression> post_expression;
		source_token_list::iterator postfix_expression_pos;
		shared_ptr<literal> identifier;
		public:
			postfix_expression(shared_ptr<root_node> r);
			~postfix_expression();
			shared_ptr<postfix_expression> parse_postfix_expression();
			const bool get_valid();
			const postfix_operation_kind get_postfix_operation_kind();
			shared_ptr<primary_expression> get_raw_primary_expression();
			const postfix_expression_kind get_postfix_expression_kind();
			shared_ptr<postfix_expression> get_postfix_expression();
			shared_ptr<function_argument_list> get_argument_list();
			shared_ptr<literal> get_identifier();
			source_token_list::iterator get_position();
			shared_ptr<subscript> get_subscript();
	};

	enum unary_operation_kind {
		UNARY_OPERATION_PLUS, UNARY_OPERATION_MINUS, UNARY_OPERATION_NOT, UNARY_OPERATION_COMPLEMENT, UNARY_OPERATION_INCREMENT, UNARY_OPERATION_DECREMENT, UNARY_OPERATION_NONE
	};

	class unary_expression : public root_node {
		shared_ptr<postfix_expression> raw_postfix_expression;
		bool valid;
		unary_operation_kind kind;
		source_token_list::iterator unary_expression_pos;
		public:
			unary_expression(shared_ptr<root_node> r);
			~unary_expression();
			shared_ptr<unary_expression> parse_unary_expression();
			const bool get_valid();
			const unary_operation_kind get_unary_operation_kind();
			shared_ptr<postfix_expression> get_raw_postfix_expression();
			source_token_list::iterator get_position();
	};

	enum binary_operation_kind {
		BINARY_OPERATION_EXPONENT, BINARY_OPERATION_MULTIPLY, BINARY_OPERATION_DIVIDE, BINARY_OPERATION_MODULUS, BINARY_OPERATION_PLUS,
		BINARY_OPERATION_MINUS, BINARY_OPERATION_SHIFT_LEFT, BINARY_OPERATION_SHIFT_RIGHT, BINARY_OPERATION_LESS_THAN,
		BINARY_OPERATION_GREATER_THAN, BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO, BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO,
		BINARY_OPERATION_EQUALS_EQUALS, BINARY_OPERATION_NOT_EQUAL, BINARY_OPERATION_BITWISE_AND, BINARY_OPERATION_EXCLUSIVE_OR,
		BINARY_OPERATION_BITWISE_OR, BINARY_OPERATION_LOGICAL_AND, BINARY_OPERATION_LOGICAL_OR, BINARY_OPERATION_EQUALS,
		BINARY_OPERATION_PLUS_EQUALS, BINARY_OPERATION_MINUS_EQUALS, BINARY_OPERATION_MULTIPLY_EQUALS, BINARY_OPERATION_DIVIDE_EQUALS,
		BINARY_OPERATION_MODULUS_EQUALS, BINARY_OPERATION_SHIFT_LEFT_EQUALS, BINARY_OPERATION_SHIFT_RIGHT_EQUALS,
		BINARY_OPERATION_BITWISE_AND_EQUALS, BINARY_OPERATION_BITWISE_OR_EQUALS, BINARY_OPERATION_EXCLUSIVE_OR_EQUALS,
		BINARY_OPERATION_EXPONENT_EQUALS, BINARY_OPERATION_QUESTION_MARK, BINARY_OPERATION_COLON, BINARY_OPERATION_COMMA, BINARY_OPERATION_POINT, BINARY_OPERATION_NONE
	};

	enum binary_expression_kind {
		BINARY_EXPRESSION_UNARY_EXPRESSION, BINARY_EXPRESSION_BINARY_EXPRESSION, BINARY_EXPRESSION_NONE
	};

	class binary_expression : public root_node {
		shared_ptr<root_node> lhs;
		shared_ptr<root_node> rhs;
		binary_expression_kind lhs_kind;
		binary_expression_kind rhs_kind;
		binary_expression_kind expression_kind;
		binary_operation_kind operation_kind;
		bool valid;
		source_token_list::iterator binary_expression_pos;

		shared_ptr<binary_expression> exponent_expression();
		shared_ptr<binary_expression> multiplicative_expression();
		shared_ptr<binary_expression> additive_expression();
		shared_ptr<binary_expression> shift_expression();
		shared_ptr<binary_expression> relational_expression();
		shared_ptr<binary_expression> equality_expression();
		shared_ptr<binary_expression> and_expression();
		shared_ptr<binary_expression> exclusive_or_expression();
		shared_ptr<binary_expression> inclusive_or_expression();
		shared_ptr<binary_expression> logical_and_expression();
		shared_ptr<binary_expression> logical_or_expression();
		shared_ptr<binary_expression> conditional_expression();
		shared_ptr<binary_expression> assignment_expression();
		shared_ptr<binary_expression> cast_expression();
		public:
			binary_expression(shared_ptr<root_node> r);
			~binary_expression();
			shared_ptr<root_node> get_lhs();
			shared_ptr<root_node> get_rhs();
			const binary_expression_kind get_lhs_kind();
			const binary_expression_kind get_rhs_kind();
			const binary_expression_kind get_expression_kind();
			const binary_operation_kind get_binary_operation_kind();
			const bool get_valid();
			source_token_list::iterator get_position();
			shared_ptr<binary_expression> parse_binary_expression();
	};

	enum sequence_kind {
		SEQUENCE_LIST, SEQUENCE_TUPLE, SEQUENCE_NONE
	};

	class sequence : public root_node {
		sequence_kind _sequence;
		vector<shared_ptr<binary_expression>> binary_expression_list;
		bool valid;
		source_token_list::iterator sequence_pos;
		public:
			sequence(shared_ptr<root_node> r);
			~sequence();
			const bool get_valid();
			source_token_list::iterator get_position();
			const sequence_kind get_sequence_kind();
			vector<shared_ptr<binary_expression>> get_expression_list();
			shared_ptr<sequence> parse_sequence();
	};

	class dictionary : public root_node {
		vector<shared_ptr<binary_expression>> key_list;
		vector<shared_ptr<binary_expression>> value_list;
		bool valid;
		source_token_list::iterator dictionary_pos;
		public:
			dictionary(shared_ptr<root_node> r);
			~dictionary();
			const bool get_valid();
			source_token_list::iterator get_position();
			vector<shared_ptr<binary_expression>> get_key_list();
			vector<shared_ptr<binary_expression>> get_value_list();
			shared_ptr<dictionary> parse_dictionary();
	};

	enum subscript_colon_kind {
		SUBSCRIPT_COLON_ZERO, SUBSCRIPT_COLON_ONE, SUBSCRIPT_COLON_TWO, SUBSCRIPT_COLON_NONE
	};

	class subscript : public root_node {
		shared_ptr<binary_expression> start;
		shared_ptr<binary_expression> done;
		shared_ptr<binary_expression> step;
		bool valid;
		source_token_list::iterator subscript_pos;
		subscript_colon_kind sc_kind;
		public:
			subscript(shared_ptr<root_node> r);
			~subscript();
			shared_ptr<binary_expression> get_start();
			shared_ptr<binary_expression> get_final();
			shared_ptr<binary_expression> get_step();
			const bool get_valid();
			source_token_list::iterator get_position();
			shared_ptr<subscript> parse_subscript();
			const subscript_colon_kind get_subscript_colon_kind();
	};

	class function_argument_list : public root_node {
		vector<shared_ptr<binary_expression>> argument_list;
		bool valid;
		source_token_list::iterator function_argument_list_pos;
		public:
			function_argument_list(shared_ptr<root_node> r);
			~function_argument_list();
			const bool get_valid();
			source_token_list::iterator get_position();
			vector<shared_ptr<binary_expression>> get_argument_list();
			shared_ptr<function_argument_list> parse_function_argument_list();
	};

	enum declspec_kind {
		DECLSPECS_IMMUT, DECLSPECS_NONE
	};

	class declspec_list : public root_node {
		vector<declspec_kind> delsp_list;
		bool valid;
		source_token_list::iterator declspec_list_pos;
		public:
			declspec_list(shared_ptr<root_node> r);
			~declspec_list();
			const bool get_valid();
			source_token_list::iterator get_position();
			vector<declspec_kind> get_declspecs_list();
			shared_ptr<declspec_list> parse_declspec_list();
	};

	class declaration : public root_node {
		shared_ptr<literal> identifier;
		shared_ptr<declspec_list> delsp_list;
		bool valid;
		source_token_list::iterator declaration_pos;
		shared_ptr<binary_expression> expr_statement;
		public:
			declaration(shared_ptr<root_node> r);
			~declaration();
			shared_ptr<literal> get_identifier();
			shared_ptr<declspec_list> get_declspec_list();
			shared_ptr<binary_expression> get_binary_expression();
			const bool get_valid();
			source_token_list::iterator get_position();
			shared_ptr<declaration> parse_declaration();
	};

	enum statement_kind {
		STATEMENT_DECLARATION, STATEMENT_EXPRESSION, STATEMENT_NONE
	};

	class statement : public root_node {
		statement_kind kind;
		shared_ptr<binary_expression> b_expression;
		shared_ptr<declaration> decl;
		bool valid;
		source_token_list::iterator statement_pos;
		public:
			statement(shared_ptr<root_node> root);
			~statement();
			const bool get_valid();
			const statement_kind get_statement_kind();
			shared_ptr<binary_expression> get_binary_expression();
			shared_ptr<declaration> get_declaration();
			source_token_list::iterator get_position();
			shared_ptr<statement> parse_statement();
	};
}
#endif
