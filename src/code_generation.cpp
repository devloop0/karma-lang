#include "includes/code_generation.hpp"

using namespace karma_lang;

namespace karma_lang {

	code_generation_symbol_table::code_generation_symbol_table() {
		raw_string_list = vector<string>();
		sym_table = vector<shared_ptr<symbol>>();
		type_information_list = vector<type_information>();
	}

	code_generation_symbol_table::~code_generation_symbol_table() {

	}

	tuple<type_information, shared_ptr<symbol>, string> code_generation_symbol_table::add_symbol(type_information t_inf, shared_ptr<symbol> sym, int num) {
		string code_name = "r" + to_string(num);
		raw_string_list.push_back(code_name);
		sym_table.push_back(sym);
		type_information_list.push_back(t_inf);
		return make_tuple(t_inf, sym, code_name);
	}

	vector<string> code_generation_symbol_table::find_all_symbol_code_names(type_information t_inf, shared_ptr<symbol> sym) {
		vector<string> ret;
		for (int i = 0; i < sym_table.size(); i++)
			if (sym_table[i]->get_identifier()->get_raw_literal()->get_raw_string() == sym->get_identifier()->get_raw_literal()->get_raw_string())
				if (sym->get_type_information() == t_inf)
					ret.push_back(raw_string_list[i]);
		return ret;
	}

	code_generation_utilities::code_generation_utilities() {

	}

	code_generation_utilities::~code_generation_utilities() {

	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, int one, int two) {
		string s = op + " r" + to_string(one) + " r" + to_string(two);
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_unary_instruction(int tab, string op, int one) {
		string s = op + " r" + to_string(one);
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, int one, string two) {
		string s = op + " r" + to_string(one) + " [" + two + "]";
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_unary_instruction(int tab, string op, string var) {
		string s = op + " [" + var + "]";
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, string var, int one) {
		string s = op + " [" + var + "] r" + to_string(one);
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, string one, string two) {
		string s = op + " [" + one + "] [" + two + "]";
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_label_instruction(int tab, int label) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + ".L" + to_string(label) + "";
	}

	string code_generation_utilities::generate_jump_instruction(int tab, int one, int two) {
		string s = vm_instruction_list::jmp + " r" + to_string(one) + " .L" + to_string(two);
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_jump_instruction(int tab, int one, string two) {
		string s = vm_instruction_list::jmp + " r" + to_string(one) + " " + two;
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_jump_instruction(int tab, string one, int two) {
		string s = vm_instruction_list::jmp + " [" + one + "] .L" + to_string(two);
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_exit_instruction(int tab) {
		string s = vm_instruction_list::exit;
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_sequence_instruction(int tab, string op, int one, vector<int> indices) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += op + " r" + to_string(one);
		for (int i = 0; i < indices.size(); i++)
			ret += " r" + to_string(indices[i]);
		return ret;
	}

	string code_generation_utilities::generate_temp_name(int one) {
		return "__temp__" + to_string(one);
	}

	string code_generation_utilities::generate_function_header(int tab, string name, vector<string> reg_list, bool immut) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += (immut ? vm_instruction_list::ifunc : vm_instruction_list::func) + " " + name + " ";
		for (int i = 0; i < reg_list.size(); i++)
			ret += reg_list[i] + " ";
		return ret;
	}

	string code_generation_utilities::generate_structure_header(int tab, string name, vector<string> member_list, bool immut) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += (immut ? vm_instruction_list::istruc : vm_instruction_list::struc) + " " + name + " ";
		for (int i = 0; i < member_list.size(); i++)
			ret += member_list[i] + (i == member_list.size() - 1 ? "" : " ");
		return ret;
	}

	string code_generation_utilities::generate_structure_footer(int tab) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::estruc;
		return ret;
	}

	string code_generation_utilities::generate_function_footer(int tab) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::efunc;
		return ret;
	}

	string code_generation_utilities::generate_module_header(int tab, string mod_name, bool immut) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += (immut ? vm_instruction_list::imodule : vm_instruction_list::module) + " " + mod_name;
		return ret;
	}

	string code_generation_utilities::generate_module_footer(int tab) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::emodule;
		return ret;
	}

	string code_generation_utilities::generate_return_statement(int tab, string one) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::ret + " " + one;
		return ret;
	}

	string code_generation_utilities::generate_return_statement(int tab, int one) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::ret + " r" + to_string(one);
		return ret;
	}

	string code_generation_utilities::generate_enum_statement(int tab, string e, vector<string> elist) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::_enum + " " + e;
		for (int i = 0; i < elist.size(); i++)
			ret += " " + elist[i];
		return ret;
	}

	string code_generation_utilities::generate_scope_statement(int tab) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::scope;
		return ret;
	}

	string code_generation_utilities::generate_escope_statement(int tab) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::escope;
		return ret;
	}

	string code_generation_utilities::generate_lambda_header(int tab, string name, vector<string> param_list, bool immut) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += (immut ? vm_instruction_list::ilambda : vm_instruction_list::lambda) + " " + name + " ";
		for (int i = 0; i < param_list.size(); i++)
			ret += param_list[i] + " ";
		return ret;
	}

	string code_generation_utilities::generate_lambda_footer(int tab) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::elambda;
		return ret;
	}

	string code_generation_utilities::generate_break_instruction(int tab, int label) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::brk + " .L" + to_string(label);
		return ret;
	}

	string code_generation_utilities::generate_continue_instruction(int tab, int label) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::cont + " .L" + to_string(label);
		return ret;
	}

	string code_generation_utilities::generate_import_instruction(int tab, string file, string name) {
		string ret;
		for (int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::imprt + " " + file + " " + name;
		return ret;
	}

	generate_code::generate_code(shared_ptr<analyze_ast> aa) {
		ann_root_node = aa->get_annotated_root_node();
		code_gen_sym_table = make_shared<code_generation_symbol_table>();
		instruction_list = vector<string>();
		pre_loop_stack = vector<int>();
		post_loop_stack = vector<int>();
		number = 0;
		tab_count = 0;
		label_count = 0;
		temp_count = 0;
		sym_table_list = aa->get_symbol_table();
		d_reporter = aa->get_annotated_root_node()->get_diagnostics_reporter();
		function_stack_list = vector<string>();
	}

	generate_code::~generate_code() {

	}

	pair<string, int> generate_code::descend_literal(shared_ptr<annotated_literal> alit) {
		if (alit->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_pair("", number);
		string temp = alit->get_raw_literal()->get_raw_string();
		string temp2;
		if (alit->get_raw_literal()->get_token_kind() == token_kind::TOKEN_IDENTIFIER);
		else
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$" + temp));
		number++;
		string str = alit->get_raw_literal()->get_raw_string();
		if (function_stack_list.size() > 0 && str == builtins::builtin__self__)
			str = function_stack_list[function_stack_list.size() - 1];
		return make_pair(alit->get_literal_kind() == literal_kind::LITERAL_IDENTIFIER ? str : "", number - 1);
	}

	pair<string, int> generate_code::descend_primary_expression(shared_ptr<annotated_primary_expression> aprexpr) {
		if (aprexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_pair("", number);
		if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_LITERAL) {
			pair<string, int> ret = descend_literal(aprexpr->get_raw_literal());
			return make_pair(ret.first, ret.second);
		}
		else if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION ||
			aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION) {
			int store = number;
			vector<string> temp = descend_binary_expression(aprexpr->get_raw_parenthesized_expression());
			string temp_name = code_generation_utilities().generate_temp_name(temp_count);
			temp_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, temp_name, store));
			string name = temp_name;
			if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION)
				return make_pair(name, store);
			else {
				string insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, store);
				instruction_list.push_back(insn1);
				name.clear();
				return make_pair(name, store);
			}
			return make_pair(name, store);
		}
		else if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_SEQUENCE) {
			int orig = number;
			number++;
			string insn1;
			shared_ptr<annotated_sequence> aseq = aprexpr->get_sequence();
			vector<shared_ptr<annotated_binary_expression>> abexpr_list = aseq->get_annotated_binary_expression_list();
			vector<int> list_indices;
			for (int i = 0; i < abexpr_list.size(); i++) {
				int store_inner = number;
				descend_binary_expression(abexpr_list[i]);
				list_indices.push_back(store_inner);
			}
			sequence_kind seq_kind = aseq->get_sequence_kind();
			string temp = code_generation_utilities().generate_sequence_instruction(tab_count, seq_kind == sequence_kind::SEQUENCE_TUPLE ? vm_instruction_list::tupl : vm_instruction_list::list, orig, list_indices);
			instruction_list.push_back(temp);
			string s = code_generation_utilities().generate_temp_name(temp_count);
			temp_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, s, orig));
			return make_pair(s, orig);
		}
		else if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_DICTIONARY) {
			int orig = number;
			int key_store = number + 1;
			int value_store = number + 2;
			number += 3;
			vector<int> key_indices;
			vector<int> value_indices;
			vector<int> total_indices;
			string insn1;
			string insn2;
			shared_ptr<annotated_dictionary> adict = aprexpr->get_dictionary();
			vector<shared_ptr<annotated_binary_expression>> key_list = adict->get_key_list();
			vector<shared_ptr<annotated_binary_expression>> value_list = adict->get_value_list();
			for (int i = 0; i < key_list.size(); i++) {
				int store_inner = number;
				descend_binary_expression(key_list[i]);
				key_indices.push_back(store_inner);
			}
			for (int i = 0; i < value_list.size(); i++) {
				int store_inner = number;
				descend_binary_expression(value_list[i]);
				value_indices.push_back(store_inner);
			}
			if (key_indices.size() != value_indices.size()) {
				d_reporter->print(diagnostic_messages::unreachable, adict->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(1);
			}
			for (int i = 0; i < key_indices.size(); i++) {
				total_indices.push_back(key_indices[i]);
				total_indices.push_back(value_indices[i]);
			}
			string temp = code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::dict, orig, total_indices);
			instruction_list.push_back(temp);
			string s = code_generation_utilities().generate_temp_name(temp_count);
			temp_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, s, orig));
			return make_pair(s, orig);
		}
		else if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_LAMBDA) {
			int orig = number;
			string temp = code_generation_utilities().generate_temp_name(temp_count);
			descend_function(aprexpr->get_lambda());
			return make_pair(temp, orig);
		}
		else if (aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_NONE) {
			int orig = number;
			string name = "^";
			number++;
			return make_pair(name, orig);
		}
		d_reporter->print(diagnostic_messages::unreachable, aprexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		exit(1);
	}

	pair<string, int> generate_code::descend_postfix_expression(shared_ptr<annotated_linearized_postfix_expression> apoexpr) {
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if (apoexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_pair("", number);
		if (apoexpr->get_postfix_operation_kind_list().size() != apoexpr->get_annotated_root_node_list().size())
			return make_pair("", number);
		vector<postfix_operation_kind> pokl = apoexpr->get_postfix_operation_kind_list();
		vector<shared_ptr<annotated_root_node>> arnl = apoexpr->get_annotated_root_node_list();
		tuple<string, int> pai = descend_primary_expression(apoexpr->get_raw_annotated_primary_expression());
		string full_name = get<0>(pai);
		int first = get<1>(pai);
		for (int i = 0; i < pokl.size(); i++) {
			if (pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT || pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT) {
				int store = number;
				number++;
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, store, full_name));
				instruction_list.push_back(code_generation_utilities().generate_unary_instruction(tab_count, pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT ? vm_instruction_list::podec :
					vm_instruction_list::poinc, full_name));
				string temp = code_generation_utilities().generate_temp_name(temp_count);
				temp_count++;
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, temp, store));
				full_name = temp;
			}
			else if (pokl[i] == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
				shared_ptr<annotated_subscript> asubscr = static_pointer_cast<annotated_subscript>(apoexpr->get_annotated_root_node_list()[i]);
				subscript_colon_kind sck = asubscr->get_subscript_colon_kind();
				shared_ptr<annotated_binary_expression> start = asubscr->get_start();
				shared_ptr<annotated_binary_expression> end = asubscr->get_final();
				shared_ptr<annotated_binary_expression> step = asubscr->get_step();
				int store = number;
				number++;
				if (sck == subscript_colon_kind::SUBSCRIPT_COLON_ZERO) {
					if (start == nullptr) {
						int store1 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store1, "$0"));
						number++;
						int store2 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store2, full_name + "@size"));
						number++;
						int store3 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store3, "$1"));
						number++;
						full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
					}
					else {
						int store1 = number;
						descend_binary_expression(start);
						full_name += "|r" + to_string(store1);
					}
				}
				else if (sck == subscript_colon_kind::SUBSCRIPT_COLON_ONE) {
					int store1 = -1;
					int store2 = -1;
					if (start == nullptr) {
						store1 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$0"));
						number++;
					}
					else {
						store1 = number;
						descend_binary_expression(start);
					}
					if (end == nullptr) {
						store2 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, full_name + "@size"));
						number++;
					}
					else {
						store2 = number;
						descend_binary_expression(end);
					}
					int store3 = number;
					instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$1"));
					number++;
					full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
				}
				else if (sck == subscript_colon_kind::SUBSCRIPT_COLON_TWO) {
					int store1 = -1;
					int store2 = -1;
					int store3 = -1;
					if (start == nullptr) {
						store1 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$0"));
						number++;
					}
					else {
						store1 = number;
						descend_binary_expression(start);
					}
					if (end == nullptr) {
						store2 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, full_name + "@size"));
						number++;
					}
					else {
						store2 = number;
						descend_binary_expression(end);
					}
					if (step == nullptr) {
						store3 = number;
						instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$1"));
						number++;
					}
					else {
						store3 = number;
						descend_binary_expression(step);
					}
					full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
				}
			}
			else if (pokl[i] == postfix_operation_kind::POSTFIX_FUNCTION_CALL) {
				shared_ptr<annotated_function_argument_list> afarg_list = static_pointer_cast<annotated_function_argument_list>(apoexpr->get_annotated_root_node_list()[i]);
				vector<int> store_list;
				full_name += ":";
				for (int i = 0; i < afarg_list->get_argument_list().size(); i++) {
					shared_ptr<annotated_binary_expression> abexpr = afarg_list->get_argument_list()[i];
					store_list.push_back(number);
					descend_binary_expression(abexpr);
				}
				for (int i = 0; i < store_list.size(); i++)
					full_name += "r" + to_string(store_list[i]) + (i == store_list.size() - 1 ? "" : ",");
			}
			else if (pokl[i] == postfix_operation_kind::POSTFIX_DOT_OPERATOR) {
				shared_ptr<annotated_literal> alit = static_pointer_cast<annotated_literal>(apoexpr->get_annotated_root_node_list()[i]);
				full_name += "@" + alit->get_raw_literal()->get_raw_string();
			}
			else if (pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_NONE) break;
			else {
				d_reporter->print(diagnostic_messages::instruction_not_supported, apoexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(1);
			}
		}
		if (full_name == "");
		else
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, first, full_name));
		return make_pair(full_name, first);
	}

	string generate_code::descend_unary_expression(shared_ptr<annotated_unary_expression> auexpr) {
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if (auexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			make_tuple(vector<string>(), "", postfix_operation_kind::POSTFIX_OPERATION_NONE);
		int store_begin = number;
		pair<string, int> pai = descend_postfix_expression(auexpr->get_raw_annotated_linearized_postfix_expression());
		string name = pai.first;
		int first = pai.second;
		string insn1;
		if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_MINUS)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::neg, store_begin);
		else if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_PLUS)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::pos, store_begin);
		else if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_COMPLEMENT)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::cmpl, store_begin);
		else if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_NOT)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store_begin);
		else if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT || auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_INCREMENT) {
			if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT)
				insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::prdec, name);
			else
				insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::princ, name);
		}
		if (insn1 == "");
		else {
			instruction_list.push_back(insn1);
			if (auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT ||
				auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_INCREMENT) {
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, first, name));
			}
		}
		return name;
	}

	vector<string> generate_code::descend_binary_expression(shared_ptr<annotated_binary_expression> abexpr) {
		if (abexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return vector<string>();
		int store = number;
		string lhs_name = "";
		string rhs_name = "";
		tuple<int, int, int> lhs_ternary;
		tuple<int, int, int> rhs_ternary;
		bool lt_check = false;
		bool rt_check = false;
		binary_operation_kind lhs_bop_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		binary_operation_kind rhs_bop_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		if (abexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			vector<string> pai = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(abexpr->get_lhs()));
			lhs_name = pai[0];
			lhs_bop_kind = static_pointer_cast<annotated_binary_expression>(abexpr->get_lhs())->get_binary_operation_kind();
		}
		else if (abexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION) {
			lhs_name = descend_unary_expression(static_pointer_cast<annotated_unary_expression>(abexpr->get_lhs()));
		}
		else if (abexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			tuple<int, int, int> pai = descend_ternary_expression(static_pointer_cast<annotated_ternary_expression>(abexpr->get_lhs()));
			lt_check = true;
			lhs_ternary = pai;
		}
		else {
			d_reporter->print(diagnostic_messages::unreachable, abexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			exit(1);
		}
		int store2 = number;
		vector<string> temp2;
		if (abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_NONE) {
			int prev_store = store;
			if (lt_check) {
				store = number;
				instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, get<0>(lhs_ternary), label_count));
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, get<2>(lhs_ternary)));
				number++;
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$true"));
				number++;
				instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number - 1, label_count + 1));
				instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
				label_count++;
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store, get<1>(lhs_ternary)));
				instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
				label_count++;
				instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, prev_store, store));
			}
			vector<string> temp;
			rhs_name = lhs_name;
			temp.push_back(lhs_name);
			temp.push_back(rhs_name);
			return temp;
		}
		else if (abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			vector<string> temp = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(abexpr->get_rhs()));
			rhs_name = temp[0];
			rhs_bop_kind = static_pointer_cast<annotated_binary_expression>(abexpr->get_rhs())->get_binary_operation_kind();
		}
		else if (abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			tuple<int, int, int> pai = descend_ternary_expression(static_pointer_cast<annotated_ternary_expression>(abexpr->get_rhs()));
			rt_check = true;
			rhs_ternary = pai;
		}
		else {
			rhs_name = descend_unary_expression(static_pointer_cast<annotated_unary_expression>(abexpr->get_rhs()));
		}
		int prev_store = store;
		int prev_store2 = store2;
		binary_operation_kind bopk = abexpr->get_binary_operation_kind();
		if (lt_check) {
			int prev_store = store;
			store = number;
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, get<0>(lhs_ternary), label_count));
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, get<2>(lhs_ternary)));
			number++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$true"));
			number++;
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number - 1, label_count + 1));
			instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store, get<1>(lhs_ternary)));
			instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, prev_store, store));
		}
		if (rt_check) {
			int prev_store = store2;
			store2 = number;
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, get<0>(lhs_ternary), label_count));
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, get<2>(lhs_ternary)));
			number++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$true"));
			number++;
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number - 1, label_count + 1));
			instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store2, get<1>(lhs_ternary)));
			instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, prev_store, store2));
		}
		bool rhs_bop_bool = rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_EQUALS || rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_DIVIDE_EQUALS ||
			rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_BITWISE_OR_EQUALS || rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_BITWISE_AND_EQUALS ||
			rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR_EQUALS || rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_EXPONENT_EQUALS ||
			rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_MINUS_EQUALS || rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_MODULUS_EQUALS ||
			rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_MULTIPLY_EQUALS || rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_PLUS_EQUALS ||
			rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT_EQUALS || rhs_bop_kind == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT_EQUALS;
		instruction_list.push_back(code_generation_utilities().generate_binary_operation_instruction(tab_count, bopk, store, lhs_name, store2, rhs_name, rhs_bop_bool));
		vector<string> temp{ "", "" };
		return temp;
	}

	tuple<int, int, int> generate_code::descend_ternary_expression(shared_ptr<annotated_ternary_expression> atexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (atexpr->get_type_information() == bad)
			return make_tuple(-1, -1, -1);
		int store1 = number;
		descend_binary_expression(static_pointer_cast<annotated_binary_expression>(atexpr->get_condition()));
		int store2 = number;
		descend_binary_expression(static_pointer_cast<annotated_binary_expression>(atexpr->get_true_path()));
		int store3 = number;
		descend_binary_expression(static_pointer_cast<annotated_binary_expression>(atexpr->get_false_path()));
		return make_tuple(store1, store2, store3);
	}

	bool generate_code::descend_declaration(shared_ptr<annotated_declaration> adecl) {
		if (adecl->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		int store = number;
		descend_binary_expression(adecl->get_binary_expression());
		string str = adecl->get_identifier()->get_raw_literal()->get_raw_string();
		if (adecl->get_immutable())
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::imov, str, store));
		else
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, str, store));
		return true;
	}

	bool generate_code::descend_function(shared_ptr<annotated_function> afunc) {
		if (afunc->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		string fname = "";
		if (afunc->get_lambda_kind() == lambda_kind::LAMBDA_NO) {
			string str = afunc->get_identifier()->get_raw_literal()->get_raw_string();
			fname = str;
		}
		else {
			fname = code_generation_utilities().generate_temp_name(temp_count);
			temp_count++;
		}
		function_stack_list.push_back(fname);
		if (afunc->get_function_declaration_definition_kind() == function_declaration_definition_kind::FUNCTION_KIND_FORWARD_DECLARATION)
			return true;
		vector<string> param_list;
		for (int i = 0; i < afunc->get_parameter_list().size(); i++) {
			string param = afunc->get_parameter_list()[i]->get_identifier()->get_raw_literal()->get_raw_string();
			param_list.push_back(param);
		}
		if (afunc->get_function_va_args_kind() == function_va_args_kind::FUNCTION_VA_ARGS_YES)
			param_list.push_back(token_keywords::va_args);
		if (afunc->get_lambda_kind() == lambda_kind::LAMBDA_NO)
			instruction_list.push_back(code_generation_utilities().generate_function_header(tab_count, fname, param_list, afunc->get_immutable()));
		else
			instruction_list.push_back(code_generation_utilities().generate_lambda_header(tab_count, fname, param_list, afunc->get_immutable()));
		for (int i = 0; i < afunc->get_statement_list().size(); i++)
			descend_statement(afunc->get_statement_list()[i]);
		if (afunc->get_lambda_kind() == lambda_kind::LAMBDA_NO)
			instruction_list.push_back(code_generation_utilities().generate_function_footer(tab_count));
		else
			instruction_list.push_back(code_generation_utilities().generate_lambda_footer(tab_count));
		return true;
	}

	bool generate_code::descend_structure(shared_ptr<annotated_structure> astruc) {
		if (astruc->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		string str = astruc->get_identifier()->get_raw_literal()->get_raw_string();
		string sname = str;
		if (astruc->get_structure_declaration_definition_kind() == structure_declaration_definition_kind::STRUCTURE_KIND_FORWARD_DECLARATION)
			return true;
		vector<pair<string, int>> struc_member_list;
		vector<string> member_list;
		for (int i = 0; i < astruc->get_declaration_list().size(); i++)
			member_list.push_back(astruc->get_declaration_list()[i]->get_identifier()->get_raw_literal()->get_raw_string());
		instruction_list.push_back(code_generation_utilities().generate_structure_header(tab_count, sname, member_list, astruc->get_immutable()));
		for (int i = 0; i < astruc->get_declaration_list().size(); i++) {
			int store = number;
			descend_binary_expression(astruc->get_declaration_list()[i]->get_binary_expression());
			string name = astruc->get_declaration_list()[i]->get_identifier()->get_raw_literal()->get_raw_string();
			struc_member_list.push_back(make_pair(name, store));
		}
		for (int i = 0; i < struc_member_list.size(); i++) {
			pair<string, int> pai = struc_member_list[i];
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, astruc->get_immutable() ? vm_instruction_list::imov : vm_instruction_list::dmov, pai.first, pai.second));
		}
		instruction_list.push_back(code_generation_utilities().generate_structure_footer(tab_count));
		return true;
	}

	bool generate_code::descend_module(shared_ptr<annotated_module> amod) {
		if (amod->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		if (amod->get_module_kind() == module_declaration_definition_kind::MODULE_KIND_DECLARATION)
			return true;
		string str = amod->get_identifier()->get_raw_literal()->get_raw_string();
		string mname = str;
		instruction_list.push_back(code_generation_utilities().generate_module_header(tab_count, mname, amod->get_immutable()));
		for (int i = 0; i < amod->get_statement_list().size(); i++)
			descend_statement(amod->get_statement_list()[i]);
		instruction_list.push_back(code_generation_utilities().generate_module_footer(tab_count));
		return true;
	}

	bool generate_code::descend_return_statement(shared_ptr<annotated_return_statement> aret) {
		if (aret->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		int store = number;
		if (aret->get_return_statement_kind() == return_statement_kind::RETURN_STATEMENT_EMPTY) {
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::imov, number, "$true"));
			number++;
			instruction_list.push_back(code_generation_utilities().generate_return_statement(tab_count, number - 1));
		}
		else {
			descend_binary_expression(aret->get_annotated_binary_expression());
			instruction_list.push_back(code_generation_utilities().generate_return_statement(tab_count, store));
		}
		return true;
	}

	bool generate_code::descend_conditional_statement(shared_ptr<annotated_conditional_statement> acond) {
		if (acond->get_type_information() == type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		vector<pair<shared_ptr<annotated_binary_expression>, vector<shared_ptr<annotated_statement>>>> acond_list = acond->get_conditional_list();
		int final_label = label_count;
		label_count++;
		for (int i = 0; i < acond_list.size(); i++) {
			int label_store = label_count;
			label_count++;
			if (acond_list[i].first != nullptr) {
				int store = number;
				descend_binary_expression(acond_list[i].first);
				instruction_list.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store));
				instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_store));
			}
			instruction_list.push_back(code_generation_utilities().generate_scope_statement(tab_count));
			for (int j = 0; j < acond_list[i].second.size(); j++)
				descend_statement(acond_list[i].second[j]);
			int temp = number;
			number++;
			instruction_list.push_back(code_generation_utilities().generate_escope_statement(tab_count));
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, temp, "$true"));
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, temp, final_label));
			instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_store));
		}
		instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, final_label));
		return true;
	}

	bool generate_code::descend_enum_statement(shared_ptr<annotated_enum_statement> aenum) {
		if (aenum->get_type_information() == type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		string str = aenum->get_identifier()->get_raw_literal()->get_raw_string();
		string ename = str;
		vector<string> vlist;
		for (int i = 0; i < aenum->get_identifier_list().size(); i++)
			vlist.push_back(aenum->get_identifier_list()[i]->get_raw_literal()->get_raw_string());
		instruction_list.push_back(code_generation_utilities().generate_enum_statement(tab_count, ename, vlist));
		return true;
	}

	bool generate_code::descend_while_statement(shared_ptr<annotated_while_statement> awhile) {
		if (awhile->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		int store = number;
		int come_back = label_count;
		label_count++;
		int done = label_count;
		pre_loop_stack.push_back(come_back);
		post_loop_stack.push_back(done);
		label_count++;
		instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, come_back));
		instruction_list.push_back(code_generation_utilities().generate_scope_statement(tab_count));
		descend_binary_expression(awhile->get_condition());
		instruction_list.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store));
		instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, done));
		for (int i = 0; i < awhile->get_statement_list().size(); i++) {
			shared_ptr<annotated_statement> astmt = awhile->get_statement_list()[i];
			descend_statement(astmt);
		}
		instruction_list.push_back(code_generation_utilities().generate_escope_statement(tab_count));
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$true"));
		instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number, come_back));
		number++;
		instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, done));
		pre_loop_stack.pop_back();
		post_loop_stack.pop_back();
		return true;
	}

	bool generate_code::descend_for_statement(shared_ptr<annotated_for_statement> afor) {
		if (afor->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		int begin = label_count;
		label_count++;
		int end = label_count;
		label_count++;
		string index = code_generation_utilities().generate_temp_name(temp_count);
		temp_count++;
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, index, "$0"));
		int store_sequence = number;
		descend_binary_expression(afor->get_expression());
		string sequence = code_generation_utilities().generate_temp_name(temp_count);
		temp_count++;
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, sequence, store_sequence));
		instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, begin));
		instruction_list.push_back(code_generation_utilities().generate_scope_statement(tab_count));
		int store_index_compare = number;
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, index));
		number++;
		int store_sequence_size = number;
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, sequence + "@size"));
		number++;
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lt, store_index_compare, store_sequence_size));
		instruction_list.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store_index_compare));
		instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store_index_compare, end));
		pre_loop_stack.push_back(begin);
		post_loop_stack.push_back(end);
		shared_ptr<annotated_declaration> adecl = afor->get_loop_variable();
		string str = adecl->get_identifier()->get_raw_literal()->get_raw_string();
		string ident_name = str;
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, ident_name, sequence + "|" + index));
		for (int i = 0; i < afor->get_statement_list().size(); i++) {
			shared_ptr<annotated_statement> astmt = afor->get_statement_list()[i];
			descend_statement(astmt);
		}
		instruction_list.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::poinc, index));
		instruction_list.push_back(code_generation_utilities().generate_escope_statement(tab_count));
		instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$true"));
		instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number, begin));
		number++;
		instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, end));
		pre_loop_stack.pop_back();
		post_loop_stack.pop_back();
		return true;
	};

	bool generate_code::descend_break_continue_statement(shared_ptr<annotated_break_continue_statement> abreak_continue) {
		if (abreak_continue->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		if (abreak_continue->get_break_continue_statement_kind() == break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_BREAK)
			instruction_list.push_back(code_generation_utilities().generate_break_instruction(tab_count, post_loop_stack[post_loop_stack.size() - 1]));
		else if (abreak_continue->get_break_continue_statement_kind() == break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_CONTINUE)
			instruction_list.push_back(code_generation_utilities().generate_continue_instruction(tab_count, pre_loop_stack[pre_loop_stack.size() - 1]));
		return true;
	}

	bool generate_code::descend_import_statement(shared_ptr<annotated_import_statement> aimport) {
		if (aimport->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		if (aimport->get_import_statement_kind() == import_statement_kind::IMPORT_STATEMENT_ALIASED)
			instruction_list.push_back(code_generation_utilities().generate_import_instruction(tab_count, aimport->get_file_to_import()->get_raw_literal()->get_raw_string(),
				aimport->get_import_alias()->get_raw_literal()->get_raw_string()));
		else if (aimport->get_import_statement_kind() == import_statement_kind::IMPORT_STATEMENT_UNALIASED)
			instruction_list.push_back(code_generation_utilities().generate_import_instruction(tab_count, aimport->get_file_to_import()->get_raw_literal()->get_raw_string(),
				aimport->get_file_to_import()->get_raw_literal()->get_raw_string()));
		else
			return false;
		return true;
	}

	bool generate_code::descend_match_statement(shared_ptr<annotated_match_statement> amatch) {
		if (amatch->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		int reference = number;
		descend_binary_expression(amatch->get_condition());
		int final_label = label_count;
		label_count++;
		for (int i = 0; i < amatch->get_match_arm_list().size(); i++) {
			int expr = number;
			descend_binary_expression(amatch->get_match_arm_list()[i].first);
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dmov, number, reference));
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::equ, number, expr));
			int condition = number;
			number++;
			int next_label = label_count;
			label_count++;
			instruction_list.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, condition));
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, condition, next_label));
			instruction_list.push_back(code_generation_utilities().generate_scope_statement(tab_count));
			for (int j = 0; j < amatch->get_match_arm_list()[i].second.size(); j++)
				descend_statement(amatch->get_match_arm_list()[i].second[j]);
			instruction_list.push_back(code_generation_utilities().generate_escope_statement(tab_count));
			instruction_list.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "$true"));
			instruction_list.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number, final_label));
			number++;
			instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, next_label));
		}
		instruction_list.push_back(code_generation_utilities().generate_scope_statement(tab_count));
		for (int i = 0; i < amatch->get_default_arm().size(); i++)
			descend_statement(amatch->get_default_arm()[i]);
		instruction_list.push_back(code_generation_utilities().generate_escope_statement(tab_count));
		instruction_list.push_back(code_generation_utilities().generate_label_instruction(tab_count, final_label));
		return true;
	}

	bool generate_code::descend_statement(shared_ptr<annotated_statement> astmt) {
		if (astmt->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return false;
		if (astmt->get_statement_kind() == statement_kind::STATEMENT_EXPRESSION) {
			descend_binary_expression(astmt->get_binary_expression());
			return true;
		}
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_DECLARATION)
			return descend_declaration(astmt->get_declaration());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_FUNCTION)
			return descend_function(astmt->get_function());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_STRUCTURE)
			return descend_structure(astmt->get_structure());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_MODULE)
			return descend_module(astmt->get_module());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_RETURN_STATEMENT)
			return descend_return_statement(astmt->get_return_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_CONDITIONAL_STATEMENT)
			return descend_conditional_statement(astmt->get_conditional_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_ENUM_STATEMENT)
			return descend_enum_statement(astmt->get_enum_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_WHILE_STATMENT)
			return descend_while_statement(astmt->get_while_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_FOR_STATEMENT)
			return descend_for_statement(astmt->get_for_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_BREAK_CONTINUE_STATEMENT)
			return descend_break_continue_statement(astmt->get_break_continue_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_IMPORT_STATEMENT)
			return descend_import_statement(astmt->get_import_statement());
		else if (astmt->get_statement_kind() == statement_kind::STATEMENT_MATCH_STATEMENT)
			return descend_match_statement(astmt->get_match_statement());
		d_reporter->print(diagnostic_messages::unreachable, astmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		exit(1);
	}

	vector<string> generate_code::perform_code_generation() {
		for(int i = 0; i < ann_root_node->get_annotated_statement_list().size(); i++)
			descend_statement(ann_root_node->get_annotated_statement_list()[i]);
		return instruction_list;
	}

	string code_generation_utilities::generate_binary_operation_instruction(int tab_count, binary_operation_kind bopk, int store, string name, int store2, string name2, bool rhs_important) {
		if(bopk == binary_operation_kind::BINARY_OPERATION_PLUS)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::add, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_MINUS)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sub, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_MULTIPLY)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mul, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_DIVIDE)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::div, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_EXPONENT)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::exp, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_LESS_THAN)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lt, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_GREATER_THAN)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::gt, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lte, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::gte, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_MODULUS)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mod, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shl, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shr, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_AND)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::band, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_OR)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::bor, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::exor, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_LOGICAL_AND)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::land, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_LOGICAL_OR)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::equ, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_NOT_EQUAL)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::nequ, store, store2));
		else if (bopk == binary_operation_kind::BINARY_OPERATION_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_PLUS_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::adde, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::adde, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_MINUS_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sube, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sube, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_MULTIPLY_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mule, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mule, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_DIVIDE_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dive, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::dive, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_EXPONENT_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::expe, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::expe, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_MODULUS_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mode, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mode, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shle, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shle, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shre, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shre, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_AND_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::bande, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::bande, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_OR_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::bore, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::bore, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR_EQUALS) {
			if (name2 == "" || !rhs_important)
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::exore, name, store2));
			else
				return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::exore, name, name2));
		}
		else if (bopk == binary_operation_kind::BINARY_OPERATION_POINT)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::cast, store, store2));
		else
			return "";
	}
}
