#include "../includes/code_generation.hpp"

using namespace karma_lang;

namespace karma_lang {

	const string vm_instruction_list::mov = "mov";
	const string vm_instruction_list::inc = "inc";
	const string vm_instruction_list::dec = "dec";
	const string vm_instruction_list::cmpl = "cmpl";
	const string vm_instruction_list::neg = "neg";
	const string vm_instruction_list::pos = "pos";
	const string vm_instruction_list::bneg = "bneg";
	const string vm_instruction_list::tyof = "tyof";
	const string vm_instruction_list::add = "add";
	const string vm_instruction_list::mul = "mul";
	const string vm_instruction_list::div = "div";
	const string vm_instruction_list::mod = "mod";
	const string vm_instruction_list::exp = "exp";
	const string vm_instruction_list::sub = "sub";
	const string vm_instruction_list::shl = "shl";
	const string vm_instruction_list::shr = "shr";
	const string vm_instruction_list::cequ = "cequ";
	const string vm_instruction_list::snequ = "snequ";
	const string vm_instruction_list::cnequ = "cnequ";
	const string vm_instruction_list::lt = "lt";
	const string vm_instruction_list::lte = "lte";
	const string vm_instruction_list::gt = "gt";
	const string vm_instruction_list::gte = "gte";
	const string vm_instruction_list::bor = "bor";
	const string vm_instruction_list::band = "band";
	const string vm_instruction_list::exor = "exor";
	const string vm_instruction_list::land = "land";
	const string vm_instruction_list::lor = "lor";
	const string vm_instruction_list::cast = "cast";
	const string vm_instruction_list::exit = "exit";
	const string vm_instruction_list::jmp = "jmp";
	const string vm_instruction_list::list = "list";
	const string vm_instruction_list::tupl = "tupl";
	const string vm_instruction_list::sequ = "sequ";
	const string vm_instruction_list::subs = "subs";
	const string vm_instruction_list::dict = "dict";
	const string vm_instruction_list::subss = "subss";

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
		for(int i = 0; i < sym_table.size(); i++)
			if(sym_table[i]->get_identifier()->get_raw_literal()->get_raw_string() == sym->get_identifier()->get_raw_literal()->get_raw_string())
				if(sym->get_type_information() == t_inf)
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
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_unary_instruction(int tab, string op, int one) {
		string s = op + " r" + to_string(one);
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, int one, string two) {
		string s = op + " r" + to_string(one) + " " + "[" + two + "]";
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_unary_instruction(int tab, string op, string var) {
		string s = op + " " + "[" + var + "]";
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, string var, int one) {
		string s = op + " [" + var + "] r" + to_string(one);
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_label_instruction(int tab, int label) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + ".L" + to_string(label) + ":";
	}

	string code_generation_utilities::generate_jump_instruction(int tab, int one, int two) {
		string s = vm_instruction_list::jmp + " r" + to_string(one) + " .L" + to_string(two);
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_jump_instruction(int tab, int one, string two) {
		string s = vm_instruction_list::jmp + " r" + to_string(one) + " " + two;
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_jump_instruction(int tab, string one, int two) {
		string s = vm_instruction_list::jmp + " [" + one + "] .L" + to_string(two);
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_exit_instruction(int tab) {
		string s = vm_instruction_list::exit;
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_sequence_instruction(int tab, string op, int one, vector<int> indices) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		ret += op + " r" + to_string(one);
		for(int i = 0; i < indices.size(); i++) 
			ret += " r" + to_string(indices[i]);
		return ret;
	}

	string code_generation_utilities::generate_subss_instruction(int tab, int store, string var, int one, int two, int three) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::subss + " r" + to_string(store) + " [" + var + "] r" + to_string(one) + " r" + to_string(two) + " r" + to_string(three);
		return ret;
	}

	string code_generation_utilities::generate_subs_instruction(int tab, int store, string var, int one, int two, int three) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::subs + " r" + to_string(store) + " [" + var + "] r" + to_string(one) + " r" + to_string(two) + " r" + to_string(three);
		return ret;
	}

	generate_code::generate_code(shared_ptr<analyze_ast> aa) {
		ann_root_node = aa->get_annotated_root_node();
		code_gen_sym_table = make_shared<code_generation_symbol_table>();
		instruction_list = vector<string>();
		number = 0;
		tab_count = 0;
		label_count = 0;
		sym_table = aa->get_symbol_table();
		d_reporter = aa->get_annotated_root_node()->get_diagnostics_reporter();
	}

	generate_code::~generate_code() {

	}

	pair<vector<string>, string> generate_code::descend_literal(shared_ptr<annotated_literal> alit) {
		if(alit->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_pair(vector<string>(), "");
		string s = code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, alit->get_raw_literal()->get_raw_string());
		number++;
		vector<string> ret;
		ret.push_back(s);
		return make_pair(ret, alit->get_literal_kind() == literal_kind::LITERAL_IDENTIFIER ? alit->get_raw_literal()->get_raw_string() : "");
	}

	pair<vector<string>, string> generate_code::descend_primary_expression(shared_ptr<annotated_primary_expression> aprexpr) {
		if(aprexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_pair(vector<string>(), "");
		if(aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_LITERAL) {
			pair<vector<string>, string> ret = descend_literal(aprexpr->get_raw_literal());
			return ret;
		}
		else if(aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION ||
				aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION) {
			int store = number;
			vector<string> ret = descend_binary_expression(aprexpr->get_raw_parenthesized_expression()).first;
			string name = descend_binary_expression(aprexpr->get_raw_parenthesized_expression()).second[0];
			if(aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION)
				return make_pair(ret, name);
			else {
				string insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, store);
				ret.push_back(insn1);
				return make_pair(ret, name);
			}
			return make_pair(ret, name);
		}
		else if(aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_SEQUENCE) {
			int orig = number;
			int store = number + 1;
			number += 2;
			string insn1;
			shared_ptr<annotated_sequence> aseq = aprexpr->get_sequence();
			vector<shared_ptr<annotated_binary_expression>> abexpr_list = aseq->get_annotated_binary_expression_list();
			vector<int> list_indices;
			vector<string> ret;
			for(int i = 0; i < abexpr_list.size(); i++) {
				int store_inner = number;
				vector<string> insn_list = descend_binary_expression(abexpr_list[i]).first;
				ret.insert(ret.end(), insn_list.begin(), insn_list.end());
				if(i == 0) {
					insn1 = code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store, store_inner);
					ret.push_back(insn1);
				}
				else {
					if(aseq->get_sequence_kind() == sequence_kind::SEQUENCE_LIST)
						ret.push_back(insn1);
				}
				list_indices.push_back(store_inner);
			}
			sequence_kind seq_kind = aseq->get_sequence_kind();
			string temp = code_generation_utilities().generate_sequence_instruction(tab_count, seq_kind == sequence_kind::SEQUENCE_TUPLE ? vm_instruction_list::tupl : vm_instruction_list::list, orig, list_indices);
			ret.push_back(temp);
			return make_pair(ret, "");
		}
		else if(aprexpr->get_primary_expression_kind() == primary_expression_kind::PRIMARY_EXPRESSION_DICTIONARY) {
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
			vector<string> ret;
			for(int i = 0; i < key_list.size(); i++) {
				int store_inner = number;
				vector<string> insn_list = descend_binary_expression(key_list[i]).first;
				ret.insert(ret.end(), insn_list.begin(), insn_list.end());
				if(i == 0) {
					insn1 = code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, key_store, store_inner);
					ret.push_back(insn1);
				}
				else
					ret.push_back(insn1);
				key_indices.push_back(store_inner);
			}
			for(int i = 0; i < value_list.size(); i++) {
				int store_inner = number;
				vector<string> insn_list = descend_binary_expression(value_list[i]).first;
				ret.insert(ret.end(), insn_list.begin(), insn_list.end());
				if(i == 0) {
					insn2 = code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, value_store, store_inner);
					ret.push_back(insn2);
				}
				else
					ret.push_back(insn2);
				value_indices.push_back(store_inner);
			}
			if(key_indices.size() != value_indices.size()) {
				d_reporter->print(diagnostic_messages::unreachable, adict->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(1);
			}
			if(key_indices.size() == 0) {
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
				key_indices.push_back(number);
				value_indices.push_back(number);
				number++;
			}
			for(int i = 0; i < key_indices.size(); i++) {
				total_indices.push_back(key_indices[i]);
				total_indices.push_back(value_indices[i]);
			}
			string temp = code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::dict, orig, total_indices);
			ret.push_back(temp);
			return make_pair(ret, "");
		}
		d_reporter->print(diagnostic_messages::unreachable, aprexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		exit(1);
	}

	tuple<vector<string>, string, postfix_operation_kind> generate_code::descend_postfix_expression(shared_ptr<annotated_linearized_postfix_expression> apoexpr) {
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if(apoexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_tuple(vector<string>(), "", postfix_operation_kind::POSTFIX_OPERATION_NONE);
		if(apoexpr->get_postfix_operation_kind_list().size() != apoexpr->get_annotated_root_node_list().size())
			return make_tuple(vector<string>(), "", postfix_operation_kind::POSTFIX_OPERATION_NONE);
		vector<postfix_operation_kind> pokl = apoexpr->get_postfix_operation_kind_list();
		vector<shared_ptr<annotated_root_node>> arnl = apoexpr->get_annotated_root_node_list();
		pair<vector<string>, string> pai = descend_primary_expression(apoexpr->get_raw_annotated_primary_expression());
		vector<string> ret = pai.first;
		string full_name = pai.second;
		postfix_operation_kind pok = postfix_operation_kind::POSTFIX_OPERATION_NONE;
		for(int i = 0; i < pokl.size(); i++) {
			if(pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT || pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT) {
				if(full_name == "")
					full_name = apoexpr->get_type_information().get_literal()->get_raw_literal()->get_raw_string();
				string instruction;
				if(pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT)
					instruction = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::inc, full_name);
				else
					instruction = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::dec, full_name);
				pok = pokl[i];
			}
			else if(pokl[i] == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
				if(full_name == "")
					full_name = apoexpr->get_type_information().get_literal()->get_raw_literal()->get_raw_string();
				if(apoexpr->get_raw_annotated_primary_expression()->get_sequence() == nullptr) {
				}
				shared_ptr<annotated_subscript> asubscr = static_pointer_cast<annotated_subscript>(apoexpr->get_annotated_root_node_list()[i]);
				subscript_colon_kind sck = asubscr->get_subscript_colon_kind();
				shared_ptr<annotated_binary_expression> start = asubscr->get_start();
				shared_ptr<annotated_binary_expression> end = asubscr->get_final();
				shared_ptr<annotated_binary_expression> step = asubscr->get_step();
				int store = number;
				number++;
				if(sck == subscript_colon_kind::SUBSCRIPT_COLON_ZERO) {
					if(start == nullptr) {
						int store1 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store1, "0"));
						number++;
						int store2 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store2, full_name + ".size"));
						number++;
						int store3 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store3, "1"));
						number++;
						ret.push_back(code_generation_utilities().generate_subs_instruction(tab_count, store, full_name, store1, store2, store3));
						full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
					}
					else {
						int store1 = number;
						vector<string> temp = descend_binary_expression(start).first;
						ret.insert(ret.end(), temp.begin(), temp.end());
						int store2 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store1));
						number++;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::add, store2, number));
						number++;
						int store3 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
						number++;
						ret.push_back(code_generation_utilities().generate_subs_instruction(tab_count, store, full_name, store1, store2, store3));
						full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
					}
				}
				else if(sck == subscript_colon_kind::SUBSCRIPT_COLON_ONE) {
					int store1 = -1;
					int store2 = -1;
					if(start == nullptr) {
						store1 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "0"));
						number++;
					}
					else {
						store1 = number;
						vector<string> temp = descend_binary_expression(start).first;
						ret.insert(ret.end(), temp.begin(), temp.end());
					}
					if(end == nullptr) {
						store2 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, full_name + ".size"));
						number++;
					}
					else {
						store2 = number;
						vector<string> temp = descend_binary_expression(end).first;
						ret.insert(ret.end(), temp.begin(), temp.end());
					}
					int store3 = number;
					ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
					number++;
					ret.push_back(code_generation_utilities().generate_subss_instruction(tab_count, store, full_name, store1, store2, store3));
					full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
				}
				else if(sck == subscript_colon_kind::SUBSCRIPT_COLON_TWO) {
					int store1 = -1;
					int store2 = -1;
					int store3 = -1;
					if(start == nullptr) {
						store1 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "0"));
						number++;
					}
					else {
						store1 = number;
						vector<string> temp = descend_binary_expression(start).first;
						ret.insert(ret.end(), temp.begin(), temp.end());
					}
					if(end == nullptr) {
						store2 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, full_name + ".size"));
						number++;
					}
					else {
						store2 = number;
						vector<string> temp = descend_binary_expression(end).first;
						ret.insert(ret.end(), temp.begin(), temp.end());
					}
					if(step == nullptr) {
						store3 = number;
						ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
						number++;
					}
					else {
						store3 = number;
						vector<string> temp = descend_binary_expression(step).first;
						ret.insert(ret.end(), temp.begin(), temp.end());
					}
					ret.push_back(code_generation_utilities().generate_subss_instruction(tab_count, store, full_name, store1, store2, store3));
					full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
				}
			}
			else if(pokl[i] == postfix_operation_kind::POSTFIX_OPERATION_NONE) break;
			else {
				d_reporter->print(diagnostic_messages::instruction_not_supported, apoexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(1);
			}
		}
		return make_tuple(ret, full_name, pok);
	}

	tuple<vector<string>, string, postfix_operation_kind> generate_code::descend_unary_expression(shared_ptr<annotated_unary_expression> auexpr) {
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if(auexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			make_tuple(vector<string>(), "", postfix_operation_kind::POSTFIX_OPERATION_NONE);
		int store_begin = number;
		tuple<vector<string>, string, postfix_operation_kind> tup = descend_postfix_expression(auexpr->get_raw_annotated_linearized_postfix_expression());
		vector<string> ret = get<0>(tup);
		string name = get<1>(tup);
		postfix_operation_kind op = get<2>(tup);
		string insn1;
		if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_MINUS)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::neg, store_begin);
		else if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_COMPLEMENT)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::cmpl, store_begin);
		else if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_NOT)
			insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store_begin);
		else if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT || auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_INCREMENT) {
			if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT)
				insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::dec, name);
			else
				insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::inc, name);
		}
		if(insn1 == "");
		else
			ret.push_back(insn1);
		return make_tuple(ret, name, op);
	}

	pair<vector<string>, vector<string>> generate_code::descend_binary_expression(shared_ptr<annotated_binary_expression> abexpr) {
		if(abexpr->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return make_pair(vector<string>(), vector<string>());
		int store = number;
		postfix_operation_kind pok_lhs = postfix_operation_kind::POSTFIX_OPERATION_NONE;
		postfix_operation_kind pok_rhs = postfix_operation_kind::POSTFIX_OPERATION_NONE;
		string lhs_name = "";
		string rhs_name = "";
		tuple<int, int, int> lhs_ternary;
		tuple<int, int, int> rhs_ternary;
		bool lt_check = false;
		bool rt_check = false;
		vector<string> ret;
		if(abexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			pair<vector<string>, vector<string>> pai = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(abexpr->get_lhs()));
			ret = pai.first;
			lhs_name = pai.second[0];
		}
		else if(abexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION) {
			tuple<vector<string>, string, postfix_operation_kind> tup = descend_unary_expression(static_pointer_cast<annotated_unary_expression>(abexpr->get_lhs()));
			ret = get<0>(tup);
			lhs_name = get<1>(tup);
			pok_lhs = get<2>(tup);
		}
		else if(abexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			pair<vector<string>, tuple<int, int, int>> pai = descend_ternary_expression(static_pointer_cast<annotated_ternary_expression>(abexpr->get_lhs()));
			ret = pai.first;
			lt_check = true;
			lhs_ternary = pai.second;
		}
		else {
			d_reporter->print(diagnostic_messages::unreachable, abexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			exit(1);
		}
		int store2 = number;
		if(abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_NONE) {
			int prev_store = store;
			if(lt_check) {
				store = number;
				ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, get<0>(lhs_ternary), label_count));
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, get<1>(lhs_ternary)));
				number++;
				ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, "true", label_count + 1));
				ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
				label_count++;
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store, get<2>(lhs_ternary)));
				ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
				label_count++;
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, prev_store, store));
			}
			vector<string> temp;
			rhs_name = lhs_name;
			temp.push_back(lhs_name);
			temp.push_back(rhs_name);
			return make_pair(ret, temp);
		}
		else if(abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			pair<vector<string>, vector<string>> temp = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(abexpr->get_rhs()));
			ret.insert(ret.end(), temp.first.begin(), temp.first.end());
			rhs_name = temp.second[1];
		}
		else if(abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			pair<vector<string>, tuple<int, int, int>> pai = descend_ternary_expression(static_pointer_cast<annotated_ternary_expression>(abexpr->get_rhs()));
			ret.insert(ret.end(), pai.first.begin(), pai.first.end());
			rt_check = true;
			rhs_ternary = pai.second;
		}
		else {
			tuple<vector<string>, string, postfix_operation_kind> tup = descend_unary_expression(static_pointer_cast<annotated_unary_expression>(abexpr->get_rhs()));
			ret.insert(ret.end(), get<0>(tup).begin(), get<0>(tup).end());
			rhs_name = get<1>(tup);
			pok_rhs = get<2>(tup);
		}
		if(lhs_name == "");
		else {
			store = number;
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store, lhs_name));
			number++;
		}
		if(rhs_name == "");
		else {
			store2 = number;
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store2, rhs_name));
			number++;
		}
		if(lt_check) {
			int prev_store = store;
			store = number;
			ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, get<0>(lhs_ternary), label_count));
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, get<1>(lhs_ternary)));
			number++;
			ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, "true", label_count + 1));
			ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store, get<2>(lhs_ternary)));
			ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, prev_store, store));
		}
		if(rt_check) {
			int prev_store = store2;
			store2 = number;
			ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, get<0>(lhs_ternary), label_count));
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, get<1>(lhs_ternary)));
			number++;
			ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, "true", label_count + 1));
			ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store2, get<2>(lhs_ternary)));
			ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
			label_count++;
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, prev_store, store2));
		}
		binary_operation_kind bopk = abexpr->get_binary_operation_kind();
		ret.push_back(code_generation_utilities().generate_binary_operation_instruction(tab_count, bopk, store, lhs_name, store2));
		vector<string> temp;
		temp.push_back(lhs_name);
		temp.push_back(rhs_name);
		return make_pair(ret, temp);
	}

	pair<vector<string>, tuple<int, int, int>> generate_code::descend_ternary_expression(shared_ptr<annotated_ternary_expression> atexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if(atexpr->get_type_information() == bad)
			return make_pair(vector<string>(), make_tuple(-1, -1, -1));
		int store1 = number;
		vector<string> ret = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(atexpr->get_condition())).first;
		int store2 = number;
		vector<string> temp1 = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(atexpr->get_true_path())).first;
		ret.insert(ret.end(), temp1.begin(), temp1.end());
		int store3 = number;
		vector<string> temp2 = descend_binary_expression(static_pointer_cast<annotated_binary_expression>(atexpr->get_false_path())).first;
		ret.insert(ret.end(), temp2.begin(), temp2.end());
		return make_pair(ret, make_tuple(store1, store2, store3));
	}

	vector<string> generate_code::descend_declaration(shared_ptr<annotated_declaration> adecl) {
		if(adecl->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return vector<string>();
		int store = number;
		vector<string> expr = descend_binary_expression(adecl->get_binary_expression()).first;
		expr.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, adecl->get_identifier()->get_raw_literal()->get_raw_string(), store));
		return expr;
	}

	vector<string> generate_code::descend_statement(shared_ptr<annotated_statement> astmt) {
		if(astmt->get_type_information() == type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE))
			return vector<string>();
		if(astmt->get_statement_kind() == statement_kind::STATEMENT_EXPRESSION)
			return descend_binary_expression(astmt->get_binary_expression()).first;
		else if(astmt->get_statement_kind() == statement_kind::STATEMENT_DECLARATION)
			return descend_declaration(astmt->get_declaration());
		d_reporter->print(diagnostic_messages::unreachable, astmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		exit(1);
	}

	vector<string> generate_code::perform_code_generation() {
		for(int i = 0; i < ann_root_node->get_annotated_statement_list().size(); i++) {
			vector<string> temp = descend_statement(ann_root_node->get_annotated_statement_list()[i]);
			instruction_list.insert(instruction_list.end(), temp.begin(), temp.end());
		}
		return instruction_list;
	}

	string code_generation_utilities::generate_binary_operation_instruction(int tab_count, binary_operation_kind bopk, int store, string name, int store2) {
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
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_NOT_EQUAL)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::snequ, store, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_EQUALS)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, name, store2));
		else if(bopk == binary_operation_kind::BINARY_OPERATION_POINT)
			return (code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::cast, store, store2));
		else
			return "";
	}
}
