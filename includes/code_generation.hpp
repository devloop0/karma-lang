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
			const static string func;
			const static string efunc;
			const static string struc;
			const static string estruc;
			const static string module;
			const static string emodule;
			const static string imodule;
			const static string istruc;
			const static string imov;
			const static string ret;
			const static string _enum;
			const static string ifunc;
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
			string generate_binary_operation_instruction(int tab, binary_operation_kind bopk, int store, string name, int store2, string name2, bool rhs_important);
			string generate_temp_name(int one);
			string generate_call_instruction(int tab, string dest, string name, vector<string> reg_list);
			string generate_function_header(int tab, string name, vector<string> param_list, bool immut);
			string generate_function_footer();
			string generate_structure_header(int tab, string name, vector<string> member_list, bool immut);
			string generate_structure_footer(int tab);
			string generate_module_header(int tab, string mod_name, bool immut);
			string generate_module_footer(int tab);
			string generate_return_statement(int tab, string one);
			string generate_return_statement(int tab, int one);
			string generate_enum_statement(int tab, string e, vector<string> elist);
	};

	class generate_code {
		shared_ptr<annotated_root_node> ann_root_node;
		vector<shared_ptr<symbol_table>> sym_table_list;
		shared_ptr<diagnostics_reporter> d_reporter;
		shared_ptr<code_generation_symbol_table> code_gen_sym_table;
		vector<string> instruction_list;
		vector<string> identifier_list;
		int number;
		int label_count;
		int tab_count;
		int temp_count;
		int scope_count;

		pair<string, int> descend_literal(shared_ptr<annotated_literal> alit);
		pair<string, int> descend_primary_expression(shared_ptr<annotated_primary_expression> prexpr);
		tuple<string, postfix_operation_kind, int> descend_postfix_expression(shared_ptr<annotated_linearized_postfix_expression> apoexpr);
		pair<string, postfix_operation_kind> descend_unary_expression(shared_ptr<annotated_unary_expression> auexpr);
		vector<string> descend_binary_expression(shared_ptr<annotated_binary_expression> abexpr);
		bool descend_declaration(shared_ptr<annotated_declaration> adecl, bool in_module);
		bool descend_statement(shared_ptr<annotated_statement> astmt, bool in_module);
		tuple<int, int, int> descend_ternary_expression(shared_ptr<annotated_ternary_expression> atexpr);
		bool descend_function(shared_ptr<annotated_function> afunc, bool in_module);
		bool descend_structure(shared_ptr<annotated_structure> astruc, bool in_module);
		bool descend_module(shared_ptr<annotated_module> amod, bool in_module);
		bool descend_return_statement(shared_ptr<annotated_return_statement> aret, bool in_module);
		bool descend_conditional_statement(shared_ptr<annotated_conditional_statement> acond, bool in_module);
		bool descend_enum_statement(shared_ptr<annotated_enum_statement> aenum, bool in_module);

		vector<tuple<string, string, int>> name_list;
		public:
			generate_code(shared_ptr<analyze_ast> aa);
			~generate_code();
			vector<string> perform_code_generation();
	};
}

#endif
