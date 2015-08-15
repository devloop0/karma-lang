#include "includes/compiler.hpp"

using namespace karma_lang;
using namespace unified_includes;

namespace karma_lang {

	compiler::compiler(string path) : source_file(path), dest_file("") {
		file temp(path);
		if (temp.get_parent_path() == "")
			dest_file = file(temp.get_name() + file::dot + dharma_vm_source_file_extension);
		else
			dest_file = file(temp.get_parent_path() + file::platform_path_separator + temp.get_name() + file::dot +
				dharma_vm_source_file_extension);
	}

	compiler::compiler(file f) : source_file(f), dest_file("") {
		file temp = f;
		if (temp.get_parent_path() == "")
			dest_file = file(temp.get_name() + file::dot + dharma_vm_source_file_extension);
		else
			dest_file = file(temp.get_parent_path() + file::platform_path_separator + temp.get_name() + file::dot +
				dharma_vm_source_file_extension);
	}

	compiler::~compiler() {

	}

	shared_ptr<source_token_list> compiler::get_source_token_list() {
		return src_tok_list;
	}

	vector<shared_ptr<statement>> compiler::get_statement_list() {
		return stmt_list;
	}

	vector<shared_ptr<annotated_statement>> compiler::get_annotated_statement_list() {
		return astmt_list;
	}

	vector<string> compiler::get_generated_code() {
		return generated_code;
	}

	vector<shared_ptr<symbol_table>> compiler::get_symbol_table_list() {
		return sym_table_list;
	}

	const bool compiler::compile(vector<shared_ptr<symbol_table>> existing, vector<scope_kind> sk_list, bool write_to_file) {
		string file = source_file.get_full_name();
		lexer l(comment_remover(source_file.get_source()).remove_comments(), file);
		src_tok_list = l.lex();
		token_analyzer t(make_shared<lexer>(l));
		t.analyze_source_token_list();
		if (t.get_error_count() > 0) {
			cerr << "Errors; not generating code.";
			exit(1);
		}
		shared_ptr<root_node> root = make_shared<root_node>(make_shared<lexer>(l), make_shared<diagnostics_reporter>(l.get_source_token_list()));
		shared_ptr<parser> p = make_shared<parser>(root);
		stmt_list = root->parse_program(p);
		analyze_ast aa(root, sk_list, existing, root->get_statement_list());
		bool ret = aa.perform_semantic_analysis();
		astmt_list = aa.get_annotated_root_node()->get_annotated_statement_list();
		sym_table_list = aa.get_symbol_table();
		if (root->get_diagnostics_reporter()->get_error_count() > 0) {
			cerr << "Errors; not generating code.";
			exit(1);
		}
		generate_code gc(make_shared<analyze_ast>(aa));
		generated_code = gc.perform_code_generation();
		if (write_to_file) {
			cout << "Output file: " << dest_file.get_full_name_with_path() << '\n';
			dest_file.create_file();
			dest_file.write(generated_code);
		}
		return ret;
	}

	file compiler::get_source_file() {
		return source_file;
	}

	file compiler::get_dest_file() {
		return dest_file;
	}

	karmac::karmac(int argc, char* argv[]) : cl_parser("", "", {}), compiler_list(vector<shared_ptr<compiler>>()) {
		command_line_argument re_cla(argument_kind::SWITCH_ARGUMENT, "re", "read-eval", "Read and Eval",
			"This argument is used for testing simple programs. Enter the program, "
			"and once you are done, on a new line, and type '$$EOF$$' without the single-quotes.\n"
			"The program will output a temp file which can be run with dharma-vm.", false, true);
		command_line_argument l_cla(argument_kind::SWITCH_ARGUMENT, "l", "logo", "Karmac Logo",
			"This argument is used to display the karmac logo.", false, true);
		command_line_argument fl_cla(argument_kind::ANY_ARGUMENT_MULTI, "", "", "File List",
			"This argument lists all of the files that need to be compiled by the compiler.", false, true);
		command_line_argument u_cla(argument_kind::SWITCH_ARGUMENT, "u", "usage", "Karmac Usage",
			"This argument shows the basic usage for the karmac compiler.", false, true);
		command_line_argument d_cla(argument_kind::SWITCH_ARGUMENT, "d", "debug", "Debug Mode", 
			"This argument turns on the debug mode for developing with the 'karmac -re' command.", false, true);
		command_line_argument clk_cla(argument_kind::SWITCH_ARGUMENT, "clk", "compile-libkarma", "Compile libkarma",
			"This argument should be run the first time 'karmac' is run in order to build libkarma, which (will) contain "
			"essential features for karma programs.", false, true);
		command_line_argument_list = { re_cla, l_cla, fl_cla, u_cla, d_cla, clk_cla };
		cl_parser = command_line_parser("karmac -- the reference compiler implementation for the karma programming language.",
			"Use this command to compile karma source files which can be run by the dharma-vm.",
			command_line_argument_list);
		command_line_argument_result_list = cl_parser.parse(argc, argv);
		bool re = false, fl = false, l = false, u = false, d = false, clk = false;
		vector<string> string_file_list;
		for (int i = 0; i < command_line_argument_result_list.size(); i++) {
			if (command_line_argument_result_list[i].get_present()) {
				if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "Read and Eval") {
					re = true;
					if (fl) {
						cerr << "'Read and Eval' cannot be used in conjunction with a 'File List.'\n";
						exit(1);
					}
				}
				else if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "File List") {
					fl = true;
					if (re) {
						cerr << "'Read and Eval' cannot be used in conjunction with a 'File List.'\n";
						exit(1);
					}
					string_file_list = command_line_argument_result_list[i].get_argument_list();
				}
				else if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "Karmac Usage")
					u = true;
				else if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "Karmac Logo")
					l = true;
				else if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "Debug Mode")
					d = true;
				else if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "Compile libkarma")
					clk = true;
			}
		}
		if (!re && !fl) {
			if (clk);
			else {
				cerr << "Expected either a 'Read and Eval', 'File List', or 'Compile libkarma' argument.\n";
				exit(1);
			}
		}
		if (d && !re) {
			cerr << "Debug mode is only compatible with the -re argument.\n";
			exit(1);
		}
		if (l)
			cout << cl_parser.get_logo() << '\n';
		if (u)
			cout << cl_parser.get_usage() << '\n';
		if (clk) {
			for (int i = 0; i < default_include_search_path_list.size(); i++) {
				string sd = default_include_search_path_list[i];
				file dir(sd);
				vector<file> all = search_directory(dir);
				vector<file> to_compile;
				for (int i = 0; i < all.size(); i++) {
					file f = all[i];
					if (f.is_file() && f.get_extension() == karma_lang_source_file_extension)
						to_compile.push_back(f);
				}
				for (int i = 0; i < to_compile.size(); i++)
					make_shared<compiler>(to_compile[i])->compile();
			}
		}
		if (re) {
			karmac_re kr;
			kr.re(d);
			if(!d)
				cout << "Output file: " << kr.get_output_file().get_full_name() << '\n';
		}
		else if(fl) {
			vector<file> file_list;
			for (int i = 0; i < string_file_list.size(); i++) {
				file f(string_file_list[i]);
				if (f.exists() && f.is_file());
				else {
					cerr << "File not found: " << f.get_full_name() << '\n';
					exit(1);
				}
				if (f.get_extension() != karma_lang_source_file_extension) {
					cerr << "This source file needs to have the extension '.ka:' " << string_file_list[i] << '\n';
					exit(1);
				}
				file_list.push_back(f);
			}
			for (int i = 0; i < file_list.size(); i++)
				compiler_list.push_back(make_shared<compiler>(file_list[i]));
			for (int i = 0; i < compiler_list.size(); i++)
				compiler_list[i]->compile();
		}
	}

	karmac::~karmac() {

	}

	vector<shared_ptr<compiler>> karmac::get_compiler_list() {
		return compiler_list;
	}

	karmac_re::karmac_re() : output_file(file("")) {

	}

	karmac_re::~karmac_re() {

	}

	const bool karmac_re::re(bool d) {
		string source;
		string line = "";
		while (line != "$$EOF$$") {
			cout << ">>> ";
			std::getline(std::cin, line);
			if (line != "$$EOF$$")
				source += line + "\n";
		}
		string file = "$$READ AND EVAL$$";
		lexer l(comment_remover(source).remove_comments(), file);
		shared_ptr<source_token_list> stlist = l.lex();
		if (d)
			for (source_token_list::iterator it = stlist->begin(); it < stlist->end(); it++) {
				cout << (*it)->get_tab_count() << ' ' << (*it)->get_raw_string() << ' ' << (*it)->get_column_start() << '-' <<
					(*it)->get_column_end() << ' ' << (*it)->get_line_number() << '\n';
			}
		token_analyzer t(make_shared<lexer>(l));
		t.analyze_source_token_list();
		if (t.get_error_count() > 0) {
			cout << "Errors; not generating code.";
			exit(1);
		}
		shared_ptr<root_node> root = make_shared<root_node>(make_shared<lexer>(l), make_shared<diagnostics_reporter>(l.get_source_token_list()));
		shared_ptr<parser> p = make_shared<parser>(root);
		vector<shared_ptr<statement>> stmt_list = root->parse_program(p);
		analyze_ast aa(root, { scope_kind::SCOPE_GLOBAL }, { make_shared<symbol_table>() }, root->get_statement_list());
		if (d) {
			cout << "\n============\nValidity: " << aa.perform_semantic_analysis() << "\nSize: ";
			cout << aa.get_annotated_root_node()->get_annotated_statement_list().size() << "\n============\n";
		}
		else
			aa.perform_semantic_analysis();
		if (root->get_diagnostics_reporter()->get_error_count() > 0) {
			cout << "Errors; not generating code.";
			exit(1);
		}
		generate_code gc(make_shared<analyze_ast>(aa));
		vector<string> list = gc.perform_code_generation();
		if (!d) {
			random_device rd;
			mt19937 mt(rd());
			uniform_real_distribution<double> urd(0, 1000);
			output_file = unified_includes::file("__temp__" + to_string((int) urd(mt)) + file::dot + dharma_vm_source_file_extension);
			output_file.create_file();
			output_file.write(list);
		}
		else
			for (int i = 0; i < list.size(); i++)
				cout << list[i] << '\n';
	}

	file karmac_re::get_output_file() {
		return output_file;
	}
}