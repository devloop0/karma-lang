#include "../includes/token_analyze.hpp"

using namespace karma_lang;

namespace karma_lang {

	token_analyzer::token_analyzer(shared_ptr<lexer> l) {
		lex = l;
		dreporter = make_shared<diagnostics_reporter>(lex->get_source_token_list());
	}	

	token_analyzer::~token_analyzer() {

	}

	shared_ptr<token> token_analyzer::analyze_source_token_list() {
		shared_ptr<token> tok = nullptr;
		for(source_token_list::iterator it = lex->get_source_token_list()->begin(); it < lex->get_source_token_list()->end(); ++it) {
			if((*it)->get_token_kind() == token_kind::TOKEN_ERROR) {
				dreporter->print(diagnostic_messages::malformed_token, it, diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR);
				tok = *it;
			}
		}
		return tok;
	}
}
