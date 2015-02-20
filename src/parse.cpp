#include "../includes/parse.hpp"
#include <algorithm>

using namespace karma_lang;

namespace karma_lang {

	source_token_list::iterator root_node::get_position() {
		return pos;
	}

	source_token_list::iterator root_node::set_position(source_token_list::iterator it) {
		pos = it;
		return pos;
	}

	root_node::root_node(shared_ptr<lexer> l, shared_ptr<diagnostics_reporter> dr) : parser(nullptr), pos(l->get_source_token_list()->begin()), lex(l), dreporter(dr) {

	}

	root_node::~root_node() {

	}

	vector<shared_ptr<statement>> root_node::get_statement_list() {
		return statement_list;
	}

	vector<shared_ptr<statement>> root_node::parse_program(shared_ptr<parser> p) {
		root = p->get_root_node();
		vector<shared_ptr<statement>> ret;
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end()) {
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE) {
				root->set_position(root->get_position() + 1);
				continue;
			}
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if(stmt->get_valid()) ret.push_back(stmt);
			else cout << "error.";
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE) root->set_position(root->get_position() + 1);
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE)
					root->set_position(root->get_position() + 1);
			}
		}
		lex = root->lex;
		dreporter = root->dreporter;
		pos = root->pos;
		statement_list = ret;
		root->statement_list = ret;
		return statement_list;
	}

	shared_ptr<lexer> root_node::get_lexer() {
		return lex;
	}

	shared_ptr<diagnostics_reporter> root_node::get_diagnostics_reporter() {
		return dreporter;
	}

	parser::parser(shared_ptr<root_node> r) : root(r) {

	}

	parser::~parser() {

	}

	shared_ptr<root_node> parser::get_root_node() {
		return root;
	}

	literal::literal(shared_ptr<root_node> r) : root_node(*r), literal_pos(r->get_position()) {
		valid = false;
		kind = literal_kind::LITERAL_NONE;
		raw_literal = nullptr;
	}

	literal::~literal() {

	}

	const bool literal::get_valid() {
		return valid;
	}

	const literal_kind literal::get_literal_kind() {
		return kind;
	}

	shared_ptr<token> literal::get_raw_literal() {
		return raw_literal;
	}

	source_token_list::iterator literal::get_position() {
		return literal_pos;
	}

	shared_ptr<literal> literal::parse_literal() {
		source_token_list::iterator save = root->get_position();
		literal_pos = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			kind = literal_kind::LITERAL_NONE;
			raw_literal = nullptr;
			return make_shared<literal>(*this);
		}
		else {
			if ((*save)->get_token_kind() == token_kind::TOKEN_STRING) {
				valid = true;
				kind = literal_kind::LITERAL_STRING;
				raw_literal = *root->get_position();
				root->set_position(root->get_position() + 1);
				return make_shared<literal>(*this);
			}
			else if ((*save)->get_token_kind() == token_kind::TOKEN_DECIMAL_NUMBER) {
				valid = true;
				kind = literal_kind::LITERAL_DECIMAL_NUMBER;
				raw_literal = *root->get_position();
				root->set_position(root->get_position() + 1);
				return make_shared<literal>(*this);
			}
			else if ((*save)->get_token_kind() == token_kind::TOKEN_REGULAR_NUMBER || (*save)->get_token_kind() == token_kind::TOKEN_HEX_NUMBER) {
				valid = true;
				kind = literal_kind::LITERAL_REGULAR_HEX_NUMBER;
				raw_literal = *root->get_position();
				root->set_position(root->get_position() + 1);
				return make_shared<literal>(*this);
			}
			else if ((*save)->get_token_kind() == token_kind::TOKEN_IDENTIFIER) {
				valid = true;
				kind = literal_kind::LITERAL_IDENTIFIER;
				raw_literal = *root->get_position();
				root->set_position(root->get_position() + 1);
				return make_shared<literal>(*this);
			}
			else if((*save)->get_token_kind() == token_kind::TOKEN_TRUE || (*save)->get_token_kind() == token_kind::TOKEN_FALSE) {
				valid = true;
				kind = literal_kind::LITERAL_BOOLEAN;
				raw_literal = *root->get_position();
				root->set_position(root->get_position() + 1);
				return make_shared<literal>(*this);
			}
			else if((*save)->get_token_kind() == token_kind::TOKEN_NIL) {
				valid = true;
				kind = literal_kind::LITERAL_NIL;
				raw_literal = *root->get_position();
				root->set_position(root->get_position() + 1);
				return make_shared<literal>(*this);
			}
		}
		valid = false;
		kind = literal_kind::LITERAL_NONE;
		raw_literal = nullptr;
		return make_shared<literal>(*this);
	}

	primary_expression::primary_expression(shared_ptr<root_node> r) : root_node(*r), primary_expression_pos(r->get_position()) {
		valid = false;
		kind = primary_expression_kind::PRIMARY_EXPRESSION_NONE;
		raw_literal = nullptr;
		raw_parenthesized_expression = nullptr;
		seq = nullptr;
	}

	primary_expression::~primary_expression() {

	}

	shared_ptr<dictionary> primary_expression::get_dictionary() {
		return dict;
	}

	shared_ptr<sequence> primary_expression::get_sequence() {
		return seq;
	}

	const bool primary_expression::get_valid() {
		return valid;
	}

	const primary_expression_kind primary_expression::get_primary_expression_kind() {
		return kind;
	}

	shared_ptr<literal> primary_expression::get_raw_literal() {
		return raw_literal;
	}

	source_token_list::iterator primary_expression::get_position() {
		return primary_expression_pos;
	}

	shared_ptr<binary_expression> primary_expression::get_raw_parenthesized_expression() {
		return raw_parenthesized_expression;
	}

	shared_ptr<primary_expression> primary_expression::parse_primary_expression() {
		source_token_list::iterator save = root->get_position();
		primary_expression_pos = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			kind = primary_expression_kind::PRIMARY_EXPRESSION_NONE;
			raw_literal = nullptr;
			raw_parenthesized_expression = nullptr;
			seq = nullptr;
			dict = nullptr;
			return make_shared<primary_expression>(*this);
		}
		else {
			if ((*save)->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS || (*save)->get_token_kind() == token_kind::TOKEN_OPEN_BRACKET) {
				root->set_position(root->get_position() + 1);
				source_token_list::iterator save1 = root->get_position();
				if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
					valid = false;
					kind = primary_expression_kind::PRIMARY_EXPRESSION_NONE;
					raw_literal = nullptr;
					raw_parenthesized_expression = nullptr;
					seq = nullptr;
					dict = nullptr;
					return make_shared<primary_expression>(*this);
				}
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				if (bexpr->get_valid()) {
					source_token_list::iterator save2 = root->get_position();
					if (save2 >= root->get_lexer()->get_source_token_list()->end()) {
						if((*save)->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS)
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_parenthesis, save2 - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						else
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_bracket, save2 - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, save2 - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
					if ((*save2)->get_token_kind() == token_kind::TOKEN_CLOSE_PARENTHESIS || (*save2)->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
						root->set_position(root->get_position() + 1);
						valid = true;
						if((*save)->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS)
							kind = primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION;
						else
							kind = primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION;
						raw_literal = nullptr;
						seq = nullptr;
						dict = nullptr;
						raw_parenthesized_expression = bexpr;
						return make_shared<primary_expression>(*this);
					}
					else {
						valid = false;
						if((*save)->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_parenthesis, save2, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							kind = primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION;
						}
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_bracket, save2, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							kind = primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION;
						}
						raw_literal = nullptr;
						seq = nullptr;
						dict = nullptr;
						raw_parenthesized_expression = bexpr;
						return make_shared<primary_expression>(*this);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					if((*save)->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS)
						kind = primary_expression_kind::PRIMARY_EXPRESSION_PARENTHESIZED_EXPRESSION;
					else
						kind = primary_expression_kind::PRIMARY_EXPRESSION_BRACKETED_EXPRESSION;
					raw_literal = nullptr;
					seq = nullptr;
					dict = nullptr;
					raw_parenthesized_expression = bexpr;
					return make_shared<primary_expression>(*this);
				}
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_TUPLE || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_LIST) {
				shared_ptr<sequence> seq = make_shared<sequence>(root)->parse_sequence();
				if(seq->get_valid())
					valid = true;
				else
					valid = false;
				kind = primary_expression_kind::PRIMARY_EXPRESSION_SEQUENCE;
				raw_literal = nullptr;
				raw_parenthesized_expression = nullptr;
				this->seq = seq;
				dict = nullptr;
				return make_shared<primary_expression>(*this);
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_DICT) {
				shared_ptr<dictionary> dict = make_shared<dictionary>(root)->parse_dictionary();
				if(dict->get_valid())
					valid = true;
				else
					valid = false;
				kind = primary_expression_kind::PRIMARY_EXPRESSION_DICTIONARY;
				raw_literal = nullptr;
				raw_parenthesized_expression = nullptr;
				this->dict = dict;
				return make_shared<primary_expression>(*this);
			}
			else {
				source_token_list::iterator save1 = root->get_position();
				if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
					valid = false;
					kind = primary_expression_kind::PRIMARY_EXPRESSION_NONE;
					raw_literal = nullptr;
					raw_parenthesized_expression = nullptr;
					seq = nullptr;
					dict = nullptr;
					return make_shared<primary_expression>(*this);
				}
				shared_ptr<literal> lit = make_shared<literal>(root)->parse_literal();
				if (lit->get_valid()) {
					valid = true;
					kind = primary_expression_kind::PRIMARY_EXPRESSION_LITERAL;
					raw_literal = lit;
					raw_parenthesized_expression = nullptr;
					seq = nullptr;
					dict = nullptr;
					return make_shared<primary_expression>(*this);
				}
				else {
					valid = false;
					kind = primary_expression_kind::PRIMARY_EXPRESSION_LITERAL;
					raw_literal = lit;
					seq = nullptr;
					raw_parenthesized_expression = nullptr;
					dict = nullptr;
					return make_shared<primary_expression>(*this);
				}
			}
		}
		valid = false;
		kind = primary_expression_kind::PRIMARY_EXPRESSION_NONE;
		raw_literal = nullptr;
		raw_parenthesized_expression = nullptr;
		seq = nullptr;
		dict = nullptr;
		return make_shared<primary_expression>(*this);
	}

	postfix_expression::postfix_expression(shared_ptr<root_node> r) : root_node(*r), postfix_expression_pos(r->get_position()) {
		valid = false;
		kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
		raw_primary_expression = nullptr;
		expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_NONE;
		argument_list = nullptr;
		post_expression = nullptr;
		identifier = nullptr;
		subscr = nullptr;
	}

	postfix_expression::~postfix_expression() {

	}

	shared_ptr<primary_expression> postfix_expression::get_raw_primary_expression() {
		return raw_primary_expression;
	}

	const bool postfix_expression::get_valid() {
		return valid;
	}

	const postfix_operation_kind postfix_expression::get_postfix_operation_kind() {
		return kind;
	}

	source_token_list::iterator postfix_expression::get_position() {
		return postfix_expression_pos;
	}

	const postfix_expression_kind postfix_expression::get_postfix_expression_kind() {
		return expression_kind;
	}

	shared_ptr<postfix_expression> postfix_expression::get_postfix_expression() {
		return post_expression;
	}

	shared_ptr<function_argument_list>  postfix_expression::get_argument_list() {
		return argument_list;
	}

	shared_ptr<literal> postfix_expression::get_identifier() {
		return identifier;
	}

	shared_ptr<subscript> postfix_expression::get_subscript() {
		return subscr;
	}

	shared_ptr<postfix_expression> postfix_expression::parse_postfix_expression() {
		source_token_list::iterator save = root->get_position();
		postfix_expression_pos = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
			raw_primary_expression = nullptr;
			expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_NONE;
			post_expression = nullptr;
			identifier = nullptr;
			argument_list = nullptr;
			subscr = nullptr;
			return make_shared<postfix_expression>(*this);
		}
		shared_ptr<primary_expression> pexpr = make_shared<primary_expression>(root)->parse_primary_expression();
		if (pexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
				raw_primary_expression = pexpr;
				expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_PRIMARY_EXPRESSION;
				post_expression = nullptr;
				identifier = nullptr;
				argument_list = nullptr;
				subscr = nullptr;
				return make_shared<postfix_expression>(*this);
			}
			if ((*save1)->get_token_kind() != token_kind::TOKEN_DOT && (*save1)->get_token_kind() != token_kind::TOKEN_INCREMENT && (*save1)->get_token_kind() != token_kind::TOKEN_DECREMENT && (*save1)->get_token_kind() != token_kind::TOKEN_OPEN_PARENTHESIS &&
					(*save1)->get_token_kind() != token_kind::TOKEN_OPEN_BRACKET) {
				valid = true;
				kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
				raw_primary_expression = pexpr;
				post_expression = nullptr;
				expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_PRIMARY_EXPRESSION;
				identifier = nullptr;
				argument_list = nullptr;
				subscr = nullptr;
				return make_shared<postfix_expression>(*this);
			}
			shared_ptr<postfix_expression> post_expr = make_shared<postfix_expression>(root);
			post_expr->valid = true;
			post_expr->kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
			post_expr->raw_primary_expression = pexpr;
			post_expr->expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_PRIMARY_EXPRESSION;
			post_expr->post_expression = nullptr;
			post_expr->postfix_expression_pos = postfix_expression_pos;
			post_expr->identifier = nullptr;
			post_expr->argument_list = nullptr;
			post_expr->subscr = nullptr;
			valid = true;
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_INCREMENT || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DECREMENT) {
				if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_INCREMENT)
					kind = postfix_operation_kind::POSTFIX_OPERATION_INCREMENT;
				else
					kind = postfix_operation_kind::POSTFIX_OPERATION_DECREMENT;
				root->set_position(root->get_position() + 1);
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS) {
				shared_ptr<function_argument_list> fa_list = make_shared<function_argument_list>(root)->parse_function_argument_list();
				kind = postfix_operation_kind::POSTFIX_FUNCTION_CALL;
				argument_list = fa_list;
				if(fa_list->get_valid());
				else {
					valid = false;
					return make_shared<postfix_expression>(*this);
				}
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DOT) {
				root->set_position(root->get_position() + 1);
				kind = postfix_operation_kind::POSTFIX_DOT_OPERATOR;
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<literal> ident = make_shared<literal>(root)->parse_literal();
				identifier = ident;
				if(ident->get_valid());
				else {
					valid = false;
					return make_shared<postfix_expression>(*this);
				}
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				shared_ptr<subscript> subscr = make_shared<subscript>(root)->parse_subscript();
				kind = postfix_operation_kind::POSTFIX_SUBSCRIPT;
				this->subscr = subscr;
				if(subscr->get_valid());
				else {
					valid = false;
					return make_shared<postfix_expression>(*this);
				}
			}
			post_expression = post_expr;
			expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_POSTFIX_EXPRESSION;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end()) {
				if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_INCREMENT || (*root->get_position())->get_token_kind() == token_kind::TOKEN_DECREMENT) {
					post_expression = make_shared<postfix_expression>(*this);
					if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_INCREMENT)
						kind = postfix_operation_kind::POSTFIX_OPERATION_INCREMENT;
					else
						kind = postfix_operation_kind::POSTFIX_OPERATION_DECREMENT;
					root->set_position(root->get_position() + 1);
				}
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS) {
					post_expression = make_shared<postfix_expression>(*this);
					kind = postfix_operation_kind::POSTFIX_FUNCTION_CALL;
					shared_ptr<function_argument_list> fa_list = make_shared<function_argument_list>(root)->parse_function_argument_list();
					argument_list = fa_list;
					if(fa_list->get_valid());
					else {
						valid = false;
						return make_shared<postfix_expression>(*this);
					}
				}
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DOT) {
					post_expression = make_shared<postfix_expression>(*this);
					kind = postfix_operation_kind::POSTFIX_DOT_OPERATOR;
					root->set_position(root->get_position() + 1);
					if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
					shared_ptr<literal> ident = make_shared<literal>(root)->parse_literal();
					identifier = ident;
					if(ident->get_valid());
					else {
						valid = false;
						return make_shared<postfix_expression>(*this);
					}
				}
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACKET) {
					post_expression = make_shared<postfix_expression>(*this);
					kind = postfix_operation_kind::POSTFIX_SUBSCRIPT;
					shared_ptr<subscript> subscr = make_shared<subscript>(root)->parse_subscript();
					this->subscr = subscr;
					if(subscr->get_valid());
					else {
						valid = false;
						return make_shared<postfix_expression>(*this);
					}
				}
				else break;
			}
			return make_shared<postfix_expression>(*this);
		}
		else {
			valid = false;
			kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
			raw_primary_expression = pexpr;
			expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_PRIMARY_EXPRESSION;
			post_expression = nullptr;
			identifier = nullptr;
			subscr = nullptr;
			return make_shared<postfix_expression>(*this);
		}
		valid = false;
		expression_kind = postfix_expression_kind::POSTFIX_EXPRESSION_NONE;
		raw_primary_expression = nullptr;
		kind = postfix_operation_kind::POSTFIX_OPERATION_NONE;
		post_expression = nullptr;
		identifier = nullptr;
		subscr = nullptr;
		return make_shared<postfix_expression>(*this);
	}

	unary_expression::unary_expression(shared_ptr<root_node> r) : root_node(*r), unary_expression_pos(r->get_position()) {
		valid = false;
		kind = unary_operation_kind::UNARY_OPERATION_NONE;
		raw_postfix_expression = nullptr;
	}

	unary_expression::~unary_expression() {

	}

	shared_ptr<postfix_expression> unary_expression::get_raw_postfix_expression() {
		return raw_postfix_expression;
	}

	const bool unary_expression::get_valid() {
		return valid;
	}

	const unary_operation_kind unary_expression::get_unary_operation_kind() {
		return kind;
	}

	source_token_list::iterator unary_expression::get_position() {
		return unary_expression_pos;
	}

	shared_ptr<unary_expression> unary_expression::parse_unary_expression() {
		source_token_list::iterator save = root->get_position();
		unary_expression_pos = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			raw_postfix_expression = nullptr;
			valid = false;
			kind = unary_operation_kind::UNARY_OPERATION_NONE;
			return make_shared<unary_expression>(*this);
		}
		switch ((*save)->get_token_kind()) {
			case token_kind::TOKEN_PLUS:
				kind = unary_operation_kind::UNARY_OPERATION_PLUS;
				root->set_position(root->get_position() + 1);
				break;
			case token_kind::TOKEN_MINUS:
				kind = unary_operation_kind::UNARY_OPERATION_MINUS;
				root->set_position(root->get_position() + 1);
				break;
			case token_kind::TOKEN_INCREMENT:
				kind = unary_operation_kind::UNARY_OPERATION_INCREMENT;
				root->set_position(root->get_position() + 1);
				break;
			case token_kind::TOKEN_DECREMENT:
				kind = unary_operation_kind::UNARY_OPERATION_DECREMENT;
				root->set_position(root->get_position() + 1);
				break;
			case token_kind::TOKEN_EXCLAMATION:
				kind = unary_operation_kind::UNARY_OPERATION_NOT;
				root->set_position(root->get_position() + 1);
				break;
			case token_kind::TOKEN_COMPLEMENT:
				kind = unary_operation_kind::UNARY_OPERATION_COMPLEMENT;
				root->set_position(root->get_position() + 1);
				break;
			default:
				kind = unary_operation_kind::UNARY_OPERATION_NONE;
				break;
		}
		source_token_list::iterator save2 = root->get_position();
		if (save2 >= root->get_lexer()->get_source_token_list()->end()) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, save2 - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			exit(0);
		}
		shared_ptr<postfix_expression> pexpr = make_shared<postfix_expression>(root)->parse_postfix_expression();
		if (pexpr->get_valid()) {
			valid = true;
			raw_postfix_expression = pexpr;
			return make_shared<unary_expression>(*this);
		}
		else {
			valid = false;
			raw_postfix_expression = pexpr;
			return make_shared<unary_expression>(*this);
		}
		root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, pexpr->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		valid = false;
		raw_postfix_expression = nullptr;
		return make_shared<unary_expression>(*this);
	}

	binary_expression::binary_expression(shared_ptr<root_node> r) : root_node(*r), binary_expression_pos(r->get_position()) {
		lhs = nullptr;
		rhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		valid = false;
	}

	binary_expression::~binary_expression() {

	}

	shared_ptr<root_node> binary_expression::get_rhs() {
		return rhs;
	}

	shared_ptr<root_node> binary_expression::get_lhs() {
		return lhs;
	}

	const binary_expression_kind binary_expression::get_lhs_kind() {
		return lhs_kind;
	}

	const binary_expression_kind binary_expression::get_rhs_kind() {
		return rhs_kind;
	}

	const binary_expression_kind binary_expression::get_expression_kind() {
		return expression_kind;
	}

	const bool binary_expression::get_valid() {
		return valid;
	}

	const binary_operation_kind binary_expression::get_binary_operation_kind() {
		return operation_kind;
	}

	source_token_list::iterator binary_expression::get_position() {
		return binary_expression_pos;
	}

	shared_ptr<binary_expression> binary_expression::exponent_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<unary_expression> uexpr = make_shared<unary_expression>(root)->parse_unary_expression();
		if (uexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end() || (save1 < root->get_lexer()->get_source_token_list()->end() && (*save1)->get_token_kind() != token_kind::TOKEN_EXPONENT)) {
				valid = true;
				lhs = uexpr;
				rhs = nullptr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save2 = root->get_position();
			if (save2 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, save2 - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			shared_ptr<binary_expression> bexpr = exponent_expression();
			if (bexpr->get_valid()) {
				valid = true;
				lhs = uexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				rhs = bexpr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_EXPONENT;
				return make_shared<binary_expression>(*this);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				lhs = uexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				rhs = bexpr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_EXPONENT;
				return make_shared<binary_expression>(*this);
			}
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_unary_expression, save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			valid = false;
			rhs = nullptr;
			lhs = uexpr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::multiplicative_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = exponent_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MULTIPLY || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DIVIDE ||
					(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MODULUS) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = exponent_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MULTIPLY || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DIVIDE ||
						(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MODULUS)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = exponent_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::additive_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = multiplicative_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_PLUS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MINUS) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = multiplicative_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_PLUS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MINUS)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = multiplicative_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::shift_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = additive_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = additive_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = additive_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::relational_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = shift_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN ||
					(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = shift_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN ||
						(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = shift_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::equality_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = relational_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NOT_EQUALS) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = relational_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = relational_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::and_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = equality_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_AND) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = relational_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_AND)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = equality_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::exclusive_or_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = and_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = and_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = exclusive_or_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::inclusive_or_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = exclusive_or_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_OR) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = exclusive_or_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_OR)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = exclusive_or_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::logical_and_expression() {
		source_token_list::iterator save = root->get_position();
		binary_expression_pos = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = inclusive_or_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_AND) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = inclusive_or_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = inclusive_or_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::logical_or_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = logical_and_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs = nullptr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_OR) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = logical_and_expression();
				if (exp_expr->get_valid()) {
					valid = true;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					lhs = bexpr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = exp_expr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)) {
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = logical_and_expression();
				if (bexpr1->get_valid()) {
					valid = true;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs = bexpr1;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					rhs = bexpr1;
					lhs = make_shared<binary_expression>(*this);
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			return make_shared<binary_expression>(*this);
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::conditional_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr1 = logical_or_expression();
		if (bexpr1->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				lhs = bexpr1;
				rhs = nullptr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if ((*save1)->get_token_kind() == token_kind::TOKEN_QUESTION_MARK) {
				root->set_position(root->get_position() + 1);
				source_token_list::iterator save2 = root->get_position();
				if (save2 >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr2 = make_shared<binary_expression>(root)->parse_binary_expression();
				if (bexpr2->get_valid()) {
					shared_ptr<binary_expression> tmp = make_shared<binary_expression>(root);
					tmp->binary_expression_pos = binary_expression_pos;
					tmp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					tmp->lhs = bexpr1;
					tmp->rhs = bexpr2;
					tmp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					tmp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					tmp->operation_kind = binary_operation_kind::BINARY_OPERATION_QUESTION_MARK;
					tmp->valid = true;
					source_token_list::iterator save3 = root->get_position();
					if (save3 >= root->get_lexer()->get_source_token_list()->end()) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
					if ((*save3)->get_token_kind() == token_kind::TOKEN_COLON) {
						root->set_position(root->get_position() + 1);
						source_token_list::iterator save4 = root->get_position();
						if (save4 >= root->get_lexer()->get_source_token_list()->end()) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							exit(0);
						}
						shared_ptr<binary_expression> bexpr3 = conditional_expression();
						expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
						operation_kind = binary_operation_kind::BINARY_OPERATION_COLON;
						rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
						lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
						lhs = tmp;
						rhs = bexpr3;
						valid = true;
						return make_shared<binary_expression>(*this);
					}
					else {
						valid = false;
						lhs = tmp;
						rhs = nullptr;
						lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
						rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
						operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
						expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
						return make_shared<binary_expression>(*this);
					}
				}
				else {
					shared_ptr<binary_expression> tmp = make_shared<binary_expression>(root);
					tmp->binary_expression_pos = binary_expression_pos;
					tmp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					tmp->lhs = bexpr1;
					tmp->rhs = bexpr2;
					tmp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					tmp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					tmp->operation_kind = binary_operation_kind::BINARY_OPERATION_QUESTION_MARK;
					tmp->valid = false;
					valid = false;
					lhs = tmp;
					rhs = nullptr;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					return make_shared<binary_expression>(*this);
				}
				source_token_list::iterator save3 = root->get_position();
				if (save3 >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
			}
			else {
				valid = true;
				lhs = bexpr1;
				rhs = nullptr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
		}
		else {
			valid = false;
			rhs = nullptr;
			lhs = bexpr1;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::assignment_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			lhs = nullptr;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<unary_expression> uexpr = make_shared<unary_expression>(root)->parse_unary_expression();
		if (uexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				valid = true;
				rhs = nullptr;
				lhs = uexpr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			if (((*save1)->get_token_kind() == token_kind::TOKEN_EXPONENT_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_PLUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MINUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MULTIPLY_EQUALS ||
						(*save1)->get_token_kind() == token_kind::TOKEN_DIVIDE_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MODULUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT_EQUALS ||
						(*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_AND_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_OR_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_EQUALS));
			else if((*save1)->get_token_kind() == token_kind::TOKEN_NEW_LINE) {
				valid = true;
				rhs = nullptr;
				lhs = uexpr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			else {
				root->set_position(save);
				shared_ptr<binary_expression> bexpr = conditional_expression();
				if (bexpr->get_valid()) {
					valid = true;
					rhs = nullptr;
					lhs = bexpr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					return make_shared<binary_expression>(*this);
				}
				else {
					valid = false;
					rhs = nullptr;
					lhs = bexpr;
					rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					return make_shared<binary_expression>(*this);
				}
			}
			if ((*save1)->get_token_kind() == token_kind::TOKEN_EXPONENT_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_PLUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MINUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MULTIPLY_EQUALS ||
					(*save1)->get_token_kind() == token_kind::TOKEN_DIVIDE_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MODULUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT_EQUALS ||
					(*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_AND_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_OR_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_EQUALS) {
				source_token_list::iterator save2 = root->get_position();
				root->set_position(root->get_position() + 1);
				source_token_list::iterator save3 = root->get_position();
				if (save3 >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr = assignment_expression();
				valid = true;
				lhs = uexpr;
				rhs = bexpr;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				if ((*save2)->get_token_kind() == token_kind::TOKEN_EXPONENT_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_EXPONENT_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_PLUS_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_MINUS_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_MULTIPLY_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_DIVIDE_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_MODULUS_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_BITWISE_AND_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_BITWISE_OR_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_EQUALS)
					operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS;
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				return make_shared<binary_expression>(*this);
			}
		}
		else {
			root->set_position(save);
			shared_ptr<binary_expression> bexpr = conditional_expression();
			if (bexpr->get_valid()) {
				valid = true;
				rhs = nullptr;
				lhs = bexpr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
			else {
				valid = false;
				rhs = nullptr;
				lhs = bexpr;
				rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				return make_shared<binary_expression>(*this);
			}
		}
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::cast_expression() {
		source_token_list::iterator save = root->get_position();
		binary_expression_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			rhs = nullptr;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		shared_ptr<binary_expression> bexpr = assignment_expression();
		lhs = bexpr;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
		if(root->get_position() >= root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_POINT) {
			rhs = nullptr;
			valid = bexpr->get_valid();
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			return make_shared<binary_expression>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT) {
			root->set_position(root->get_position() + 1);
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			shared_ptr<binary_expression> bexpr = assignment_expression();
			rhs = bexpr;
			rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
			valid = bexpr->get_valid();
			operation_kind = binary_operation_kind::BINARY_OPERATION_POINT;
			return make_shared<binary_expression>(*this);
		}
		root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_point_to, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		valid = false;
		rhs = nullptr;
		lhs = nullptr;
		rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		return make_shared<binary_expression>(*this);
	}

	shared_ptr<binary_expression> binary_expression::parse_binary_expression() {
		return cast_expression();
	}

	sequence::sequence(shared_ptr<root_node> r) : root_node(*r), sequence_pos(r->get_position()) {
		_sequence  = sequence_kind::SEQUENCE_NONE;
		valid = false;
	}

	sequence::~sequence() {

	}

	const bool sequence::get_valid() {
		return valid;
	}

	source_token_list::iterator sequence::get_position() {
		return sequence_pos;
	}

	const sequence_kind sequence::get_sequence_kind() {
		return _sequence;
	}

	vector<shared_ptr<binary_expression>> sequence::get_expression_list() {
		return binary_expression_list;
	}

	shared_ptr<sequence> sequence::parse_sequence() {
		source_token_list::iterator save = root->get_position();
		sequence_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			_sequence = sequence_kind::SEQUENCE_NONE;
			return make_shared<sequence>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_TUPLE) {
			_sequence = sequence_kind::SEQUENCE_TUPLE;
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save1 = root->get_position();
			if(save1 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_TUPLE) {
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<sequence>(*this);
			}
			shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
			binary_expression_list.push_back(bexpr);
			if(bexpr->get_valid());
			else {
				valid = false;
				return make_shared<sequence>(*this);
			}
			while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA) {
				root->set_position(root->get_position() + 1);
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				binary_expression_list.push_back(bexpr);
				if(bexpr->get_valid());
				else {
					valid = false;
					return make_shared<sequence>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_tuple, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_TUPLE) {
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<sequence>(*this);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_tuple, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				return make_shared<sequence>(*this);
			}
		}
		else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_LIST) {
			_sequence = sequence_kind::SEQUENCE_LIST;
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save1 = root->get_position();
			if(save1 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_LIST) {
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<sequence>(*this);
			}
			shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
			binary_expression_list.push_back(bexpr);
			if(bexpr->get_valid());
			else {
				valid = false;
				return make_shared<sequence>(*this);
			}
			while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA) {
				root->set_position(root->get_position() + 1);
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				binary_expression_list.push_back(bexpr);
				if(bexpr->get_valid());
				else {
					valid = false;
					return make_shared<sequence>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_list, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_LIST) {
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<sequence>(*this);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_list, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				return make_shared<sequence>(*this);
			}
		}
		valid = false;
		_sequence = sequence_kind::SEQUENCE_NONE;
		binary_expression_list.clear();
		return make_shared<sequence>(*this);
	}

	dictionary::dictionary(shared_ptr<root_node> r) : root_node(*r), dictionary_pos(r->get_position()) {
		valid = false;
	}

	dictionary::~dictionary() {

	}

	const bool dictionary::get_valid() {
		return valid;
	}

	source_token_list::iterator dictionary::get_position() {
		return dictionary_pos;
	}

	vector<shared_ptr<binary_expression>> dictionary::get_key_list() {
		return key_list;
	}

	vector<shared_ptr<binary_expression>> dictionary::get_value_list() {
		return value_list;
	}

	shared_ptr<dictionary> dictionary::parse_dictionary() {
		source_token_list::iterator save = root->get_position();
		dictionary_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			return make_shared<dictionary>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_DICT) {
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save1 = root->get_position();
			if(save1 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_DICT) {
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<dictionary>(*this);
			}
			shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
			key_list.push_back(bexpr);
			if(bexpr->get_valid());
			else {
				value_list.push_back(nullptr);
				valid = false;
				return make_shared<dictionary>(*this);
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_colon_to_separate_key_value_pairs, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_COLON) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_colon_to_separate_key_value_pairs, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				return make_shared<dictionary>(*this);
			}
			root->set_position(root->get_position() + 1);
			shared_ptr<binary_expression> bexpr2 = make_shared<binary_expression>(root)->parse_binary_expression();
			value_list.push_back(bexpr2);
			if(bexpr2->get_valid());
			else {
				valid = false;
				return make_shared<dictionary>(*this);
			}
			while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA) {
				root->set_position(root->get_position() + 1);
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				key_list.push_back(bexpr);
				if(bexpr->get_valid());
				else {
					value_list.push_back(nullptr);
					valid = false;
					return make_shared<dictionary>(*this);
				}
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_colon_to_separate_key_value_pairs, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				if((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_COLON) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_colon_to_separate_key_value_pairs, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					return make_shared<dictionary>(*this);
				}
				root->set_position(root->get_position() + 1);
				shared_ptr<binary_expression> bexpr2 = make_shared<binary_expression>(root)->parse_binary_expression();
				value_list.push_back(bexpr2);
				if(bexpr2->get_valid());
				else {
					valid = false;
					return make_shared<dictionary>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_colon_to_separate_key_value_pairs, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_DICT) {
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<dictionary>(*this);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_colon_to_separate_key_value_pairs, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				return make_shared<dictionary>(*this);
			}
		}
		valid = false;
		key_list.clear();
		value_list.clear();
		return make_shared<dictionary>(*this);
	}

	function_argument_list::function_argument_list(shared_ptr<root_node> r) : root_node(*r), function_argument_list_pos(r->get_position()) {
		valid = false;
	}

	function_argument_list::~function_argument_list() {

	}

	const bool function_argument_list::get_valid() {
		return valid;
	}

	source_token_list::iterator function_argument_list::get_position() {
		return function_argument_list_pos;
	}

	vector<shared_ptr<binary_expression>> function_argument_list::get_argument_list() {
		return argument_list;
	}

	shared_ptr<function_argument_list> function_argument_list::parse_function_argument_list() {
		source_token_list::iterator save = root->get_position();
		function_argument_list_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			return make_shared<function_argument_list>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS) {
			root->set_position(root->get_position() + 1);
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_PARENTHESIS) {
				valid = true;
				root->set_position(root->get_position() + 1);
				return make_shared<function_argument_list>(*this);
			}
			else {
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				argument_list.push_back(bexpr);
				if(bexpr->get_valid());
				else {
					valid = false;
					return make_shared<function_argument_list>(*this);
				}
				while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA) {
					root->set_position(root->get_position() + 1);
					if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
					shared_ptr<binary_expression> bexpr1 = make_shared<binary_expression>(root)->parse_binary_expression();
					argument_list.push_back(bexpr1);
					if(bexpr1->get_valid());
					else {
						valid = false;
						return make_shared<function_argument_list>(*this);
					}
				}
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_PARENTHESIS) {
					root->set_position(root->get_position() + 1);
					valid = true;
					return make_shared<function_argument_list>(*this);
				}
				else {
					valid = false;
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_parenthesis, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					return make_shared<function_argument_list>(*this);
				}
			}
		}
		valid = false;
		argument_list.clear();
		return make_shared<function_argument_list>(*this);
	}

	subscript::subscript(shared_ptr<root_node> r) : root_node(*r), subscript_pos(r->get_position()) {
		valid = false;
		start = nullptr;
		done = nullptr;
		step = nullptr;
	}

	subscript::~subscript() {

	}

	shared_ptr<binary_expression> subscript::get_start() {
		return start;
	}

	shared_ptr<binary_expression> subscript::get_final() {
		return done;
	}

	shared_ptr<binary_expression> subscript::get_step() {
		return step;
	}

	source_token_list::iterator subscript::get_position() {
		return subscript_pos;
	}

	const bool subscript::get_valid() {
		return valid;
	}

	shared_ptr<subscript> subscript::parse_subscript() {
		source_token_list::iterator save = root->get_position();
		subscript_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			start = nullptr;
			done = nullptr;
			step = nullptr;
			return make_shared<subscript>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACKET) {
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save1 = root->get_position();
			if(save1 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_bracket, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
				root->set_position(root->get_position() + 1);
				start = nullptr;
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_expression_defaulting_to_whole_sequence, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				root->set_position(root->get_position() + 1);
				valid = true;
				start = nullptr;
				done = nullptr;
				step = nullptr;
				return make_shared<subscript>(*this);
			}
			else {
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				start = bexpr;
				if(bexpr->get_valid());
				else {
					done = nullptr;
					step = nullptr;
					valid = false;
					return make_shared<subscript>(*this);
				}
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON)
					root->set_position(root->get_position() + 1);
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
					root->set_position(root->get_position() + 1);
					valid = true;
					done = nullptr;
					step = nullptr;
					return make_shared<subscript>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				done = nullptr;
				step = nullptr;
				return make_shared<subscript>(*this);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
				root->set_position(root->get_position() + 1);
				done = nullptr;
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				root->set_position(root->get_position() + 1);
				valid = true;
				done = nullptr;
				step = nullptr;
				return make_shared<subscript>(*this);
			}
			else {
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				done = bexpr;
				if(bexpr->get_valid());
				else {
					step = nullptr;
					valid = false;
					return make_shared<subscript>(*this);
				}
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON)
					root->set_position(root->get_position() + 1);
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
					root->set_position(root->get_position() + 1);
					valid = true;
					step = nullptr;
					return make_shared<subscript>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				step = nullptr;
				valid = false;
				return make_shared<subscript>(*this);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				step = nullptr;
				root->get_diagnostics_reporter()->print(diagnostic_messages::explicit_empty_step_size, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				root->set_position(root->get_position() + 1);
				valid = true;
				return make_shared<subscript>(*this);
			}
			else {
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				step = bexpr;
				if(bexpr->get_valid());
				else {
					valid = false;
					return make_shared<subscript>(*this);
				}
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
					valid = true;
					root->set_position(root->get_position() + 1);
					return make_shared<subscript>(*this);
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_bracket, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					return make_shared<subscript>(*this);
				}
			}
		}
		valid = false;
		step = nullptr;
		done = nullptr;
		start = nullptr;
		return make_shared<subscript>(*this);
	}

	declspec_list::declspec_list(shared_ptr<root_node> r) : root_node(*r), declspec_list_pos(r->get_position()) {
		valid = false;
	}

	declspec_list::~declspec_list() {

	}

	const bool declspec_list::get_valid() {
		return valid;
	}

	source_token_list::iterator declspec_list::get_position() {
		return declspec_list_pos;
	}

	vector<declspec_kind> declspec_list::get_declspecs_list() {
		return delsp_list;
	}

	shared_ptr<declspec_list> declspec_list::parse_declspec_list() {
		source_token_list::iterator save = root->get_position();
		declspec_list_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			return make_shared<declspec_list>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_IMMUT) {
			delsp_list.push_back(declspec_kind::DECLSPECS_IMMUT);
			root->set_position(root->get_position() + 1);
		}
		else {
			valid = true;
			return make_shared<declspec_list>(*this);
		}
		while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA) {
			root->set_position(root->get_position() + 1);
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_IMMUT)
				delsp_list.push_back(declspec_kind::DECLSPECS_IMMUT);
			root->set_position(root->get_position() + 1);
		}
		valid = true;
		return make_shared<declspec_list>(*this);
	}

	declaration::declaration(shared_ptr<root_node> r) : root_node(*r), declaration_pos(r->get_position()) {
		valid = false;
		identifier = nullptr;
		delsp_list = nullptr;
		expr_statement = nullptr;
	}

	declaration::~declaration() {

	}

	const bool declaration::get_valid() {
		return valid;
	}

	shared_ptr<literal> declaration::get_identifier() {
		return identifier;
	}

	shared_ptr<binary_expression> declaration::get_binary_expression() {
		return expr_statement;
	}

	source_token_list::iterator declaration::get_position() {
		return declaration_pos;
	}

	shared_ptr<declaration> declaration::parse_declaration() {
		source_token_list::iterator save = root->get_position();
		declaration_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			identifier = nullptr;
			expr_statement = nullptr;
			delsp_list = nullptr;
			return make_shared<declaration>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_VAR) {
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save1 = root->get_position();
			if(save1 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				identifier = nullptr;
				expr_statement = nullptr;
				delsp_list = nullptr;
				return make_shared<declaration>(*this);
			}
			shared_ptr<declspec_list> dl = make_shared<declspec_list>(root)->parse_declspec_list();
			delsp_list = dl;
			if(dl->get_valid());
			else {
				valid = false;
				identifier = nullptr;
				expr_statement = nullptr;
				return make_shared<declaration>(*this);
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_POINT) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_point_to, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				identifier = nullptr;
				expr_statement = nullptr;
				return make_shared<declaration>(*this);
			}
			root->set_position(root->get_position() + 1);
			shared_ptr<literal> ident = make_shared<literal>(root)->parse_literal();
			identifier = ident;
			if(ident->get_valid());
			else {
				valid = false;
				expr_statement = nullptr;
				return make_shared<declaration>(*this);
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_EQUALS) {
				root->set_position(root->get_position() + 1);
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				expr_statement = bexpr;
				valid = bexpr->get_valid();
				return make_shared<declaration>(*this);
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_equals, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				expr_statement = nullptr;
				return make_shared<declaration>(*this);
			}
		}
		valid = false;
		identifier = nullptr;
		expr_statement = nullptr;
		delsp_list = nullptr;
		return make_shared<declaration>(*this);
	}

	statement::statement(shared_ptr<root_node> r) : root_node(*r), statement_pos(r->get_position()) {
		valid = false;
		decl = nullptr;
		b_expression = nullptr;
		kind = statement_kind::STATEMENT_NONE;
	}

	statement::~statement() {

	}

	const bool statement::get_valid() {
		return valid;
	}

	const statement_kind statement::get_statement_kind() {
		return kind;
	}

	shared_ptr<declaration> statement::get_declaration() {
		return decl;
	}

	shared_ptr<binary_expression> statement::get_binary_expression() {
		return b_expression;
	}

	source_token_list::iterator statement::get_position() {
		return statement_pos;
	}

	shared_ptr<statement> statement::parse_statement() {
		source_token_list::iterator save = root->get_position();
		statement_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			kind = statement_kind::STATEMENT_NONE;
			valid = false;
			decl = nullptr;
			b_expression = nullptr;
			return make_shared<statement>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_VAR) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_DECLARATION;
			decl = make_shared<declaration>(root)->parse_declaration();
			valid = decl->get_valid();
			return make_shared<statement>(*this);
		}
		else {
			decl = nullptr;
			kind = statement_kind::STATEMENT_EXPRESSION;
			b_expression = make_shared<binary_expression>(root)->parse_binary_expression();
			valid = b_expression->get_valid();
			return make_shared<statement>(*this);
		}
		decl = nullptr;
		kind = statement_kind::STATEMENT_NONE;
		b_expression = nullptr;
		valid = false;
		root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		exit(0);
		return make_shared<statement>(*this);
	}
}