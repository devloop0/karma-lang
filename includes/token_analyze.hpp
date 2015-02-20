#ifndef KARMA_LANG_TOKEN_ANALYZE_HPP
#define KARMA_LANG_TOKEN_ANALYZE_HPP

#include "token.hpp"
#include "source_token_list.hpp"
#include "diagnostics_report.hpp"
#include "lex.hpp"
#include "diagnostic_messages.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::shared_ptr;

using namespace karma_lang;

namespace karma_lang {

	class token_analyzer {
		shared_ptr<lexer> lex;
		shared_ptr<diagnostics_reporter> dreporter;
		public:
			token_analyzer(shared_ptr<lexer> l);
			~token_analyzer();
			shared_ptr<token> analyze_source_token_list();
	};
}

#endif
