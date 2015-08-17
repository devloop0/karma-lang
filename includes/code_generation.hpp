#ifndef KARMA_LANG_CODE_GENERATION_HPP
#define KARMA_LANG_CODE_GENERATION_HPP

#include "includes/token.hpp"
#include "includes/token_analyze.hpp"
#include "includes/token_keywords.hpp"
#include "includes/lex.hpp"
#include "includes/parse.hpp"
#include "includes/semantics.hpp"
#include "includes/diagnostics_report.hpp"
#include "includes/diagnostic_messages.hpp"
#include "includes/unified.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::tuple;
using std::make_tuple;
using std::to_string;
using std::get;
using std::static_pointer_cast;
using std::pair;
using std::make_pair;

using namespace karma_lang;
using namespace unified_includes;

namespace karma_lang {

	class code_generation_symbol_table {
		vector<shared_ptr<symbol>> sym_table;
		vector<string> raw_string_list;
		vector<type_information> type_information_list;
		public:
			code_generation_symbol_table();
			~code_generation_symbol_table();
			tuple<type_information, shared_ptr<symbol>, string> add_symbol(type_information t_inf, shared_ptr<symbol> sym, int num);
			vector<string> find_all_symbol_code_names(type_information t_inf, shared_ptr<symbol> sym);
	};

	class code_generation_utilities {
		public:
			code_generation_utilities();
			~code_generation_utilities();
			string generate_binary_instruction(int tab, string op, int one, int two);
			string generate_unary_instruction(int tab, string op, int one);
			string generate_binary_instruction(int tab, string op, int one, string two);
			string generate_unary_instruction(int tab, string op, string var);
			string generate_binary_instruction(int tab, string op, string var, int one);
			string generate_binary_instruction(int tab, string op, string one, string two);
			string generate_label_instruction(int tab, int label);
			string generate_jump_instruction(int tab, int one, int two);
			string generate_jump_instruction(int tab, int one, string two);
			string generate_jump_instruction(int tab, string one, int two);
			string generate_exit_instruction(int tab);
			string generate_sequence_instruction(int tab, string op, int one, vector<int> indices);
			string generate_binary_operation_instruction(int tab, binary_operation_kind bopk, int store, string name, int store2, string name2, bool rhs_important);
			string generate_temp_name(int one);
			string generate_function_header(int tab, string name, vector<string> param_list, bool immut);
			string generate_function_footer(int tab);
			string generate_structure_header(int tab, string name, vector<string> member_list, bool immut);
			string generate_structure_footer(int tab);
			string generate_module_header(int tab, string mod_name, bool immut);
			string generate_module_footer(int tab);
			string generate_return_statement(int tab, string one);
			string generate_return_statement(int tab, int one);
			string generate_enum_statement(int tab, string e, vector<string> elist);
			string generate_scope_statement(int tab);
			string generate_escope_statement(int tab);
			string generate_lambda_header(int tab, string name, vector<string> param_list, bool immut);
			string generate_lambda_footer(int tab);
			string generate_break_instruction(int tab, int label);
			string generate_continue_instruction(int tab, int label);
			string generate_import_instruction(int tab, string file, string name);
	};

	class generate_code {
		shared_ptr<annotated_root_node> ann_root_node;
		vector<shared_ptr<symbol_table>> sym_table_list;
		shared_ptr<diagnostics_reporter> d_reporter;
		shared_ptr<code_generation_symbol_table> code_gen_sym_table;
		vector<string> instruction_list;
		vector<string> function_stack_list;
		vector<int> pre_loop_stack;
		vector<int> post_loop_stack;
		int number;
		int label_count;
		int tab_count;
		int temp_count;

		pair<string, int> descend_literal(shared_ptr<annotated_literal> alit);
		pair<string, int> descend_primary_expression(shared_ptr<annotated_primary_expression> prexpr);
		pair<string, int> descend_postfix_expression(shared_ptr<annotated_linearized_postfix_expression> apoexpr);
		string descend_unary_expression(shared_ptr<annotated_unary_expression> auexpr);
		vector<string> descend_binary_expression(shared_ptr<annotated_binary_expression> abexpr);
		bool descend_declaration(shared_ptr<annotated_declaration> adecl);
		bool descend_statement(shared_ptr<annotated_statement> astmt);
		tuple<int, int, int> descend_ternary_expression(shared_ptr<annotated_ternary_expression> atexpr);
		bool descend_function(shared_ptr<annotated_function> afunc);
		bool descend_structure(shared_ptr<annotated_structure> astruc);
		bool descend_module(shared_ptr<annotated_module> amod);
		bool descend_return_statement(shared_ptr<annotated_return_statement> aret);
		bool descend_conditional_statement(shared_ptr<annotated_conditional_statement> acond);
		bool descend_enum_statement(shared_ptr<annotated_enum_statement> aenum);
		bool descend_while_statement(shared_ptr<annotated_while_statement> awhile);
		bool descend_for_statement(shared_ptr<annotated_for_statement> afor);
		bool descend_break_continue_statement(shared_ptr<annotated_break_continue_statement> abreak_continue);
		bool descend_import_statement(shared_ptr<annotated_import_statement> aimport);
		bool descend_match_statement(shared_ptr<annotated_match_statement> amatch);

		public:
			generate_code(shared_ptr<analyze_ast> aa);
			~generate_code();
			vector<string> perform_code_generation();
	};
}

#endif
