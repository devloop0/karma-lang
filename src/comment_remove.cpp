#include "includes/comment_remove.hpp"

using namespace karma_lang;

namespace karma_lang {
	
	comment_remover::comment_remover(string src) : source(src), comments_removed("") {

	}

	comment_remover::comment_remover(vector<string> src) : comments_removed("") {
		for (string s : src)
			source += s + "\n";
	}

	comment_remover::~comment_remover() {

	}

	const string comment_remover::get_comments_removed() {
		return comments_removed;
	}

	string comment_remover::remove_comments() {
		string copy;
		for(int i = 0; i < source.length(); i++) {
			if(source[i] == '#') {
				i++;
				while(source[i] != '\n') i++;
				i--;
			}
			else if(source[i] == '\'') {
				copy += source[i];
				i++;
				while (source[i] != '\'' && i != source.length()) {
					if (source[i] == '\\' && i + 1 != source.length() && source[i + 1] == '\'') {
						copy += source[i];
						copy += source[i + 1];
						i++;
					}
					else
						copy += source[i];
					i++;
				}
				if (source[i] == '\'')
					copy += source[i];
			}
			else if (source[i] == '\"') {
				copy += source[i];
				i++;
				while (source[i] != '\"' && i != source.length()) {
					if (source[i] == '\\' && i + 1 != source.length() && source[i + 1] == '\"') {
						copy += source[i];
						copy += source[i + 1];
						i++;
					}
					else
						copy += source[i];
					i++;
				}
				if (source[i] == '\"')
					copy += source[i];
			}
			else
				copy += source[i];				
		}
		comments_removed = copy;
		return comments_removed;
	}
}
