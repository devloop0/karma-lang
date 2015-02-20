#include "../includes/token.hpp"

namespace karma_lang {
	token::token(int tc, int cs, int ce, string rs, string f, int l, token_kind k) : tab_count(tc), column_start(cs), column_end(ce), raw_string(rs), kind(k), file_name(f), line_number(l) {

	}

	token::~token() {

	}

	const int token::get_tab_count() {
		return tab_count;
	}

	const int token::get_column_start() {
		return column_start;
	}

	const int token::get_column_end() {
		return column_end;
	}

	const string token::get_raw_string() {
		return raw_string;
	}

	const token_kind token::get_token_kind() {
		return kind;
	}

	token_kind token::set_token_kind(token_kind k) {
		kind = k;
		return kind;
	}

	const string token::get_file_name() {
		return file_name;
	}

	const int token::get_line_number() {
		return line_number;
	}
}
