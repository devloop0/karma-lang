#ifndef KARMA_LANG_LEX_HPP
#define KARMA_LANG_LEX_HPP

#include "source_token_list.hpp"
#include "token.hpp"
#include "token_keywords.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace karma_lang;
using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

namespace karma_lang {

	class lexer {
		shared_ptr<source_token_list> stlist;
		string source;
		string file;
	public:
		lexer(string src, string f);
		shared_ptr<source_token_list> lex();
		const shared_ptr<source_token_list> get_source_token_list();
	};
}

#endif
