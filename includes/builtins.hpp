#ifndef KARMA_LANG_BUILTINS_HPP
#define KARMA_LANG_BUILTINS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::string;
using std::vector;
using std::make_shared;
using std::shared_ptr;

namespace karma_lang {

	class builtins {
		public:
			const static string builtin__va_args__;
			const static string builtin_print;
	};
}

#endif