#include "includes/token.hpp"
#include "includes/source_token_list.hpp"
#include "includes/lex.hpp"
#include "includes/comment_remove.hpp"
#include "includes/token_analyze.hpp"
#include "includes/parse.hpp"
#include "includes/semantics.hpp"
#include "includes/code_generation.hpp"
#include "includes/file.hpp"
#include "includes/command_line_parser.hpp"
#include "includes/compiler.hpp"

using namespace karma_lang;
using namespace unified_includes;

int main(int argc, char* argv[]) {
	karmac(argc, argv);
}
