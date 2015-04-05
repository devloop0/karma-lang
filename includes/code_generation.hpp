#ifndef KARMA_LANG_CODE_GENERATION_HPP
#define KARMA_LANG_CODE_GENERATION_HPP

#include "../includes/token.hpp"
#include "../includes/token_analyze.hpp"
#include "../includes/token_keywords.hpp"
#include "../includes/lex.hpp"
#include "../includes/parse.hpp"
#include "../includes/semantics.hpp"
#include "../includes/diagnostics_report.hpp"
#include "../includes/diagnostic_messages.hpp"
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

namespace karma_lang {

	class vm_instruction_list {
		public:
			const static string mov;
			const static string inc;
			const static string dec;
			const static string cmpl;
			const static string neg;
			const static string pos;
			const static string bneg;
			const static string tyof;
			const static string add;
			const static string mul;
			const static string div;
			const static string mod;
			const static string exp;
			const static string sub;
			const static string shl;
			const static string shr;
			const static string equ;
			const static string nequ;
			const static string gt;
			const static string lt;
			const static string gte;
			const static string lte;
			const static string band;
			const static string bor;
			const static string exor;
			const static string lor;
			const static string land;
			const static string cast;
			const static string exit;
			const static string jmp;
			const static string list;
			const static string tupl;
			const static string dict;
	};

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
			string generate_binary_operation_instruction(int tab, binary_operation_kind bopk, int store, string name, int store2);
			string generate_temp_name(int one);
	};

	class generate_code {
		shared_ptr<annotated_root_node> ann_root_node;
		shared_ptr<symbol_table> sym_table;
		shared_ptr<diagnostics_reporter> d_reporter;
		shared_ptr<code_generation_symbol_table> code_gen_sym_table;
		vector<string> instruction_list;
		vector<string> identifier_list;
		int number;
		int label_count;
		int tab_count;
		int temp_count;

		pair<vector<string>, string> descend_literal(shared_ptr<annotated_literal> alit);
		pair<vector<string>, string> descend_primary_expression(shared_ptr<annotated_primary_expression> prexpr);
		tuple<vector<string>, string, postfix_operation_kind> descend_postfix_expression(shared_ptr<annotated_linearized_postfix_expression> apoexpr);
		tuple<vector<string>, string, postfix_operation_kind> descend_unary_expression(shared_ptr<annotated_unary_expression> auexpr);
		pair<vector<string>, vector<string>> descend_binary_expression(shared_ptr<annotated_binary_expression> abexpr);
		vector<string> descend_declaration(shared_ptr<annotated_declaration> adecl);
		vector<string> descend_statement(shared_ptr<annotated_statement> astmt);
		pair<vector<string>, tuple<int, int, int>> descend_ternary_expression(shared_ptr<annotated_ternary_expression> atexpr);

		public:
			generate_code(shared_ptr<analyze_ast> aa);
			~generate_code();
			vector<string> perform_code_generation();
	};
}

#endif
