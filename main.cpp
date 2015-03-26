#include "includes/token.hpp"
#include "includes/source_token_list.hpp"
#include "includes/lex.hpp"
#include "includes/comment_remove.hpp"
#include "includes/token_analyze.hpp"
#include "includes/parse.hpp"
#include "includes/semantics.hpp"
#include "includes/code_generation.hpp"

using namespace karma_lang;

int main(int argc, char* argv[]) {
	string source;
	string line = "";
	while(line != "$$EOF$$") {
		cout << ">>> ";
		std::getline(std::cin, line);
		if(line != "$$EOF$$")
			source += line + "\n";
	}
	string file = "test.k";
	lexer l(comment_remover(source).remove_comments(), file);
	shared_ptr<source_token_list> stlist = l.lex();
	for(source_token_list::iterator it = stlist->begin(); it < stlist->end(); it++) {
		cout << (*it)->get_tab_count() << ' ' << (*it)->get_raw_string() << ' ' << (*it)->get_column_start() << '-' <<
			(*it)->get_column_end() << ' ' << (*it)->get_line_number() << '\n';
	}
	token_analyzer t(make_shared<lexer>(l));
	t.analyze_source_token_list();
	shared_ptr<root_node> root =  make_shared<root_node>(make_shared<lexer>(l), make_shared<diagnostics_reporter>(l.get_source_token_list()));
	shared_ptr<parser> p = make_shared<parser>(root);
	vector<shared_ptr<statement>> stmt_list = root->parse_program(p);
	analyze_ast aa(root);
	cout << "\n============\nValidity: " << aa.perform_semantic_analysis() << "\nSize: ";
	cout << aa.get_annotated_root_node()->get_annotated_statement_list().size() << "\n============\n";
	if(root->get_diagnostics_reporter()->get_error_count() > 0) {
		cout << "Errors; not generating code.";
		exit(1);
	}
	generate_code gc(make_shared<analyze_ast>(aa));
	vector<string> list = gc.perform_code_generation();
	for(int i = 0; i < list.size(); i++)
		cout << list[i] << '\n';
}
