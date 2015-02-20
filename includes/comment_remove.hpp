#ifndef KARMA_LANG_COMMENT_REMOVE_HPP
#define KARMA_LANG_COMMENT_REMOVE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;

namespace karma_lang {

	class comment_remover {
		string source;
		string comments_removed;
		public:
			comment_remover(string src);
			~comment_remover();
			string remove_comments();
			const string get_comments_removed();
	};
}

#endif
