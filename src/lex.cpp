#include "../includes/lex.hpp"

using namespace karma_lang;

namespace karma_lang {

	lexer::lexer(string src, string f) : source(src), file(f) {
		stlist = nullptr;
	}

	shared_ptr<source_token_list> lexer::lex() {
		int tabs = 0;
		int cols = 1;
		int line_number = 1;
		vector<shared_ptr<token>> tok_list;
		for (int i = 0; i < source.length(); i++, cols++) {
			switch (source[i]) {
			case '\t':
				tabs++;
				break;
			case '\n': {
				cols = 0;
				tabs = 0;
				line_number++;
				shared_ptr<token> tok = make_shared<token>(tabs, cols, cols + 1, "\n", file, line_number, token_kind::TOKEN_NEW_LINE);
				tok_list.push_back(tok);
				break;
			}
			case '+': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "+=", file, line_number, token_kind::TOKEN_PLUS_EQUALS);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '+') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "++", file, line_number, token_kind::TOKEN_INCREMENT);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "+", file, line_number, token_kind::TOKEN_PLUS);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '-': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "-=", file, line_number, token_kind::TOKEN_MINUS_EQUALS);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '-') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "--", file, line_number, token_kind::TOKEN_DECREMENT);
					tok_list.push_back(tok);
				}
				else if(source[i + 1] == '>') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "->", file, line_number, token_kind::TOKEN_POINT_DECL);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "-", file, line_number, token_kind::TOKEN_MINUS);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '*': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "*=", file, line_number, token_kind::TOKEN_MULTIPLY_EQUALS);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '*') {
					i++, cols++;
					if (source[i + 1] == '=') {
						i++, cols++;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "**=", file, line_number, token_kind::TOKEN_EXPONENT_EQUALS);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "**", file, line_number, token_kind::TOKEN_EXPONENT);
						tok_list.push_back(tok);
					}

				}
				else if(source[i + 1] == '>') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "*>", file, line_number, token_kind::TOKEN_POINT);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "*", file, line_number, token_kind::TOKEN_MULTIPLY);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '/': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "/=", file, line_number, token_kind::TOKEN_DIVIDE_EQUALS);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "/", file, line_number, token_kind::TOKEN_DIVIDE);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '%': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "%=", file, line_number, token_kind::TOKEN_MODULUS_EQUALS);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "%", file, line_number, token_kind::TOKEN_MODULUS);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '<': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "<=", file, line_number, token_kind::TOKEN_LESS_THAN_OR_EQUAL_TO);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '<') {
					i++, cols++;
					if (source[i + 1] == '=') {
						i++, cols++;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "<<=", file, line_number, token_kind::TOKEN_SHIFT_LEFT_EQUALS);
						tok_list.push_back(tok);
					}
					else if(source[i + 1] == '<') {
						i++, cols++;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "<<<", file, line_number, token_kind::TOKEN_OPEN_TUPLE);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "<<", file, line_number, token_kind::TOKEN_SHIFT_LEFT);
						tok_list.push_back(tok);
					}
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "<", file, line_number, token_kind::TOKEN_LESS_THAN);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '>': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ">=", file, line_number, token_kind::TOKEN_GREATER_THAN_OR_EQUAL_TO);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '>') {
					i++, cols++;
					if (source[i + 1] == '=') {
						i++, cols++;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ">>=", file, line_number, token_kind::TOKEN_SHIFT_RIGHT_EQUALS);
						tok_list.push_back(tok);
					}
					else if(source[i + 1] == '>') {
						i++, cols++;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ">>>", file, line_number, token_kind::TOKEN_CLOSE_TUPLE);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ">>", file, line_number, token_kind::TOKEN_SHIFT_RIGHT);
						tok_list.push_back(tok);
					}
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ">", file, line_number, token_kind::TOKEN_GREATER_THAN);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '[': {
					int save = cols;
					if(source[i + 1] == '[' && source[i + 2] == '[') {
						i += 2, cols += 2;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "[[[", file, line_number, token_kind::TOKEN_OPEN_LIST);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "[", file, line_number, token_kind::TOKEN_OPEN_BRACKET);
						tok_list.push_back(tok);
					}
				  }
				  break;
			case ']': {
					int save = cols;
					if(source[i + 1] == ']' && source[i + 2] == ']') {
						i += 2, cols += 2;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "]]]", file, line_number, token_kind::TOKEN_CLOSE_LIST);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "]", file, line_number, token_kind::TOKEN_CLOSE_BRACKET);
						tok_list.push_back(tok);
					}
				  }
				  break;
			case '{': {
					int save = cols;
					if(source[i + 1] == '{' && source[i + 2] == '{') {
						i += 2, cols += 2;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "{{{", file, line_number, token_kind::TOKEN_OPEN_DICT);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "{", file, line_number, token_kind::TOKEN_OPEN_BRACE);
						tok_list.push_back(tok);
					}
				  }
				  break;
			case '}': {
					int save = cols;
					if(source[i + 1] == '}' && source[i + 2] == '}') {
						i += 2, cols += 2;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "}}}", file, line_number, token_kind::TOKEN_CLOSE_DICT);
						tok_list.push_back(tok);
					}
					else {
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "}", file, line_number, token_kind::TOKEN_CLOSE_BRACE);
						tok_list.push_back(tok);
					}
				  }
				  break;
			case '^': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "^=", file, line_number, token_kind::TOKEN_BITWISE_EXCLUSIVE_OR_EQUALS);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "^", file, line_number, token_kind::TOKEN_BITWISE_EXCLUSIVE_OR);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '|': {
				int save = cols;
				if (source[i + 1] == '|') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "||", file, line_number, token_kind::TOKEN_LOGICAL_OR);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "|=", file, line_number, token_kind::TOKEN_BITWISE_OR_EQUALS);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "|", file, line_number, token_kind::TOKEN_BITWISE_OR);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '&': {
				int save = cols;
				if (source[i + 1] == '&') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "&&", file, line_number, token_kind::TOKEN_LOGICAL_AND);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "&=", file, line_number, token_kind::TOKEN_BITWISE_AND);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "&", file, line_number, token_kind::TOKEN_BITWISE_AND);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '!': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "!=", file, line_number, token_kind::TOKEN_NOT_EQUALS);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "!", file, line_number, token_kind::TOKEN_EXCLAMATION);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '=': {
				int save = cols;
				if (source[i + 1] == '=') {
					i++, cols++;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "==", file, line_number, token_kind::TOKEN_DOUBLE_EQUALS);
					tok_list.push_back(tok);
				}
				else {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "=", file, line_number, token_kind::TOKEN_EQUALS);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '(': {
				int save = cols;
				shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "(", file, line_number, token_kind::TOKEN_OPEN_PARENTHESIS);
				tok_list.push_back(tok);
			}
					  break;
			case '?': {
				int save = cols;
				shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "?", file, line_number, token_kind::TOKEN_QUESTION_MARK);
				tok_list.push_back(tok);
			}
					  break;
			case ':': {
				int save = cols;
				shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ":", file, line_number, token_kind::TOKEN_COLON);
				tok_list.push_back(tok);
			}
					  break;
			case ')': {
				int save = cols;
				shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ")", file, line_number, token_kind::TOKEN_CLOSE_PARENTHESIS);
				tok_list.push_back(tok);
			}
					  break;
			case ',': {
				int save = cols;
				shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ",", file, line_number, token_kind::TOKEN_COMMA);
				tok_list.push_back(tok);
			}
					  break;
			case '~': {
				int save = cols;
				shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, "~", file, line_number, token_kind::TOKEN_COMPLEMENT);
				tok_list.push_back(tok);
			}
					  break;
			case '0': {
				string acc = "0";
				int save = cols;
				if (source[i + 1] == 'x' || source[i + 1] == 'X') {
					acc += source[i + 1];
					i += 2, cols += 2;
					while (isdigit(source[i]) || source[i] == 'a' || source[i] == 'b' || source[i] == 'c' || source[i] == 'd' ||
						source[i] == 'e' || source[i] == 'f' || source[i] == 'A' || source[i] == 'B' ||
						source[i] == 'C' || source[i] == 'D' || source[i] == 'E' || source[i] == 'F') {
						acc += source[i];
						i++, cols++;
					}
					i--, cols--;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_HEX_NUMBER);
					tok_list.push_back(tok);
				}
				else if (source[i + 1] == '.') {
					acc += source[i + 1];
					i += 2, cols += 2;
					while (isdigit(source[i])) {
						acc += source[i];
						i++, cols++;
					}
					i--, cols--;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_DECIMAL_NUMBER);
					tok_list.push_back(tok);
				}
				else {
					i++, cols++;
					bool decimal_flag = true;
					while (isdigit(source[i]) || (decimal_flag && source[i] == '.')) {
						if (source[i] == '.')
							decimal_flag = false;
						acc += source[i];
						i++, cols++;
					}
					i--, cols--;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_REGULAR_NUMBER);
					tok_list.push_back(tok);
				}
			}
					  break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': {
				int save = cols;
				string acc = string(1, source[i]);
				bool decimal_flag = false;
				i++, cols++;
				while (isdigit(source[i]) || (!decimal_flag && source[i] == '.')) {
					if (source[i] == '.')
						decimal_flag = true;
					acc += source[i];
					i++, cols++;
				}
				i--, cols--;
				shared_ptr<token> tok = nullptr;
				if (decimal_flag)
					tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_DECIMAL_NUMBER);
				else
					tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_REGULAR_NUMBER);
				tok_list.push_back(tok);
			}
					  break;
			case '.': {
				int save = cols;
				if(!isdigit(source[i + 1])) {
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, ".", file, line_number, token_kind::TOKEN_DOT);
					tok_list.push_back(tok);	
				}
				else { 
					string acc = string(1, source[i]);
					if (isdigit(source[i + 1])) {
						i++, cols++;
						while (isdigit(source[i])) {
							acc += source[i];
							i++, cols++;
						}
						i--, cols--;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_DECIMAL_NUMBER);
						tok_list.push_back(tok);
					}
				}
			}
					  break;
			case '\'':
			case '\"': {
				char c = source[i];
				int save = cols;
				string acc = string(1, source[i]);
				i++, cols++;
				while (source[i] != c && source[i] != '\n') {
					acc += source[i];
					if (source[i] == '\\' && (source[i + 1] == '\'' || source[i + 1] == '\"')) {
						acc += source[i + 1];
						i++, cols++;
					}
					i++, cols++;
				}
				acc += source[i];
				shared_ptr<token> tok;
				if (source[i] == c)
					tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_STRING);
				else
					tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_ERROR);
				tok_list.push_back(tok);
			}
					   break;
			default: {
				int save = cols;
				if (isalnum(source[i]) || source[i] == '_') {
					string acc = string(1, source[i]);
					i++, cols++;
					while (isalnum(source[i]) || source[i] == '_') {
						acc += source[i];
						i++, cols++;
					}
					i--, cols--;
					token_kind tkind = token_kind::TOKEN_IDENTIFIER;
					if(acc == token_keywords::_true)
						tkind = token_kind::TOKEN_TRUE;
					else if(acc == token_keywords::_false)
						tkind = token_kind::TOKEN_FALSE;
					else if(acc == token_keywords::var)
						tkind = token_kind::TOKEN_VAR;
					else if(acc == token_keywords::immut)
						tkind = token_kind::TOKEN_IMMUT;
					else if (acc == token_keywords::nil)
						tkind = token_kind::TOKEN_NIL;
					else if (acc == token_keywords::func)
						tkind = token_kind::TOKEN_FUNC;
					shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, tkind);
					tok_list.push_back(tok);
				}
				else {
					int save = cols;
					string acc;
					while (!isspace(source[i]) && i != source.length()) {
						acc += source[i];
						i++, cols++;
					}
					if (acc.length() != 0) {
						i--, cols--;
						shared_ptr<token> tok = make_shared<token>(tabs, save, cols + 1, acc, file, line_number, token_kind::TOKEN_ERROR);
						tok_list.push_back(tok);
					}
				}
			}
					 break;
			}
		}
		stlist = make_shared<source_token_list>(tok_list);
		return stlist;
	}

	const shared_ptr<source_token_list> lexer::get_source_token_list() {
		return stlist;
	}
}
