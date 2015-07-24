#include "../includes/semantics.hpp"

using namespace karma_lang;

namespace karma_lang {

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = value_kind::VALUE_NONE;
		list_information = nullptr;
		key_information = nullptr;
		value_information = nullptr;
		return_type_information = nullptr;
		tuple_information = vector<shared_ptr<type_information>>();
		lit = nullptr;
		class_name = "";
	}

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = vk;
		list_information = nullptr;
		key_information = nullptr;
		value_information = nullptr;
		return_type_information = nullptr;
		tuple_information = vector<shared_ptr<type_information>>();
		lit = nullptr;
		class_name = "";
	}

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, shared_ptr<type_information> li) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = vk;
		list_information = li;
		key_information = nullptr;
		value_information = nullptr;
		return_type_information = nullptr;
		tuple_information = vector<shared_ptr<type_information>>();
		lit = nullptr;
		class_name = "";
	}

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, shared_ptr<type_information> ki, shared_ptr<type_information> vi) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = vk;
		list_information = nullptr;
		key_information = ki;
		value_information = vi;
		return_type_information = nullptr;
		tuple_information = vector<shared_ptr<type_information>>();
		lit = nullptr;
		class_name = "";
	}

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, vector<shared_ptr<type_information>> ti) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = vk;
		list_information = nullptr;
		key_information = nullptr;
		value_information = nullptr;
		return_type_information = nullptr;
		tuple_information = ti;
		lit = nullptr;
		class_name = "";
	}

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, type_information rti) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = vk;
		list_information = nullptr;
		key_information = nullptr;
		value_information = nullptr;
		tuple_information = vector<shared_ptr<type_information>>();
		return_type_information = make_shared<type_information>(rti);
		lit = nullptr;
		class_name = "";
	}

	type_information::type_information(type_information t_inf, value_kind vk) {
		t_kind = t_inf.t_kind;
		tp_kind = t_inf.tp_kind;
		c_kind = t_inf.c_kind;
		v_kind = vk;
		list_information = t_inf.list_information;
		key_information = t_inf.key_information;
		value_information = t_inf.value_information;
		tuple_information = t_inf.tuple_information;
		return_type_information = t_inf.return_type_information;
		lit = t_inf.lit;
		class_name = t_inf.class_name;
	}

	type_information::type_information(type_information t_inf, shared_ptr<literal> l) {
		t_kind = t_inf.t_kind;
		tp_kind = t_inf.tp_kind;
		c_kind = t_inf.c_kind;
		v_kind = t_inf.v_kind;
		list_information = t_inf.list_information;
		key_information = t_inf.key_information;
		value_information = t_inf.value_information;
		tuple_information = t_inf.tuple_information;
		return_type_information = t_inf.return_type_information;
		lit = l;
		class_name = t_inf.class_name;
	}

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind ck, value_kind vk, string cn) {
		t_kind = tk;
		tp_kind = tpk;
		c_kind = ck;
		v_kind = vk;
		list_information = nullptr;
		key_information = nullptr;
		value_information = nullptr;
		tuple_information = vector<shared_ptr<type_information>>();
		return_type_information = nullptr;
		lit = nullptr;
		class_name = cn;
	}

	type_information::~type_information() {

	}

	shared_ptr<literal> type_information::get_literal() {
		return lit;
	}

	const type_kind type_information::get_type_kind() {
		return t_kind;
	}

	const type_pure_kind type_information::get_type_pure_kind() {
		return tp_kind;
	}

	const type_class_kind type_information::get_type_class_kind() {
		return c_kind;
	}

	const bool type_information::operator==(type_information t_inf) {
		bool ret = (t_kind == t_inf.t_kind && tp_kind == t_inf.tp_kind && c_kind == t_inf.c_kind && class_name == t_inf.class_name);
		return ret;
	}

	const bool type_information::operator!=(type_information t_inf) {
		return !(*this == t_inf);
	}

	const value_kind type_information::get_value_kind() {
		return v_kind;
	}

	shared_ptr<type_information> type_information::get_value_information() {
		return value_information;
	}

	shared_ptr<type_information> type_information::get_key_information() {
		return key_information;
	}

	shared_ptr<type_information> type_information::get_list_information() {
		return list_information;
	}

	vector<shared_ptr<type_information>> type_information::get_tuple_information() {
		return tuple_information;
	}

	string type_information::get_class_name() {
		return class_name;
	}

	symbol::symbol(type_information ti, immut_kind ik, shared_ptr<literal> ident, function_kind fk, structure_kind sk, vector<type_information> fa, shared_ptr<symbol_table> st, function_declaration_definition_kind fddk, function_va_args_kind fvak,
		structure_declaration_definition_kind sddk, module_kind mk, enum_kind ek, enum_statement_kind esk) : t_inf(ti) {
		i_kind = ik;
		identifier = ident;
		f_kind = fk;
		function_arguments = fa;
		class_type_information = st;
		fdd_kind = fddk;
		s_kind = sk;
		sdd_kind = sddk;
		fva_kind = fvak;
		m_kind = mk;
		e_kind = ek;
		es_kind = esk;
	}

	symbol::~symbol() {

	}

	type_information symbol::get_type_information() {
		return t_inf;
	}

	const immut_kind symbol::get_immut_kind() {
		return i_kind;
	}

	const function_kind symbol::get_function_kind() {
		return f_kind;
	}

	shared_ptr<literal> symbol::get_identifier() {
		return identifier;
	}

	vector<type_information> symbol::get_function_arguments() {
		return function_arguments;
	}

	shared_ptr<symbol_table> symbol::get_class_type_information() {
		return class_type_information;
	}

	const function_declaration_definition_kind symbol::get_function_declaration_definition_kind() {
		return fdd_kind;
	}

	function_declaration_definition_kind symbol::set_function_declaration_definition_kind(function_declaration_definition_kind fddk) {
		fdd_kind = fddk;
		return fdd_kind;
	}

	const structure_declaration_definition_kind symbol::get_structure_declaration_definition_kind() {
		return sdd_kind;
	}

	structure_declaration_definition_kind symbol::set_structure_declaration_definition_kind(structure_declaration_definition_kind sddk) {
		sdd_kind = sddk;
		return sdd_kind;
	}

	const structure_kind symbol::get_structure_kind() {
		return s_kind;
	}

	const function_va_args_kind symbol::get_function_va_args_kind() {
		return fva_kind;
	}

	const enum_kind symbol::get_enum_kind() {
		return e_kind;
	}

	const enum_statement_kind symbol::get_enum_statement_kind() {
		return es_kind;
	}

	enum_statement_kind symbol::set_enum_statement_kind(enum_statement_kind esk) {
		es_kind = esk;
		return esk;
	}

	symbol_table::symbol_table() {
		sym_table = vector<shared_ptr<symbol>>();
	}

	symbol_table::~symbol_table() {

	}

	vector<shared_ptr<symbol>> symbol_table::get_symbol_table() {
		return sym_table;
	}

	shared_ptr<symbol> symbol_table::operator[](int i) {
		return sym_table[i];
	}

	vector<shared_ptr<symbol>> symbol_table::find_all_symbols(shared_ptr<literal> lit) {
		vector<shared_ptr<symbol>> ret;
		for (int i = 0; i < sym_table.size(); i++)
			if (sym_table[i]->get_identifier()->get_raw_literal()->get_raw_string() == lit->get_raw_literal()->get_raw_string())
			ret.push_back(sym_table[i]);
		return ret;
	}

	vector<shared_ptr<symbol>> symbol_table::find_all_symbols(shared_ptr<annotated_literal> alit) {
		vector<shared_ptr<symbol>> ret;
		for (int i = 0; i < sym_table.size(); i++)
			if (sym_table[i]->get_identifier()->get_raw_literal()->get_raw_string() == alit->get_raw_literal()->get_raw_string())
			ret.push_back(sym_table[i]);
		return ret;
	}

	shared_ptr<symbol> symbol_table::add_symbol(shared_ptr<root_node> root, source_token_list::iterator pos, shared_ptr<symbol> sym) {
		sym_table.push_back(sym);
		return sym;
	}

	analyze_ast::analyze_ast(shared_ptr<root_node> r, vector<scope_kind> skl, vector<shared_ptr<symbol_table>> stl, vector<shared_ptr<statement>> stmt_list) {
		root = r;
		sym_table_list = stl;
		ann_root_node = make_shared<annotated_root_node>(r, vector<shared_ptr<annotated_statement>>());
		s_kind_list = skl;
		statement_list = stmt_list;
	}

	analyze_ast::~analyze_ast() {

	}

	shared_ptr<root_node> analyze_ast::get_root_node() {
		return root;
	}

	vector<shared_ptr<symbol_table>> analyze_ast::get_symbol_table() {
		return sym_table_list;
	}

	shared_ptr<annotated_root_node> analyze_ast::get_annotated_root_node() {
		return ann_root_node;
	}

	pair<vector<shared_ptr<symbol>>, bool> analyze_ast::find_all_symbols(shared_ptr<annotated_literal> sym) {
		for (int i = sym_table_list.size() - 1; i >= 0; i--) {
			vector<shared_ptr<symbol>> stl = sym_table_list[i]->find_all_symbols(sym);
			if (stl.size() > 0)
				return make_pair(stl, i == sym_table_list.size() - 1);
		}
		return make_pair(vector<shared_ptr<symbol>>(), false);
	}

	pair<vector<shared_ptr<symbol>>, bool> analyze_ast::find_all_symbols(shared_ptr<literal> lit) {
		for (int i = sym_table_list.size() - 1; i >= 0; i--) {
			vector<shared_ptr<symbol>> stl = sym_table_list[i]->find_all_symbols(lit);
			if (stl.size() > 0)
				return make_pair(stl, i == sym_table_list.size() - 1);
		}
		return make_pair(vector<shared_ptr<symbol>>(), false);
	}

	vector<shared_ptr<statement>> analyze_ast::get_statement_list() {
		return statement_list;
	}

	const bool analyze_ast::perform_semantic_analysis() {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (s_kind_list.size() == 1 && s_kind_list[0] == scope_kind::SCOPE_GLOBAL) {
			sym_table_list[0]->add_symbol(root, root->get_lexer()->get_source_token_list()->end(), make_shared<symbol>(_any, immut_kind::IMMUT_YES, make_shared<literal>(root, make_shared<token>(-1, -1, -1, builtins::builtin_print, "", -1, token_kind::TOKEN_IDENTIFIER)), function_kind::FUNCTION_YES, structure_kind::STRUCTURE_NONE,
				vector<type_information>(), make_shared<symbol_table>(), function_declaration_definition_kind::FUNCTION_KIND_DEFINITION, function_va_args_kind::FUNCTION_VA_ARGS_YES, structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE));
		}
		bool ret = true;
		if (root->get_diagnostics_reporter()->get_error_count() > 0)
			ret = false;
		if (s_kind_list.size() == 1 && s_kind_list[0] == scope_kind::SCOPE_GLOBAL); //Add builtin symbols here
		vector<shared_ptr<statement>> stmt_list = statement_list;
		for (int i = 0; i < stmt_list.size(); i++) {
			if (!stmt_list[i]->get_valid())
				ret = false;
			else {
				shared_ptr<annotated_statement> astmt = analyze_statement(stmt_list[i]);
				if (astmt->get_type_information() == bad) ret = false;
				ann_root_node->add_annotated_statement(astmt);
			}
		}
		bool one = (s_kind_list.size() == 1 && s_kind_list[0] == scope_kind::SCOPE_GLOBAL);
		bool two = (s_kind_list.size() > 0 && s_kind_list[s_kind_list.size() - 1] == scope_kind::SCOPE_MODULE);
		if (one) {
			for (int i = 0; i < sym_table_list.size(); i++) {
				vector<shared_ptr<symbol>> sym_tab = sym_table_list[i]->get_symbol_table();
				for (int j = 0; j < sym_tab.size(); j++) {
					shared_ptr<symbol> sym = sym_tab[j];
					if (sym->get_function_declaration_definition_kind() == function_declaration_definition_kind::FUNCTION_KIND_FORWARD_DECLARATION)
						root->get_diagnostics_reporter()->print(diagnostic_messages::function_declared_not_defined, sym->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					if (sym->get_structure_declaration_definition_kind() == structure_declaration_definition_kind::STRUCTURE_KIND_FORWARD_DECLARATION)
						root->get_diagnostics_reporter()->print(diagnostic_messages::structure_declared_not_defined, sym->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					if (sym->get_enum_statement_kind() == enum_statement_kind::ENUM_STATEMENT_DECLARATION)
						root->get_diagnostics_reporter()->print(diagnostic_messages::enum_declared_but_not_defined, sym->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				}
			}
		}
		else if (two) {
			vector<shared_ptr<symbol>> sym_tab = sym_table_list[sym_table_list.size() - 1]->get_symbol_table();
			for (int j = 0; j < sym_tab.size(); j++) {
				shared_ptr<symbol> sym = sym_tab[j];
				if (sym->get_function_declaration_definition_kind() == function_declaration_definition_kind::FUNCTION_KIND_FORWARD_DECLARATION)
					root->get_diagnostics_reporter()->print(diagnostic_messages::function_declared_not_defined, sym->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				if (sym->get_structure_declaration_definition_kind() == structure_declaration_definition_kind::STRUCTURE_KIND_FORWARD_DECLARATION)
					root->get_diagnostics_reporter()->print(diagnostic_messages::structure_declared_not_defined, sym->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				if (sym->get_enum_statement_kind() == enum_statement_kind::ENUM_STATEMENT_DECLARATION)
					root->get_diagnostics_reporter()->print(diagnostic_messages::enum_declared_but_not_defined, sym->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			}
		}
		return ret;
	}

	shared_ptr<annotated_return_statement> analyze_ast::analyze_return_statement(shared_ptr<return_statement> ret) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!ret->get_valid())
			return make_shared<annotated_return_statement>(ann_root_node, ret, nullptr, bad);
		type_information _ret_any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NOT_APPLICABLE);
		if (ret->get_return_statement_kind() == return_statement_kind::RETURN_STATEMENT_EMPTY)
			return make_shared<annotated_return_statement>(ann_root_node, ret, nullptr, _ret_any);
		shared_ptr<annotated_binary_expression> abexpr = analyze_binary_expression(ret->get_binary_expression());
		vector<scope_kind>::iterator it = find(s_kind_list.begin(), s_kind_list.end(), scope_kind::SCOPE_FUNCTION);
		if (it >= s_kind_list.end()) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::return_statements_can_only_be_in_functions, ret->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_return_statement>(ann_root_node, ret, nullptr, bad);
		}
		return make_shared<annotated_return_statement>(ann_root_node, ret, abexpr, _ret_any);
	}

	shared_ptr<annotated_conditional_statement> analyze_ast::analyze_conditional_statement(shared_ptr<conditional_statement> cond) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!cond->get_valid())
			return make_shared<annotated_conditional_statement>(ann_root_node, cond, nullptr, vector<shared_ptr<annotated_statement>>(), nullptr, vector<shared_ptr<annotated_statement>>(), bad);
		type_information _cond_any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NOT_APPLICABLE);
		shared_ptr<annotated_binary_expression> ic = analyze_binary_expression(cond->get_if_conditional());
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		vector<shared_ptr<annotated_statement>> isl;
		shared_ptr<symbol_table> sym_table = make_shared<symbol_table>();
		s_kind_list.push_back(scope_kind::SCOPE_CONDITIONAL);
		sym_table_list.push_back(sym_table);
		for (int i = 0; i < cond->get_if_statement_list().size(); i++) {
			shared_ptr<annotated_statement> astmt = analyze_statement(cond->get_if_statement_list()[i]);
			if (astmt->get_statement_kind() == statement_kind::STATEMENT_FUNCTION || astmt->get_statement_kind() == statement_kind::STATEMENT_MODULE ||
				astmt->get_statement_kind() == statement_kind::STATEMENT_STRUCTURE || astmt->get_statement_kind() == statement_kind::STATEMENT_ENUM_STATEMENT) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::modules_functions_structures_enums_not_expected_here, astmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_conditional_statement>(ann_root_node, cond, nullptr, vector<shared_ptr<annotated_statement>>(), nullptr, vector<shared_ptr<annotated_statement>>(), bad);
			}
			isl.push_back(astmt);
		}
		s_kind_list.pop_back();
		sym_table_list.pop_back();
		shared_ptr<annotated_binary_expression> ec = nullptr;
		vector<shared_ptr<annotated_statement>> esl;
		if (cond->get_conditional_else_conditional_kind() == conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NOT_PRESENT ||
			cond->get_conditional_else_conditional_kind() == conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE);
		else
			ec = analyze_binary_expression(cond->get_else_conditional());
		if (cond->get_conditional_else_statement_kind() == conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE ||
			cond->get_conditional_else_statement_kind() == conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NOT_PRESENT);
		else {
			shared_ptr<symbol_table> sym_table2 = make_shared<symbol_table>();
			s_kind_list.push_back(scope_kind::SCOPE_CONDITIONAL);
			sym_table_list.push_back(sym_table2);
			for (int i = 0; i < cond->get_else_statement_list().size(); i++) {
				shared_ptr<annotated_statement> astmt = analyze_statement(cond->get_else_statement_list()[i]);
				if (astmt->get_statement_kind() == statement_kind::STATEMENT_FUNCTION || astmt->get_statement_kind() == statement_kind::STATEMENT_MODULE ||
					astmt->get_statement_kind() == statement_kind::STATEMENT_STRUCTURE || astmt->get_statement_kind() == statement_kind::STATEMENT_ENUM_STATEMENT) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::modules_functions_structures_enums_not_expected_here, astmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_conditional_statement>(ann_root_node, cond, nullptr, vector<shared_ptr<annotated_statement>>(), nullptr, vector<shared_ptr<annotated_statement>>(), bad);
				}
				esl.push_back(astmt);
			}
			sym_table_list.pop_back();
			s_kind_list.pop_back();
		}
		return make_shared<annotated_conditional_statement>(ann_root_node, cond, ic, isl, ec, esl, _cond_any);
	}

	shared_ptr<annotated_enum_statement> analyze_ast::analyze_enum_statement(shared_ptr<enum_statement> _enum) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!_enum->get_valid())
			return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
		type_information parent_enum(type_kind::TYPE_ENUM, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, value_kind::VALUE_NOT_APPLICABLE);
		type_information child_enum(type_kind::TYPE_ENUM_CHILD, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, value_kind::VALUE_LVALUE);
		if (_enum->get_identifier()->get_literal_kind() != literal_kind::LITERAL_IDENTIFIER) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, _enum->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
		}
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, _enum->get_identifier(), parent_enum);
		shared_ptr<symbol_table> sym = make_shared<symbol_table>();
		s_kind_list.push_back(scope_kind::SCOPE_ENUM);
		sym_table_list.push_back(sym);
		vector<shared_ptr<annotated_literal>> ailist;
		vector<string> test;
		for (int i = 0; i < _enum->get_identifier_list().size(); i++) {
			shared_ptr<literal> lit = _enum->get_identifier_list()[i];
			if (lit->get_literal_kind() != literal_kind::LITERAL_IDENTIFIER) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, lit->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
			}
			string ident_name = lit->get_raw_literal()->get_raw_string();
			if (find(test.begin(), test.end(), ident_name) != test.end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::enum_names_must_be_unique, lit->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
			}
			ailist.push_back(make_shared<annotated_literal>(ann_root_node, lit, child_enum));
		}
		s_kind_list.pop_back();
		sym_table_list.pop_back();
		pair<vector<shared_ptr<symbol>>, bool> found_symbols_pair = find_all_symbols(alit);
		vector<shared_ptr<symbol>> found_symbols = found_symbols_pair.first;
		bool current_scope = found_symbols_pair.second;
		if (found_symbols.size() == 0 || !current_scope) {
			shared_ptr<symbol> sym = make_shared<symbol>(parent_enum, immut_kind::IMMUT_YES, _enum->get_identifier(), function_kind::FUNCTION_NONE, structure_kind::STRUCTURE_NONE, vector<type_information>(), make_shared<symbol_table>(),
				function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE, structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE,
				enum_kind::ENUM_YES, _enum->get_enum_statement_kind());
			sym_table_list[sym_table_list.size() - 1]->add_symbol(_enum, _enum->get_position(), sym);
			if(_enum->get_enum_statement_kind() == enum_statement_kind::ENUM_STATEMENT_DECLARATION)
				return make_shared<annotated_enum_statement>(ann_root_node, _enum, alit, vector<shared_ptr<annotated_literal>>(), parent_enum);
		}
		else if (found_symbols.size() == 1) {
			shared_ptr<symbol> sym = found_symbols[0];
			if (sym->get_enum_statement_kind() == enum_statement_kind::ENUM_STATEMENT_DEFINITION) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, _enum->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
			}
			else {
				if (sym->get_enum_statement_kind() == enum_statement_kind::ENUM_STATEMENT_DECLARATION && _enum->get_enum_statement_kind() == enum_statement_kind::ENUM_STATEMENT_DEFINITION)
					sym->set_enum_statement_kind(enum_statement_kind::ENUM_STATEMENT_DEFINITION);
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, _enum->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
				}
			}
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, _enum->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_enum_statement>(ann_root_node, _enum, nullptr, vector<shared_ptr<annotated_literal>>(), bad);
		}
		return make_shared<annotated_enum_statement>(ann_root_node, _enum, alit, ailist, parent_enum);
	}

	shared_ptr<annotated_while_statement> analyze_ast::analyze_while_statement(shared_ptr<while_statement> _while) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!_while->get_valid())
			return make_shared<annotated_while_statement>(ann_root_node, _while, nullptr, vector<shared_ptr<annotated_statement>>(), bad);
		shared_ptr<annotated_binary_expression> abexpr = analyze_binary_expression(_while->get_condition());
		type_information _while_type(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NOT_APPLICABLE);

		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NONE);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NONE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NONE);
		if (abexpr->get_type_information() == _boolean || abexpr->get_type_information() == _any || abexpr->get_type_information() == _nil);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, _while->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_while_statement>(ann_root_node, _while, nullptr, vector<shared_ptr<annotated_statement>>(), bad);
		}
		vector<shared_ptr<statement>> stmt_list = _while->get_statement_list();
		vector<shared_ptr<annotated_statement>> astmt_list;
		shared_ptr<symbol_table> sym_table = make_shared<symbol_table>();
		s_kind_list.push_back(scope_kind::SCOPE_LOOP);
		sym_table_list.push_back(sym_table);
		for (int i = 0; i < stmt_list.size(); i++) {
			shared_ptr<annotated_statement> astmt = analyze_statement(stmt_list[i]);
			if (astmt->get_statement_kind() == statement_kind::STATEMENT_FUNCTION || astmt->get_statement_kind() == statement_kind::STATEMENT_MODULE ||
				astmt->get_statement_kind() == statement_kind::STATEMENT_STRUCTURE || astmt->get_statement_kind() == statement_kind::STATEMENT_ENUM_STATEMENT) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::modules_functions_structures_enums_not_expected_here, astmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_while_statement>(ann_root_node, _while, nullptr, vector<shared_ptr<annotated_statement>>(), bad);
			}
			astmt_list.push_back(astmt);
		}
		s_kind_list.pop_back();
		sym_table_list.pop_back();
		return make_shared<annotated_while_statement>(ann_root_node, _while, abexpr, astmt_list, _while_type);
	}

	shared_ptr<annotated_statement> analyze_ast::analyze_statement(shared_ptr<statement> stmt) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		shared_ptr<annotated_binary_expression> abexpr = nullptr;
		shared_ptr<annotated_declaration> adecl = nullptr;
		shared_ptr<annotated_function> afunc = nullptr;
		shared_ptr<annotated_structure> astruc = nullptr;
		shared_ptr<annotated_module> amod = nullptr;
		shared_ptr<annotated_return_statement> aret = nullptr;
		shared_ptr<annotated_conditional_statement> acond = nullptr;
		shared_ptr<annotated_enum_statement> aenum = nullptr;
		shared_ptr<annotated_while_statement> awhile = nullptr;
		if (!stmt->get_valid())
			return make_shared<annotated_statement>(ann_root_node, stmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, bad);
		if (stmt->get_statement_kind() == statement_kind::STATEMENT_DECLARATION)
			adecl = analyze_declaration(stmt->get_declaration());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_EXPRESSION)
			abexpr = analyze_binary_expression(stmt->get_binary_expression());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_FUNCTION)
			afunc = analyze_function(stmt->get_function());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_STRUCTURE)
			astruc = analyze_structure(stmt->get_structure());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_MODULE)
			amod = analyze_module(stmt->get_module());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_RETURN_STATEMENT)
			aret = analyze_return_statement(stmt->get_return_statement());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_CONDITIONAL_STATEMENT)
			acond = analyze_conditional_statement(stmt->get_conditional_statement());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_ENUM_STATEMENT)
			aenum = analyze_enum_statement(stmt->get_enum_statement());
		else if (stmt->get_statement_kind() == statement_kind::STATEMENT_WHILE_STATMENT)
			awhile = analyze_while_statement(stmt->get_while_statement());
		else
			return make_shared<annotated_statement>(ann_root_node, stmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, bad);
		type_information ret = bad;
		if (adecl != nullptr) ret = adecl->get_type_information();
		else if (abexpr != nullptr) ret = abexpr->get_type_information();
		else if (afunc != nullptr) ret = afunc->get_type_information();
		else if (astruc != nullptr) ret = astruc->get_type_information();
		else if (amod != nullptr) ret = amod->get_type_information();
		else if (aret != nullptr) ret = aret->get_type_information();
		else if (acond != nullptr) ret = acond->get_type_information();
		else if (aenum != nullptr) ret = aenum->get_type_information();
		else if (awhile != nullptr) ret = awhile->get_type_information();
		ret = type_information(ret, value_kind::VALUE_NOT_APPLICABLE);
		return make_shared<annotated_statement>(ann_root_node, stmt, abexpr, adecl, afunc, astruc, amod, aret, acond, aenum, awhile, ret);
	}

	shared_ptr<annotated_module> analyze_ast::analyze_module(shared_ptr<module> mod) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!mod->get_valid())
			return make_shared<annotated_module>(ann_root_node, mod, nullptr, vector<shared_ptr<annotated_statement>>(), vector<type_information>(), bad);
		bool immut = false;
		if (mod->get_declspec_list()->get_declspecs_list().size() == 1)
			immut = true;
		else if (mod->get_declspec_list()->get_declspecs_list().size() == 0)
			immut = false;
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_only_one_immut, mod->get_declspec_list()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			immut = true;
		}
		type_information _module(type_kind::TYPE_MODULE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, value_kind::VALUE_NOT_APPLICABLE, mod->get_identifier()->get_raw_literal()->get_raw_string());
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, mod->get_identifier(), _any);
		vector<shared_ptr<statement>> stmt_list = mod->get_statement_list();
		vector<shared_ptr<annotated_statement>> astmt_list;
		shared_ptr<symbol_table> sym_table = make_shared<symbol_table>();
		pair<vector<shared_ptr<symbol>>, bool> results_pair = find_all_symbols(alit);
		if (results_pair.first.size() == 0 || !results_pair.second);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, mod->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_module>(ann_root_node, mod, nullptr, vector<shared_ptr<annotated_statement>>(), vector<type_information>(), bad);
		}
		sym_table_list.push_back(sym_table);
		s_kind_list.push_back(scope_kind::SCOPE_MODULE);
		analyze_ast aa(root, s_kind_list, sym_table_list, stmt_list);
		aa.perform_semantic_analysis();
		s_kind_list.pop_back();
		sym_table_list.pop_back();
		sym_table_list[sym_table_list.size() - 1]->add_symbol(root, mod->get_position(), make_shared<symbol>(_any, immut_kind::IMMUT_NO, mod->get_identifier(), function_kind::FUNCTION_NONE, structure_kind::STRUCTURE_NONE, vector<type_information>(),
			sym_table, function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE, structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_YES, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE));
		vector<type_information> t_inf_list;
		for (int i = 0; i < aa.get_statement_list().size(); i++)
			t_inf_list.push_back(_any);
		return make_shared<annotated_module>(ann_root_node, mod, alit, aa.get_annotated_root_node()->get_annotated_statement_list(), t_inf_list, _module);
	}

	shared_ptr<annotated_function> analyze_ast::analyze_function(shared_ptr<function> func) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!func->get_valid())
			return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
		bool immut = false;
		if (func->get_declspec_list()->get_declspecs_list().size() == 1)
			immut = true;
		else if (func->get_declspec_list()->get_declspecs_list().size() == 0)
			immut = false;
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_only_one_immut, func->get_declspec_list()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			immut = true;
		}
		type_information _any_func(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_NOT_APPLICABLE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE);
		type_information _tuple(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, func->get_identifier(), _any_func);
		if (alit->get_literal_kind() == literal_kind::LITERAL_IDENTIFIER);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
		}
		vector<shared_ptr<declaration>> decl_list = func->get_parameter_list();
		vector<shared_ptr<annotated_declaration>> ann_decl_list;
		shared_ptr<symbol_table> sym_table = make_shared<symbol_table>();
		vector<type_information> t_inf_list;
		for (int i = 0; i < decl_list.size(); i++) {
			if (!decl_list[i]->get_partial())
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			if (decl_list[i]->get_declspec_list()->get_declspecs_list().size() > 0) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::immut_not_allowed_for_function_parameters, decl_list[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
			pair<shared_ptr<annotated_declaration>, shared_ptr<symbol>> pai = analyze_parameter(decl_list[i]);
			shared_ptr<annotated_declaration> adecl = pai.first;
			shared_ptr<symbol> sym = pai.second;
			if (sym == nullptr)
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			vector<shared_ptr<symbol>> results = sym_table->find_all_symbols(adecl->get_identifier());
			if (results.size() > 0) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_of_a_function_parameter + adecl->get_identifier()->get_raw_literal()->get_raw_string() + ".", adecl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
			sym_table->add_symbol(root, func->get_position(), sym);
			t_inf_list.push_back(sym->get_type_information());
			ann_decl_list.push_back(adecl);
		}
		pair<vector<shared_ptr<symbol>>, bool> symbols_found_pair = find_all_symbols(alit);
		vector<shared_ptr<symbol>> symbols_found = symbols_found_pair.first;
		bool current_scope = symbols_found_pair.second;
		for (int i = 0; i < symbols_found.size(); i++) {
			shared_ptr<symbol> sym = symbols_found[i];
			if (sym->get_function_kind() == function_kind::FUNCTION_YES && t_inf_list.size() == sym->get_function_arguments().size()) {
				if (sym->get_function_va_args_kind() == func->get_function_va_args_kind());
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::ambiguous_function_overload, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
				}
			}
			else if (sym->get_function_kind() == function_kind::FUNCTION_YES && t_inf_list.size() != sym->get_function_arguments().size()) {
				int sym_size = sym->get_function_arguments().size();
				int actual_size = t_inf_list.size();
				if (sym->get_function_va_args_kind() != func->get_function_va_args_kind()) {
					// stored: func -> temp(var ->x, var ->y, _) {} 2
					// now: func -> temp(var ->x) {} 1
					if (sym->get_function_va_args_kind() == function_va_args_kind::FUNCTION_VA_ARGS_YES && func->get_function_va_args_kind() == function_va_args_kind::FUNCTION_VA_ARGS_NO) {
						if (sym_size > actual_size) {
							symbols_found.erase(symbols_found.begin() + i, symbols_found.begin() + i + 1);
							i = -1;
						}
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::ambiguous_function_overload, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
						}
					}
					// stored: func -> temp(var ->x) {} 1
					// now: func -> temp(var ->x, var ->y, _) {} 2
					else {
						if (actual_size > sym_size) {
							symbols_found.erase(symbols_found.begin() + i, symbols_found.begin() + i + 1);
							i = -1;
						}
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::ambiguous_function_overload, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
						}
					}
				}
				else {
					if (sym->get_function_va_args_kind() == function_va_args_kind::FUNCTION_VA_ARGS_YES) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::ambiguous_function_overload, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
					}
					else {
						symbols_found.erase(symbols_found.begin() + i, symbols_found.begin() + i + 1);
						i = -1;
					}
				}
			}
			else if (sym->get_function_kind() == function_kind::FUNCTION_NO || sym->get_function_kind() == function_kind::FUNCTION_NONE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
			else {
				symbols_found.erase(symbols_found.begin() + i, symbols_found.begin() + i + 1);
				i = -1;
			}
		}
		if (symbols_found.size() == 0 || !current_scope) {
			shared_ptr<symbol> func_sym = make_shared<symbol>(_any, immut_kind::IMMUT_NO, func->get_identifier(), function_kind::FUNCTION_YES, structure_kind::STRUCTURE_NO, t_inf_list, nullptr, func->get_function_kind(), func->get_function_va_args_kind(),
				structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE);
			sym_table_list[sym_table_list.size() - 1]->add_symbol(root, func->get_position(), func_sym);
			if (func->get_function_kind() == function_declaration_definition_kind::FUNCTION_KIND_FORWARD_DECLARATION)
				return make_shared<annotated_function>(ann_root_node, func, alit, ann_decl_list, vector<shared_ptr<annotated_statement>>(), func->get_function_kind(), _any_func);
		}
		else if (symbols_found.size() == 1) {
			shared_ptr<symbol> sym = symbols_found[0];
			if (sym->get_function_kind() == function_kind::FUNCTION_YES && t_inf_list.size() == sym->get_function_arguments().size() && sym->get_function_va_args_kind() == func->get_function_va_args_kind()) {
				if (sym->get_function_declaration_definition_kind() == function_declaration_definition_kind::FUNCTION_KIND_FORWARD_DECLARATION && func->get_function_kind() == function_declaration_definition_kind::FUNCTION_KIND_DEFINITION)
					sym->set_function_declaration_definition_kind(function_declaration_definition_kind::FUNCTION_KIND_DEFINITION);
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
				}
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, func->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
		}
		vector<shared_ptr<statement>> stmt_list = func->get_statement_list();
		for (int i = 0; i < stmt_list.size(); i++) {
			shared_ptr<statement> stmt = stmt_list[i];
			if (stmt->get_statement_kind() == statement_kind::STATEMENT_FUNCTION) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::function_definitions_cannot_be_nested, stmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
			else if (stmt->get_statement_kind() == statement_kind::STATEMENT_STRUCTURE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::structure_definitions_cannot_be_nested, stmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
			else if (stmt->get_statement_kind() == statement_kind::STATEMENT_MODULE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::modules_cannot_be_in_functions, stmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
			else if (stmt->get_statement_kind() == statement_kind::STATEMENT_ENUM_STATEMENT) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::enums_not_allowed_in_functions, stmt->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_function>(ann_root_node, func, nullptr, vector<shared_ptr<annotated_declaration>>(), vector<shared_ptr<annotated_statement>>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, bad);
			}
		}
		sym_table->add_symbol(root, root->get_lexer()->get_source_token_list()->end(), make_shared<symbol>(_tuple, immut_kind::IMMUT_YES, make_shared<literal>(root, make_shared<token>(-1, -1, -1, builtins::builtin__va_args__, "", -1, token_kind::TOKEN_IDENTIFIER)), function_kind::FUNCTION_NONE, structure_kind::STRUCTURE_NONE,
			vector<type_information>(), make_shared<symbol_table>(), function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE, structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE));
		s_kind_list.push_back(scope_kind::SCOPE_FUNCTION);
		sym_table_list.push_back(sym_table);
		analyze_ast aa(root, s_kind_list, sym_table_list, stmt_list);
		aa.perform_semantic_analysis();
		sym_table_list.pop_back();
		s_kind_list.pop_back();
		return make_shared<annotated_function>(ann_root_node, func, alit, ann_decl_list, aa.get_annotated_root_node()->get_annotated_statement_list(), func->get_function_kind(), _any_func);
	}

	shared_ptr<annotated_structure> analyze_ast::analyze_structure(shared_ptr<structure> struc) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!struc->get_valid())
			return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
		bool immut = false;
		if (struc->get_declspec_list()->get_declspecs_list().size() == 1)
			immut = true;
		else if (struc->get_declspec_list()->get_declspecs_list().size() == 0)
			immut = false;
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_only_one_immut, struc->get_declspec_list()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			immut = true;
		}
		type_information _custom(type_kind::TYPE_CUSTOM, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, value_kind::VALUE_NOT_APPLICABLE, struc->get_identifier()->get_raw_literal()->get_raw_string());
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, struc->get_identifier(), _custom);
		if (alit->get_literal_kind() == literal_kind::LITERAL_IDENTIFIER);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, struc->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
		}
		shared_ptr<symbol_table> sym_table = make_shared<symbol_table>();
		vector<shared_ptr<declaration>> decl_list = struc->get_declaration_list();
		vector<shared_ptr<annotated_declaration>> adecl_list;
		vector<type_information> t_inf_list;
		for (int i = 0; i < decl_list.size(); i++) {
			shared_ptr<declaration> decl = decl_list[i];
			if (decl->get_partial()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::partial_declarations_not_allowed, decl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
			}
			pair<shared_ptr<annotated_declaration>, shared_ptr<symbol>> pai = analyze_structure_declaration(decl);
			shared_ptr<annotated_declaration> adecl = pai.first;
			shared_ptr<symbol> sym = pai.second;
			if (sym == nullptr)
				return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
			if (adecl->get_binary_expression()->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES ||
				adecl->get_binary_expression()->get_type_information() == _any);
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::structure_declarations_need_pure_types, adecl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
			}
			vector<shared_ptr<symbol>> results = sym_table->find_all_symbols(adecl->get_identifier());
			if (results.size() > 0) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_of_a_function_parameter + adecl->get_identifier()->get_raw_literal()->get_raw_string() + ".", adecl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
			}
			sym_table->add_symbol(root, struc->get_position(), sym);
			adecl_list.push_back(adecl);
			t_inf_list.push_back(_any);
		}
		pair<vector<shared_ptr<symbol>>, bool> results_pair = find_all_symbols(alit);
		vector<shared_ptr<symbol>> results = results_pair.first;
		bool current_scope = results_pair.second;
		if (results.size() == 0 || !current_scope) {
			shared_ptr<symbol> struc_sym = make_shared<symbol>(_custom, immut_kind::IMMUT_NO, struc->get_identifier(), function_kind::FUNCTION_NO, structure_kind::STRUCTURE_YES,
				vector<type_information>(), sym_table, function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE, struc->get_structure_kind(), module_kind::MODULE_NONE, enum_kind::ENUM_NONE,
				enum_statement_kind::ENUM_STATEMENT_NONE);
			sym_table_list[sym_table_list.size() - 1]->add_symbol(root, struc->get_position(), struc_sym);
			if (struc->get_structure_kind() == structure_declaration_definition_kind::STRUCTURE_KIND_FORWARD_DECLARATION)
				return make_shared<annotated_structure>(ann_root_node, struc, alit, adecl_list, struc->get_structure_kind(), t_inf_list, _custom);
		}
		else if (results.size() == 1) {
			shared_ptr<symbol> sym = results[0];
			if (sym->get_identifier()->get_raw_literal()->get_raw_string() == alit->get_raw_literal()->get_raw_string() && ((immut && sym->get_immut_kind() == immut_kind::IMMUT_YES) ||
				(!immut && sym->get_immut_kind() == immut_kind::IMMUT_NO))) {
				if (sym->get_structure_declaration_definition_kind() == structure_declaration_definition_kind::STRUCTURE_KIND_FORWARD_DECLARATION)
					sym->set_structure_declaration_definition_kind(structure_declaration_definition_kind::STRUCTURE_KIND_DEFINITION);
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, struc->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
				}
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, struc->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
			}
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, struc->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_structure>(ann_root_node, struc, nullptr, vector<shared_ptr<annotated_declaration>>(), structure_declaration_definition_kind::STRUCTURE_KIND_NONE, vector<type_information>(), bad);
		}
		sym_table_list.push_back(sym_table);
		sym_table_list.pop_back();
		return make_shared<annotated_structure>(ann_root_node, struc, alit, adecl_list, structure_declaration_definition_kind::STRUCTURE_KIND_DEFINITION, t_inf_list, _custom);
	}

	pair<shared_ptr<annotated_declaration>, shared_ptr<symbol>> analyze_ast::analyze_structure_declaration(shared_ptr<declaration> decl) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!decl->get_valid())
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		bool immut = false;
		function_kind fk = function_kind::FUNCTION_NO;
		if (decl->get_declspec_list()->get_declspecs_list().size() > 0) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::structure_members_cannot_be_immut, decl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		}
		if (decl->get_identifier()->get_raw_literal()->get_token_kind() != token_kind::TOKEN_IDENTIFIER) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, decl->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		}
		if (decl->get_partial()) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::partial_declarations_not_allowed, decl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		}
		shared_ptr<annotated_binary_expression> abexpr = analyze_binary_expression(decl->get_binary_expression());
		type_information t_inf = type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE);
		if (t_inf.get_type_kind() == type_kind::TYPE_NONE)
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		shared_ptr<symbol> sym = make_shared<symbol>(t_inf, immut_kind::IMMUT_NO, decl->get_identifier(), fk, structure_kind::STRUCTURE_NO, vector<type_information>(), nullptr, function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE,
			structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE);
		t_inf = type_information(t_inf, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, decl->get_identifier(), t_inf);
		return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, alit, abexpr, type_information(t_inf, value_kind::VALUE_NOT_APPLICABLE)), sym);
	}

	pair<shared_ptr<annotated_declaration>, shared_ptr<symbol>> analyze_ast::analyze_parameter(shared_ptr<declaration> decl) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!decl->get_valid())
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		bool immut = false;
		function_kind fk = function_kind::FUNCTION_NO;
		if (decl->get_declspec_list()->get_declspecs_list().size() == 1)
			immut = true;
		else if (decl->get_declspec_list()->get_declspecs_list().size() == 0)
			immut = false;
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_only_one_immut, decl->get_declspec_list()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			immut = true;
		}
		if (decl->get_identifier()->get_raw_literal()->get_token_kind() != token_kind::TOKEN_IDENTIFIER) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, decl->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		}
		if (!decl->get_partial()) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::complete_declarations_not_allowed, decl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		}
		shared_ptr<annotated_binary_expression> abexpr = nullptr;
		type_information t_inf = type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE);
		if (t_inf.get_type_kind() == type_kind::TYPE_NONE)
			return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad), nullptr);
		shared_ptr<symbol> sym = make_shared<symbol>(t_inf, immut_kind::IMMUT_NO, decl->get_identifier(), fk, structure_kind::STRUCTURE_NO, vector<type_information>(), nullptr, function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE,
			structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE);
		t_inf = type_information(t_inf, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, decl->get_identifier(), t_inf);
		return make_pair(make_shared<annotated_declaration>(ann_root_node, decl, alit, abexpr, type_information(t_inf, value_kind::VALUE_NOT_APPLICABLE)), sym);
	}

	shared_ptr<annotated_declaration> analyze_ast::analyze_declaration(shared_ptr<declaration> decl) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!decl->get_valid())
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		bool immut = false;
		function_kind fk = function_kind::FUNCTION_NO;
		if (decl->get_declspec_list()->get_declspecs_list().size() == 1)
			immut = true;
		else if (decl->get_declspec_list()->get_declspecs_list().size() == 0)
			immut = false;
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_only_one_immut, decl->get_declspec_list()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			immut = true;
		}
		vector<shared_ptr<symbol>> vec;
		for (int i = 0; i < sym_table_list[sym_table_list.size() - 1]->get_symbol_table().size(); i++)
			if (sym_table_list[sym_table_list.size() - 1]->get_symbol_table()[i]->get_identifier()->get_raw_literal()->get_raw_string() == decl->get_identifier()->get_raw_literal()->get_raw_string())
				vec.push_back(sym_table_list[sym_table_list.size() - 1]->get_symbol_table()[i]);
		if (vec.size() > 0) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, decl->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, vec[vec.size() - 1]->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		}
		if (decl->get_identifier()->get_raw_literal()->get_token_kind() != token_kind::TOKEN_IDENTIFIER) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, decl->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		}
		if (decl->get_partial()) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::partial_declarations_not_allowed, decl->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		}
		shared_ptr<annotated_binary_expression> abexpr = analyze_binary_expression(decl->get_binary_expression());
		type_information t_inf = abexpr->get_type_information();
		if (t_inf.get_type_kind() == type_kind::TYPE_NONE)
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		shared_ptr<symbol> sym = make_shared<symbol>(t_inf, immut_kind::IMMUT_NO, decl->get_identifier(), fk, structure_kind::STRUCTURE_NO, vector<type_information>(), nullptr, function_declaration_definition_kind::FUNCTION_KIND_NONE, function_va_args_kind::FUNCTION_VA_ARGS_NONE,
			structure_declaration_definition_kind::STRUCTURE_KIND_NONE, module_kind::MODULE_NONE, enum_kind::ENUM_NONE, enum_statement_kind::ENUM_STATEMENT_NONE);
		sym_table_list[sym_table_list.size() - 1]->add_symbol(root, decl->get_position(), sym);
		t_inf = type_information(t_inf, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, decl->get_identifier(), t_inf);
		return make_shared<annotated_declaration>(ann_root_node, decl, alit, abexpr, type_information(t_inf, value_kind::VALUE_NOT_APPLICABLE));
	}

	shared_ptr<annotated_binary_expression> analyze_ast::analyze_binary_expression(shared_ptr<binary_expression> bexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!bexpr->get_valid())
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information lhs = bad;
		type_information lhs_forced = bad;
		type_information rhs = bad;
		type_information rhs_forced = bad;
		shared_ptr<annotated_root_node> bexpr_lhs = nullptr;
		shared_ptr<annotated_root_node> bexpr_rhs = nullptr;
		if (bexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			shared_ptr<binary_expression> bexpr1 = static_pointer_cast<binary_expression>(bexpr->get_lhs());
			bexpr_lhs = analyze_binary_expression(bexpr1);
			lhs = static_pointer_cast<annotated_binary_expression>(bexpr_lhs)->get_type_information();
			lhs_forced = static_pointer_cast<annotated_binary_expression>(bexpr_lhs)->get_forced_type_information();
		}
		else if (bexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION) {
			shared_ptr<unary_expression> uexpr = static_pointer_cast<unary_expression>(bexpr->get_lhs());
			bexpr_lhs = analyze_unary_expression(uexpr);
			lhs = static_pointer_cast<annotated_unary_expression>(bexpr_lhs)->get_type_information();
			lhs_forced = static_pointer_cast<annotated_unary_expression>(bexpr_lhs)->get_forced_type_information();
		}
		else if (bexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			shared_ptr<ternary_expression> texpr = static_pointer_cast<ternary_expression>(bexpr->get_lhs());
			bexpr_lhs = analyze_ternary_expression(texpr);
			lhs = static_pointer_cast<annotated_ternary_expression>(bexpr_lhs)->get_type_information();
			lhs_forced = static_pointer_cast<annotated_ternary_expression>(bexpr_lhs)->get_forced_type_information();
		}
		else
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
		if (bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION) {
			shared_ptr<unary_expression> uexpr = static_pointer_cast<unary_expression>(bexpr->get_rhs());
			bexpr_rhs = analyze_unary_expression(uexpr);
			rhs = static_pointer_cast<annotated_unary_expression>(bexpr_rhs)->get_type_information();
			rhs_forced = static_pointer_cast<annotated_unary_expression>(bexpr_rhs)->get_forced_type_information();
		}
		else if (bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			shared_ptr<binary_expression> bexpr1 = static_pointer_cast<binary_expression>(bexpr->get_rhs());
			bexpr_rhs = analyze_binary_expression(bexpr1);
			rhs = static_pointer_cast<annotated_binary_expression>(bexpr_rhs)->get_type_information();
			rhs_forced = static_pointer_cast<annotated_binary_expression>(bexpr_rhs)->get_forced_type_information();
		}
		else if (bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			shared_ptr<ternary_expression> texpr = static_pointer_cast<ternary_expression>(bexpr->get_rhs());
			bexpr_rhs = analyze_ternary_expression(texpr);
			rhs = static_pointer_cast<annotated_ternary_expression>(bexpr_rhs)->get_type_information();
			rhs_forced = static_pointer_cast<annotated_ternary_expression>(bexpr_rhs)->get_forced_type_information();
		}
		else {
			bexpr_rhs = nullptr;
			rhs = bad;
			rhs_forced = bad;
		}
		if (bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_NONE || bexpr->get_rhs() == nullptr)
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, bexpr_lhs, bexpr_rhs, lhs, lhs_forced, rhs, bad, lhs, bad);
		deduce_binary_expression_type dbet(lhs, bexpr->get_binary_operation_kind(), rhs);
		type_information t_inf = dbet.deduce_type();
		if (t_inf == bad) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, bexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
		}
		if ((lhs == _int && rhs == _float) || (lhs == _float && rhs == _int))
			root->get_diagnostics_reporter()->print(diagnostic_messages::unequal_but_compatible_types, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
		binary_operation_kind bopk = bexpr->get_binary_operation_kind();
		if (bopk == binary_operation_kind::BINARY_OPERATION_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_PLUS_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_MINUS_EQUALS ||
			bopk == binary_operation_kind::BINARY_OPERATION_MULTIPLY_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_DIVIDE_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_MODULUS_EQUALS ||
			bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_EXPONENT_EQUALS ||
			bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_AND_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_OR_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR_EQUALS) {
			if (lhs.get_value_kind() != value_kind::VALUE_LVALUE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_lvalue, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
			}
			if (lhs.get_literal() != nullptr) {
				vector<shared_ptr<symbol>> results = find_all_symbols(lhs.get_literal()).first;
				if (results.size() > 0 && results[results.size() - 1]->get_immut_kind() == immut_kind::IMMUT_YES) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::immut_value_cannot_be_modified, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, results[results.size() - 1]->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
					return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
				}
			}
		}
		return make_shared<annotated_binary_expression>(ann_root_node, bexpr, bexpr_lhs, bexpr_rhs, lhs, lhs_forced, rhs, rhs_forced, t_inf, bad);
	}

	shared_ptr<annotated_ternary_expression> analyze_ast::analyze_ternary_expression(shared_ptr<ternary_expression> texpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!texpr->get_valid())
			return make_shared<annotated_ternary_expression>(ann_root_node, texpr, nullptr, nullptr, nullptr, bad, bad);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		shared_ptr<annotated_binary_expression> abexpr1 = analyze_binary_expression(static_pointer_cast<binary_expression>(texpr->get_condition()));
		shared_ptr<annotated_binary_expression> abexpr2 = analyze_binary_expression(static_pointer_cast<binary_expression>(texpr->get_true_path()));
		shared_ptr<annotated_binary_expression> abexpr3 = analyze_binary_expression(static_pointer_cast<binary_expression>(texpr->get_false_path()));
		if (abexpr1->get_type_information() == _boolean || abexpr1->get_type_information().get_type_kind() == type_kind::TYPE_ANY);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_boolean_for_ternary_expression_condition, texpr->get_condition()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_ternary_expression>(ann_root_node, texpr, nullptr, nullptr, nullptr, bad, bad);
		}
		return make_shared<annotated_ternary_expression>(ann_root_node, texpr, abexpr1, abexpr2, abexpr3, type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE), bad);
	}

	shared_ptr<annotated_unary_expression> analyze_ast::analyze_unary_expression(shared_ptr<unary_expression> uexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!uexpr->get_valid())
			return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		unary_operation_kind op_kind = uexpr->get_unary_operation_kind();
		shared_ptr<annotated_linearized_postfix_expression> alpe = analyze_postfix_expression(uexpr->get_raw_postfix_expression());
		type_information t_inf = alpe->get_type_information();
		if (op_kind != unary_operation_kind::UNARY_OPERATION_NONE) {
			if (op_kind == unary_operation_kind::UNARY_OPERATION_MINUS || op_kind == unary_operation_kind::UNARY_OPERATION_PLUS) {
				if (t_inf == _int || t_inf == _float || t_inf.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(t_inf, value_kind::VALUE_RVALUE);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, alpe, temp, _float);
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
				}
			}
			else if (op_kind == unary_operation_kind::UNARY_OPERATION_INCREMENT || op_kind == unary_operation_kind::UNARY_OPERATION_DECREMENT || op_kind == unary_operation_kind::UNARY_OPERATION_COMPLEMENT) {
				if (t_inf == _int || t_inf.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(t_inf, value_kind::VALUE_RVALUE);
					if (op_kind == unary_operation_kind::UNARY_OPERATION_DECREMENT || op_kind == unary_operation_kind::UNARY_OPERATION_INCREMENT) {
						if (t_inf.get_value_kind() != value_kind::VALUE_LVALUE) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_lvalue_for_increments_and_decrements, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
						}
						vector<shared_ptr<symbol>> sym_results = find_all_symbols(t_inf.get_literal()).first;
						if (sym_results[sym_results.size() - 1]->get_immut_kind() == immut_kind::IMMUT_YES) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::immut_value_cannot_be_modified, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, t_inf.get_literal()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
							return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
						}
						temp = type_information(t_inf, value_kind::VALUE_LVALUE);
					}
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, alpe, temp, _int);
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
				}
			}
			else if (op_kind == unary_operation_kind::UNARY_OPERATION_NOT) {
				if (t_inf == _boolean || t_inf.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(t_inf, value_kind::VALUE_RVALUE);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, alpe, temp, _boolean);
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_boolean, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
				}
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
			}
		}
		else {
			return make_shared<annotated_unary_expression>(ann_root_node, uexpr, alpe, alpe->get_type_information(), bad);
		}
		root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
	}

	shared_ptr<linearized_postfix_expression> analyze_ast::linearize_postfix_expression(shared_ptr<postfix_expression> poexpr) {
		shared_ptr<linearized_postfix_expression> ret;
		shared_ptr<primary_expression> pr_expr;
		vector<shared_ptr<root_node>> postfix_operation_list;
		vector<postfix_operation_kind> postfix_operation_kind_list;
		if (poexpr->get_postfix_operation_kind() == postfix_operation_kind::POSTFIX_OPERATION_NONE);
		else {
			postfix_operation_kind pok = poexpr->get_postfix_operation_kind();
			if (pok == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT || pok == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr);
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), pok);
			}
			else if (pok == postfix_operation_kind::POSTFIX_FUNCTION_CALL) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr->get_argument_list());
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), postfix_operation_kind::POSTFIX_FUNCTION_CALL);
			}
			else if (pok == postfix_operation_kind::POSTFIX_DOT_OPERATOR) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr->get_identifier());
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), postfix_operation_kind::POSTFIX_DOT_OPERATOR);
			}
			else if (pok == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr->get_subscript());
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), postfix_operation_kind::POSTFIX_SUBSCRIPT);
			}
			else
				return nullptr;
		}
		if (poexpr->get_postfix_expression_kind() == postfix_expression_kind::POSTFIX_EXPRESSION_PRIMARY_EXPRESSION)
			pr_expr = poexpr->get_raw_primary_expression();
		else if (poexpr->get_postfix_expression_kind() == postfix_expression_kind::POSTFIX_EXPRESSION_POSTFIX_EXPRESSION) {
			shared_ptr<linearized_postfix_expression> lpe = linearize_postfix_expression(poexpr->get_postfix_expression());
			for (int i = lpe->get_postfix_operation_list().size() - 1; i >= 0; i--)
				postfix_operation_list.insert(postfix_operation_list.begin(), lpe->get_postfix_operation_list()[i]);
			for (int i = lpe->get_postfix_operation_kind_list().size() - 1; i >= 0; i--)
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), lpe->get_postfix_operation_kind_list()[i]);
			pr_expr = lpe->get_primary_expression();
		}
		ret = make_shared<linearized_postfix_expression>(pr_expr, postfix_operation_list, postfix_operation_kind_list);
		return ret;
	}

	pair<type_information, shared_ptr<annotated_root_node>> analyze_ast::analyze_postfix_operation(type_information prev, postfix_operation_kind pok, shared_ptr<root_node> op) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _list(type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _tuple(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _dict(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _string(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if (pok == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT || pok == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT) {
			if (prev == _int || prev == _any) {
				if (prev.get_value_kind() != value_kind::VALUE_LVALUE) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_lvalue_for_increments_and_decrements, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_pair(bad, nullptr);
				}
				if (prev.get_literal() != nullptr) {
					vector<shared_ptr<symbol>> sym_results = find_all_symbols(prev.get_literal()).first;
					if (sym_results[sym_results.size() - 1]->get_immut_kind() == immut_kind::IMMUT_YES) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::immut_value_cannot_be_modified, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
						return make_pair(bad, nullptr);
					}
				}
				type_information temp = type_information(prev, value_kind::VALUE_RVALUE);
				if (prev == _any)
					temp = prev;
				return make_pair(temp, nullptr);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				type_information temp = type_information(_int, value_kind::VALUE_RVALUE);
				return make_pair(bad, nullptr);
			}
		}
		else if (pok == postfix_operation_kind::POSTFIX_FUNCTION_CALL) {
			shared_ptr<function_argument_list> farg_list = static_pointer_cast<function_argument_list>(op);
			vector<shared_ptr<binary_expression>> bexpr_list = farg_list->get_argument_list();
			vector<shared_ptr<annotated_binary_expression>> abexpr_list;
			vector<type_information> t_inf_list;
			for (int i = 0; i < bexpr_list.size(); i++) {
				shared_ptr<annotated_binary_expression> abexpr = analyze_binary_expression(bexpr_list[i]);
				abexpr_list.push_back(abexpr);
				t_inf_list.push_back(abexpr->get_type_information());
			}
			shared_ptr<annotated_function_argument_list> afarg_list = make_shared<annotated_function_argument_list>(ann_root_node, farg_list, abexpr_list, t_inf_list);
			return make_pair(_any, afarg_list);
		}
		else if (pok == postfix_operation_kind::POSTFIX_DOT_OPERATOR) {
			shared_ptr<literal> lit = static_pointer_cast<literal>(op);
			shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, lit, _any);
			return make_pair(type_information(type_information(_any, prev.get_literal()), value_kind::VALUE_LVALUE), alit);
		}
		else if (pok == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
			bool sta = false;
			bool fin = false;
			bool ste = false;
			shared_ptr<subscript> subscr = static_pointer_cast<subscript>(op);
			if (subscr->get_start() == nullptr) sta = true;
			else sta = !(analyze_binary_expression(subscr->get_start())->get_type_information() == bad);
			if (subscr->get_final() == nullptr) fin = true;
			else fin = !(analyze_binary_expression(subscr->get_final())->get_type_information() == bad);
			if (subscr->get_step() == nullptr) ste = true;
			else ste = !(analyze_binary_expression(subscr->get_step())->get_type_information() == bad);
			if (subscr->get_start() == nullptr && subscr->get_final() == nullptr && subscr->get_step() == nullptr) {
				if (prev != _list && prev != _tuple && prev != _dict && prev.get_type_kind() != _any.get_type_kind() && prev != _string) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::subscript_applicable_only_for_sequences_and_dictionaries, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_pair(bad, nullptr);
				}
				else
					root->get_diagnostics_reporter()->print(diagnostic_messages::empty_subscript_defaulting_to_whole_sequence, subscr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
					subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
					subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
					subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
				return make_pair(prev, asubscr);
			}
			if (prev != _list && prev != _tuple && prev != _dict && prev.get_type_kind() != _any.get_type_kind() && prev != _string) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::subscript_applicable_only_for_sequences_and_dictionaries, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_pair(bad, nullptr);
			}
			if (sta && fin && ste) {
				if (prev.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(prev, value_kind::VALUE_LVALUE);
					shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
						subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
						subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
						subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
					return make_pair(temp, asubscr);
				}
				else if (prev == _dict) {
					if (subscr->get_step() != nullptr || subscr->get_final() != nullptr || subscr->get_subscript_colon_kind() != subscript_colon_kind::SUBSCRIPT_COLON_ZERO) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::dictionaries_only_support_subscripting_not_slicing, subscr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						return make_pair(bad, nullptr);
					}
					type_information t_inf = analyze_binary_expression(subscr->get_start())->get_type_information();
					shared_ptr<type_information> key_inf = prev.get_key_information();
					if (t_inf == *key_inf || t_inf == _any || *key_inf == _any);
					else if (t_inf == _nil && (key_inf->get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO));
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::expected_dictionary_subscript_to_equal_its_key_type, subscr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						return make_pair(bad, nullptr);
					}
					shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
						subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
						subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
						subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
					return make_pair(type_information(type_information(*(prev.get_value_information()), value_kind::VALUE_LVALUE), prev.get_literal()), asubscr);
				}
				else if (prev == _list || prev == _tuple || prev == _string) {
					if (subscr->get_start() == nullptr);
					else {
						type_information t_inf = analyze_binary_expression(subscr->get_start())->get_type_information();
						if (t_inf == _int || t_inf == _any || t_inf == _nil);
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_subscript_for_sequence_to_be_integer, subscr->get_start()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_pair(bad, nullptr);
						}
					}
					if (subscr->get_final() == nullptr);
					else {
						type_information t_inf = analyze_binary_expression(subscr->get_final())->get_type_information();
						if (t_inf == _int || t_inf == _any || t_inf == _nil);
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_subscript_for_sequence_to_be_integer, subscr->get_final()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_pair(bad, nullptr);
						}
					}
					if (subscr->get_step() == nullptr);
					else {
						type_information t_inf = analyze_binary_expression(subscr->get_step())->get_type_information();
						if (t_inf == _int || t_inf == _any || t_inf == _nil);
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_subscript_for_sequence_to_be_integer, subscr->get_step()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_pair(bad, nullptr);
						}
					}
					if (prev == _string) {
						shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
							subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
							subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
							subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
						return make_pair(type_information(prev, value_kind::VALUE_LVALUE), asubscr);
					}
					if (subscr->get_start() != nullptr && subscr->get_final() == nullptr && subscr->get_step() == nullptr && subscr->get_subscript_colon_kind() == subscript_colon_kind::SUBSCRIPT_COLON_ZERO) {
						type_information temp = type_information(type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE), prev.get_literal());
						shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
							subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
							subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
							subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
						return make_pair(temp, asubscr);
					}
					shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
						subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
						subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
						subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
					if (subscr->get_final() != nullptr || subscr->get_step() != nullptr || subscr->get_subscript_colon_kind() == subscript_colon_kind::SUBSCRIPT_COLON_ONE || subscr->get_subscript_colon_kind() == subscript_colon_kind::SUBSCRIPT_COLON_TWO)
						return make_pair(type_information(type_information(prev, value_kind::VALUE_LVALUE), prev.get_literal()), asubscr);
					type_information temp = bad;
					if (prev == _list)
						temp = type_information(type_information(prev, value_kind::VALUE_LVALUE), prev.get_literal());
					else if (prev == _tuple)
						temp = type_information(type_information(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE), prev.get_literal());
					return make_pair(temp, asubscr);
				}
				type_information temp = bad;
				if (prev == _list)
					temp = type_information(type_information(prev, value_kind::VALUE_LVALUE), prev.get_literal());
				else if (prev == _tuple)
					temp = type_information(type_information(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE), prev.get_literal());
				shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
					subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
					subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
					subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
				return make_pair(temp, asubscr);
			}
			else
				return make_pair(bad, nullptr);
		}
		else
			return make_pair(bad, nullptr);
	}

	shared_ptr<annotated_linearized_postfix_expression> analyze_ast::analyze_postfix_expression(shared_ptr<postfix_expression> poexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		shared_ptr<linearized_postfix_expression> lpe = linearize_postfix_expression(poexpr);
		tuple<shared_ptr<annotated_primary_expression>, type_information, shared_ptr<annotated_root_node>> tup0 = analyze_primary_expression(lpe->get_primary_expression(), lpe);
		shared_ptr<annotated_primary_expression> aprexpr = get<0>(tup0);
		type_information t_inf = get<1>(tup0);
		if (!poexpr->get_valid())
			return make_shared<annotated_linearized_postfix_expression>(ann_root_node, lpe, aprexpr, vector<shared_ptr<annotated_root_node>>(), vector<postfix_operation_kind>(), vector<type_information>(), bad);
		if (lpe->get_postfix_operation_kind_list().size() != lpe->get_postfix_operation_list().size())
			return make_shared<annotated_linearized_postfix_expression>(ann_root_node, lpe, aprexpr, vector<shared_ptr<annotated_root_node>>(), vector<postfix_operation_kind>(), vector<type_information>(), bad);
		vector<type_information> t_inf_list;
		vector<shared_ptr<annotated_root_node>> ann_root_node_list;
		ann_root_node_list.push_back(get<2>(tup0));
		vector<postfix_operation_kind> pokl;
		if (lpe->get_postfix_operation_kind_list().size() != 0)
			pokl.push_back(lpe->get_postfix_operation_kind_list()[0]);
		else
			pokl.push_back(postfix_operation_kind::POSTFIX_OPERATION_NONE);
		for (int i = 1; i < lpe->get_postfix_operation_list().size(); i++) {
			pair<type_information, shared_ptr<annotated_root_node>> pai = analyze_postfix_operation(t_inf, lpe->get_postfix_operation_kind_list()[i], lpe->get_postfix_operation_list()[i]);
			t_inf = pai.first;
			t_inf_list.push_back(t_inf);
			pokl.push_back(lpe->get_postfix_operation_kind_list()[i]);
			ann_root_node_list.push_back(pai.second);
		}
		return make_shared<annotated_linearized_postfix_expression>(ann_root_node, lpe, aprexpr, ann_root_node_list, pokl, t_inf_list, t_inf);
	}

	tuple<shared_ptr<annotated_primary_expression>, type_information, shared_ptr<annotated_root_node>> analyze_ast::analyze_primary_expression(shared_ptr<primary_expression> prexpr, shared_ptr<linearized_postfix_expression> lpe) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!prexpr->get_valid() || lpe == nullptr) {
			shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
			return make_tuple(aprexpr, bad, nullptr);
		}
		primary_expression_kind prek = prexpr->get_primary_expression_kind();
		type_information t_inf = bad;
		shared_ptr<annotated_sequence> aseq = nullptr;
		shared_ptr<annotated_dictionary> adict = nullptr;
		shared_ptr<annotated_literal> alit = nullptr;
		shared_ptr<annotated_binary_expression> abexpr = nullptr;
		if (prek == primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION) {
			t_inf = analyze_binary_expression(prexpr->get_raw_parenthesized_expression())->get_type_information();
			abexpr = analyze_binary_expression(prexpr->get_raw_parenthesized_expression());
		}
		else if (prek == primary_expression_kind::PRIMARY_EXPRESSION_SEQUENCE) {
			shared_ptr<sequence> seq = prexpr->get_sequence();
			if (seq->get_sequence_kind() == sequence_kind::SEQUENCE_TUPLE) {
				aseq = analyze_tuple_expression(seq);
				t_inf = aseq->get_type_information();
			}
			else if (seq->get_sequence_kind() == sequence_kind::SEQUENCE_LIST) {
				aseq = analyze_list_expression(seq);
				t_inf = aseq->get_type_information();
			}
			else if (seq->get_sequence_kind() == sequence_kind::SEQUENCE_NONE) {
				shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
				return make_tuple(aprexpr, bad, nullptr);
			}
		}
		else if (prek == primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION) {
			type_information temp = analyze_binary_expression(prexpr->get_raw_parenthesized_expression())->get_type_information();
			if (temp.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
				root->get_diagnostics_reporter()->print(diagnostic_messages::already_a_pure_type_ignoring, prexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			t_inf = type_information(temp.get_type_kind(), type_pure_kind::TYPE_PURE_YES, temp.get_type_class_kind(), value_kind::VALUE_RVALUE);
			abexpr = analyze_binary_expression(prexpr->get_raw_parenthesized_expression());
		}
		else if (prek == primary_expression_kind::PRIMARY_EXPRESSION_DICTIONARY) {
			adict = analyze_dictionary_expression(prexpr->get_dictionary());
			t_inf = adict->get_type_information();
		}
		else if (prek == primary_expression_kind::PRIMARY_EXPRESSION_LITERAL) {
			shared_ptr<literal> lit = prexpr->get_raw_literal();
			literal_kind lk = lit->get_literal_kind();
			if (lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER)
				t_inf = type_information(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if (lk == literal_kind::LITERAL_STRING)
				t_inf = type_information(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if (lk == literal_kind::LITERAL_DECIMAL_NUMBER)
				t_inf = type_information(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if (lk == literal_kind::LITERAL_NIL)
				t_inf = type_information(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if (lk == literal_kind::LITERAL_BOOLEAN)
				t_inf = type_information(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if (lk == literal_kind::LITERAL_IDENTIFIER) {
				vector<shared_ptr<symbol>> ls_sym = find_all_symbols(lit).first;
				if (ls_sym.size() == 0) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unknown_symbol + lit->get_raw_literal()->get_raw_string() + ".", lit->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
					return make_tuple(aprexpr, bad, nullptr);
				}
				t_inf = ls_sym[ls_sym.size() - 1]->get_type_information();
				t_inf = type_information(t_inf, value_kind::VALUE_LVALUE);
				t_inf = type_information(t_inf, lit);
			}
			else {
				shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
				return make_tuple(aprexpr, bad, nullptr);
			}
			alit = make_shared<annotated_literal>(ann_root_node, lit, t_inf);
		}
		else {
			shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
			return make_tuple(aprexpr, bad, nullptr);
		}
		if (lpe->get_postfix_operation_list().size() != lpe->get_postfix_operation_kind_list().size()) {
			shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
			return make_tuple(aprexpr, bad, nullptr);
		}
		if (lpe->get_postfix_operation_list().size() == 0)
			return make_tuple(make_shared<annotated_primary_expression>(ann_root_node, prexpr, alit, abexpr, aseq, adict, t_inf), t_inf, nullptr);
		shared_ptr<root_node> poexpr = lpe->get_postfix_operation_list()[0];
		postfix_operation_kind pok = lpe->get_postfix_operation_kind_list()[0];
		pair<type_information, shared_ptr<annotated_root_node>> pai = analyze_postfix_operation(t_inf, pok, poexpr);
		return make_tuple(make_shared<annotated_primary_expression>(ann_root_node, prexpr, alit, abexpr, aseq, adict, t_inf), pai.first, pai.second);
	}

	shared_ptr<annotated_sequence> analyze_ast::analyze_list_expression(shared_ptr<sequence> seq) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!seq->get_valid())
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), bad);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		vector<shared_ptr<binary_expression>> bexpr_list = seq->get_expression_list();
		if (bexpr_list.size() == 0) {
			type_information ret(type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), ret);
		}
		vector<shared_ptr<annotated_binary_expression>> abexpr_list;
		abexpr_list.push_back(analyze_binary_expression(bexpr_list[0]));
		type_information t_inf = bad;
		t_inf = analyze_binary_expression(bexpr_list[0])->get_type_information();
		vector<type_information> t_inf_list;
		t_inf_list.push_back(t_inf);
		for (int i = 1; i < bexpr_list.size(); i++) {
			abexpr_list.push_back(analyze_binary_expression(bexpr_list[i]));
			type_information in_t_inf = analyze_binary_expression(bexpr_list[i])->get_type_information();
			t_inf_list.push_back(in_t_inf);
			if (t_inf.get_type_kind() == type_kind::TYPE_ANY || t_inf == in_t_inf || in_t_inf.get_type_kind() == type_kind::TYPE_ANY);
			else if ((t_inf == _nil && in_t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) || (in_t_inf == _nil && t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO));
			else if ((t_inf == _int && in_t_inf == _float) || (t_inf == _float && in_t_inf == _int)) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unequal_but_compatible_types_list_dict, bexpr_list[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, bexpr_list[0]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_same_type_for_all_elements_in_list_or_dictionary, bexpr_list[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, bexpr_list[0]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
				t_inf = bad;
				break;
			}
		}
		shared_ptr<type_information> inner = make_shared<type_information>(t_inf);
		type_information outer(type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE, inner);
		return make_shared<annotated_sequence>(ann_root_node, seq, abexpr_list, t_inf_list, outer);
	}

	shared_ptr<annotated_dictionary> analyze_ast::analyze_dictionary_expression(shared_ptr<dictionary> dict) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!dict->get_valid() || (dict->get_key_list().size() != dict->get_value_list().size()))
			return make_shared<annotated_dictionary>(ann_root_node, dict, vector<shared_ptr<annotated_binary_expression>>(), vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(),
				vector<type_information>(), bad);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		vector<shared_ptr<binary_expression>> kl = dict->get_key_list();
		vector<shared_ptr<binary_expression>> vl = dict->get_value_list();
		if (kl.size() == 0 || vl.size() == 0) {
			type_information ret(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			return make_shared<annotated_dictionary>(ann_root_node, dict, vector<shared_ptr<annotated_binary_expression>>(), vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(),
				vector<type_information>(), ret);
		}
		type_information kt_inf = bad;
		type_information vt_inf = bad;
		vector<shared_ptr<annotated_binary_expression>> klist;
		vector<shared_ptr<annotated_binary_expression>> vlist;
		vector<type_information> ktlist;
		vector<type_information> vtlist;
		kt_inf = analyze_binary_expression(kl[0])->get_type_information();
		klist.push_back(analyze_binary_expression(kl[0]));
		ktlist.push_back(kt_inf);
		vt_inf = analyze_binary_expression(vl[0])->get_type_information();
		vlist.push_back(analyze_binary_expression(vl[0]));
		vtlist.push_back(vt_inf);
		for (int i = 1; i < kl.size(); i++) {
			type_information in_kt_inf = analyze_binary_expression(kl[i])->get_type_information();
			klist.push_back(analyze_binary_expression(kl[i]));
			ktlist.push_back(in_kt_inf);
			if (kt_inf.get_type_kind() == type_kind::TYPE_ANY || kt_inf == in_kt_inf || in_kt_inf.get_type_kind() == type_kind::TYPE_ANY);
			else if ((kt_inf == _nil && in_kt_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) || (in_kt_inf == _nil && kt_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO));
			else if ((kt_inf == _int && in_kt_inf == _float) || (kt_inf == _float && in_kt_inf == _int)) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unequal_but_compatible_types_list_dict, kl[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, kl[0]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_same_type_for_all_elements_in_list_or_dictionary, kl[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, kl[0]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
				kt_inf = bad;
				break;
			}
		}
		for (int i = 1; i < vl.size(); i++) {
			type_information in_vt_inf = analyze_binary_expression(vl[i])->get_type_information();
			vlist.push_back(analyze_binary_expression(vl[i]));
			vtlist.push_back(in_vt_inf);
			if (vt_inf.get_type_kind() == type_kind::TYPE_ANY || vt_inf == in_vt_inf || in_vt_inf.get_type_kind() == type_kind::TYPE_ANY);
			else if ((vt_inf == _nil && in_vt_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) || (in_vt_inf == _nil && vt_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO));
			else if ((vt_inf == _int && in_vt_inf == _float) || (vt_inf == _float && in_vt_inf == _int)) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unequal_but_compatible_types_list_dict, vl[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, vl[0]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_same_type_for_all_elements_in_list_or_dictionary, vl[i]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, vl[0]->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
				vt_inf = bad;
				break;
			}
		}
		shared_ptr<type_information> inf_v = make_shared<type_information>(vt_inf);
		shared_ptr<type_information> inf_k = make_shared<type_information>(kt_inf);
		type_information outer(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE, inf_k, inf_v);
		return make_shared<annotated_dictionary>(ann_root_node, dict, klist, vlist, ktlist, vtlist, outer);
	}

	shared_ptr<annotated_sequence> analyze_ast::analyze_tuple_expression(shared_ptr<sequence> seq) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (!seq->get_valid())
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), bad);
		vector<shared_ptr<binary_expression>> bexpr_list = seq->get_expression_list();
		if (bexpr_list.size() == 0) {
			shared_ptr<type_information> inner = make_shared<type_information>(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			type_information ret(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), ret);
		}
		vector<shared_ptr<type_information>> inner;
		vector<shared_ptr<annotated_binary_expression>> abel;
		vector<type_information> tlist;
		vector<type_information> ftlist;
		for (int i = 0; i < bexpr_list.size(); i++) {
			inner.push_back(make_shared<type_information>(analyze_binary_expression(bexpr_list[i])->get_type_information()));
			abel.push_back(analyze_binary_expression(bexpr_list[i]));
			tlist.push_back(analyze_binary_expression(bexpr_list[i])->get_type_information());
			ftlist.push_back(analyze_binary_expression(bexpr_list[i])->get_type_information());
		}
		type_information outer(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		return make_shared<annotated_sequence>(ann_root_node, seq, abel, tlist, outer);
	}

	deduce_binary_expression_type::deduce_binary_expression_type(type_information l, binary_operation_kind b, type_information r) : lhs(l), rhs(r) {
		b_kind = b;
	}

	deduce_binary_expression_type::~deduce_binary_expression_type() {

	}

	type_information deduce_binary_expression_type::get_lhs_type_information() {
		return lhs;
	}

	const binary_operation_kind deduce_binary_expression_type::get_binary_operation_kind() {
		return b_kind;
	}

	type_information deduce_binary_expression_type::get_rhs_type_information() {
		return rhs;
	}

	type_information deduce_binary_expression_type::deduce_type() {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if (rhs == bad || lhs == bad)
			return bad;
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);

		type_information _pure_int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _pure_float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _pure_nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _pure_any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if (b_kind == binary_operation_kind::BINARY_OPERATION_PLUS || b_kind == binary_operation_kind::BINARY_OPERATION_MINUS || b_kind == binary_operation_kind::BINARY_OPERATION_MULTIPLY || b_kind == binary_operation_kind::BINARY_OPERATION_DIVIDE ||
			b_kind == binary_operation_kind::BINARY_OPERATION_MODULUS || b_kind == binary_operation_kind::BINARY_OPERATION_EXPONENT) {
			if (lhs == _int || lhs == _float || lhs == _any) {
				if (rhs == _int || rhs == _float || rhs == _any) {
					if (lhs == _int && rhs == _int)
						return _int;
					else if (lhs == _any || rhs == _any) {
						return _any;
					}
					else
						return _float;
				}
				else
					return bad;
			}
			return bad;
		}
		else if (b_kind == binary_operation_kind::BINARY_OPERATION_GREATER_THAN || b_kind == binary_operation_kind::BINARY_OPERATION_LESS_THAN || b_kind == binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO ||
			b_kind == binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO) {
			if (lhs == _int || lhs == _float || lhs == _any) {
				if (rhs == _int || rhs == _float || lhs == _any)
					return _boolean;
				else if (lhs == _any || rhs == _any)
					return _boolean;
				else
					return bad;
			}
			return bad;
		}
		else if (b_kind == binary_operation_kind::BINARY_OPERATION_LOGICAL_AND || b_kind == binary_operation_kind::BINARY_OPERATION_LOGICAL_OR) {
			if ((lhs == _boolean || lhs == _any) && (rhs == _boolean || rhs == _any))
				return _boolean;
			else if (lhs == _any || rhs == _any)
				return _boolean;
			else
				return bad;
		}
		else if (b_kind == binary_operation_kind::BINARY_OPERATION_BITWISE_AND || b_kind == binary_operation_kind::BINARY_OPERATION_BITWISE_OR || b_kind == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT || b_kind == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT ||
			b_kind == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR) {
			if (lhs == _any || rhs == _any)
				return _any;
			if ((lhs == _any || lhs == _int) && (rhs == _int || rhs == _any))
				return _int;
			else
				return bad;
		}
		else if (b_kind == binary_operation_kind::BINARY_OPERATION_POINT) {
			if (lhs == _int || lhs == _float) {
				if (rhs == _pure_int)
					return _int;
				else if (rhs == _pure_float)
					return _float;
				else if (rhs == _any || rhs == _pure_any)
					return _any;
				else
					return bad;
			}
			else if (lhs == _pure_int || lhs == _pure_float) {
				if (rhs == _pure_int)
					return _pure_int;
				else if (rhs == _pure_float)
					return _pure_float;
				else if (rhs == _any || rhs == _pure_any)
					return _pure_any;
				else
					return bad;
			}
			else if (rhs == _pure_any) {
				if (lhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					return _any;
				else if (lhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
					return _pure_any;
				else
					return bad;
			}
			else if (lhs == _pure_any) {
				if (rhs == _any || rhs == _pure_any)
					return lhs;
				else if (rhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
					return rhs;
				else
					return bad;
			}
			else if (rhs == _pure_nil)
				return bad;
			else if (lhs == _any) {
				if (rhs == _pure_int)
					return _int;
				else if (rhs == _pure_float)
					return _float;
				else if (rhs == _any || rhs == _pure_any)
					return _pure_any;
				else if (rhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
					return type_information(rhs.get_type_kind(), lhs.get_type_pure_kind(), lhs.get_type_class_kind());
				else
					return bad;
			}
			else if (lhs.get_type_kind() == rhs.get_type_kind() && rhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
				return type_information(rhs.get_type_kind(), lhs.get_type_pure_kind(), lhs.get_type_class_kind());
			else
				return bad;
		}
		else if (b_kind == binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS || b_kind == binary_operation_kind::BINARY_OPERATION_NOT_EQUAL) {
			if (lhs == _int || lhs == _float || lhs == _nil) {
				if (rhs == _int || rhs == _float || rhs == _nil)
					return _boolean;
				else if (lhs == _any || rhs == _any)
					return _any;
				else
					return bad;
			}
			else if (lhs == _nil || rhs == _nil)
				return _boolean;
			else if (lhs == _any || rhs == _any)
				return _any;
			else if (lhs == rhs)
				return _boolean;
			else if (lhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES && rhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
				return _boolean;
			else
				return bad;
		}
		else {
			if (lhs == _int || lhs == _float) {
				if (rhs == _int || rhs == _float || rhs == _nil) {
					return lhs;
				}
				else if (rhs == _any)
					return lhs;
				else
					return bad;
			}
			else if (lhs == _nil)
				return bad;
			else if (rhs == _nil)
				return lhs;
			else if (lhs == _any || rhs == _any)
				return lhs;
			else if (lhs == rhs)
				return lhs;
			else
				return bad;
		}
		return bad;
	}

	linearized_postfix_expression::linearized_postfix_expression(shared_ptr<primary_expression> pre, vector<shared_ptr<root_node>> pol, vector<postfix_operation_kind> pokl) {
		pr_expr = pre;
		postfix_operation_list = pol;
		postfix_operation_kind_list = pokl;
	}

	linearized_postfix_expression::~linearized_postfix_expression() {

	}

	shared_ptr<primary_expression> linearized_postfix_expression::get_primary_expression() {
		return pr_expr;
	}

	vector<shared_ptr<root_node>> linearized_postfix_expression::get_postfix_operation_list() {
		return postfix_operation_list;
	}

	vector<postfix_operation_kind> linearized_postfix_expression::get_postfix_operation_kind_list() {
		return postfix_operation_kind_list;
	}

	annotated_root_node::annotated_root_node(shared_ptr<root_node> r, vector<shared_ptr<annotated_statement>> asl) : pos(r->get_position()) {
		lex = r->get_lexer();
		dreporter = r->get_diagnostics_reporter();
		statement_list = asl;
	}

	annotated_root_node::~annotated_root_node() {

	}

	source_token_list::iterator annotated_root_node::get_position() {
		return pos;
	}

	vector<shared_ptr<annotated_statement>> annotated_root_node::get_annotated_statement_list() {
		return statement_list;
	}

	source_token_list::iterator annotated_root_node::set_position(source_token_list::iterator p) {
		pos = p;
		return p;
	}

	shared_ptr<diagnostics_reporter> annotated_root_node::get_diagnostics_reporter() {
		return dreporter;
	}

	shared_ptr<lexer> annotated_root_node::get_lexer() {
		return lex;
	}

	shared_ptr<annotated_statement> annotated_root_node::add_annotated_statement(shared_ptr<annotated_statement> as) {
		statement_list.push_back(as);
		return as;
	}

	annotated_literal::annotated_literal(shared_ptr<annotated_root_node> arn, shared_ptr<literal> l, type_information ti) : annotated_root_node(*arn), literal_pos(l->get_position()),
		t_inf(ti) {
		raw_literal = l->get_raw_literal();
		kind = l->get_literal_kind();
	}

	annotated_literal::~annotated_literal() {

	}

	const literal_kind annotated_literal::get_literal_kind() {
		return kind;
	}

	shared_ptr<token> annotated_literal::get_raw_literal() {
		return raw_literal;
	}

	source_token_list::iterator annotated_literal::get_position() {
		return pos;
	}

	type_information annotated_literal::get_type_information() {
		return t_inf;
	}

	annotated_primary_expression::annotated_primary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<primary_expression> prexpr, shared_ptr<annotated_literal> al,
		shared_ptr<annotated_binary_expression> abe, shared_ptr<annotated_sequence> as, shared_ptr<annotated_dictionary> ad, type_information ti) : annotated_root_node(*arn), primary_expression_pos(prexpr->get_position()),
		t_inf(ti) {
		raw_literal = al;
		kind = prexpr->get_primary_expression_kind();
		raw_parenthesized_expression = abe;
		seq = as;
		dict = ad;
	}

	annotated_primary_expression::~annotated_primary_expression() {

	}

	const primary_expression_kind annotated_primary_expression::get_primary_expression_kind() {
		return kind;
	}

	shared_ptr<annotated_literal> annotated_primary_expression::get_raw_literal() {
		return raw_literal;
	}

	source_token_list::iterator annotated_primary_expression::get_position() {
		return pos;
	}

	shared_ptr<annotated_binary_expression> annotated_primary_expression::get_raw_parenthesized_expression() {
		return raw_parenthesized_expression;
	}

	shared_ptr<annotated_sequence> annotated_primary_expression::get_sequence() {
		return seq;
	}

	shared_ptr<annotated_dictionary> annotated_primary_expression::get_dictionary() {
		return dict;
	}

	type_information annotated_primary_expression::get_type_information() {
		return t_inf;
	}

	annotated_linearized_postfix_expression::annotated_linearized_postfix_expression(shared_ptr<annotated_root_node> arn, shared_ptr<linearized_postfix_expression> lpe, shared_ptr<annotated_primary_expression> ape,
		vector<shared_ptr<annotated_root_node>> arnl, vector<postfix_operation_kind> pokl, vector<type_information> til, type_information ti) :
		annotated_root_node(*arn), postfix_expression_pos(lpe->get_primary_expression()->get_position()), t_inf(ti) {
		aprexpr = ape;
		annotated_root_node_list = arnl;
		postfix_operation_kind_list = pokl;
		t_inf_list = til;
	}

	annotated_linearized_postfix_expression::~annotated_linearized_postfix_expression() {

	}

	vector<postfix_operation_kind> annotated_linearized_postfix_expression::get_postfix_operation_kind_list() {
		return postfix_operation_kind_list;
	}

	shared_ptr<annotated_primary_expression> annotated_linearized_postfix_expression::get_raw_annotated_primary_expression() {
		return aprexpr;
	}

	source_token_list::iterator annotated_linearized_postfix_expression::get_position() {
		return postfix_expression_pos;
	}

	vector<shared_ptr<annotated_root_node>> annotated_linearized_postfix_expression::get_annotated_root_node_list() {
		return annotated_root_node_list;
	}

	vector<type_information> annotated_linearized_postfix_expression::get_type_information_list() {
		return t_inf_list;
	}

	type_information annotated_linearized_postfix_expression::get_type_information() {
		return t_inf;
	}

	annotated_unary_expression::annotated_unary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<unary_expression> uexpr,
		shared_ptr<annotated_linearized_postfix_expression> alpe, type_information ti, type_information fti) : annotated_root_node(*arn), unary_expression_pos(uexpr->get_position()),
		t_inf(ti), forced_t_inf(fti) {
		raw_postfix_expression = alpe;
		kind = uexpr->get_unary_operation_kind();
	}

	annotated_unary_expression::~annotated_unary_expression() {

	}

	const unary_operation_kind annotated_unary_expression::get_unary_operation_kind() {
		return kind;
	}

	shared_ptr<annotated_linearized_postfix_expression> annotated_unary_expression::get_raw_annotated_linearized_postfix_expression() {
		return raw_postfix_expression;
	}

	source_token_list::iterator annotated_unary_expression::get_position() {
		return unary_expression_pos;
	}

	type_information annotated_unary_expression::get_type_information() {
		return t_inf;
	}

	type_information annotated_unary_expression::get_forced_type_information() {
		return forced_t_inf;
	}

	annotated_binary_expression::annotated_binary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<binary_expression> bexpr, shared_ptr<annotated_root_node> l,
		shared_ptr<annotated_root_node> r, type_information lti, type_information ltfi, type_information rti, type_information rtfi, type_information ti, type_information fti) : annotated_root_node(*arn), binary_expression_pos(bexpr->get_position()),
		lhs_t_inf(lti), lhs_forced_t_inf(ltfi), rhs_t_inf(rti), rhs_forced_t_inf(rtfi), t_inf(ti), forced_t_inf(fti) {
		lhs = l;
		rhs = r;
		lhs_kind = bexpr->get_lhs_kind();
		rhs_kind = bexpr->get_rhs_kind();
		expression_kind = bexpr->get_expression_kind();
		operation_kind = bexpr->get_binary_operation_kind();
	}

	annotated_binary_expression::~annotated_binary_expression() {

	}

	shared_ptr<annotated_root_node> annotated_binary_expression::get_lhs() {
		return lhs;
	}

	shared_ptr<annotated_root_node> annotated_binary_expression::get_rhs() {
		return rhs;
	}

	const binary_expression_kind annotated_binary_expression::get_expression_kind() {
		return expression_kind;
	}

	const binary_expression_kind annotated_binary_expression::get_lhs_kind() {
		return lhs_kind;
	}

	const binary_expression_kind annotated_binary_expression::get_rhs_kind() {
		return rhs_kind;
	}

	const binary_operation_kind annotated_binary_expression::get_binary_operation_kind() {
		return operation_kind;
	}

	source_token_list::iterator annotated_binary_expression::get_position() {
		return binary_expression_pos;
	}

	type_information annotated_binary_expression::get_lhs_type_information() {
		return lhs_t_inf;
	}

	type_information annotated_binary_expression::get_lhs_forced_type_information() {
		return lhs_forced_t_inf;
	}

	type_information annotated_binary_expression::get_rhs_type_information() {
		return rhs_t_inf;
	}

	type_information annotated_binary_expression::get_rhs_forced_type_information() {
		return rhs_forced_t_inf;
	}

	type_information annotated_binary_expression::get_type_information() {
		return t_inf;
	}

	type_information annotated_binary_expression::get_forced_type_information() {
		return forced_t_inf;
	}

	type_information annotated_binary_expression::set_forced_type_information(type_information t_inf) {
		forced_t_inf = t_inf;
		return t_inf;
	}

	type_information annotated_binary_expression::set_lhs_forced_type_information(type_information t_inf) {
		lhs_forced_t_inf = t_inf;
		return t_inf;
	}

	type_information annotated_binary_expression::set_rhs_forced_type_information(type_information t_inf) {
		rhs_forced_t_inf = t_inf;
		return t_inf;
	}

	annotated_sequence::annotated_sequence(shared_ptr<annotated_root_node> arn, shared_ptr<sequence> seq, vector<shared_ptr<annotated_binary_expression>> abel,
		vector<type_information> til, type_information ti) : annotated_root_node(*arn), sequence_pos(seq->get_position()), t_inf(ti) {
		_sequence = seq->get_sequence_kind();
		annotated_binary_expression_list = abel;
		t_inf_list = til;
	}

	annotated_sequence::~annotated_sequence() {

	}

	source_token_list::iterator annotated_sequence::get_position() {
		return sequence_pos;
	}

	const sequence_kind annotated_sequence::get_sequence_kind() {
		return _sequence;
	}

	vector<shared_ptr<annotated_binary_expression>> annotated_sequence::get_annotated_binary_expression_list() {
		return annotated_binary_expression_list;
	}

	vector<type_information> annotated_sequence::get_type_information_list() {
		return t_inf_list;
	}

	type_information annotated_sequence::get_type_information() {
		return t_inf;
	}

	annotated_dictionary::annotated_dictionary(shared_ptr<annotated_root_node> arn, shared_ptr<dictionary> dict, vector<shared_ptr<annotated_binary_expression>> klist,
		vector<shared_ptr<annotated_binary_expression>> vlist, vector<type_information> ktil, vector<type_information> vtil,
		type_information ti) : t_inf(ti), annotated_root_node(*arn), dictionary_pos(dict->get_position()) {
		key_list = klist;
		value_list = vlist;
		key_t_inf_list = ktil;
		value_t_inf_list = vtil;
	}

	annotated_dictionary::~annotated_dictionary() {

	}

	vector<shared_ptr<annotated_binary_expression>> annotated_dictionary::get_key_list() {
		return key_list;
	}

	vector<shared_ptr<annotated_binary_expression>> annotated_dictionary::get_value_list() {
		return value_list;
	}

	source_token_list::iterator annotated_dictionary::get_position() {
		return dictionary_pos;
	}

	vector<type_information> annotated_dictionary::get_key_type_information_list() {
		return key_t_inf_list;
	}

	vector<type_information> annotated_dictionary::get_value_type_information_list() {
		return value_t_inf_list;
	}

	type_information annotated_dictionary::get_type_information() {
		return t_inf;
	}

	annotated_subscript::annotated_subscript(shared_ptr<annotated_root_node> arn, shared_ptr<subscript> subscr,
		shared_ptr<annotated_binary_expression> s, shared_ptr<annotated_binary_expression> d,
		shared_ptr<annotated_binary_expression> ste, type_information sti,
		type_information dti, type_information steti) : annotated_root_node(*arn), subscript_pos(subscr->get_position()),
		start_t_inf(sti), done_t_inf(dti), step_t_inf(steti) {
		start = s;
		done = d;
		step = ste;
		sc_kind = subscr->get_subscript_colon_kind();
	}

	annotated_subscript::~annotated_subscript() {

	}

	shared_ptr<annotated_binary_expression> annotated_subscript::get_start() {
		return start;
	}

	shared_ptr<annotated_binary_expression> annotated_subscript::get_final() {
		return done;
	}

	shared_ptr<annotated_binary_expression> annotated_subscript::get_step() {
		return step;
	}

	source_token_list::iterator annotated_subscript::get_position() {
		return subscript_pos;
	}

	const subscript_colon_kind annotated_subscript::get_subscript_colon_kind() {
		return sc_kind;
	}

	type_information annotated_subscript::get_start_type_information() {
		return start_t_inf;
	}

	type_information annotated_subscript::get_step_type_information() {
		return step_t_inf;
	}

	type_information annotated_subscript::get_final_type_information() {
		return done_t_inf;
	}

	annotated_declaration::annotated_declaration(shared_ptr<annotated_root_node> arn, shared_ptr<declaration> decl,
		shared_ptr<annotated_literal> al, shared_ptr<annotated_binary_expression> abe, type_information ti) : annotated_root_node(*arn), declaration_pos(decl->get_position()),
		t_inf(ti) {
		identifier = al;
		delsp_list = decl->get_declspec_list();
		expr_statement = abe;
		immutable = decl->get_declspec_list()->get_declspecs_list().size() > 0;
	}

	annotated_declaration::~annotated_declaration() {

	}

	shared_ptr<annotated_literal> annotated_declaration::get_identifier() {
		return identifier;
	}

	shared_ptr<declspec_list> annotated_declaration::get_declspec_list() {
		return delsp_list;
	}

	shared_ptr<annotated_binary_expression> annotated_declaration::get_binary_expression() {
		return expr_statement;
	}

	source_token_list::iterator annotated_declaration::get_position() {
		return declaration_pos;
	}

	type_information annotated_declaration::get_type_information() {
		return t_inf;
	}

	const bool annotated_declaration::get_immutable() {
		return immutable;
	}

	annotated_statement::annotated_statement(shared_ptr<annotated_root_node> arn, shared_ptr<statement> stmt, shared_ptr<annotated_binary_expression> abe,
		shared_ptr<annotated_declaration> adecl, shared_ptr<annotated_function> afunc, shared_ptr<annotated_structure> astruc,
		shared_ptr<annotated_module> amod, shared_ptr<annotated_return_statement> aret, shared_ptr<annotated_conditional_statement> acond,
		shared_ptr<annotated_enum_statement> aenum, shared_ptr<annotated_while_statement> awhile, type_information ti) : annotated_root_node(*arn),
		statement_pos(stmt->get_position()), t_inf(ti) {
		kind = stmt->get_statement_kind();
		b_expression = abe;
		decl = adecl;
		func = afunc;
		struc = astruc;
		mod = amod;
		ret = aret;
		cond = acond;
		_enum = aenum;
		wloop = awhile;
	}

	annotated_statement::~annotated_statement() {

	}

	const statement_kind annotated_statement::get_statement_kind() {
		return kind;
	}

	shared_ptr<annotated_binary_expression> annotated_statement::get_binary_expression() {
		return b_expression;
	}

	shared_ptr<annotated_declaration> annotated_statement::get_declaration() {
		return decl;
	}

	shared_ptr<annotated_function> annotated_statement::get_function() {
		return func;
	}

	type_information annotated_statement::get_type_information() {
		return t_inf;
	}

	source_token_list::iterator annotated_statement::get_position() {
		return statement_pos;
	}

	shared_ptr<annotated_structure> annotated_statement::get_structure() {
		return struc;
	}

	shared_ptr<annotated_module> annotated_statement::get_module() {
		return mod;
	}

	shared_ptr<annotated_return_statement> annotated_statement::get_return_statement() {
		return ret;
	}

	shared_ptr<annotated_conditional_statement> annotated_statement::get_conditional_statement() {
		return cond;
	}

	shared_ptr<annotated_enum_statement> annotated_statement::get_enum_statement() {
		return _enum;
	}

	shared_ptr<annotated_while_statement> annotated_statement::get_while_statement() {
		return wloop;
	}

	annotated_ternary_expression::annotated_ternary_expression(shared_ptr<annotated_root_node> arn, shared_ptr<ternary_expression> texpr, shared_ptr<annotated_root_node> c,
		shared_ptr<annotated_root_node> tp, shared_ptr<annotated_root_node> fp, type_information ti, type_information fti) : annotated_root_node(*arn), ternary_expression_pos(texpr->get_position()), t_inf(ti),
		forced_t_inf(fti) {
		condition = c;
		true_path = tp;
		false_path = fp;
	}

	annotated_ternary_expression::~annotated_ternary_expression() {

	}

	shared_ptr<annotated_root_node> annotated_ternary_expression::get_condition() {
		return condition;
	}

	shared_ptr<annotated_root_node> annotated_ternary_expression::get_true_path() {
		return true_path;
	}

	shared_ptr<annotated_root_node> annotated_ternary_expression::get_false_path() {
		return false_path;
	}

	source_token_list::iterator annotated_ternary_expression::get_position() {
		return ternary_expression_pos;
	}

	type_information annotated_ternary_expression::get_type_information() {
		return t_inf;
	}

	type_information annotated_ternary_expression::get_forced_type_information() {
		return forced_t_inf;
	}

	annotated_function::annotated_function(shared_ptr<annotated_root_node> arn, shared_ptr<function> func, shared_ptr<annotated_literal> alit,
		vector<shared_ptr<annotated_declaration>> parm_list, vector<shared_ptr<annotated_statement>> stmt_list,
		function_declaration_definition_kind fddk, type_information ti) : annotated_root_node(*arn), t_inf(ti),
		function_pos(func->get_position()) {
		identifier = alit;
		delsp_list = func->get_declspec_list();
		parameter_list = parm_list;
		statement_list = stmt_list;
		fdd_kind = fddk;
		fva_kind = func->get_function_va_args_kind();
		immutable = func->get_declspec_list()->get_declspecs_list().size() > 0;
	}

	annotated_function::~annotated_function() {

	}

	shared_ptr<annotated_literal> annotated_function::get_identifier() {
		return identifier;
	}

	shared_ptr<declspec_list> annotated_function::get_declspec_list() {
		return delsp_list;
	}

	source_token_list::iterator annotated_function::get_position() {
		return function_pos;
	}

	vector<shared_ptr<annotated_declaration>> annotated_function::get_parameter_list() {
		return parameter_list;
	}

	vector<shared_ptr<annotated_statement>> annotated_function::get_statement_list() {
		return statement_list;
	}

	const function_declaration_definition_kind annotated_function::get_function_declaration_definition_kind() {
		return fdd_kind;
	}

	type_information annotated_function::get_type_information() {
		return t_inf;
	}

	const function_va_args_kind annotated_function::get_function_va_args_kind() {
		return fva_kind;
	}

	const bool annotated_function::get_immutable() {
		return immutable;
	}

	annotated_structure::annotated_structure(shared_ptr<annotated_root_node> arn, shared_ptr<structure> struc, shared_ptr<annotated_literal> alit,
		vector<shared_ptr<annotated_declaration>> adecl_list, structure_declaration_definition_kind sddk, vector<type_information> til, type_information ti) :
		annotated_root_node(*arn), t_inf(ti), structure_pos(struc->get_position()) {
		identifier = alit;
		declaration_list = adecl_list;
		type_information_list = til;
		sdd_kind = sddk;
		immutable = struc->get_declspec_list()->get_declspecs_list().size() > 0;
	}

	annotated_structure::~annotated_structure() {

	}

	shared_ptr<annotated_literal> annotated_structure::get_identifier() {
		return identifier;
	}

	vector<shared_ptr<annotated_declaration>> annotated_structure::get_declaration_list() {
		return declaration_list;
	}

	source_token_list::iterator annotated_structure::get_position() {
		return structure_pos;
	}

	type_information annotated_structure::get_type_information() {
		return t_inf;
	}

	vector<type_information> annotated_structure::get_type_information_list() {
		return type_information_list;
	}

	const structure_declaration_definition_kind annotated_structure::get_structure_declaration_definition_kind() {
		return sdd_kind;
	}

	const bool annotated_structure::get_immutable() {
		return immutable;
	}

	annotated_function_argument_list::annotated_function_argument_list(shared_ptr<annotated_root_node> arn, shared_ptr<function_argument_list> fargl, vector<shared_ptr<annotated_binary_expression>> abexprl,
		vector<type_information> t_inf_list) : annotated_root_node(*arn), function_argument_list_pos(fargl->get_position()) {
		argument_list = abexprl;
		type_information_list = t_inf_list;
	}

	annotated_function_argument_list::~annotated_function_argument_list() {

	}

	vector<shared_ptr<annotated_binary_expression>> annotated_function_argument_list::get_argument_list() {
		return argument_list;
	}

	source_token_list::iterator annotated_function_argument_list::get_position() {
		return function_argument_list_pos;
	}

	vector<type_information> annotated_function_argument_list::get_type_information_list() {
		return type_information_list;
	}

	annotated_module::annotated_module(shared_ptr<annotated_root_node> arn, shared_ptr<module> mod, shared_ptr<annotated_literal> alit,
		vector<shared_ptr<annotated_statement>> astmt_list, vector<type_information> til, type_information ti) : annotated_root_node(*arn), t_inf(ti),
		module_pos(mod->get_position()) {
		identifier = alit;
		statement_list = astmt_list;
		t_inf_list = til;
		immutable = mod->get_declspec_list()->get_declspecs_list().size() > 0;
	}

	annotated_module::~annotated_module() {

	}

	shared_ptr<annotated_literal> annotated_module::get_identifier() {
		return identifier;
	}

	vector<shared_ptr<annotated_statement>> annotated_module::get_statement_list() {
		return statement_list;
	}

	source_token_list::iterator annotated_module::get_position() {
		return module_pos;
	}

	type_information annotated_module::get_type_information() {
		return t_inf;
	}

	vector<type_information> annotated_module::get_type_information_list() {
		return t_inf_list;
	}

	const bool annotated_module::get_immutable() {
		return immutable;
	}

	annotated_return_statement::annotated_return_statement(shared_ptr<annotated_root_node> arn, shared_ptr<return_statement> ret, shared_ptr<annotated_binary_expression> abexpr,
		type_information ti) : annotated_root_node(*arn), t_inf(ti), return_statement_pos(ret->get_position()) {
		b_expression = abexpr;
		rs_kind = ret->get_return_statement_kind();
	}

	annotated_return_statement::~annotated_return_statement() {

	}

	shared_ptr<annotated_binary_expression> annotated_return_statement::get_annotated_binary_expression() {
		return b_expression;
	}

	source_token_list::iterator annotated_return_statement::get_position() {
		return return_statement_pos;
	}

	const return_statement_kind annotated_return_statement::get_return_statement_kind() {
		return rs_kind;
	}

	type_information annotated_return_statement::get_type_information() {
		return t_inf;
	}

	annotated_conditional_statement::annotated_conditional_statement(shared_ptr<annotated_root_node> arn, shared_ptr<conditional_statement> cond, shared_ptr<annotated_binary_expression> ic,
		vector<shared_ptr<annotated_statement>> isl, shared_ptr<annotated_binary_expression> ec, vector<shared_ptr<annotated_statement>> esl, type_information ti) :
		annotated_root_node(*arn), conditional_statement_pos(cond->get_position()), t_inf(ti) {
		if_conditional = ic;
		else_conditional = ec;
		if_statement_list = isl;
		else_statement_list = esl;
		cec_kind = cond->get_conditional_else_conditional_kind();
		ces_kind = cond->get_conditional_else_statement_kind();
	}

	annotated_conditional_statement::~annotated_conditional_statement() {

	}

	shared_ptr<annotated_binary_expression> annotated_conditional_statement::get_if_conditional() {
		return if_conditional;
	}

	shared_ptr<annotated_binary_expression> annotated_conditional_statement::get_else_conditional() {
		return else_conditional;
	}

	vector<shared_ptr<annotated_statement>> annotated_conditional_statement::get_if_statement_list() {
		return if_statement_list;
	}

	vector<shared_ptr<annotated_statement>> annotated_conditional_statement::get_else_statement_list() {
		return else_statement_list;
	}

	source_token_list::iterator annotated_conditional_statement::get_position() {
		return conditional_statement_pos;
	}

	type_information annotated_conditional_statement::get_type_information() {
		return t_inf;
	}

	const conditional_else_conditional_kind annotated_conditional_statement::get_conditional_else_conditional_kind() {
		return cec_kind;
	}

	const conditional_else_statement_kind annotated_conditional_statement::get_conditional_else_statement_kind() {
		return ces_kind;
	}

	annotated_enum_statement::annotated_enum_statement(shared_ptr<annotated_root_node> arn, shared_ptr<enum_statement> _enum, shared_ptr<annotated_literal> alit,
		vector<shared_ptr<annotated_literal>> ailist, type_information ti) : annotated_root_node(*arn), enum_statement_pos(_enum->get_position()), t_inf(ti) {
		identifier = alit;
		identifier_list = ailist;
		es_kind = _enum->get_enum_statement_kind();
	}

	annotated_enum_statement::~annotated_enum_statement() {

	}

	vector<shared_ptr<annotated_literal>> annotated_enum_statement::get_identifier_list() {
		return identifier_list;
	}

	shared_ptr<annotated_literal> annotated_enum_statement::get_identifier() {
		return identifier;
	}

	const enum_statement_kind annotated_enum_statement::get_enum_statement_kind() {
		return es_kind;
	}

	source_token_list::iterator annotated_enum_statement::get_position() {
		return enum_statement_pos;
	}

	type_information annotated_enum_statement::get_type_information() {
		return t_inf;
	}

	annotated_while_statement::annotated_while_statement(shared_ptr<annotated_root_node> arn, shared_ptr<while_statement> _while, shared_ptr<annotated_binary_expression> cond,
		vector<shared_ptr<annotated_statement>> astmt_list, type_information ti) : annotated_root_node(*arn), t_inf(ti), while_statement_pos(_while->get_position()) {
		condition = cond;
		statement_list = astmt_list;
	}

	annotated_while_statement::~annotated_while_statement() {

	}

	shared_ptr<annotated_binary_expression> annotated_while_statement::get_condition() {
		return condition;
	}

	vector<shared_ptr<annotated_statement>> annotated_while_statement::get_statement_list() {
		return statement_list;
	}

	source_token_list::iterator annotated_while_statement::get_position() {
		return while_statement_pos;
	}

	type_information annotated_while_statement::get_type_information() {
		return t_inf;
	}
}
