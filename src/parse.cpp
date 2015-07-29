#include "../includes/parse.hpp"

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
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE) root->set_position(root->get_position() + 1);
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				while(root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE)
					root->set_position(root->get_position() + 1);
			}
			if (!stmt->get_valid()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::error_parsing_statement, root->get_position() < root->get_lexer()->get_source_token_list()->end() ? root->get_position() :
					root->get_lexer()->get_source_token_list()->end() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
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

	literal::literal(shared_ptr<root_node> r, shared_ptr<token> tok) : root_node(*r), literal_pos(r->get_lexer()->get_source_token_list()->end()) {
		valid = true;
		kind = literal_kind::LITERAL_IDENTIFIER;
		raw_literal = tok;
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
		lambda = nullptr;
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

	shared_ptr<function> primary_expression::get_lambda() {
		return lambda;
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
			else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LAMBDA) {
				lambda = make_shared<function>(root)->parse_function(true);
				valid = lambda->get_valid();
				raw_literal = nullptr;
				raw_parenthesized_expression = nullptr;
				kind = primary_expression_kind::PRIMARY_EXPRESSION_LAMBDA;
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
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACKET) {
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

	ternary_expression::ternary_expression(shared_ptr<root_node> r) : binary_expression(r), ternary_expression_pos(r->get_position()) {
		condition = nullptr;
		true_path = nullptr;
		false_path = nullptr;
		valid = false;
	}

	ternary_expression::~ternary_expression() {

	}

	shared_ptr<root_node> ternary_expression::get_condition() {
		return condition;
	}

	shared_ptr<root_node> ternary_expression::get_true_path() {
		return true_path;
	}

	shared_ptr<root_node> ternary_expression::get_false_path() {
		return false_path;
	}

	const bool ternary_expression::get_valid() {
		return valid;
	}

	source_token_list::iterator ternary_expression::get_position() {
		return ternary_expression_pos;
	}

	shared_ptr<ternary_expression> ternary_expression::parse_ternary_expression() {
		source_token_list::iterator save = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
			ret->condition = nullptr;
			ret->true_path = nullptr;
			ret->false_path = nullptr;
			ret->valid = false;
			return ret;
		}
		shared_ptr<binary_expression> bexpr1 = binary_expression::logical_or_expression();
		if(bexpr1->get_valid()) {
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
				ret->condition = nullptr;
				ret->true_path = nullptr;
				ret->false_path = nullptr;
				ret->valid = false;
				return ret;
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_QUESTION_MARK) {
				root->set_position(root->get_position() + 1);
				shared_ptr<binary_expression> bexpr2 = make_shared<binary_expression>(root)->parse_binary_expression();
				if(bexpr2->get_valid()) {
					if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
						root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(1);
					}
					if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
						root->set_position(root->get_position() + 1);
						if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
							root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							exit(1);
						}
						shared_ptr<binary_expression> bexpr3 = binary_expression::conditional_expression();
						if(bexpr3->get_valid()) {
							shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
							ret->condition = bexpr1;
							ret->true_path = bexpr2;
							ret->false_path = bexpr3;
							ret->valid = true;
							return ret;
						}
						else {
							root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
							shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
							ret->condition = nullptr;
							ret->true_path = nullptr;
							ret->false_path = nullptr;
							ret->valid = false;
							return ret;
						}
					}
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
						ret->condition = nullptr;
						ret->true_path = nullptr;
						ret->false_path = nullptr;
						ret->valid = false;
						return ret;
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_ternary_expression, save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
					ret->condition = nullptr;
					ret->true_path = nullptr;
					ret->false_path = nullptr;
					ret->valid = false;
					return ret;
				}
			}
			else {
				shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
				ret->condition = nullptr;
				ret->true_path = nullptr;
				ret->false_path = nullptr;
				ret->valid = false;
				return ret;
			}
		}
		else {
			shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
			ret->condition = nullptr;
			ret->true_path = nullptr;
			ret->false_path = nullptr;
			ret->valid = false;
			return ret;
		}
		shared_ptr<ternary_expression> ret = make_shared<ternary_expression>(root);
		ret->condition = nullptr;
		ret->true_path = nullptr;
		ret->false_path = nullptr;
		ret->valid = false;
		return ret;
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
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<unary_expression> uexpr = make_shared<unary_expression>(root)->parse_unary_expression();
		if (uexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end() || (save1 < root->get_lexer()->get_source_token_list()->end() && (*save1)->get_token_kind() != token_kind::TOKEN_EXPONENT)) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = uexpr;
				ret->rhs = nullptr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			root->set_position(root->get_position() + 1);
			source_token_list::iterator save2 = root->get_position();
			if (save2 >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, save2 - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			shared_ptr<binary_expression> bexpr = exponent_expression();
			if (bexpr->get_valid()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = uexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				ret->rhs = bexpr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_EXPONENT;
				ret->binary_expression_pos = save;
				return ret;
			}
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = false;
				ret->lhs = uexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				ret->rhs = bexpr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_EXPONENT;
				ret->binary_expression_pos = save;
				return ret;
			}
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_unary_expression, save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = uexpr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::multiplicative_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = exponent_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MULTIPLY || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DIVIDE ||
					(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MODULUS) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = exponent_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MULTIPLY || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DIVIDE ||
						(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MODULUS)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = exponent_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MULTIPLY)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DIVIDE)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MODULUS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::additive_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = multiplicative_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_PLUS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MINUS) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = multiplicative_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_PLUS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MINUS)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = multiplicative_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_PLUS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_MINUS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::shift_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = additive_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = additive_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = additive_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::relational_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = shift_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN ||
					(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = shift_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN ||
						(*(root->get_position()))->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = shift_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LESS_THAN_OR_EQUAL_TO;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_GREATER_THAN_OR_EQUAL_TO;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, loop_save, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::equality_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = relational_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NOT_EQUALS) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = relational_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = relational_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_DOUBLE_EQUALS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS_EQUALS;
					else if ((*loop_save)->get_token_kind() == token_kind::TOKEN_NOT_EQUALS)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_NOT_EQUAL;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::and_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = equality_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_AND) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = relational_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_AND)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = equality_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_AND)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::exclusive_or_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = and_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = and_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = exclusive_or_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::inclusive_or_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = exclusive_or_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_OR) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = exclusive_or_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BITWISE_OR)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = exclusive_or_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_BITWISE_OR)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::logical_and_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = inclusive_or_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_AND) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = inclusive_or_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = inclusive_or_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_AND)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_AND;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::logical_or_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = logical_and_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			bool first_check = false;
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_OR) {
				first_check = true;
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = logical_and_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->binary_expression_pos = bexpr->get_position();
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
			}
			if(!first_check)
				return bexpr;
			shared_ptr<binary_expression> temp2 = make_shared<binary_expression>(root);
			bool second_check = false;
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)) {
				second_check = true;
				source_token_list::iterator loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> bexpr1 = logical_and_expression();
				if (bexpr1->get_valid()) {
					temp2->valid = true;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs = bexpr1;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp2->valid = false;
					temp2->rhs = bexpr1;
					temp2->lhs = temp;
					temp2->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp2->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp2->binary_expression_pos = temp->get_position();
					if ((*loop_save)->get_token_kind() == token_kind::TOKEN_LOGICAL_OR)
						temp2->operation_kind = binary_operation_kind::BINARY_OPERATION_LOGICAL_OR;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp = make_shared<binary_expression>(root);
				temp->valid = temp2->valid;
				temp->rhs = temp2->rhs;
				temp->lhs = temp2->lhs;
				temp->lhs_kind = temp2->lhs_kind;
				temp->rhs_kind = temp2->rhs_kind;
				temp->operation_kind = temp2->operation_kind;
				temp->expression_kind = temp2->expression_kind;
				temp->binary_expression_pos = temp2->binary_expression_pos;
			}
			if(!second_check)
				return temp;
			return temp2;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::conditional_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr1 = logical_or_expression();
		if (bexpr1->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr1;
				ret->rhs = nullptr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			if ((*save1)->get_token_kind() == token_kind::TOKEN_QUESTION_MARK) {
				root->set_position(save);
				shared_ptr<ternary_expression> tern = make_shared<ternary_expression>(root)->parse_ternary_expression();
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = tern->get_valid();
				ret->lhs = tern;
				ret->rhs = nullptr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_TERNARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = save;
				return ret;
			}
			else {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr1;
				ret->rhs = nullptr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = bexpr1->get_position();
				return ret;
			}
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = bexpr1;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = bexpr1->get_position();
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::assignment_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<unary_expression> uexpr = make_shared<unary_expression>(root)->parse_unary_expression();
		if (uexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				root->set_position(save);
				shared_ptr<binary_expression> bexpr = conditional_expression();
				if (bexpr->get_valid()) {
					temp->valid = true;
					temp->rhs = nullptr;
					temp->lhs = bexpr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					temp->binary_expression_pos = bexpr->get_position();
					return temp;
				}
				else {
					temp->valid = false;
					temp->rhs = nullptr;
					temp->lhs = bexpr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					temp->binary_expression_pos = bexpr->get_position();
					return temp;
				}
			}
			if (((*save1)->get_token_kind() == token_kind::TOKEN_EXPONENT_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_PLUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MINUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MULTIPLY_EQUALS ||
						(*save1)->get_token_kind() == token_kind::TOKEN_DIVIDE_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_MODULUS_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT_EQUALS ||
						(*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_AND_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_OR_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS || (*save1)->get_token_kind() == token_kind::TOKEN_EQUALS));
			else if((*save1)->get_token_kind() == token_kind::TOKEN_NEW_LINE) {
				temp->valid = true;
				temp->rhs = nullptr;
				temp->lhs = uexpr;
				temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				temp->binary_expression_pos = uexpr->get_position();
				return temp;
			}
			else {
				root->set_position(save);
				shared_ptr<binary_expression> bexpr = conditional_expression();
				if (bexpr->get_valid()) {
					temp->valid = true;
					temp->rhs = nullptr;
					temp->lhs = bexpr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					temp->binary_expression_pos = bexpr->get_position();
					return temp;
				}
				else {
					temp->valid = false;
					temp->rhs = nullptr;
					temp->lhs = bexpr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
					temp->binary_expression_pos = bexpr->get_position();
					return temp;
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
				temp->lhs = uexpr;
				temp->rhs = bexpr;
				temp->valid = uexpr->get_valid() && bexpr->get_valid();
				temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_UNARY_EXPRESSION;
				temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				if ((*save2)->get_token_kind() == token_kind::TOKEN_EXPONENT_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EXPONENT_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_PLUS_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_PLUS_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_MINUS_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MINUS_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_MULTIPLY_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MULTIPLY_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_DIVIDE_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_DIVIDE_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_MODULUS_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_MODULUS_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_SHIFT_LEFT_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_LEFT_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_SHIFT_RIGHT_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_SHIFT_RIGHT_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_BITWISE_AND_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_AND_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_BITWISE_OR_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_BITWISE_OR_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EXCLUSIVE_OR_EQUALS;
				else if ((*save2)->get_token_kind() == token_kind::TOKEN_EQUALS)
					temp->operation_kind = binary_operation_kind::BINARY_OPERATION_EQUALS;
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				temp->binary_expression_pos = uexpr->get_position();
				return temp;
			}
		}
		else {
			root->set_position(save);
			shared_ptr<binary_expression> bexpr = conditional_expression();
			if (bexpr->get_valid()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->rhs = nullptr;
				ret->lhs = bexpr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->pos = bexpr->get_position();
				return ret;
			}
			else {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = false;
				ret->rhs = nullptr;
				ret->lhs = bexpr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = bexpr->get_position();
				return ret;
			}
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::cast_expression() {
		source_token_list::iterator save = root->get_position();
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> bexpr = assignment_expression();
		if (bexpr->get_valid()) {
			source_token_list::iterator save1 = root->get_position();
			if (save1 >= root->get_lexer()->get_source_token_list()->end()) {
				shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
				ret->valid = true;
				ret->lhs = bexpr;
				ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->rhs = nullptr;
				ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
				ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
				ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
				ret->binary_expression_pos = bexpr->get_position();
				return ret;
			}
			shared_ptr<binary_expression> temp = make_shared<binary_expression>(root);
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT) {
				source_token_list::iterator pre_loop_save = root->get_position();
				root->set_position(root->get_position() + 1);
				if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				shared_ptr<binary_expression> exp_expr = assignment_expression();
				if (exp_expr->get_valid()) {
					temp->valid = true;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_POINT)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_POINT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::malformed_binary_expression, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					temp->valid = false;
					temp->lhs = bexpr;
					temp->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->rhs = exp_expr;
					temp->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					temp->expression_kind = binary_expression_kind::BINARY_EXPRESSION_BINARY_EXPRESSION;
					if ((*pre_loop_save)->get_token_kind() == token_kind::TOKEN_POINT)
						temp->operation_kind = binary_operation_kind::BINARY_OPERATION_POINT;
					else {
						root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position() >= root->get_lexer()->get_source_token_list()->end() ? root->get_position() - 1 : root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
						exit(0);
					}
				}
				temp->binary_expression_pos = bexpr->get_position();
				return temp;
			}
			return bexpr;
		}
		else {
			shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
			ret->valid = false;
			ret->rhs = nullptr;
			ret->lhs = nullptr;
			ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
			ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
			ret->binary_expression_pos = save;
			return ret;
		}
		shared_ptr<binary_expression> ret = make_shared<binary_expression>(root);
		ret->valid = false;
		ret->rhs = nullptr;
		ret->lhs = nullptr;
		ret->lhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->rhs_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->expression_kind = binary_expression_kind::BINARY_EXPRESSION_NONE;
		ret->operation_kind = binary_operation_kind::BINARY_OPERATION_NONE;
		ret->binary_expression_pos = save;
		return ret;
	}

	shared_ptr<binary_expression> binary_expression::parse_binary_expression() {
		shared_ptr<binary_expression> bexpr = cast_expression();
		lhs = bexpr->lhs;
		rhs = bexpr->rhs;
		lhs_kind = bexpr->lhs_kind;
		rhs_kind = bexpr->rhs_kind;
		expression_kind = bexpr->expression_kind;
		operation_kind = bexpr->operation_kind;
		return bexpr;
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
		sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_NONE;
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

	const subscript_colon_kind subscript::get_subscript_colon_kind() {
		return sc_kind;
	}

	shared_ptr<subscript> subscript::parse_subscript() {
		source_token_list::iterator save = root->get_position();
		sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ZERO;
		subscript_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			start = nullptr;
			done = nullptr;
			step = nullptr;
			sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_NONE;
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
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ONE;
				root->set_position(root->get_position() + 1);
				start = nullptr;
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				root->set_position(root->get_position() + 1);
				valid = true;
				start = nullptr;
				done = nullptr;
				step = nullptr;
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ZERO;
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
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ZERO;
					return make_shared<subscript>(*this);
				}
				if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
					root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					exit(0);
				}
				if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ONE;
					root->set_position(root->get_position() + 1);
				}
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
					root->set_position(root->get_position() + 1);
					valid = true;
					done = nullptr;
					step = nullptr;
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ZERO;
					return make_shared<subscript>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				done = nullptr;
				step = nullptr;
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_NONE;
				return make_shared<subscript>(*this);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
				root->set_position(root->get_position() + 1);
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_TWO;
				done = nullptr;
			}
			else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				root->set_position(root->get_position() + 1);
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ONE;
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
				if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_TWO;
					root->set_position(root->get_position() + 1);
				}
				else if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
					root->set_position(root->get_position() + 1);
					valid = true;
					step = nullptr;
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_ONE;
					return make_shared<subscript>(*this);
				}
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				step = nullptr;
				valid = false;
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_TWO;
				return make_shared<subscript>(*this);
			}
			if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACKET) {
				step = nullptr;
				root->get_diagnostics_reporter()->print(diagnostic_messages::explicit_empty_step_size, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING);
				root->set_position(root->get_position() + 1);
				sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_TWO;
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
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_TWO;
					root->set_position(root->get_position() + 1);
					return make_shared<subscript>(*this);
				}
				else {
					root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_bracket, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
					valid = false;
					sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_TWO;
					return make_shared<subscript>(*this);
				}
			}
		}
		valid = false;
		step = nullptr;
		done = nullptr;
		start = nullptr;
		sc_kind = subscript_colon_kind::SUBSCRIPT_COLON_NONE;
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
		partial = false;
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

	shared_ptr<declspec_list> declaration::get_declspec_list() {
		return delsp_list;
	}

	const bool declaration::get_partial() {
		return partial;
	}

	shared_ptr<declaration> declaration::parse_declaration(bool partial, bool colon) {
		this->partial = partial;
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
			if((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_POINT_DECL) {
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
			if (partial) {
				valid = true;
				expr_statement = nullptr;
				return make_shared<declaration>(*this);
			}
			if(root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::unexpected_end_of_file, root->get_position() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				exit(0);
			}
			if(!colon && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_EQUALS) {
				root->set_position(root->get_position() + 1);
				shared_ptr<binary_expression> bexpr = make_shared<binary_expression>(root)->parse_binary_expression();
				expr_statement = bexpr;
				valid = bexpr->get_valid();
				return make_shared<declaration>(*this);
			}
			else if (colon && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COLON) {
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

	function::function(shared_ptr<root_node> r) : root_node(*r), function_pos(r->get_position()) {
		identifier = nullptr;
		parameter_list = vector<shared_ptr<declaration>>();
		statement_list = vector<shared_ptr<statement>>();
		delsp_list = nullptr;
		valid = false;
		f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
		fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
		l_kind = lambda_kind::LAMBA_NONE;
	}

	function::~function() {

	}

	shared_ptr<literal> function::get_identifier() {
		return identifier;
	}

	shared_ptr<declspec_list> function::get_declspec_list() {
		return delsp_list;
	}

	vector<shared_ptr<statement>> function::get_statement_list() {
		return statement_list;
	}

	vector<shared_ptr<declaration>> function::get_parameter_list() {
		return parameter_list;
	}

	const bool function::get_valid() {
		return valid;
	}

	source_token_list::iterator function::get_position() {
		return function_pos;
	}

	const function_declaration_definition_kind function::get_function_kind() {
		return f_kind;
	}

	const function_va_args_kind function::get_function_va_args_kind() {
		return fva_kind;
	}

	const lambda_kind function::get_lambda_kind() {
		return l_kind;
	}

	shared_ptr<function> function::parse_function(bool lambda) {
		source_token_list::iterator save = root->get_position();
		function_pos = root->get_position();
		if (save == root->get_lexer()->get_source_token_list()->end()) {
			statement_list = vector<shared_ptr<statement>>();
			parameter_list = vector<shared_ptr<declaration>>();
			identifier = nullptr;
			delsp_list = nullptr;
			valid = false;
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
			l_kind = lambda_kind::LAMBA_NONE;
			return make_shared<function>(*this);
		}
		if (lambda) {
			if ((*save)->get_token_kind() != token_kind::TOKEN_LAMBDA) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_func_keyword_to_open_a_function, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				statement_list = vector<shared_ptr<statement>>();
				parameter_list = vector<shared_ptr<declaration>>();
				identifier = nullptr;
				delsp_list = nullptr;
				valid = false;
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
				l_kind = lambda_kind::LAMBA_NONE;
				return make_shared<function>(*this);
			}
			l_kind = lambda_kind::LAMBDA_YES;
		}
		else {
			if ((*save)->get_token_kind() != token_kind::TOKEN_FUNC) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_func_keyword_to_open_a_function, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				statement_list = vector<shared_ptr<statement>>();
				parameter_list = vector<shared_ptr<declaration>>();
				identifier = nullptr;
				delsp_list = nullptr;
				valid = false;
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
				l_kind = lambda_kind::LAMBA_NONE;
				return make_shared<function>(*this);
			}
			l_kind = lambda_kind::LAMBDA_NO;
		}
		root->set_position(root->get_position() + 1);
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_func_keyword_to_open_a_function, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			statement_list = vector<shared_ptr<statement>>();
			parameter_list = vector<shared_ptr<declaration>>();
			identifier = nullptr;
			delsp_list = nullptr;
			valid = false;
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
			return make_shared<function>(*this);
		}
		shared_ptr<declspec_list> dl = make_shared<declspec_list>(root)->parse_declspec_list();
		if (dl->get_valid());
		else {
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			parameter_list = vector<shared_ptr<declaration>>();
			identifier = nullptr;
			delsp_list = nullptr;
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
			return make_shared<function>(*this);
		}
		delsp_list = dl;
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT_DECL);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_point_to, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			parameter_list = vector<shared_ptr<declaration>>();
			identifier = nullptr;
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
			return make_shared<function>(*this);
		}
		root->set_position(root->get_position() + 1);
		if (lambda) {
			identifier = nullptr;
		}
		else {
			shared_ptr<literal> ident = make_shared<literal>(root)->parse_literal();
			if (ident->get_valid());
			else {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_identifier, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				statement_list = vector<shared_ptr<statement>>();
				parameter_list = vector<shared_ptr<declaration>>();
				identifier = nullptr;
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
				return make_shared<function>(*this);
			}
			identifier = ident;
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_PARENTHESIS);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_open_parenthesis, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			parameter_list = vector<shared_ptr<declaration>>();
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
			return make_shared<function>(*this);
		}
		fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NO;
		bool process = true;
		bool skip = false;
		root->set_position(root->get_position() + 1);
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			parameter_list = vector<shared_ptr<declaration>>();
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
			return make_shared<function>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_VA_ARGS) {
			root->set_position(root->get_position() + 1);
			fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_YES;
			process = false;
		}
		if (process) {
			shared_ptr<declaration> decl = make_shared<declaration>(root)->parse_declaration(true, false);
			if (decl->get_valid());
			else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_PARENTHESIS)
				skip = true;
			else {
				valid = false;
				statement_list = vector<shared_ptr<statement>>();
				parameter_list = vector<shared_ptr<declaration>>();
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
				return make_shared<function>(*this);
			}
			if (!skip && decl->get_valid())
				parameter_list.push_back(decl);
		}
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA && !skip) {
			root->set_position(root->get_position() + 1);
			if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				valid = false;
				statement_list = vector<shared_ptr<statement>>();
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
				return make_shared<function>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_VA_ARGS) {
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_YES;
				root->set_position(root->get_position() + 1);
				break;
			}
			shared_ptr<declaration> decl = make_shared<declaration>(root)->parse_declaration(true, false);
			if (decl->get_valid());
			else {
				valid = false;
				statement_list = vector<shared_ptr<statement>>();
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				fva_kind = function_va_args_kind::FUNCTION_VA_ARGS_NONE;
				return make_shared<function>(*this);
			}
			parameter_list.push_back(decl);
		}
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_PARENTHESIS);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_open_parenthesis, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			return make_shared<function>(*this);
		}
		root->set_position(root->get_position() + 1);
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE) {
			valid = true;
			statement_list = vector<shared_ptr<statement>>();
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_FORWARD_DECLARATION;
			if (l_kind == lambda_kind::LAMBDA_YES) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::lambdas_cannot_be_partially_declared, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
			}
			return make_shared<function>(*this);
		}
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_an_open_brace, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			return make_shared<function>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() > root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if (stmt->get_valid());
			else {
				valid = false;
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				return make_shared<function>(*this);
			}
			if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				valid = false;
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				return make_shared<function>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
				return make_shared<function>(*this);
			}
			root->set_position(root->get_position() + 1);
			statement_list.push_back(stmt);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			f_kind = function_declaration_definition_kind::FUNCTION_KIND_NONE;
			return make_shared<function>(*this);
		}
		else
			root->set_position(root->get_position() + 1);
		valid = true;
		f_kind = function_declaration_definition_kind::FUNCTION_KIND_DEFINITION;
		return make_shared<function>(*this);
	}

	structure::structure(shared_ptr<root_node> r) : root_node(*r), structure_pos(r->get_position()) {
		identifier = nullptr;
		delsp_list = nullptr;
		declaration_list = vector<shared_ptr<declaration>>();
		valid = false;
		sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
	}

	structure::~structure() {

	}

	shared_ptr<literal> structure::get_identifier() {
		return identifier;
	}
	
	vector<shared_ptr<declaration>> structure::get_declaration_list() {
		return declaration_list;
	}

	const bool structure::get_valid() {
		return valid;
	}

	source_token_list::iterator structure::get_position() {
		return structure_pos;
	}

	const structure_declaration_definition_kind structure::get_structure_kind() {
		return sdd_kind;
	}

	shared_ptr<declspec_list> structure::get_declspec_list() {
		return delsp_list;
	}

	shared_ptr<structure> structure::parse_structure() {
		source_token_list::iterator save = root->get_position();
		structure_pos = root->get_position();
		if (save == root->get_lexer()->get_source_token_list()->end()) {
			identifier = nullptr;
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			delsp_list = nullptr;
			return make_shared<structure>(*this);
		}
		if ((*save)->get_token_kind() == token_kind::TOKEN_STRUCT);
		else {
			identifier = nullptr;
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			delsp_list = nullptr;
			return make_shared<structure>(*this);
		}
		root->set_position(root->get_position() + 1);
		delsp_list = make_shared<declspec_list>(root)->parse_declspec_list();
		if (delsp_list->get_valid());
		else {
			identifier = nullptr;
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			return make_shared<structure>(*this);
		}
		if (root->get_position() == root->get_lexer()->get_source_token_list()->end()) {
			identifier = nullptr;
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			return make_shared<structure>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT_DECL);
		else {
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_a_point_to, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			identifier = nullptr;
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			return make_shared<structure>(*this);
		}
		root->set_position(root->get_position() + 1);
		shared_ptr<literal> lit = make_shared<literal>(root)->parse_literal();
		if (lit->get_valid());
		else {
			identifier = nullptr;
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			return make_shared<structure>(*this);
		}
		identifier = lit;
		if (root->get_position() == root->get_lexer()->get_source_token_list()->end()) {
			declaration_list = vector<shared_ptr<declaration>>();
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			return make_shared<structure>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else {
			declaration_list = vector<shared_ptr<declaration>>();
			valid = true;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_FORWARD_DECLARATION;
			return make_shared<structure>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() > root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<declaration> decl = make_shared<declaration>(root)->parse_declaration(false, true);
			if (decl->get_valid());
			else {
				valid = false;
				sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
				return make_shared<structure>(*this);
			}
			if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				valid = false;
				sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
				return make_shared<structure>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
				return make_shared<structure>(*this);
			}
			root->set_position(root->get_position() + 1);
			declaration_list.push_back(decl);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_NONE;
			return make_shared<structure>(*this);
		}
		else
			root->set_position(root->get_position() + 1);
		valid = true;
		sdd_kind = structure_declaration_definition_kind::STRUCTURE_KIND_DEFINITION;
		return make_shared<structure>(*this);
	}

	module::module(shared_ptr<root_node> r) : root_node(*r), module_pos(root->get_position()) {
		identifier = nullptr;
		statement_list = vector<shared_ptr<statement>>();
		valid = false;
		delsp_list = nullptr;
	}

	module::~module() {

	}

	shared_ptr<literal> module::get_identifier() {
		return identifier;
	}

	vector<shared_ptr<statement>> module::get_statement_list() {
		return statement_list;
	}

	const bool module::get_valid() {
		return valid;
	}

	source_token_list::iterator module::get_position() {
		return module_pos;
	}

	shared_ptr<declspec_list> module::get_declspec_list() {
		return delsp_list;
	}

	shared_ptr<module> module::parse_module() {
		source_token_list::iterator save = root->get_position();
		module_pos = root->get_position();
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			identifier = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			delsp_list = nullptr;
			return make_shared<module>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MODULE);
		else {
			identifier = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			delsp_list = nullptr;
			return make_shared<module>(*this);
		}
		root->set_position(root->get_position() + 1);
		delsp_list = make_shared<declspec_list>(root)->parse_declspec_list();
		if (delsp_list->get_valid());
		else {
			identifier = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<module>(*this);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			identifier = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<module>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT_DECL);
		else {
			identifier = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<module>(*this);
		}
		root->set_position(root->get_position() + 1);
		identifier = make_shared<literal>(root)->parse_literal();
		if (!identifier->get_valid() || root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<module>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else {
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			root->get_diagnostics_reporter()->print(diagnostic_messages::partial_declarations_not_allowed, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<module>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() > root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if (stmt->get_valid());
			else {
				valid = false;
				return make_shared<module>(*this);
			}
			if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				valid = false;
				return make_shared<module>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				return make_shared<module>(*this);
			}
			root->set_position(root->get_position() + 1);
			statement_list.push_back(stmt);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			return make_shared<module>(*this);
		}
		else
			root->set_position(root->get_position() + 1);
		valid = true;
		return make_shared<module>(*this);
	}

	return_statement::return_statement(shared_ptr<root_node> r) : root_node(*r), return_statement_pos(r->get_position()) {
		valid = false;
		b_expression = nullptr;
		rs_kind = return_statement_kind::RETURN_STATEMENT_NONE;
	}

	return_statement::~return_statement() {

	}

	shared_ptr<binary_expression> return_statement::get_binary_expression() {
		return b_expression;
	}

	const bool return_statement::get_valid() {
		return valid;
	}

	source_token_list::iterator return_statement::get_position() {
		return return_statement_pos;
	}

	const return_statement_kind return_statement::get_return_statement_kind() {
		return rs_kind;
	}

	shared_ptr<return_statement> return_statement::parse_return_statement() {
		source_token_list::iterator save = root->get_position();
		return_statement_pos = save;
		if (save > root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			b_expression = nullptr;
			rs_kind = return_statement_kind::RETURN_STATEMENT_NONE;
			return make_shared<return_statement>(*this);
		}
		if ((*save)->get_token_kind() == token_kind::TOKEN_RETURN);
		else {
			valid = false;
			b_expression = nullptr;
			rs_kind = return_statement_kind::RETURN_STATEMENT_NONE;
			return make_shared<return_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		if (root->get_position() > root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			b_expression = nullptr;
			rs_kind = return_statement_kind::RETURN_STATEMENT_NONE;
			return make_shared<return_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE) {
			valid = true;
			b_expression = nullptr;
			rs_kind = return_statement_kind::RETURN_STATEMENT_EMPTY;
			return make_shared<return_statement>(*this);
		}
		b_expression = make_shared<binary_expression>(root)->parse_binary_expression();
		if (b_expression->get_valid());
		else {
			valid = false;
			rs_kind = return_statement_kind::RETURN_STATEMENT_NONE;
			return make_shared<return_statement>(*this);
		}
		valid = true;
		rs_kind = return_statement_kind::RETURN_STATEMENT_EXPRESSION;
		return make_shared<return_statement>(*this);
	}

	conditional_statement::conditional_statement(shared_ptr<root_node> r) : root_node(*r), conditional_statement_pos(r->get_position()) {
		if_conditional = nullptr;
		if_statement_list = vector<shared_ptr<statement>>();
		else_conditional = nullptr;
		else_statement_list = vector<shared_ptr<statement>>();
		valid = false;
		ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
		cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
	}

	conditional_statement::~conditional_statement() {

	}

	shared_ptr<binary_expression> conditional_statement::get_if_conditional() {
		return if_conditional;
	}

	vector<shared_ptr<statement>> conditional_statement::get_if_statement_list() {
		return if_statement_list;
	}

	shared_ptr<binary_expression> conditional_statement::get_else_conditional() {
		return else_conditional;
	}

	vector<shared_ptr<statement>> conditional_statement::get_else_statement_list() {
		return else_statement_list;
	}

	const bool conditional_statement::get_valid() {
		return valid;
	}

	source_token_list::iterator conditional_statement::get_position() {
		return conditional_statement_pos;
	}

	const conditional_else_statement_kind conditional_statement::get_conditional_else_statement_kind() {
		return ces_kind;
	}

	const conditional_else_conditional_kind conditional_statement::get_conditional_else_conditional_kind() {
		return cec_kind;
	}

	shared_ptr<conditional_statement> conditional_statement::parse_conditional_statement() {
		source_token_list::iterator save = root->get_position();
		conditional_statement_pos = save;
		if (save > root->get_lexer()->get_source_token_list()->end()) {
			if_conditional = nullptr;
			if_statement_list = vector<shared_ptr<statement>>();
			else_statement_list = vector<shared_ptr<statement>>();
			else_conditional = nullptr;
			valid = false;
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		if ((*save)->get_token_kind() == token_kind::TOKEN_IF);
		else {
			if_conditional = nullptr;
			else_conditional = nullptr;
			if_statement_list = vector<shared_ptr<statement>>();
			else_statement_list = vector<shared_ptr<statement>>();
			valid = false;
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		if_conditional = make_shared<binary_expression>(root)->parse_binary_expression();
		if (if_conditional->get_valid());
		else {
			else_conditional = nullptr;
			if_statement_list = vector<shared_ptr<statement>>();
			else_statement_list = vector<shared_ptr<statement>>();
			valid = false;
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		if (root->get_position() > root->get_lexer()->get_source_token_list()->end()) {
			else_conditional = nullptr;
			if_statement_list = vector<shared_ptr<statement>>();
			else_statement_list = vector<shared_ptr<statement>>();
			valid = false;
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else {
			else_conditional = nullptr;
			if_statement_list = vector<shared_ptr<statement>>();
			else_statement_list = vector<shared_ptr<statement>>();
			valid = false;
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() > root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if (stmt->get_valid());
			else {
				else_conditional = nullptr;
				else_statement_list = vector<shared_ptr<statement>>();
				valid = false;
				ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
				cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
				return make_shared<conditional_statement>(*this);
			}
			if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				valid = false;
				else_conditional = nullptr;
				else_statement_list = vector<shared_ptr<statement>>();
				cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
				valid = false;
				ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
				return make_shared<conditional_statement>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				valid = false;
				else_conditional = nullptr;
				else_statement_list = vector<shared_ptr<statement>>();
				ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
				cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
				return make_shared<conditional_statement>(*this);
			}
			root->set_position(root->get_position() + 1);
			if_statement_list.push_back(stmt);
		}
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE);
		else {
			valid = false;
			else_conditional = nullptr;
			else_statement_list = vector<shared_ptr<statement>>();
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NONE;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT_DECL);
		else {
			valid = true;
			else_conditional = nullptr;
			else_statement_list = vector<shared_ptr<statement>>();
			ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_NOT_PRESENT;
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NONE;
			return make_shared<conditional_statement>(*this);
		}
		ces_kind = conditional_else_statement_kind::CONDITIONAL_ELSE_STATEMENT_PRESENT;
		root->set_position(root->get_position() + 1);
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE)
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_NOT_PRESENT;
		else {
			cec_kind = conditional_else_conditional_kind::CONDITIONAL_ELSE_CONDITIONAL_PRESENT;
			else_conditional = make_shared<binary_expression>(root)->parse_binary_expression();
			if (else_conditional->get_valid());
			else {
				else_statement_list = vector<shared_ptr<statement>>();
				valid = false;
				make_shared<conditional_statement>(*this);
			}
		}
		if (root->get_position() > root->get_lexer()->get_source_token_list()->end()) {
			else_statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<conditional_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else {
			valid = false;
			else_statement_list = vector<shared_ptr<statement>>();
			return make_shared<conditional_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() > root->get_lexer()->get_source_token_list()->end() || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if (stmt->get_valid());
			else {
				valid = false;
				return make_shared<conditional_statement>(*this);
			}
			if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
				valid = false;
				return make_shared<conditional_statement>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_NEW_LINE) {
				root->get_diagnostics_reporter()->print(diagnostic_messages::expected_new_line, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				return make_shared<conditional_statement>(*this);
			}
			root->set_position(root->get_position() + 1);
			else_statement_list.push_back(stmt);
		}
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE);
		else {
			valid = false;
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_brace, root->get_position() < root->get_lexer()->get_source_token_list()->end() ? root->get_position() :
				root->get_lexer()->get_source_token_list()->end() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<conditional_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		valid = true;
		return make_shared<conditional_statement>(*this);
	}

	enum_statement::enum_statement(shared_ptr<root_node> r) : root_node(*r), enum_statement_pos(r->get_position()) {
		identifier_list = vector<shared_ptr<literal>>();
		identifier = nullptr;
		valid = false;
		es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
	}

	enum_statement::~enum_statement() {

	}

	vector<shared_ptr<literal>> enum_statement::get_identifier_list() {
		return identifier_list;
	}

	shared_ptr<literal> enum_statement::get_identifier() {
		return identifier;
	}

	const bool enum_statement::get_valid() {
		return valid;
	}

	source_token_list::iterator enum_statement::get_position() {
		return enum_statement_pos;
	}

	const enum_statement_kind enum_statement::get_enum_statement_kind() {
		return es_kind;
	}

	shared_ptr<enum_statement> enum_statement::parse_enum_statement() {
		source_token_list::iterator save = root->get_position();
		enum_statement_pos = save;
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			identifier_list = vector<shared_ptr<literal>>();
			identifier = nullptr;
			valid = false;
			es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
			return make_shared<enum_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_ENUM);
		else {
			identifier_list = vector<shared_ptr<literal>>();
			identifier = nullptr;
			valid = false;
			es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
			return make_shared<enum_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			identifier_list = vector<shared_ptr<literal>>();
			identifier = nullptr;
			valid = false;
			es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
			return make_shared<enum_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_POINT_DECL);
		else {
			identifier_list = vector<shared_ptr<literal>>();
			identifier = nullptr;
			valid = false;
			es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
			return make_shared<enum_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		identifier = make_shared<literal>(root)->parse_literal();
		if (identifier->get_valid());
		else {
			identifier_list = vector<shared_ptr<literal>>();
			valid = false;
			es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
			return make_shared<enum_statement>(*this);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			identifier_list = vector<shared_ptr<literal>>();
			valid = true;
			es_kind = enum_statement_kind::ENUM_STATEMENT_DECLARATION;
			return make_shared<enum_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE) {
			identifier_list = vector<shared_ptr<literal>>();
			valid = true;
			es_kind = enum_statement_kind::ENUM_STATEMENT_DECLARATION;
			return make_shared<enum_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else {
			identifier_list = vector<shared_ptr<literal>>();
			valid = false;
			es_kind = enum_statement_kind::ENUM_STATEMENT_NONE;
			return make_shared<enum_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		es_kind = enum_statement_kind::ENUM_STATEMENT_DEFINITION;
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			shared_ptr<literal> lit = make_shared<literal>(root)->parse_literal();
			if (lit->get_valid());
			else {
				valid = false;
				return make_shared<enum_statement>(*this);
			}
			identifier_list.push_back(lit);
			if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_COMMA)
				root->set_position(root->get_position() + 1);
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
		}
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE);
		else {
			valid = false;
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_brace, root->get_position() < root->get_lexer()->get_source_token_list()->end() ? root->get_position() :
				root->get_lexer()->get_source_token_list()->end() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<enum_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		valid = true;
		return make_shared<enum_statement>(*this);
	}

	while_statement::while_statement(shared_ptr<root_node> r) : root_node(*r), while_statement_pos(r->get_position()) {
		valid = false;
		statement_list = vector<shared_ptr<statement>>();
		condition = nullptr;
	}

	while_statement::~while_statement() {

	}

	const bool while_statement::get_valid() {
		return valid;
	}

	vector<shared_ptr<statement>> while_statement::get_statement_list() {
		return statement_list;
	}

	source_token_list::iterator while_statement::get_position() {
		return while_statement_pos;
	}

	shared_ptr<binary_expression> while_statement::get_condition() {
		return condition;
	}

	shared_ptr<while_statement> while_statement::parse_while_statement() {
		source_token_list::iterator save = root->get_position();
		while_statement_pos = save;
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			condition = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			return make_shared<while_statement>(*this);
		}
		if ((*save)->get_token_kind() == token_kind::TOKEN_WHILE);
		else {
			valid = false;
			condition = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			return make_shared<while_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		condition = make_shared<binary_expression>(root)->parse_binary_expression();
		if (condition->get_valid());
		else {
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			return make_shared<while_statement>(*this);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			return make_shared<while_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_OPEN_BRACE);
		else {
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			return make_shared<while_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if (stmt->get_valid());
			else {
				valid = false;
				return make_shared<while_statement>(*this);
			}
			statement_list.push_back(stmt);
			if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE);
			else {
				valid = false;
				return make_shared<while_statement>(*this);
			}
		}
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE);
		else {
			valid = false;
			root->get_diagnostics_reporter()->print(diagnostic_messages::expected_close_brace, root->get_position() < root->get_lexer()->get_source_token_list()->end() ? root->get_position() :
				root->get_lexer()->get_source_token_list()->end() - 1, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
			return make_shared<while_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		valid = true;
		return make_shared<while_statement>(*this);
	}

	for_statement::for_statement(shared_ptr<root_node> r) : root_node(*r), for_statement_pos(r->get_position()) {
		loop_variable = nullptr;
		b_expression = nullptr;
		statement_list = vector<shared_ptr<statement>>();
		valid = false;
	}

	for_statement::~for_statement() {

	}

	source_token_list::iterator for_statement::get_position() {
		return for_statement_pos;
	}

	shared_ptr<declaration> for_statement::get_loop_variable() {
		return loop_variable;
	}

	shared_ptr<binary_expression> for_statement::get_expression() {
		return b_expression;
	}

	vector<shared_ptr<statement>> for_statement::get_statement_list() {
		return statement_list;
	}

	const bool for_statement::get_valid() {
		return valid;
	}

	shared_ptr<for_statement> for_statement::parse_for_statement() {
		source_token_list::iterator save = root->get_position();
		for_statement_pos = save;
		if (save >= root->get_lexer()->get_source_token_list()->end()) {
			loop_variable = nullptr;
			b_expression = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		if ((*save)->get_token_kind() == token_kind::TOKEN_FOR);
		else {
			loop_variable = nullptr;
			b_expression = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		loop_variable = make_shared<declaration>(root)->parse_declaration(true, false);
		if (loop_variable->get_valid());
		else {
			b_expression = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		if (root->get_position() > root->get_lexer()->get_source_token_list()->end()) {
			b_expression = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_COLON) {
			b_expression = nullptr;
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		b_expression = make_shared<binary_expression>(root)->parse_binary_expression();
		if (b_expression->get_valid());
		else {
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			statement_list = vector<shared_ptr<statement>>();
			valid = false;
			return make_shared<for_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() != token_kind::TOKEN_OPEN_BRACE) {
			valid = false;
			statement_list = vector<shared_ptr<statement>>();
			return make_shared<for_statement>(*this);
		}
		root->set_position(root->get_position() + 1);
		while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() != token_kind::TOKEN_CLOSE_BRACE) {
			while (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
				break;
			shared_ptr<statement> stmt = make_shared<statement>(root)->parse_statement();
			if (stmt->get_valid());
			else {
				valid = false;
				return make_shared<for_statement>(*this);
			}
			if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				root->set_position(root->get_position() + 1);
			statement_list.push_back(stmt);
		}
		if (root->get_position() < root->get_lexer()->get_source_token_list()->end() && (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CLOSE_BRACE)
			root->set_position(root->get_position() + 1);
		else {
			valid = false;
			return make_shared<for_statement>(*this);
		}
		valid = true;
		return make_shared<for_statement>(*this);
	}

	break_continue_statement::break_continue_statement(shared_ptr<root_node> r) : root_node(*r), break_continue_statement_pos(r->get_position()) {
		valid = false;
		bcs_kind = break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_NONE;
	}

	break_continue_statement::~break_continue_statement() {

	}

	source_token_list::iterator break_continue_statement::get_position() {
		return break_continue_statement_pos;
	}

	const bool break_continue_statement::get_valid() {
		return valid;
	}

	const break_continue_statement_kind break_continue_statement::get_break_continue_statement_kind() {
		return bcs_kind;
	}

	shared_ptr<break_continue_statement> break_continue_statement::parse_break_continue_statement() {
		source_token_list::iterator save = root->get_position();
		break_continue_statement_pos = save;
		if (root->get_position() >= root->get_lexer()->get_source_token_list()->end()) {
			valid = false;
			bcs_kind = break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_NONE;
			return make_shared<break_continue_statement>(*this);
		}
		if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BREAK) {
			valid = true;
			root->set_position(root->get_position() + 1);
			bcs_kind = break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_BREAK;
			return make_shared<break_continue_statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CONTINUE) {
			valid = true;
			root->set_position(root->get_position() + 1);
			bcs_kind = break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_CONTINUE;
			return make_shared<break_continue_statement>(*this);
		}
		valid = false;
		bcs_kind = break_continue_statement_kind::BREAK_CONTINUE_STATEMENT_NONE;
		return make_shared<break_continue_statement>(*this);
	}

	statement::statement(shared_ptr<root_node> r) : root_node(*r), statement_pos(r->get_position()) {
		valid = false;
		decl = nullptr;
		struc = nullptr;
		b_expression = nullptr;
		mod = nullptr;
		ret = nullptr;
		cond = nullptr;
		_enum = nullptr;
		wloop = nullptr;
		floop = nullptr;
		break_continue = nullptr;
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

	shared_ptr<function> statement::get_function() {
		return func;
	}

	shared_ptr<structure> statement::get_structure() {
		return struc;
	}

	shared_ptr<module> statement::get_module() {
		return mod;
	}

	shared_ptr<return_statement> statement::get_return_statement() {
		return ret;
	}

	shared_ptr<conditional_statement> statement::get_conditional_statement() {
		return cond;
	}

	shared_ptr<enum_statement> statement::get_enum_statement() {
		return _enum;
	}

	shared_ptr<while_statement> statement::get_while_statement() {
		return wloop;
	}

	shared_ptr<for_statement> statement::get_for_statement() {
		return floop;
	}

	shared_ptr<break_continue_statement> statement::get_break_continue_statement() {
		return break_continue;
	}

	shared_ptr<statement> statement::parse_statement() {
		source_token_list::iterator save = root->get_position();
		statement_pos = root->get_position();
		if(save >= root->get_lexer()->get_source_token_list()->end()) {
			kind = statement_kind::STATEMENT_NONE;
			valid = false;
			decl = nullptr;
			b_expression = nullptr;
			func = nullptr;
			struc = nullptr;
			mod = nullptr;
			ret = nullptr;
			cond = nullptr;
			_enum = nullptr;
			wloop = nullptr;
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		if((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_VAR) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_DECLARATION;
			decl = make_shared<declaration>(root)->parse_declaration(false, false);
			valid = decl->get_valid();
			func = nullptr;
			struc = nullptr;
			mod = nullptr;
			ret = nullptr;
			_enum = nullptr;
			cond = nullptr;
			wloop = nullptr;
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_FUNC) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_FUNCTION;
			decl = nullptr;
			struc = nullptr;
			mod = nullptr;
			func = make_shared<function>(root)->parse_function(false);
			valid = func->get_valid();
			ret = nullptr;
			cond = nullptr;
			_enum = nullptr;
			wloop = nullptr;
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_STRUCT) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_STRUCTURE;
			decl = nullptr;
			struc = make_shared<structure>(root)->parse_structure();
			valid = struc->get_valid();
			func = nullptr;
			mod = nullptr;
			ret = nullptr;
			cond = nullptr;
			_enum = nullptr;
			wloop = nullptr;
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_MODULE) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_MODULE;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			ret = nullptr;
			cond = nullptr;
			_enum = nullptr;
			floop = nullptr;
			mod = make_shared<module>(root)->parse_module();
			valid = mod->get_valid();
			wloop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_RETURN) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_RETURN_STATEMENT;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			mod = nullptr;
			cond = nullptr;
			_enum = nullptr;
			floop = nullptr;
			ret = make_shared<return_statement>(root)->parse_return_statement();
			valid = ret->get_valid();
			wloop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_IF) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_CONDITIONAL_STATEMENT;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			mod = nullptr;
			cond = make_shared<conditional_statement>(root)->parse_conditional_statement();
			ret = nullptr;
			_enum = nullptr;
			floop = nullptr;
			valid = cond->get_valid();
			wloop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_ENUM) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_ENUM_STATEMENT;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			mod = nullptr;
			cond = nullptr;
			ret = nullptr;
			_enum = make_shared<enum_statement>(root)->parse_enum_statement();
			valid = _enum->get_valid();
			wloop = nullptr;
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_WHILE) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_WHILE_STATMENT;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			mod = nullptr;
			cond = nullptr;
			ret = nullptr;
			_enum = nullptr;
			wloop = make_shared<while_statement>(root)->parse_while_statement();
			valid = wloop->get_valid();
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_FOR) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_FOR_STATEMENT;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			mod = nullptr;
			cond = nullptr;
			ret = nullptr;
			_enum = nullptr;
			wloop = nullptr;
			floop = make_shared<for_statement>(root)->parse_for_statement();
			valid = floop->get_valid();
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		else if ((*(root->get_position()))->get_token_kind() == token_kind::TOKEN_BREAK || (*(root->get_position()))->get_token_kind() == token_kind::TOKEN_CONTINUE) {
			b_expression = nullptr;
			kind = statement_kind::STATEMENT_BREAK_CONTINUE_STATEMENT;
			decl = nullptr;
			struc = nullptr;
			func = nullptr;
			mod = nullptr;
			cond = nullptr;
			ret = nullptr;
			_enum = nullptr;
			wloop = nullptr;
			floop = nullptr;
			break_continue = make_shared<break_continue_statement>(root)->parse_break_continue_statement();
			valid = break_continue->get_valid();
			return make_shared<statement>(*this);
		}
		else {
			decl = nullptr;
			func = nullptr;
			struc = nullptr;
			mod = nullptr;
			ret = nullptr;
			cond = nullptr;
			_enum = nullptr;
			kind = statement_kind::STATEMENT_EXPRESSION;
			b_expression = make_shared<binary_expression>(root)->parse_binary_expression();
			valid = b_expression->get_valid();
			wloop = nullptr;
			floop = nullptr;
			break_continue = nullptr;
			return make_shared<statement>(*this);
		}
		decl = nullptr;
		kind = statement_kind::STATEMENT_NONE;
		b_expression = nullptr;
		valid = false;
		func = nullptr;
		struc = nullptr;
		ret = nullptr;
		mod = nullptr;
		cond = nullptr;
		_enum = nullptr;
		wloop = nullptr;
		floop = nullptr;
		break_continue = nullptr;
		root->get_diagnostics_reporter()->print(diagnostic_messages::unreachable, root->get_position(), diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
		exit(1);
		return make_shared<statement>(*this);
	}
}
