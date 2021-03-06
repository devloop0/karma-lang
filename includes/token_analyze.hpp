#ifndef KARMA_LANG_TOKEN_ANALYZE_HPP
#define KARMA_LANG_TOKEN_ANALYZE_HPP

#include "includes/token.hpp"
#include "includes/source_token_list.hpp"
#include "includes/diagnostics_report.hpp"
#include "includes/lex.hpp"
#include "includes/diagnostic_messages.hpp"
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
		int error_count;
		public:
			token_analyzer(shared_ptr<lexer> l);
			~token_analyzer();
			shared_ptr<token> analyze_source_token_list();
			const int get_error_count();
	};
}

#endif
