#ifndef KARMA_LANG_DIAGNOSTICS_REPORT_HPP
#define KARMA_LANG_DIAGNOSTICS_REPORT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include "../includes/token.hpp"
#include "../includes/source_token_list.hpp"

#ifdef WIN32
#include <Windows.h>
#endif

using std::string;
using std::cout;
using std::cerr;
using std::vector;
using std::shared_ptr;

using namespace karma_lang;

namespace karma_lang {

	enum diagnostics_reporter_kind {
		DIAGNOSTICS_REPORTER_ERROR, DIAGNOSTICS_REPORTER_WARNING, DIAGNOSTICS_REPORTER_NOTE
	};

	class diagnostics_reporter {
		shared_ptr<source_token_list> stlist;
		public:
			diagnostics_reporter(shared_ptr<source_token_list> stl);
			~diagnostics_reporter();
			shared_ptr<token> print(string message, source_token_list::iterator current_pos, diagnostics_reporter_kind kind);	
	};
}

#endif
