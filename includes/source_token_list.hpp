#ifndef KARMA_LANG_SOURCE_TOKEN_LIST_HPP
#define KARMA_LANG_SOURCE_TOKEN_LIST_HPP

#include "includes/token.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;

using namespace karma_lang;

namespace karma_lang {
	
	class source_token_list {
		vector<shared_ptr<token>> token_list;
	public:
		class iterator {
			int index;
			source_token_list* stlist;
		public:
			iterator(source_token_list* st, int i);
			typedef shared_ptr<token> value_type;
			typedef iterator self_type;
			typedef int size_type;
			iterator(source_token_list* st);
			~iterator();
			self_type operator+(int i);
			self_type operator-(int i);
			self_type operator++();
			self_type operator++(size_type dummy);
			const value_type operator*();
			const bool operator==(self_type rhs);
			const bool operator!=(self_type rhs);
			const bool operator>(self_type rhs);
			const bool operator<(self_type rhs);
			const bool operator<=(self_type rhs);
			const bool operator>=(self_type rhs);
			self_type operator+=(int i);
			self_type operator-=(int i);
		};
		source_token_list(vector<shared_ptr<token>> tl);
		~source_token_list();
		iterator begin();
		iterator end();
	};
}

#endif
