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
	const string vm_instruction_list::subsl = "subsl";
	const string vm_instruction_list::subsll = "subsll";
	const string vm_instruction_list::dict = "dict";
	const string vm_instruction_list::subsd = "subsd";

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
		string s = op + " r" + to_string(one) + " " + two;
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_unary_instruction(int tab, string op, string var) {
		string s = op + " " + var;
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		return ret + s;
	}

	string code_generation_utilities::generate_binary_instruction(int tab, string op, string var, int one) {
		string s = op + " " + var + " r" + to_string(one);
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
		string s = vm_instruction_list::jmp + one + " .L" + to_string(two);
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

	vector<string> code_generation_utilities::integer_float_check(int& tab_count, int& number, int& label_count, string name, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, name));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "0"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store3 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1.0"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, number, store3));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, number, store));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		number++;
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::integer_float_check(int& tab_count, int& number, int& label_count, int one, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, one));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "0"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store3 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1.0"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, number, store3));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, number, store));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, number, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		number++;
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}
	vector<string> code_generation_utilities::integer_check(int& tab_count, int& number, int& label_count, string name, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, name));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "0"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::integer_check(int& tab_count, int& number, int& label_count, int one, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, one));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "0"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::boolean_check(int& tab_count, int& number, int& label_count, string name, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, name));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "true"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::boolean_check(int& tab_count, int& number, int& label_count, int one, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, one));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "true"));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::subscript_check(int& tab_count, int& number, int& label_count, int one, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, one));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int temp = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		number++;
		vector<int> indices;
		indices.push_back(temp);
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::list, number, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store3 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::tupl, number, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int temp2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		indices.push_back(temp2);
		number++;
		int store5 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::dict, number, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store4 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store));
		number++;
		int store6 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store4, store3));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, store, store4));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store6, store5));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, store, store6));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::subscript_check(int& tab_count, int& number, int& label_count, string name, string instruction) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, name));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int temp = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		number++;
		vector<int> indices;
		indices.push_back(temp);
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::list, number, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store3 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::tupl, number, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int temp2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		indices.push_back(temp2);
		number++;
		int store5 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::dict, number, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store4 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store));
		number++;
		int store6 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, store));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store4, store3));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, store, store4));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store6, store5));
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, store, store6));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		tab_count++;
		if(instruction != "")
			ret.push_back(instruction);
		tab_count--;
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::dict_check(int& tab_count, int& number, int& label_count, int one, vector<string> instruction_list, bool yes) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, one));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int temp = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		number++;
		int temp2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		number++;
		vector<int> indices;
		indices.push_back(temp);
		indices.push_back(temp2);
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::dict, store2, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, store2));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		if(yes);
		else
			ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		if(instruction_list.size() == 0)
			ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		else 
			for(int i = 0; i < instruction_list.size(); i++)
				ret.push_back(instruction_list[i]);
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::dict_check(int& tab_count, int& number, int& label_count, string var, vector<string> instruction_list, bool yes) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, var));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int temp = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		number++;
		int temp2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, "1"));
		number++;
		vector<int> indices;
		indices.push_back(temp);
		indices.push_back(temp2);
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_sequence_instruction(tab_count, vm_instruction_list::dict, store2, indices));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, store2));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		if(yes);
		else
			ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		if(instruction_list.size() == 0)
			ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		else
			for(int i = 0; i < instruction_list.size(); i++) 
				ret.push_back(instruction_list[i]);
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		label_count++;
		return ret;
	}

	vector<string> code_generation_utilities::variable_check(int& tab_count, int& number, int& label_count, int one, int two, bool compatible) {
		vector<string> ret;
		int store = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, one));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		int store2 = number;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, number, two));
		ret.push_back(code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::tyof, number));
		number++;
		ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, compatible ? vm_instruction_list::cequ : vm_instruction_list::sequ, store, store2));
		ret.push_back(code_generation_utilities().generate_jump_instruction(tab_count, store, label_count));
		ret.push_back(code_generation_utilities().generate_exit_instruction(tab_count));
		ret.push_back(code_generation_utilities().generate_label_instruction(tab_count, label_count));
		label_count++;
		return ret;
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

	string code_generation_utilities::generate_subsl_instruction(int tab, int store, string var, int one, int two, int three) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::subsl + " r" + to_string(store) + " " + var + " r" + to_string(one) + " r" + to_string(two) + " r" + to_string(three);
		return ret;
	}

	string code_generation_utilities::generate_subsd_instruction(int tab, int store, string var, int one, int two, int three) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::subsd + " r" + to_string(store) + " " + var + " r" + to_string(one) + " r" + to_string(two) + " r" + to_string(three);
		return ret;
	}

	string code_generation_utilities::generate_subsll_instruction(int tab, int store, string var, int one, int two, int three) {
		string ret;
		for(int i = 0; i < tab; i++)
			ret += "\t";
		ret += vm_instruction_list::subsll + " r" + to_string(store) + " " + var + " r" + to_string(one) + " r" + to_string(two) + " r" + to_string(three);
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
					if(aseq->get_sequence_kind() == sequence_kind::SEQUENCE_LIST) {
						vector<string> checks = code_generation_utilities().variable_check(tab_count, number, label_count, store, store_inner, true);
						ret.insert(ret.end(), checks.begin(), checks.end());
						ret.push_back(insn1);
					}
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
				else {
					vector<string> checks = code_generation_utilities().variable_check(tab_count, number, label_count, key_store, store_inner, true);
					ret.insert(ret.end(), checks.begin(), checks.end());
					ret.push_back(insn1);
				}
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
				else {
					vector<string> checks = code_generation_utilities().variable_check(tab_count, number, label_count, value_store, store_inner, true);
					ret.insert(ret.end(), checks.begin(), checks.end());
					ret.push_back(insn2);
				}
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
				vector<string> temp = code_generation_utilities().integer_check(tab_count, number, label_count, full_name, instruction);
				ret.insert(ret.end(), temp.begin(), temp.end());
			}
			else if(pokl[i] == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
				if(full_name == "")
					full_name = apoexpr->get_type_information().get_literal()->get_raw_literal()->get_raw_string();
				vector<string> subscriptable = code_generation_utilities().subscript_check(tab_count, number, label_count, full_name, "");
				ret.insert(ret.end(), subscriptable.begin(), subscriptable.end());
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
						vector<string> temp;
						temp.push_back(code_generation_utilities().generate_subsd_instruction(tab_count, store, full_name, store1, store2, store3));
						vector<string> temp2 = code_generation_utilities().dict_check(tab_count, number, label_count, full_name, temp, true);
						ret.insert(ret.end(), temp2.begin(), temp2.end());
						ret.push_back(code_generation_utilities().generate_subsl_instruction(tab_count, store, full_name, store1, store2, store3));
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
						vector<string> temp1;
						temp1.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store2, "0"));
						temp1.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, store3, "0"));
						temp1.push_back(code_generation_utilities().generate_subsd_instruction(tab_count, store, full_name, store1, store2, store3));
						vector<string> temp2 = code_generation_utilities().dict_check(tab_count, number, label_count, full_name, temp1, true);
						ret.insert(ret.end(), temp2.begin(), temp2.end());
						ret.push_back(code_generation_utilities().generate_subsl_instruction(tab_count, store, full_name, store1, store2, store3));
						full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
					}
				}
				else if(sck == subscript_colon_kind::SUBSCRIPT_COLON_ONE) {
					vector<string> not_dict = code_generation_utilities().dict_check(tab_count, number, label_count, full_name, vector<string>(), false);
					ret.insert(ret.end(), not_dict.begin(), not_dict.end());
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
					ret.push_back(code_generation_utilities().generate_subsll_instruction(tab_count, store, full_name, store1, store2, store3));
					full_name += "/r" + to_string(store1) + "/r" + to_string(store2) + "/r" + to_string(store3);
				}
				else if(sck == subscript_colon_kind::SUBSCRIPT_COLON_TWO) {
					vector<string> not_dict = code_generation_utilities().dict_check(tab_count, number, label_count, full_name, vector<string>(), false);
					ret.insert(ret.end(), not_dict.begin(), not_dict.end());
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
					ret.push_back(code_generation_utilities().generate_subsll_instruction(tab_count, store, full_name, store1, store2, store3));
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
		if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_MINUS) {
			string insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::neg, store_begin);
			vector<string> temp = code_generation_utilities().integer_float_check(tab_count, number, label_count, name, insn1);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
		else if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_COMPLEMENT) {
			string insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::cmpl, store_begin);
			vector<string> temp = code_generation_utilities().integer_check(tab_count, number, label_count, name, insn1);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
		else if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_NOT) {
			string insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::bneg, store_begin);
			vector<string> temp = code_generation_utilities().boolean_check(tab_count, number, label_count, name, insn1);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
		else if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT || auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_INCREMENT) {
			string insn1;
			if(auexpr->get_unary_operation_kind() == unary_operation_kind::UNARY_OPERATION_DECREMENT)
				insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::dec, name);
			else
				insn1 = code_generation_utilities().generate_unary_instruction(tab_count, vm_instruction_list::inc, name);
			vector<string> temp = code_generation_utilities().integer_check(tab_count, number, label_count, name, insn1);
			ret.insert(ret.begin(), temp.begin(), temp.end());
		}
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
		else {
			d_reporter->print(diagnostic_messages::unreachable, abexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			exit(1);
		}
		int store2 = number;
		if(abexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_NONE) {
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
		vector<string> checks = code_generation_utilities().variable_check(tab_count, number, label_count, store, store2, true);
		ret.insert(ret.end(), checks.begin(), checks.end());
		if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_PLUS || abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_MINUS ||
				abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_MULTIPLY || abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_DIVIDE ||
				abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_EXPONENT) {
			vector<string> additional_checks = code_generation_utilities().integer_float_check(tab_count, number, label_count, store, "");
			ret.insert(ret.end(), additional_checks.begin(), additional_checks.end());
			if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_PLUS)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::add, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_MINUS)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sub, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_MULTIPLY)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mul, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_DIVIDE)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::div, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_EXPONENT)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::exp, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_LESS_THAN)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lt, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_GREATER_THAN)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::gt, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lte, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::gte, store, store2));
		}
		else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT || abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT ||
				abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_BITWISE_AND || abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_BITWISE_OR ||
				abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR || abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_MODULUS) {
			vector<string> additional_checks = code_generation_utilities().integer_check(tab_count, number, label_count, store, "");
			ret.insert(ret.end(), additional_checks.begin(), additional_checks.end());
			if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_MODULUS)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mod, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shl, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::shr, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_BITWISE_AND)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::band, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_BITWISE_OR)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::bor, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::exor, store, store2));
		}
		else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_LOGICAL_AND || abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_LOGICAL_OR) {
			vector<string> additional_checks = code_generation_utilities().boolean_check(tab_count, number, label_count, store, "");
			ret.insert(ret.end(), additional_checks.begin(), additional_checks.end());
			if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_LOGICAL_AND)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::land, store, store2));
			else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_LOGICAL_OR)
				ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::lor, store, store2));
		}
		else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS)
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::sequ, store, store2));
		else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_NOT_EQUAL)
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::snequ, store, store2));
		else if(abexpr->get_binary_operation_kind() == binary_operation_kind::BINARY_OPERATION_EQUALS)
			ret.push_back(code_generation_utilities().generate_binary_instruction(tab_count, vm_instruction_list::mov, lhs_name, store2));
		else {
			d_reporter->print(diagnostic_messages::instruction_not_supported, abexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			exit(1);
		}
		vector<string> temp;
		temp.push_back(lhs_name);
		temp.push_back(rhs_name);
		return make_pair(ret, temp);
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
}
