#include "../includes/source_token_list.hpp"

using namespace karma_lang;

namespace karma_lang {

	source_token_list::source_token_list(vector<shared_ptr<token>> tl) {
		token_list = tl;
	}	

	source_token_list::~source_token_list() {

	}

	source_token_list::iterator::iterator(source_token_list* st) {
		index = 0;
		stlist = st;	
	}

	source_token_list::iterator::iterator(source_token_list* st, int i) {
		stlist = st;
		index = i;
	}

	source_token_list::iterator::~iterator() {
	
	}

	source_token_list::iterator source_token_list::begin() {
		return iterator(this, 0);
	}

	source_token_list::iterator::self_type source_token_list::end() {
		return iterator(this, token_list.size());
	}

	const source_token_list::iterator::value_type source_token_list::iterator::operator*() {
		return stlist->token_list[index];
	}	

	const bool source_token_list::iterator::operator==(source_token_list::iterator::self_type rhs) {
		return index == rhs.index;
	}

	const bool source_token_list::iterator::operator!=(source_token_list::iterator::self_type rhs) {
		return index != rhs.index;
	}

	const bool source_token_list::iterator::operator<(source_token_list::iterator::self_type rhs) {
		return index < rhs.index;
	}

	const bool source_token_list::iterator::operator>(source_token_list::iterator::self_type rhs) {
		return index > rhs.index;
	}

	const bool source_token_list::iterator::operator<=(source_token_list::iterator::self_type rhs) {
		return index <= rhs.index;
	}

	const bool source_token_list::iterator::operator>=(source_token_list::iterator::self_type rhs) {
		return index >= rhs.index;
	}

	source_token_list::iterator::self_type source_token_list::iterator::operator+(int i) {
		return iterator(stlist, index + i);
	}

	source_token_list::iterator::self_type source_token_list::iterator::operator+=(int i) {
		index += i;
		return *this;
	}
	
	source_token_list::iterator::self_type source_token_list::iterator::operator-(int i) {
		return iterator(stlist, index - i);
	}

	source_token_list::iterator::self_type source_token_list::iterator::operator-=(int i) {
		index -= i;
		return *this;
	}

	source_token_list::iterator::self_type source_token_list::iterator::operator++() {
		index++;
		return *this;
	}

	source_token_list::iterator::self_type source_token_list::iterator::operator++(int dummy) {
		source_token_list::iterator::self_type sf = *this;
		index++;
		return sf;
	}
}
