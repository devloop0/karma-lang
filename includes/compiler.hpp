#ifndef KARMA_LANG_COMPILER_HPP
#define KARMA_LANG_COMPILER_HPP

#include "includes/code_generation.hpp"
#include "includes/command_line_parser.hpp"
#include "includes/comment_remove.hpp"
#include "includes/diagnostic_messages.hpp"
#include "includes/diagnostics_report.hpp"
#include "includes/file.hpp"
#include "includes/lex.hpp"
#include "includes/parse.hpp"
#include "includes/semantics.hpp"
#include "includes/source_token_list.hpp"
#include "includes/token.hpp"
#include "includes/token_analyze.hpp"
#include "includes/token_keywords.hpp"
#include "includes/unified.hpp"
#include <random>

using namespace unified_includes;
using namespace karma_lang;

using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;

namespace karma_lang {

	class compiler {
		file source_file, dest_file;
		shared_ptr<source_token_list> src_tok_list;
		vector<shared_ptr<statement>> stmt_list;
		vector<shared_ptr<annotated_statement>> astmt_list;
		vector<shared_ptr<symbol_table>> sym_table_list;
		vector<string> generated_code;
		public:
			compiler(string path);
			compiler(file f);
			~compiler();
			shared_ptr<source_token_list> get_source_token_list();
			vector<shared_ptr<statement>> get_statement_list();
			vector<shared_ptr<annotated_statement>> get_annotated_statement_list();
			vector<string> get_generated_code();
			const bool compile(vector<shared_ptr<symbol_table>> existing = { make_shared<symbol_table>() },
				vector<scope_kind> sk_list = { scope_kind::SCOPE_GLOBAL }, bool write_to_file = true);
			vector<shared_ptr<symbol_table>> get_symbol_table_list();
			file get_source_file();
			file get_dest_file();
	};

	class karmac_re {
		file output_file;
		public:
			karmac_re();
			~karmac_re();
			const bool re(bool d);
			file get_output_file();
	};

	class karmac {
		command_line_parser cl_parser;
		vector<command_line_argument> command_line_argument_list;
		vector<command_line_argument_result> command_line_argument_result_list;
		vector<shared_ptr<compiler>> compiler_list;
		public:
			karmac(int argc, char* argv[]);
			~karmac();
			vector<shared_ptr<compiler>> get_compiler_list();
	};
}

#endif