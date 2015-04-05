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
		bool ret = (t_kind == t_inf.t_kind && tp_kind == t_inf.tp_kind && c_kind == t_inf.c_kind);
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

	symbol::symbol(type_information ti, immut_kind ik, shared_ptr<literal> ident, function_kind fk, vector<type_information> fa, shared_ptr<symbol_table> st) : t_inf(ti) {
		i_kind = ik;
		identifier = ident;
		f_kind = fk;
		function_arguments = fa;
		class_type_information = st;
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
		for(int i = 0; i < sym_table.size(); i++)
			if(sym_table[i]->get_identifier()->get_raw_literal()->get_raw_string() == lit->get_raw_literal()->get_raw_string())
				ret.push_back(sym_table[i]);
		return ret;
	}

	vector<shared_ptr<symbol>> symbol_table::find_all_symbols(shared_ptr<annotated_literal> alit) {
		vector<shared_ptr<symbol>> ret;
		for(int i = 0; i < sym_table.size(); i++)
			if(sym_table[i]->get_identifier()->get_raw_literal()->get_raw_string() == alit->get_raw_literal()->get_raw_string())
				ret.push_back(sym_table[i]);
		return ret;
	}

	shared_ptr<symbol> symbol_table::add_symbol(shared_ptr<symbol> sym) {
		sym_table.push_back(sym);
		return sym;
	}

	analyze_ast::analyze_ast(shared_ptr<root_node> r) {
		root = r;
		sym_table = make_shared<symbol_table>();
		ann_root_node = make_shared<annotated_root_node>(r, vector<shared_ptr<annotated_statement>>());
	}

	analyze_ast::~analyze_ast() {

	}

	shared_ptr<root_node> analyze_ast::get_root_node() {
		return root;
	}

	shared_ptr<symbol_table> analyze_ast::get_symbol_table() {
		return sym_table;
	}

	shared_ptr<annotated_root_node> analyze_ast::get_annotated_root_node() {
		return ann_root_node;
	}

	const bool analyze_ast::perform_semantic_analysis() {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		bool ret = true;
		if(root->get_diagnostics_reporter()->get_error_count() > 0)
			ret = false;
		vector<shared_ptr<statement>> stmt_list = root->get_statement_list();
		for(int i = 0; i < stmt_list.size(); i++) {
			if(!stmt_list[i]->get_valid())
				ret = false;
			else {
				shared_ptr<annotated_statement> astmt = analyze_statement(stmt_list[i]);
				if(astmt->get_type_information() == bad) ret = false;
				ann_root_node->add_annotated_statement(astmt);
			}
		}
		return ret;
	}

	shared_ptr<annotated_statement> analyze_ast::analyze_statement(shared_ptr<statement> stmt) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		shared_ptr<annotated_binary_expression> abexpr = nullptr;
		shared_ptr<annotated_declaration> adecl = nullptr;
		if(!stmt->get_valid())
			return make_shared<annotated_statement>(ann_root_node, stmt, nullptr, nullptr, bad);
		if(stmt->get_statement_kind() == statement_kind::STATEMENT_DECLARATION)
			adecl = analyze_declaration(stmt->get_declaration());
		else if(stmt->get_statement_kind() == statement_kind::STATEMENT_EXPRESSION)
			abexpr = analyze_binary_expression(stmt->get_binary_expression());
		else
			return make_shared<annotated_statement>(ann_root_node, stmt, nullptr, nullptr, bad);
		type_information ret = bad;
		if(adecl != nullptr) ret = adecl->get_type_information();
		else if(abexpr != nullptr) ret = abexpr->get_type_information();
		ret = type_information(ret, value_kind::VALUE_NOT_APPLICABLE);
		return make_shared<annotated_statement>(ann_root_node, stmt, abexpr, adecl, ret);
	}

	shared_ptr<annotated_declaration> analyze_ast::analyze_declaration(shared_ptr<declaration> decl) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if(!decl->get_valid())
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		bool immut = false;
		function_kind fk = function_kind::FUNCTION_NO;
		if(decl->get_declspec_list()->get_declspecs_list().size() == 1)
			immut = true;
		else if(decl->get_declspec_list()->get_declspecs_list().size() == 0)
			immut = false;
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_only_one_immut, decl->get_declspec_list()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			immut = true;
		}
		vector<shared_ptr<symbol>> vec = sym_table->find_all_symbols(decl->get_identifier());
		if(vec.size() > 0) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::repeated_declaration_ignoring_this_one, decl->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, vec[vec.size() - 1]->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		}
		if(decl->get_identifier()->get_raw_literal()->get_token_kind() != token_kind::TOKEN_IDENTIFIER) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, decl->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		}
		shared_ptr<annotated_binary_expression> abexpr = analyze_binary_expression(decl->get_binary_expression());
		type_information t_inf = abexpr->get_type_information();
		if(t_inf.get_type_kind() == type_kind::TYPE_NONE)
			return make_shared<annotated_declaration>(ann_root_node, decl, nullptr, nullptr, bad);
		shared_ptr<symbol> sym = make_shared<symbol>(t_inf, immut ? immut_kind::IMMUT_YES : immut_kind::IMMUT_NO, decl->get_identifier(), fk, vector<type_information>(), nullptr);
		sym_table->add_symbol(sym);
		t_inf = type_information(t_inf, value_kind::VALUE_LVALUE);
		shared_ptr<annotated_literal> alit = make_shared<annotated_literal>(ann_root_node, decl->get_identifier(), t_inf);
		return make_shared<annotated_declaration>(ann_root_node, decl, alit, abexpr, type_information(t_inf, value_kind::VALUE_NOT_APPLICABLE));
	}

	shared_ptr<annotated_binary_expression> analyze_ast::analyze_binary_expression(shared_ptr<binary_expression> bexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if(!bexpr->get_valid())
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information lhs = bad;
		type_information lhs_forced = bad;
		type_information rhs = bad;
		type_information rhs_forced = bad;
		shared_ptr<annotated_root_node> bexpr_lhs = nullptr;
		shared_ptr<annotated_root_node> bexpr_rhs = nullptr;
		if(bexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			shared_ptr<binary_expression> bexpr1 = static_pointer_cast<binary_expression>(bexpr->get_lhs());
			bexpr_lhs = analyze_binary_expression(bexpr1);
			lhs = static_pointer_cast<annotated_binary_expression>(bexpr_lhs)->get_type_information();
			lhs_forced = static_pointer_cast<annotated_binary_expression>(bexpr_lhs)->get_forced_type_information();
		}
		else if(bexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION) {
			shared_ptr<unary_expression> uexpr = static_pointer_cast<unary_expression>(bexpr->get_lhs());
			bexpr_lhs = analyze_unary_expression(uexpr);
			lhs = static_pointer_cast<annotated_unary_expression>(bexpr_lhs)->get_type_information();
			lhs_forced = static_pointer_cast<annotated_unary_expression>(bexpr_lhs)->get_forced_type_information();
		}
		else if(bexpr->get_lhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
			shared_ptr<ternary_expression> texpr = static_pointer_cast<ternary_expression>(bexpr->get_lhs());
			bexpr_lhs = analyze_ternary_expression(texpr);
			lhs = static_pointer_cast<annotated_ternary_expression>(bexpr_lhs)->get_type_information();
			lhs_forced = static_pointer_cast<annotated_ternary_expression>(bexpr_lhs)->get_forced_type_information();
		}
		else
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
		if(bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION) {
			shared_ptr<unary_expression> uexpr = static_pointer_cast<unary_expression>(bexpr->get_rhs());
			bexpr_rhs = analyze_unary_expression(uexpr);
			rhs = static_pointer_cast<annotated_unary_expression>(bexpr_rhs)->get_type_information();
			rhs_forced = static_pointer_cast<annotated_unary_expression>(bexpr_rhs)->get_forced_type_information();
		}
		else if(bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION) {
			shared_ptr<binary_expression> bexpr1 = static_pointer_cast<binary_expression>(bexpr->get_rhs());
			bexpr_rhs = analyze_binary_expression(bexpr1);
			rhs = static_pointer_cast<annotated_binary_expression>(bexpr_rhs)->get_type_information();
			rhs_forced = static_pointer_cast<annotated_binary_expression>(bexpr_rhs)->get_forced_type_information();
		}
		else if(bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION) {
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
		if(bexpr->get_rhs_kind() == binary_expression_kind::BINARY_EXPRESSION_NONE || bexpr->get_rhs() == nullptr)
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, bexpr_lhs, bexpr_rhs, lhs, lhs_forced, rhs, bad, lhs, bad);
		deduce_binary_expression_type dbet(lhs, bexpr->get_binary_operation_kind(), rhs);
		type_information t_inf = dbet.deduce_type();
		if(t_inf == bad) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, bexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
		}
		if((lhs == _int && rhs == _float) || (lhs == _float && rhs == _int))
			root->get_diagnostics_reporter()->print(diagnostic_messages::unequal_but_compatible_types, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
		binary_operation_kind bopk = bexpr->get_binary_operation_kind();
		if(bopk == binary_operation_kind::BINARY_OPERATION_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_PLUS_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_MINUS_EQUALS ||
				bopk == binary_operation_kind::BINARY_OPERATION_MULTIPLY_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_DIVIDE_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_MODULUS_EQUALS ||
				bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_EXPONENT_EQUALS ||
				bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_AND_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_BITWISE_OR_EQUALS || bopk == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR_EQUALS) {
			if(lhs.get_value_kind() != value_kind::VALUE_LVALUE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_lvalue, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
			}
			if(lhs.get_literal() != nullptr) {
				vector<shared_ptr<symbol>> results = sym_table->find_all_symbols(lhs.get_literal());
				if(results.size() > 0 && results[results.size() - 1]->get_immut_kind() == immut_kind::IMMUT_YES) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::immut_value_cannot_be_modified, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, results[results.size() - 1]->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
					return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
				}
				if(results[results.size() - 1]->get_type_information().get_type_kind() == type_kind::TYPE_TUPLE) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::tuples_cannot_be_modified, bexpr->get_lhs()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, results[results.size() - 1]->get_identifier()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
					return make_shared<annotated_binary_expression>(ann_root_node, bexpr, nullptr, nullptr, bad, bad, bad, bad, bad, bad);
				}
			}
		}
		return make_shared<annotated_binary_expression>(ann_root_node, bexpr, bexpr_lhs, bexpr_rhs, lhs, lhs_forced, rhs, rhs_forced, t_inf, bad);
	}

	shared_ptr<annotated_ternary_expression> analyze_ast::analyze_ternary_expression(shared_ptr<ternary_expression> texpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if(!texpr->get_valid())
			return make_shared<annotated_ternary_expression>(ann_root_node, texpr, nullptr, nullptr, nullptr, bad, bad);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		shared_ptr<annotated_binary_expression> abexpr1 = analyze_binary_expression(static_pointer_cast<binary_expression>(texpr->get_condition()));
		shared_ptr<annotated_binary_expression> abexpr2 = analyze_binary_expression(static_pointer_cast<binary_expression>(texpr->get_true_path()));
		shared_ptr<annotated_binary_expression> abexpr3 = analyze_binary_expression(static_pointer_cast<binary_expression>(texpr->get_false_path()));
		if(abexpr1->get_type_information() == _boolean || abexpr1->get_type_information().get_type_kind() == type_kind::TYPE_ANY);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_boolean_for_ternary_expression_condition, texpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_ternary_expression>(ann_root_node, texpr, nullptr, nullptr, nullptr, bad, bad);
		}
		if(abexpr2->get_type_information() == abexpr3->get_type_information() || abexpr2->get_type_information().get_type_kind() == type_kind::TYPE_ANY || abexpr3->get_type_information().get_type_kind() == type_kind::TYPE_ANY ||
				(abexpr2->get_type_information() == _int && abexpr3->get_type_information() == _float) || (abexpr2->get_type_information() == _float && abexpr3->get_type_information() == _int)) {
			if((abexpr2->get_type_information() == _int && abexpr3->get_type_information() == _float) || (abexpr2->get_type_information() == _float && abexpr3->get_type_information() == _int))
				root->get_diagnostics_reporter()->print(diagnostic_messages::unequal_but_compatible_types, abexpr2->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, texpr->get_true_path()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<annotated_ternary_expression>(ann_root_node, texpr, nullptr, nullptr, nullptr, bad, bad);
		}
		return make_shared<annotated_ternary_expression>(ann_root_node, texpr, abexpr1, abexpr2, abexpr3, type_information(abexpr2->get_type_information(), value_kind::VALUE_RVALUE), bad);
	}

	shared_ptr<annotated_unary_expression> analyze_ast::analyze_unary_expression(shared_ptr<unary_expression> uexpr) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if(!uexpr->get_valid())
			return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		unary_operation_kind op_kind = uexpr->get_unary_operation_kind();
		shared_ptr<annotated_linearized_postfix_expression> alpe = analyze_postfix_expression(uexpr->get_raw_postfix_expression());
		type_information t_inf = alpe->get_type_information();
		if(op_kind != unary_operation_kind::UNARY_OPERATION_NONE) {
			if(op_kind == unary_operation_kind::UNARY_OPERATION_MINUS || op_kind == unary_operation_kind::UNARY_OPERATION_PLUS) {
				if(t_inf == _int || t_inf == _float || t_inf.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(t_inf, value_kind::VALUE_RVALUE);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, alpe, temp, _float);
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
				}
			}
			else if(op_kind == unary_operation_kind::UNARY_OPERATION_INCREMENT || op_kind == unary_operation_kind::UNARY_OPERATION_DECREMENT || op_kind == unary_operation_kind::UNARY_OPERATION_COMPLEMENT) {
				if(t_inf == _int || t_inf.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(t_inf, value_kind::VALUE_RVALUE);
					if(op_kind == unary_operation_kind::UNARY_OPERATION_DECREMENT || op_kind == unary_operation_kind::UNARY_OPERATION_INCREMENT) {
						if(t_inf.get_value_kind() != value_kind::VALUE_LVALUE) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_lvalue_for_increments_and_decrements, uexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_shared<annotated_unary_expression>(ann_root_node, uexpr, nullptr, bad, bad);
						}
						vector<shared_ptr<symbol>> sym_results = sym_table->find_all_symbols(t_inf.get_literal());
						if(sym_results[sym_results.size() - 1]->get_immut_kind() == immut_kind::IMMUT_YES) {
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
			else if(op_kind == unary_operation_kind::UNARY_OPERATION_NOT) {
				if(t_inf == _boolean || t_inf.get_type_kind() == _any.get_type_kind()) {
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
		if(poexpr->get_postfix_operation_kind() == postfix_operation_kind::POSTFIX_OPERATION_NONE);
		else {
			postfix_operation_kind pok = poexpr->get_postfix_operation_kind();
			if(pok == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT || pok == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr);
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), pok);
			}
			else if(pok == postfix_operation_kind::POSTFIX_FUNCTION_CALL) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr->get_argument_list());
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), postfix_operation_kind::POSTFIX_FUNCTION_CALL);
			}
			else if(pok == postfix_operation_kind::POSTFIX_DOT_OPERATOR) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr->get_identifier());
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), postfix_operation_kind::POSTFIX_DOT_OPERATOR);
			}
			else if(pok == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
				postfix_operation_list.insert(postfix_operation_list.begin(), poexpr->get_subscript());
				postfix_operation_kind_list.insert(postfix_operation_kind_list.begin(), postfix_operation_kind::POSTFIX_SUBSCRIPT);
			}
			else
				return nullptr;
		}
		if(poexpr->get_postfix_expression_kind() == postfix_expression_kind::POSTFIX_EXPRESSION_PRIMARY_EXPRESSION)
			pr_expr = poexpr->get_raw_primary_expression();
		else if(poexpr->get_postfix_expression_kind() == postfix_expression_kind::POSTFIX_EXPRESSION_POSTFIX_EXPRESSION) {
			shared_ptr<linearized_postfix_expression> lpe = linearize_postfix_expression(poexpr->get_postfix_expression());
			for(int i = lpe->get_postfix_operation_list().size() - 1; i >= 0; i--)
				postfix_operation_list.insert(postfix_operation_list.begin(), lpe->get_postfix_operation_list()[i]);
			for(int i = lpe->get_postfix_operation_kind_list().size() - 1; i >= 0; i--)
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
		if(pok == postfix_operation_kind::POSTFIX_OPERATION_INCREMENT || pok == postfix_operation_kind::POSTFIX_OPERATION_DECREMENT) {
			if(prev == _int || prev == _any) {
				if(prev.get_value_kind() != value_kind::VALUE_LVALUE) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_lvalue_for_increments_and_decrements, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_pair(bad, nullptr);
				}
				if(prev.get_literal() != nullptr) {
					vector<shared_ptr<symbol>> sym_results = sym_table->find_all_symbols(prev.get_literal());
					if(sym_results[sym_results.size() - 1]->get_immut_kind() == immut_kind::IMMUT_YES) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::immut_value_cannot_be_modified, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						root->get_diagnostics_reporter()->print(diagnostic_messages::originally_declared_here, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_NOTE);
						return make_pair(bad, nullptr);
					}
				}
				type_information temp = type_information(prev, value_kind::VALUE_RVALUE);
				if(prev == _any)
					temp = prev;
				return make_pair(temp, nullptr);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::incompatible_types, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				type_information temp = type_information(_int, value_kind::VALUE_RVALUE);
				return make_pair(bad, nullptr);
			}
		}
		else if(pok == postfix_operation_kind::POSTFIX_FUNCTION_CALL) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::instruction_not_supported, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(bad, nullptr);
		}
		else if(pok == postfix_operation_kind::POSTFIX_DOT_OPERATOR) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::instruction_not_supported, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_pair(bad, nullptr);
		}
		else if(pok == postfix_operation_kind::POSTFIX_SUBSCRIPT) {
			bool sta = false;
			bool fin = false;
			bool ste = false;
			shared_ptr<subscript> subscr = static_pointer_cast<subscript>(op);
			if(subscr->get_start() == nullptr) sta = true;
			else sta = !(analyze_binary_expression(subscr->get_start())->get_type_information() == bad);
			if(subscr->get_final() == nullptr) fin = true;
			else fin = !(analyze_binary_expression(subscr->get_final())->get_type_information() == bad);
			if(subscr->get_step() == nullptr) ste = true;
			else ste = !(analyze_binary_expression(subscr->get_step())->get_type_information() == bad);
			if(subscr->get_start() == nullptr && subscr->get_final() == nullptr && subscr->get_step() == nullptr) {
				if(prev != _list && prev != _tuple && prev != _dict && prev.get_type_kind() != _any.get_type_kind() && prev != _string) {
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
			if(prev != _list && prev != _tuple && prev != _dict && prev.get_type_kind() != _any.get_type_kind() && prev != _string) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::subscript_applicable_only_for_sequences_and_dictionaries, op->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_pair(bad, nullptr);
			}
			if(sta && fin && ste) {
				if(prev.get_type_kind() == _any.get_type_kind()) {
					type_information temp = type_information(prev, value_kind::VALUE_LVALUE);
					shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
							subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
							subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
							subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
					return make_pair(temp, asubscr);
				}
				else if(prev == _dict) {
					if(subscr->get_step() != nullptr || subscr->get_final() != nullptr || subscr->get_subscript_colon_kind() != subscript_colon_kind::SUBSCRIPT_COLON_ZERO) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::dictionaries_only_support_subscripting_not_slicing, subscr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						return make_pair(bad, nullptr);
					}
					type_information t_inf = analyze_binary_expression(subscr->get_start())->get_type_information();
					shared_ptr<type_information> key_inf = prev.get_key_information();
					if(t_inf != *key_inf && t_inf.get_type_kind() != _any.get_type_kind()) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::expected_dictionary_subscript_to_equal_its_key_type, subscr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						return make_pair(bad, nullptr);
					}
					shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
							subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
							subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
							subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
					return make_pair(type_information(type_information(*(prev.get_value_information()), value_kind::VALUE_LVALUE), prev.get_literal()), asubscr);
				}
				else if(prev == _list || prev == _tuple || prev == _string) {
					if(subscr->get_start() == nullptr);
					else {
						type_information t_inf = analyze_binary_expression(subscr->get_start())->get_type_information();
						if(t_inf != _int && t_inf.get_type_kind() != _any.get_type_kind()) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_subscript_for_sequence_to_be_integer, subscr->get_start()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_pair(bad, nullptr);
						}
					}
					if(subscr->get_final() == nullptr);
					else {
						type_information t_inf = analyze_binary_expression(subscr->get_final())->get_type_information();
						if(t_inf != _int && t_inf.get_type_kind() != _any.get_type_kind()) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_subscript_for_sequence_to_be_integer, subscr->get_final()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_pair(bad, nullptr);
						}
					}
					if(subscr->get_step() == nullptr);
					else {
						type_information t_inf = analyze_binary_expression(subscr->get_step())->get_type_information();
						if(t_inf != _int && t_inf.get_type_kind() != _any.get_type_kind()) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_subscript_for_sequence_to_be_integer, subscr->get_step()->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							return make_pair(bad, nullptr);
						}
					}
					if(prev == _string) {
						shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
								subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
								subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
								subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
						return make_pair(type_information(prev, value_kind::VALUE_LVALUE), asubscr);
					}
					if(subscr->get_start() != nullptr && subscr->get_final() == nullptr && subscr->get_step() == nullptr && subscr->get_subscript_colon_kind() == subscript_colon_kind::SUBSCRIPT_COLON_ZERO) {
						type_information temp = type_information(type_information(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE), prev.get_literal());
						shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
								subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
								subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
								subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
						return make_pair(temp, asubscr);
					}
					type_information temp = bad;
					if(prev == _list)
						temp = type_information(type_information(prev, value_kind::VALUE_LVALUE), prev.get_literal());
					else if(prev == _tuple)
						temp = type_information(type_information(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_LVALUE), prev.get_literal());
					shared_ptr<annotated_subscript> asubscr = make_shared<annotated_subscript>(ann_root_node, subscr, subscr->get_start() == nullptr ? nullptr : analyze_binary_expression(subscr->get_start()),
							subscr->get_final() == nullptr ? nullptr : analyze_binary_expression(subscr->get_final()), subscr->get_step() == nullptr ? nullptr : analyze_binary_expression(subscr->get_step()),
							subscr->get_start() == nullptr ? bad : analyze_binary_expression(subscr->get_start())->get_type_information(), subscr->get_final() == nullptr ? bad : analyze_binary_expression(subscr->get_final())->get_type_information(),
							subscr->get_step() == nullptr ? bad : analyze_binary_expression(subscr->get_step())->get_type_information());
					return make_pair(temp, asubscr);
				}
				type_information temp = bad;
				if(prev == _list)
					temp = type_information(type_information(prev, value_kind::VALUE_LVALUE), prev.get_literal());
				else if(prev == _tuple)
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
		if(!poexpr->get_valid())
			return make_shared<annotated_linearized_postfix_expression>(ann_root_node, lpe, aprexpr, vector<shared_ptr<annotated_root_node>>(), vector<postfix_operation_kind>(), vector<type_information>(), bad);
		if(lpe->get_postfix_operation_kind_list().size() != lpe->get_postfix_operation_list().size())
			return make_shared<annotated_linearized_postfix_expression>(ann_root_node, lpe, aprexpr, vector<shared_ptr<annotated_root_node>>(), vector<postfix_operation_kind>(), vector<type_information>(), bad);
		vector<type_information> t_inf_list;
		vector<shared_ptr<annotated_root_node>> ann_root_node_list;
		ann_root_node_list.push_back(get<2>(tup0));
		vector<postfix_operation_kind> pokl;
		if(lpe->get_postfix_operation_kind_list().size() != 0)
			pokl.push_back(lpe->get_postfix_operation_kind_list()[0]);
		else
			pokl.push_back(postfix_operation_kind::POSTFIX_OPERATION_NONE);
		for(int i = 1; i < lpe->get_postfix_operation_list().size(); i++) {
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
		if(!prexpr->get_valid() || lpe == nullptr) {
			shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
			return make_tuple(aprexpr, bad, nullptr);
		}
		primary_expression_kind prek = prexpr->get_primary_expression_kind();
		type_information t_inf = bad;
		shared_ptr<annotated_sequence> aseq = nullptr;
		shared_ptr<annotated_dictionary> adict = nullptr;
		shared_ptr<annotated_literal> alit = nullptr;
		shared_ptr<annotated_binary_expression> abexpr = nullptr;
		if(prek == primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION) {
			t_inf = analyze_binary_expression(prexpr->get_raw_parenthesized_expression())->get_type_information();
			abexpr = analyze_binary_expression(prexpr->get_raw_parenthesized_expression());
		}
		else if(prek == primary_expression_kind::PRIMARY_EXPRESSION_SEQUENCE) {
			shared_ptr<sequence> seq = prexpr->get_sequence();
			if(seq->get_sequence_kind() == sequence_kind::SEQUENCE_TUPLE) {
				aseq = analyze_tuple_expression(seq);
				t_inf = aseq->get_type_information();
			}
			else if(seq->get_sequence_kind() == sequence_kind::SEQUENCE_LIST) {
				aseq = analyze_list_expression(seq);
				t_inf = aseq->get_type_information();
			}
			else if(seq->get_sequence_kind() == sequence_kind::SEQUENCE_NONE) {
				shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
				return make_tuple(aprexpr, bad, nullptr);
			}
		}
		else if(prek == primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION) {
			type_information temp = analyze_binary_expression(prexpr->get_raw_parenthesized_expression())->get_type_information();
			t_inf = type_information(temp.get_type_kind(), type_pure_kind::TYPE_PURE_YES, temp.get_type_class_kind(), value_kind::VALUE_RVALUE);
			abexpr = analyze_binary_expression(prexpr->get_raw_parenthesized_expression());
		}
		else if(prek == primary_expression_kind::PRIMARY_EXPRESSION_DICTIONARY) {
			adict = analyze_dictionary_expression(prexpr->get_dictionary());
			t_inf = adict->get_type_information();
		}
		else if(prek == primary_expression_kind::PRIMARY_EXPRESSION_LITERAL) {
			shared_ptr<literal> lit = prexpr->get_raw_literal();
			literal_kind lk = lit->get_literal_kind();
			if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER)
				t_inf = type_information(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if(lk == literal_kind::LITERAL_STRING)
				t_inf = type_information(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if(lk == literal_kind::LITERAL_DECIMAL_NUMBER)
				t_inf = type_information(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if(lk == literal_kind::LITERAL_NIL)
				t_inf = type_information(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if(lk == literal_kind::LITERAL_BOOLEAN)
				t_inf = type_information(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			else if(lk == literal_kind::LITERAL_IDENTIFIER) {
				vector<shared_ptr<symbol>> ls_sym = sym_table->find_all_symbols(lit);
				if(ls_sym.size() == 0) {
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
		if(lpe->get_postfix_operation_list().size() != lpe->get_postfix_operation_kind_list().size()) {
			shared_ptr<annotated_primary_expression> aprexpr = make_shared<annotated_primary_expression>(ann_root_node, prexpr, nullptr, nullptr, nullptr, nullptr, bad);
			return make_tuple(aprexpr, bad, nullptr);
		}
		if(lpe->get_postfix_operation_list().size() == 0)
			return make_tuple(make_shared<annotated_primary_expression>(ann_root_node, prexpr, alit, abexpr, aseq, adict, t_inf), t_inf, nullptr);
		shared_ptr<root_node> poexpr = lpe->get_postfix_operation_list()[0];
		postfix_operation_kind pok = lpe->get_postfix_operation_kind_list()[0];
		pair<type_information, shared_ptr<annotated_root_node>> pai =  analyze_postfix_operation(t_inf, pok, poexpr);
		return make_tuple(make_shared<annotated_primary_expression>(ann_root_node, prexpr, alit, abexpr, aseq, adict, t_inf), pai.first, pai.second);
	}

	shared_ptr<annotated_sequence> analyze_ast::analyze_list_expression(shared_ptr<sequence> seq) {
		type_information bad(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, value_kind::VALUE_NONE);
		if(!seq->get_valid())
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), bad);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		vector<shared_ptr<binary_expression>> bexpr_list = seq->get_expression_list();
		if(bexpr_list.size() == 0) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_at_least_one_element_in_sequence, seq->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			shared_ptr<type_information> inner = make_shared<type_information>(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			type_information ret(type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE, inner);
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), ret);
		}
		vector<shared_ptr<annotated_binary_expression>> abexpr_list;
		abexpr_list.push_back(analyze_binary_expression(bexpr_list[0]));
		type_information t_inf = bad;
		t_inf = analyze_binary_expression(bexpr_list[0])->get_type_information();
		vector<type_information> t_inf_list;
		t_inf_list.push_back(t_inf);
		for(int i = 1; i < bexpr_list.size(); i++) {
			abexpr_list.push_back(analyze_binary_expression(bexpr_list[i]));
			type_information in_t_inf = analyze_binary_expression(bexpr_list[i])->get_type_information();
			t_inf_list.push_back(in_t_inf);
			if(t_inf.get_type_kind() == type_kind::TYPE_ANY || t_inf == in_t_inf || in_t_inf.get_type_kind() == type_kind::TYPE_ANY);
			else if((t_inf == _int && in_t_inf == _float) || (t_inf == _float && in_t_inf == _int)) {
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
		if(!dict->get_valid() || (dict->get_key_list().size() != dict->get_value_list().size()))
			return make_shared<annotated_dictionary>(ann_root_node, dict, vector<shared_ptr<annotated_binary_expression>>(), vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(),
					vector<type_information>(), bad);
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		vector<shared_ptr<binary_expression>> kl = dict->get_key_list();
		vector<shared_ptr<binary_expression>> vl = dict->get_value_list();
		if(kl.size() == 0 || vl.size() == 0) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_at_least_one_element_in_sequence, dict->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			shared_ptr<type_information> inner_value = make_shared<type_information>(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			shared_ptr<type_information> inner_key = make_shared<type_information>(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			type_information ret(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE, inner_key, inner_value);
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
		for(int i = 1; i < kl.size(); i++) {
			type_information in_kt_inf = analyze_binary_expression(kl[i])->get_type_information();
			klist.push_back(analyze_binary_expression(kl[i]));
			ktlist.push_back(in_kt_inf);
			if(kt_inf.get_type_kind() == type_kind::TYPE_ANY || kt_inf == in_kt_inf || in_kt_inf.get_type_kind() == type_kind::TYPE_ANY);
			else if((kt_inf == _int && in_kt_inf == _float) || (kt_inf == _float && in_kt_inf == _int)) {
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
		for(int i = 1; i < vl.size(); i++) {
			type_information in_vt_inf = analyze_binary_expression(vl[i])->get_type_information();
			vlist.push_back(analyze_binary_expression(vl[i]));
			vtlist.push_back(in_vt_inf);
			if(vt_inf.get_type_kind() == type_kind::TYPE_ANY || vt_inf == in_vt_inf || in_vt_inf.get_type_kind() == type_kind::TYPE_ANY);
			else if((vt_inf == _int && in_vt_inf == _float) || (vt_inf == _float && in_vt_inf == _int)) {
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
		if(!seq->get_valid())
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), bad);
		vector<shared_ptr<binary_expression>> bexpr_list = seq->get_expression_list();
		if(bexpr_list.size() == 0) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_at_least_one_element_in_sequence, seq->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
			shared_ptr<type_information> inner = make_shared<type_information>(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
			type_information ret(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE, inner);
			return make_shared<annotated_sequence>(ann_root_node, seq, vector<shared_ptr<annotated_binary_expression>>(), vector<type_information>(), ret);
		}
		vector<shared_ptr<type_information>> inner;
		vector<shared_ptr<annotated_binary_expression>> abel;
		vector<type_information> tlist;
		vector<type_information> ftlist;
		for(int i = 0; i < bexpr_list.size(); i++) {
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
		if(rhs == bad || lhs == bad)
			return bad;
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _any(type_kind::TYPE_ANY, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);

		type_information _pure_int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _pure_float(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		type_information _pure_nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, value_kind::VALUE_RVALUE);
		if(b_kind == binary_operation_kind::BINARY_OPERATION_PLUS || b_kind == binary_operation_kind::BINARY_OPERATION_MINUS || b_kind == binary_operation_kind::BINARY_OPERATION_MULTIPLY || b_kind == binary_operation_kind::BINARY_OPERATION_DIVIDE ||
				b_kind == binary_operation_kind::BINARY_OPERATION_MODULUS || b_kind == binary_operation_kind::BINARY_OPERATION_EXPONENT) {
			if(lhs == _int || lhs == _float || lhs.get_type_kind() == _any.get_type_kind()) {
				if(rhs == _int || rhs == _float || rhs.get_type_kind() == _any.get_type_kind()) {
					if(lhs == _int && rhs == _int)
						return _int;
					else if(lhs.get_type_kind() == type_kind::TYPE_ANY || rhs.get_type_kind() == type_kind::TYPE_ANY)
						return _any;
					else
						return _float;
				}
				else
					return bad;
			}
			return bad;
		}
		else if(b_kind == binary_operation_kind::BINARY_OPERATION_GREATER_THAN || b_kind == binary_operation_kind::BINARY_OPERATION_LESS_THAN || b_kind == binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO ||
				b_kind == binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO) {
			if(lhs == _int || lhs == _float || lhs.get_type_kind() == _any.get_type_kind()) {
				if(rhs == _int || rhs == _float || lhs.get_type_kind() == _any.get_type_kind())
					return _boolean;
				else if(lhs.get_type_kind() == type_kind::TYPE_ANY || rhs.get_type_kind() == type_kind::TYPE_ANY)
					return _any;
				else
					return bad;
			}
			else if(lhs == rhs)
				return lhs;
			return bad;
		}
		else if(b_kind == binary_operation_kind::BINARY_OPERATION_LOGICAL_AND || b_kind == binary_operation_kind::BINARY_OPERATION_LOGICAL_OR) {
			if((lhs == _boolean || lhs.get_type_kind() == _any.get_type_kind()) && (rhs == _boolean || rhs.get_type_kind() == _any.get_type_kind()))
				return _boolean;
			else if(lhs.get_type_kind() == type_kind::TYPE_ANY || rhs.get_type_kind() == type_kind::TYPE_ANY)
				return _any;
			else
				return bad;
		}
		else if(b_kind == binary_operation_kind::BINARY_OPERATION_BITWISE_AND || b_kind == binary_operation_kind::BINARY_OPERATION_BITWISE_OR || b_kind == binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT || b_kind == binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT ||
				b_kind == binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR) {
			if(lhs.get_type_kind() == type_kind::TYPE_ANY || rhs.get_type_kind() == type_kind::TYPE_ANY)
				return _any;
			if((lhs.get_type_kind() == _any.get_type_kind() || lhs == _int) && (rhs == _int || rhs.get_type_kind() == _any.get_type_kind()))
				return _int;
			else
				return bad;
		}
		else if(b_kind == binary_operation_kind::BINARY_OPERATION_POINT) {
			if(lhs == _int || lhs == _float) {
				if(rhs == _pure_int)
					return _int;
				else if(rhs == _pure_float)
					return _float;
				else if(rhs.get_type_kind() == _any.get_type_kind())
					return rhs;
				else
					return bad;
			}
			else if(rhs == _pure_nil)
				return bad;
			else if(lhs.get_type_kind() == _any.get_type_kind()) {
				if(rhs == _pure_int)
					return _int;
				else if(rhs == _pure_float)
					return _float;
				else if(rhs.get_type_kind() == _any.get_type_kind())
					return rhs;
				else
					return bad;
			}
			else if(lhs.get_type_kind() == rhs.get_type_kind() && rhs.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
				return type_information(rhs.get_type_kind(), lhs.get_type_pure_kind(), lhs.get_type_class_kind());
			else
				return bad;
		}
		else if(b_kind == binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS || b_kind == binary_operation_kind::BINARY_OPERATION_NOT_EQUAL) {
			if(lhs == _int || lhs == _float || lhs == _nil) {
				if(rhs == _int || rhs == _float || rhs == _nil)
					return _boolean;
				else if(lhs.get_type_kind() == type_kind::TYPE_ANY || rhs.get_type_kind() == type_kind::TYPE_ANY)
					return _any;
				else
					return bad;
			}
			else if(lhs == _nil || rhs == _nil)
				return _boolean;
			else if(lhs.get_type_kind() == _any.get_type_kind() || rhs.get_type_kind() == _any.get_type_kind())
				return _any;
		}
		else {
			if(lhs == _int || lhs == _float) {
				if(rhs == _int || rhs == _float || rhs == _nil) {
					return lhs;
				}
				else if(rhs.get_type_kind() == _any.get_type_kind())
					return lhs;
				else
					return bad;
			}
			else if(lhs == _nil)
				return _nil;
			else if(rhs == _nil)
				return lhs;
			else if(lhs.get_type_kind() == _any.get_type_kind() || rhs.get_type_kind() == _any.get_type_kind())
				return lhs;
			else if(lhs == rhs)
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

	annotated_statement::annotated_statement(shared_ptr<annotated_root_node> arn, shared_ptr<statement> stmt, shared_ptr<annotated_binary_expression> abe,
			shared_ptr<annotated_declaration> adecl, type_information ti) : annotated_root_node(*arn), statement_pos(stmt->get_position()), t_inf(ti) {
		kind = stmt->get_statement_kind();
		b_expression = abe;
		decl = adecl;
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

	type_information annotated_statement::get_type_information() {
		return t_inf;
	}

	source_token_list::iterator annotated_statement::get_position() {
		return statement_pos;
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
}
