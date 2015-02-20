#ifndef KARMA_LANG_TOKEN_KEYWORDS_HPP
#define KARMA_LANG_TOKEN_KEYWORDS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "../includes/diagnostic_messages.hpp"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::shared_ptr;
using std::make_shared;

using namespace karma_lang;

namespace karma_lang {

	class token_keywords {
		public:
			const static string _true;
			const static string _false;
			const static string var;	
			const static string immut;
			const static string nil;
	};
}

#endif
