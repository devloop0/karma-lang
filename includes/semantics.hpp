#ifndef KARMA_LANG_SEMANTICS_HPP
#define KARMA_LANG_SEMANTICS_HPP

#include "../includes/parse.hpp"
#include "../includes/lex.hpp"
#include "../includes/token.hpp"
#include "../includes/token_keywords.hpp"
#include "../includes/diagnostic_messages.hpp"
#include "../includes/diagnostics_report.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <tuple>

using std::cout;
using std::string;
using std::vector;
using std::static_pointer_cast;
using std::make_tuple;
using std::tuple;
using std::get;
using std::pair;
using std::make_pair;

using namespace karma_lang;

namespace karma_lang {

	enum type_kind {
		TYPE_INT, TYPE_DECIMAL, TYPE_STRING, TYPE_FUNCTION, TYPE_BOOLEAN, TYPE_NIL, TYPE_ANY, TYPE_CUSTOM, TYPE_LIST, TYPE_TUPLE, TYPE_DICT, TYPE_MODULE, 
		TYPE_ENUM, TYPE_ENUM_CHILD, TYPE_NONE
	};

	enum type_pure_kind {
		TYPE_PURE_YES, TYPE_PURE_NO, TYPE_PURE_NONE
	};

	enum immut_kind {
		IMMUT_YES, IMMUT_NO, IMMUT_NONE
	};

	enum function_kind {
		FUNCTION_YES, FUNCTION_NO, FUNCTION_NONE
	};

	enum structure_kind {
		STRUCTURE_YES, STRUCTURE_NO, STRUCTURE_NONE
	};

	enum type_class_kind {
		TYPE_CLASS_NO, TYPE_CLASS_YES, TYPE_CLASS_NONE
	};

	enum module_kind {
		MODULE_YES, MODULE_NO, MODULE_NONE
	};

	enum enum_kind {
		ENUM_YES, ENUM_NO, ENUM_NONE
	};

	enum value_kind {
		VALUE_LVALUE, VALUE_RVALUE, VALUE_NOT_APPLICABLE, VALUE_NONE
	};

	class type_information {
		type_kind t_kind;
		type_pure_kind tp_kind;
		type_class_kind c_kind;
		value_kind v_kind;
		shared_ptr<type_information> return_type_information;
		shared_ptr<type_information> list_information;
		shared_ptr<type_information> key_information;
		shared_ptr<type_information> value_information;
		vector<shared_ptr<type_information>> tuple_information;
		shared_ptr<literal> lit;
		string class_name;
		public:
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck);
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk);
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, string cn);
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, shared_ptr<type_information> li);
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, shared_ptr<type_information> ki, shared_ptr<type_information> vi);
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, vector<shared_ptr<type_information>> ti);
			type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, type_information rti);
			type_information(type_information t_inf, value_kind vk);
			type_information(type_information t_inf, shared_ptr<literal> l);
			~type_information();
			const type_kind get_type_kind();
			const type_pure_kind get_type_pure_kind();
			const type_class_kind get_type_class_kind();
			const bool operator==(type_information t_inf);
			const bool operator!=(type_information t_inf);
			const value_kind get_value_kind();
			shared_ptr<type_information> get_list_information();
			shared_ptr<type_information> get_key_information();
			shared_ptr<type_information> get_value_information();
			vector<shared_ptr<type_information>> get_tuple_information();
			shared_ptr<literal> get_literal();
			string get_class_name();
	};

	class annotated_binary_expression;
	class annotated_statement;
	class annotated_sequence;
	class annotated_dictionary;
	class annotated_subscript;
	class linearized_postfix_expression;

	class annotated_root_node {
		protected:
			shared_ptr<lexer> lex;
			shared_ptr<diagnostics_reporter> dreporter;
			source_token_list::iterator pos;
			vector<shared_ptr<annotated_statement>> statement_list;
		public:
			annotated_root_node(shared_ptr<root_node> r, vector<shared_ptr<annotated_statement>> asl);
			~annotated_root_node();
			source_token_list::iterator get_position();
			vector<shared_ptr<annotated_statement>> get_annotated_statement_list();
			source_token_list::iterator set_position(source_token_list::iterator it);
			shared_ptr<diagnostics_reporter> get_diagnostics_reporter();
			shared_ptr<lexer> get_lexer();
			shared_ptr<annotated_statement> add_annotated_statement(shared_ptr<annotated_statement> as);
	};

	class annotated_literal : public annotated_root_node {
		shared_ptr<token> raw_literal;
		literal_kind kind;
		source_token_list::iterator literal_pos;
		type_information t_inf;
		public:
			annotated_literal(shared_ptr<annotated_root_node> arn, shared_ptr<literal> lit, type_information ti);
			~annotated_literal();
			const literal_kind get_literal_kind();
			shared_ptr<token> get_raw_literal();
			source_token_list::iterator get_position();
			type_information get_type_information();
	};

	class annotated_primary_expression : public annotated_root_node {
		shared_ptr<annotated_literal> raw_literal;
		primary_expression_kind kind;
		shared_ptr<annotated_binary_expression> raw_parenthesized_expression;
		shared_ptr<annotated_sequence> seq;
		shared_ptr<annotated_dictionary> dict;
		source_token_list::iterator primary_expression_pos;
		type_information t_inf;
		public:
			annotated_primary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<primary_expression> prexpr, shared_ptr<annotated_literal> l,
				shared_ptr<annotated_binary_expression> abe, shared_ptr<annotated_sequence> as, shared_ptr<annotated_dictionary> ad,
				type_information ti);
			~annotated_primary_expression();
			const primary_expression_kind get_primary_expression_kind();
			shared_ptr<annotated_literal> get_raw_literal();
			source_token_list::iterator get_position();
			shared_ptr<annotated_binary_expression> get_raw_parenthesized_expression();
			shared_ptr<annotated_sequence> get_sequence();
			shared_ptr<annotated_dictionary> get_dictionary();
			type_information get_type_information();
	};

	class annotated_linearized_postfix_expression : public annotated_root_node {
		shared_ptr<annotated_primary_expression> aprexpr;
		vector<shared_ptr<annotated_root_node>> annotated_root_node_list;
		vector<postfix_operation_kind> postfix_operation_kind_list;
		source_token_list::iterator postfix_expression_pos;
		vector<type_information> t_inf_list;
		type_information t_inf;
		public:
			annotated_linearized_postfix_expression(shared_ptr<annotated_root_node> arn, shared_ptr<linearized_postfix_expression> lpe, shared_ptr<annotated_primary_expression> ape,
				vector<shared_ptr<annotated_root_node>> arnl, vector<postfix_operation_kind> pokl, vector<type_information> til,
				type_information ti);
			~annotated_linearized_postfix_expression();
			vector<postfix_operation_kind> get_postfix_operation_kind_list();
			shared_ptr<annotated_primary_expression> get_raw_annotated_primary_expression();
			source_token_list::iterator get_position();
			vector<shared_ptr<annotated_root_node>> get_annotated_root_node_list();
			vector<type_information> get_type_information_list();
			type_information get_type_information();
	};

	class annotated_unary_expression : public annotated_root_node {
		shared_ptr<annotated_linearized_postfix_expression> raw_postfix_expression;
		unary_operation_kind kind;
		source_token_list::iterator unary_expression_pos;
		type_information t_inf;
		type_information forced_t_inf;
		public:
			annotated_unary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<unary_expression> uexpr,
				shared_ptr<annotated_linearized_postfix_expression> alpe,type_information ti, type_information fti);
			~annotated_unary_expression();
			const unary_operation_kind get_unary_operation_kind();
			shared_ptr<annotated_linearized_postfix_expression> get_raw_annotated_linearized_postfix_expression();
			source_token_list::iterator get_position();
			type_information get_type_information();
			type_information get_forced_type_information();
	};

	class annotated_ternary_expression : public annotated_root_node {
		shared_ptr<annotated_root_node> condition;
		shared_ptr<annotated_root_node> true_path;
		shared_ptr<annotated_root_node> false_path;
		source_token_list::iterator ternary_expression_pos;
		type_information t_inf;
		type_information forced_t_inf;
		public:
			annotated_ternary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<ternary_expression> texpr, shared_ptr<annotated_root_node> c, shared_ptr<annotated_root_node> tp, shared_ptr<annotated_root_node> fp,
					type_information ti, type_information fti);
			~annotated_ternary_expression();
			shared_ptr<annotated_root_node> get_condition();
			shared_ptr<annotated_root_node> get_true_path();
			shared_ptr<annotated_root_node> get_false_path();
			source_token_list::iterator get_position();
			type_information get_type_information();
			type_information get_forced_type_information();
	};

	class annotated_binary_expression : public annotated_root_node {
		shared_ptr<annotated_root_node> lhs;
		shared_ptr<annotated_root_node> rhs;
		binary_expression_kind lhs_kind;
		binary_expression_kind rhs_kind;
		binary_expression_kind expression_kind;
		binary_operation_kind operation_kind;
		source_token_list::iterator binary_expression_pos;
		type_information lhs_t_inf;
		type_information lhs_forced_t_inf;
		type_information rhs_t_inf;
		type_information rhs_forced_t_inf;
		type_information t_inf;
		type_information forced_t_inf;
		public:
			annotated_binary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<binary_expression> bexpr, shared_ptr<annotated_root_node> l,
				shared_ptr<annotated_root_node> r, type_information lti, type_information lfti, type_information rti, type_information rtfi,
				type_information ti, type_information fti);
			~annotated_binary_expression();
			shared_ptr<annotated_root_node> get_lhs();
			shared_ptr<annotated_root_node> get_rhs();
			const binary_expression_kind get_lhs_kind();
			const binary_expression_kind get_rhs_kind();
			const binary_expression_kind get_expression_kind();
			const binary_operation_kind get_binary_operation_kind();
			source_token_list::iterator get_position();
			type_information get_lhs_type_information();
			type_information get_lhs_forced_type_information();
			type_information get_rhs_type_information();
			type_information get_rhs_forced_type_information();
			type_information get_type_information();
			type_information get_forced_type_information();
			type_information set_forced_type_information(type_information t_inf);
			type_information set_lhs_forced_type_information(type_information t_inf);
			type_information set_rhs_forced_type_information(type_information t_inf);
	};

	class annotated_sequence : public annotated_root_node {
		sequence_kind _sequence;
		vector<shared_ptr<annotated_binary_expression>> annotated_binary_expression_list;
		source_token_list::iterator sequence_pos;
		vector<type_information> t_inf_list;
		type_information t_inf;
		public:
			annotated_sequence(shared_ptr<annotated_root_node> arn, shared_ptr<sequence> seq, vector<shared_ptr<annotated_binary_expression>> abel,
				vector<type_information> til, type_information ti);
			~annotated_sequence();
			source_token_list::iterator get_position();
			const sequence_kind get_sequence_kind();
			vector<shared_ptr<annotated_binary_expression>> get_annotated_binary_expression_list();
			vector<type_information> get_type_information_list();
			type_information get_type_information();
	};

	class annotated_dictionary : public annotated_root_node {
		vector<shared_ptr<annotated_binary_expression>> key_list;
		vector<shared_ptr<annotated_binary_expression>> value_list;
		source_token_list::iterator dictionary_pos;
		vector<type_information> key_t_inf_list;
		vector<type_information> value_t_inf_list;
		type_information t_inf;
		public:
			annotated_dictionary(shared_ptr<annotated_root_node> r, shared_ptr<dictionary> dict, vector<shared_ptr<annotated_binary_expression>> klist,
				vector<shared_ptr<annotated_binary_expression>> vlist, vector<type_information> ktil, vector<type_information> vtil, type_information ti);
			~annotated_dictionary();
			source_token_list::iterator get_position();
			vector<shared_ptr<annotated_binary_expression>> get_key_list();
			vector<shared_ptr<annotated_binary_expression>> get_value_list();
			vector<type_information> get_key_type_information_list();
			vector<type_information> get_value_type_information_list();
			type_information get_type_information();
	};

	class annotated_function_argument_list : public annotated_root_node {
		vector<shared_ptr<annotated_binary_expression>> argument_list;
		source_token_list::iterator function_argument_list_pos;
		vector<type_information> type_information_list;
		public:
			annotated_function_argument_list(shared_ptr<annotated_root_node> arn, shared_ptr<function_argument_list> fargl, vector<shared_ptr<annotated_binary_expression>> abexpr_list,
				vector<type_information> t_inf_list);
			~annotated_function_argument_list();
			vector<shared_ptr<annotated_binary_expression>> get_argument_list();
			source_token_list::iterator get_position();
			vector<type_information> get_type_information_list();
	};

	class annotated_subscript : public annotated_root_node {
		shared_ptr<annotated_binary_expression> start;
		shared_ptr<annotated_binary_expression> done;
		shared_ptr<annotated_binary_expression> step;
		source_token_list::iterator subscript_pos;
		subscript_colon_kind sc_kind;
		type_information start_t_inf;
		type_information done_t_inf;
		type_information step_t_inf;
		public:
			annotated_subscript(shared_ptr<annotated_root_node> arn, shared_ptr<subscript> subscr,
				shared_ptr<annotated_binary_expression> s, shared_ptr<annotated_binary_expression> d, 
				shared_ptr<annotated_binary_expression> ste, type_information sti, type_information dti,
				type_information steti);
			~annotated_subscript();
			shared_ptr<annotated_binary_expression> get_start();
			shared_ptr<annotated_binary_expression> get_final();
			shared_ptr<annotated_binary_expression> get_step();
			source_token_list::iterator get_position();
			const subscript_colon_kind get_subscript_colon_kind();
			type_information get_start_type_information();
			type_information get_final_type_information();
			type_information get_step_type_information();
	};

	class annotated_declaration : public annotated_root_node {
		shared_ptr<annotated_literal> identifier;
		shared_ptr<declspec_list> delsp_list;
		source_token_list::iterator declaration_pos;
		shared_ptr<annotated_binary_expression> expr_statement;
		type_information t_inf;
		bool immutable;
		public:
			annotated_declaration(shared_ptr<annotated_root_node> arn, shared_ptr<declaration> decl,
				shared_ptr<annotated_literal> al, shared_ptr<annotated_binary_expression> abe,
				type_information ti);
			~annotated_declaration();
			shared_ptr<annotated_literal> get_identifier();
			shared_ptr<declspec_list> get_declspec_list();
			shared_ptr<annotated_binary_expression> get_binary_expression();
			source_token_list::iterator get_position();
			type_information get_type_information();
			const bool get_immutable();
	};

	class annotated_function : public annotated_root_node {
		shared_ptr<annotated_literal> identifier;
		shared_ptr<declspec_list> delsp_list;
		source_token_list::iterator function_pos;
		vector<shared_ptr<annotated_declaration>> parameter_list;
		vector<shared_ptr<annotated_statement>> statement_list;
		type_information t_inf;
		function_declaration_definition_kind fdd_kind;
		function_va_args_kind fva_kind;
		bool immutable;
		public:
			annotated_function(shared_ptr<annotated_root_node> arn, shared_ptr<function> func,
				shared_ptr<annotated_literal> alit, vector<shared_ptr<annotated_declaration>> parm_list,
				vector<shared_ptr<annotated_statement>> stmt_list, function_declaration_definition_kind fddk, type_information ti);
			~annotated_function();
			shared_ptr<annotated_literal> get_identifier();
			shared_ptr<declspec_list> get_declspec_list();
			source_token_list::iterator get_position();
			vector<shared_ptr<annotated_declaration>> get_parameter_list();
			vector<shared_ptr<annotated_statement>> get_statement_list();
			type_information get_type_information();
			const function_declaration_definition_kind get_function_declaration_definition_kind();
			const function_va_args_kind get_function_va_args_kind();
			const bool get_immutable();
	};

	class annotated_structure : public annotated_root_node {
		shared_ptr<annotated_literal> identifier;
		vector<shared_ptr<annotated_declaration>> declaration_list;
		source_token_list::iterator structure_pos;
		type_information t_inf;
		vector<type_information> type_information_list;
		structure_declaration_definition_kind sdd_kind;
		bool immutable;
		public:
			annotated_structure(shared_ptr<annotated_root_node> arn, shared_ptr<structure> struc,
				shared_ptr<annotated_literal> alit, vector<shared_ptr<annotated_declaration>> adecl_list, structure_declaration_definition_kind sddk,
				vector<type_information> til, type_information ti);
			~annotated_structure();
			shared_ptr<annotated_literal> get_identifier();
			vector<shared_ptr<annotated_declaration>> get_declaration_list();
			source_token_list::iterator get_position();
			type_information get_type_information();
			vector<type_information> get_type_information_list();
			const structure_declaration_definition_kind get_structure_declaration_definition_kind();
			const bool get_immutable();
	};

	class annotated_module : public annotated_root_node {
		shared_ptr<annotated_literal> identifier;
		vector<shared_ptr<annotated_statement>> statement_list;
		source_token_list::iterator module_pos;
		vector<type_information> t_inf_list;
		type_information t_inf;
		bool immutable;
		public:
			annotated_module(shared_ptr<annotated_root_node> arn, shared_ptr<module> mod,
				shared_ptr<annotated_literal> alit, vector<shared_ptr<annotated_statement>> astmt_list,
				vector <type_information> til, type_information ti);
			~annotated_module();
			shared_ptr<annotated_literal> get_identifier();
			vector<shared_ptr<annotated_statement>> get_statement_list();
			source_token_list::iterator get_position();
			vector<type_information> get_type_information_list();
			type_information get_type_information();
			const bool get_immutable();
	};

	class annotated_return_statement : public annotated_root_node {
		shared_ptr<annotated_binary_expression> b_expression;
		source_token_list::iterator return_statement_pos;
		return_statement_kind rs_kind;
		type_information t_inf;
		public:
			annotated_return_statement(shared_ptr<annotated_root_node> arn, shared_ptr<return_statement> ret, shared_ptr<annotated_binary_expression> abexpr,
				type_information ti);
			~annotated_return_statement();
			shared_ptr<annotated_binary_expression> get_annotated_binary_expression();
			source_token_list::iterator get_position();
			const return_statement_kind get_return_statement_kind();
			type_information get_type_information();
	};

	class annotated_conditional_statement : public annotated_root_node {
		shared_ptr<annotated_binary_expression> if_conditional;
		vector<shared_ptr<annotated_statement>> if_statement_list;
		shared_ptr<annotated_binary_expression> else_conditional;
		vector<shared_ptr<annotated_statement>> else_statement_list;
		source_token_list::iterator conditional_statement_pos;
		conditional_else_conditional_kind cec_kind;
		conditional_else_statement_kind ces_kind;
		type_information t_inf;
		public:
			annotated_conditional_statement(shared_ptr<annotated_root_node> arn, shared_ptr<conditional_statement> cond, shared_ptr<annotated_binary_expression> ic,
				vector<shared_ptr<annotated_statement>> isl, shared_ptr<annotated_binary_expression> ec, vector<shared_ptr<annotated_statement>> esl, type_information ti);
			~annotated_conditional_statement();
			shared_ptr<annotated_binary_expression> get_if_conditional();
			vector<shared_ptr<annotated_statement>> get_if_statement_list();
			shared_ptr<annotated_binary_expression> get_else_conditional();
			vector<shared_ptr<annotated_statement>> get_else_statement_list();
			source_token_list::iterator get_position();
			const conditional_else_conditional_kind get_conditional_else_conditional_kind();
			const conditional_else_statement_kind get_conditional_else_statement_kind();
			type_information get_type_information();
	};

	class annotated_enum_statement : public annotated_root_node {
		vector<shared_ptr<annotated_literal>> identifier_list;
		shared_ptr<annotated_literal> identifier;
		enum_statement_kind es_kind;
		source_token_list::iterator enum_statement_pos;
		type_information t_inf;
		public:
			annotated_enum_statement(shared_ptr<annotated_root_node> arn, shared_ptr<enum_statement> _enum, shared_ptr<annotated_literal> alit,
				vector<shared_ptr<annotated_literal>> ailist, type_information ti);
			~annotated_enum_statement();
			vector<shared_ptr<annotated_literal>> get_identifier_list();
			shared_ptr<annotated_literal> get_identifier();
			const enum_statement_kind get_enum_statement_kind();
			source_token_list::iterator get_position();
			type_information get_type_information();
	};

	class annotated_statement : public annotated_root_node {
		statement_kind kind;
		shared_ptr<annotated_binary_expression> b_expression;
		shared_ptr<annotated_declaration> decl;
		shared_ptr<annotated_function> func;
		shared_ptr<annotated_structure> struc;
		shared_ptr<annotated_module> mod;
		shared_ptr<annotated_return_statement> ret;
		shared_ptr<annotated_conditional_statement> cond;
		shared_ptr<annotated_enum_statement> _enum;
		source_token_list::iterator statement_pos;
		type_information t_inf;
		public:
			annotated_statement(shared_ptr<annotated_root_node> arn, shared_ptr<statement> stmt, shared_ptr<annotated_binary_expression> abe,
				shared_ptr<annotated_declaration> adecl, shared_ptr<annotated_function> afunc, shared_ptr<annotated_structure> astruc, 
				shared_ptr<annotated_module> amod, shared_ptr<annotated_return_statement> aret, shared_ptr<annotated_conditional_statement> acond, 
				shared_ptr<annotated_enum_statement> aenum, type_information ti);
			~annotated_statement();
			const statement_kind get_statement_kind();
			shared_ptr<annotated_binary_expression> get_binary_expression();
			shared_ptr<annotated_declaration> get_declaration();
			shared_ptr<annotated_function> get_function();
			shared_ptr<annotated_module> get_module();
			source_token_list::iterator get_position();
			type_information get_type_information();
			shared_ptr<annotated_structure> get_structure();
			shared_ptr<annotated_return_statement> get_return_statement();
			shared_ptr<annotated_conditional_statement> get_conditional_statement();
			shared_ptr<annotated_enum_statement> get_enum_statement();
	};

	class symbol_table;

	class symbol {
		type_information t_inf;
		shared_ptr<literal> identifier;
		immut_kind i_kind;
		function_kind f_kind;
		vector<type_information> function_arguments;
		shared_ptr<symbol_table> class_type_information;
		function_declaration_definition_kind fdd_kind;
		structure_kind s_kind;
		structure_declaration_definition_kind sdd_kind;
		function_va_args_kind fva_kind;
		module_kind m_kind;
		enum_kind e_kind;
		enum_statement_kind es_kind;
		public:
			symbol(type_information ti, immut_kind ik, shared_ptr<literal> ident, function_kind fk, structure_kind sk, vector<type_information> fa, shared_ptr<symbol_table> st, function_declaration_definition_kind fddk, function_va_args_kind fvak,
				structure_declaration_definition_kind sddk, module_kind mk, enum_kind ek, enum_statement_kind es_kind);
			~symbol();
			type_information get_type_information();
			const immut_kind get_immut_kind();
			const function_kind get_function_kind();
			shared_ptr<literal> get_identifier();
			vector<type_information> get_function_arguments();
			shared_ptr<symbol_table> get_class_type_information();
			shared_ptr<symbol_table> get_module_type_information();
			const function_declaration_definition_kind get_function_declaration_definition_kind();
			function_declaration_definition_kind set_function_declaration_definition_kind(function_declaration_definition_kind fddk);
			const structure_kind get_structure_kind();
			const structure_declaration_definition_kind get_structure_declaration_definition_kind();
			structure_declaration_definition_kind set_structure_declaration_definition_kind(structure_declaration_definition_kind sddk);
			const function_va_args_kind get_function_va_args_kind();
			const module_kind get_module_kind();
			const enum_kind get_enum_kind();
			const enum_statement_kind get_enum_statement_kind();
			enum_statement_kind set_enum_statement_kind(enum_statement_kind esk);
	};

	class symbol_table {
		vector<shared_ptr<symbol>> sym_table;
		public:
			symbol_table();
			~symbol_table();
			vector<shared_ptr<symbol>> get_symbol_table();
			shared_ptr<symbol> operator[](int i);
			vector<shared_ptr<symbol>> find_all_symbols(shared_ptr<literal> lit);
			shared_ptr<symbol> add_symbol(shared_ptr<root_node> root, source_token_list::iterator pos, shared_ptr<symbol> sym);
			vector<shared_ptr<symbol>> find_all_symbols(shared_ptr<annotated_literal> alit);
	};

	class linearized_postfix_expression {
		shared_ptr<primary_expression> pr_expr;
		vector<shared_ptr<root_node>> postfix_operation_list;
		vector<postfix_operation_kind> postfix_operation_kind_list;
		public:
			linearized_postfix_expression(shared_ptr<primary_expression> pre, vector<shared_ptr<root_node>> pol, vector<postfix_operation_kind> pokl);
			~linearized_postfix_expression();
			shared_ptr<primary_expression> get_primary_expression();
			vector<shared_ptr<root_node>> get_postfix_operation_list();
			vector<postfix_operation_kind> get_postfix_operation_kind_list();
	};

	enum scope_kind {
		SCOPE_GLOBAL, SCOPE_FUNCTION, SCOPE_MODULE, SCOPE_CONDITIONAL, SCOPE_ENUM, SCOPE_NONE
	};

	class analyze_ast {
		vector<shared_ptr<symbol_table>> sym_table_list;
		shared_ptr<root_node> root;
		shared_ptr<annotated_root_node> ann_root_node;
		vector<scope_kind> s_kind_list;
		vector<shared_ptr<statement>> statement_list;

		shared_ptr<annotated_statement> analyze_statement(shared_ptr<statement> stmt);
		shared_ptr<annotated_declaration> analyze_declaration(shared_ptr<declaration> decl);
		pair<shared_ptr<annotated_declaration>, shared_ptr<symbol>> analyze_structure_declaration(shared_ptr<declaration> decl);
		pair<shared_ptr<annotated_declaration>, shared_ptr<symbol>> analyze_parameter(shared_ptr<declaration> decl);
		shared_ptr<annotated_binary_expression> analyze_binary_expression(shared_ptr<binary_expression> bexpr);
		shared_ptr<annotated_unary_expression> analyze_unary_expression(shared_ptr<unary_expression> uexpr);
		shared_ptr<annotated_linearized_postfix_expression> analyze_postfix_expression(shared_ptr<postfix_expression> poexpr);
		shared_ptr<linearized_postfix_expression> linearize_postfix_expression(shared_ptr<postfix_expression> poexpr);
		tuple<shared_ptr<annotated_primary_expression>, type_information, shared_ptr<annotated_root_node>> analyze_primary_expression(shared_ptr<primary_expression> prexpr, shared_ptr<linearized_postfix_expression> lpe);
		pair<type_information, shared_ptr<annotated_root_node>> analyze_postfix_operation(type_information prev, postfix_operation_kind pok, shared_ptr<root_node> op);
		shared_ptr<annotated_sequence> analyze_list_expression(shared_ptr<sequence> seq);
		shared_ptr<annotated_sequence> analyze_tuple_expression(shared_ptr<sequence> seq);
		shared_ptr<annotated_dictionary> analyze_dictionary_expression(shared_ptr<dictionary> dict);
		shared_ptr<annotated_ternary_expression> analyze_ternary_expression(shared_ptr<ternary_expression> texpr);
		shared_ptr<annotated_function> analyze_function(shared_ptr<function> func);
		shared_ptr<annotated_structure> analyze_structure(shared_ptr<structure> struc);
		shared_ptr<annotated_module> analyze_module(shared_ptr<module> mod);
		shared_ptr<annotated_return_statement> analyze_return_statement(shared_ptr<return_statement> ret);
		shared_ptr<annotated_conditional_statement> analyze_conditional_statement(shared_ptr<conditional_statement> cond);
		shared_ptr<annotated_enum_statement> analyze_enum_statement(shared_ptr<enum_statement> _enum);

		pair<vector<shared_ptr<symbol>>, bool> find_all_symbols(shared_ptr<annotated_literal> sym);
		pair<vector<shared_ptr<symbol>>, bool> find_all_symbols(shared_ptr<literal> sym);

		public:
			analyze_ast(shared_ptr<root_node> r, vector<scope_kind> skl, vector<shared_ptr<symbol_table>> stl, vector<shared_ptr<statement>> stmt_list);
			~analyze_ast();
			const bool perform_semantic_analysis();
			vector<shared_ptr<symbol_table>> get_symbol_table();
			shared_ptr<root_node> get_root_node();
			shared_ptr<annotated_root_node> get_annotated_root_node();
			vector<shared_ptr<statement>> get_statement_list();
	};

	class deduce_binary_expression_type {
		type_information lhs;
		binary_operation_kind b_kind;
		type_information rhs;
		public:
			deduce_binary_expression_type(type_information l, binary_operation_kind b, type_information r);
			~deduce_binary_expression_type();
			type_information get_lhs_type_information();
			const binary_operation_kind get_binary_operation_kind();
			type_information get_rhs_type_information();
			type_information deduce_type();
	};
}

#endif
