#Karma-Lang Project
######The karma programming language's official Github page.
######Project homepage at [https://github.com/devloop0/karma-lang](https://github.com/devloop0/karma-lang).
----
###General Information

The karma programming language is currently under work, but as of now, the language contains:

 1. all expressions with operator precedence: addition, subtraction, multiplication, division, exponentiation (builtin), etc.
 2. Declaration of variables with a const qualifier.
 3. A builtin language construct to obtain the type of an arbitrary expression.
 4. Lists, tuples, and dictionaries builtin to the language.
 	* With all of these expressions, it contains susbcripting and slicing.
 
Obviously, there will be more to the language to come, but as of now, this is what the language contains. The karma-lang compiler currently supports/has:

 1. A lexer with detailed token positions in the source code (tab count, line and column information, and token-kind).
 2. A parser that supports all of the aforementioned features (with operator precedence parsing) and produces an abstract syntax tree (AST).
 3. A semantic analysis modules with type deduction, const-analysis, and lvalue and rvalue differentiation. The semantic analysis module produces an abstract syntax tree (AST) annotated with types.
 
Future work will include:

It was decided that for code generation, a virtual machine would be used to interpret the code from the karma programming language. (This will most likely be started in another repository to keep the two code bases independent from each other). Optimizations have not yet started, but are on the back burner after more features have been added to the language and the virtual machine has been implemented.

 1. ~~Code generation~~
 	* ~~It is currently up for debate which of the following options to chose for code generation:~~
    	1. ~~Native machine code (hardest of all, but guaranteed performance).~~
        2. ~~C code (easier, but harder in the future (convulted exception support and maybe, generic support).~~
        3. ~~VM (easiest of all, especially in the long run, but severe performance hit).~~ 
 2. Optimizations (control flow graph, SSA, etc.)
    * Additionallly, the compiler also generates repeated instructions, due to increments and decrements. The first part of the optimization module should be a rudimentary peep-hole optimization to remove duplicate instructions.
---
###Language sample

```
# A comment

var -> x = 1 # A variable (x), equal to one
var immut -> x = 1 # An immutable variable (x), equal to one

var -> aa = 1 + 2 ** 2 # Support for operator predecence parsing for expressions
var -> bb = aa *> [2.3] # [2.3] is the type of the expression inside of the bracket *> is casting the expression on the left-hand side to the to the expression on the right-hand side Note, it will be an error if there is not a type-of expression on the right-hand side.

var -> y = '2' 
y = "2"
# Both of the previous examples are strings

var -> z = true
z = false
# Both of the previous examples are booleans

var -> a = [[[ 1, 2 ]]] # A list
var -> b = <<< 1, '2' >>> # A tuple
var -> c = {{{ '1': 1, '2': 2 }}} # A dictionary

var -> n = nil # A nil variable

a[1:2:1] # Subscripting and slicing for lists, strings, and tuples (the first part is the start position, the second part the end position, and the third part is the step size). 
# "start" <= position < "end" (increment position by "step" in the aforementioned range).
a[1::1] #Positions can be omitted as well with slicing.
```
---
###Building the compiler

The karma-lang project requires the system your running to have a working C++11 compiler (`g++` or `clang++`). Currently, the project has only been tested on Linux (x86_64), but it should also work on Windows as well. Additionally, the project also needs a working version of CMake on your system and for it to be in your `$PATH` variable.

Here are the steps required for building the compiler:

 1. `cd` into the karma-lang project's directory and `mkdir` a build directory (referred to as `build/` from here on out.
 2. `cd` into the `build/` directory, and type the following command:
 	`cmake -G "Unix Makefiles" ..`
 3. Once this has been done, a `Makefile` will be generated. Then, run `make` and the your computer should automatically start building the karma-lang compiler.
 4. After this, the karma-lang executable should reside in the build directory. When you run it, you should see a prompt like this: `>>>`.
 5. You can start typing valid statements in the karma language, and the program will read them. Once you are done, on the last line, type `$$EOF$$`, and the compiler will stop reading statements, and compile the source code; it will: lex, parse, and perform semantic analysis on the strings that you have typed. Then, it will output an error message (if there were errors), and at the end, it will print the validity of the program that you entered (`0` for not valid, and `1` for valid). It will also print how many statements there were in the program typed.

Another option to compile the program would be without CMake, but it would involve manually compiling the entire source directory and linking it on your own, something along the lines of (`g++ --std=c++11 -c src/* main.cpp` and `g++ *.o -o karma`).

---
###Developer's Information

The entire compiler is written in C++; the header files are contained in the `includes/` directory and the source files are contained in the `src/` directory.

The `main.cpp` file contains all of the necessary information to use the karma-lang API. As of now, the API isn't built for use by outside program, but it can be easily modified to be used by other programs. At each stage of the karma-lang process, the source code representation of the program is preserved, and there are no destructive changes to the syntax trees themselves.

To compile the program, you need to first remove all the comments.
```c++
std::string comments_removed = karma_lang::comment_remover(source).remove_comments();
```

Then. you need to lex the source file; for diagnostic purposes, you need to provide the name of the file you wanted lexed as a string as well; this process will return an `std::shared_ptr<karma_lang::source_token_list>` type.
```c++
karma_lang::lexer karma_lang_lexer = karma_lang::lexer(comments_removed, file_name);
std::shared_ptr<karma_lang::source_token_list> stlist = karma_lang_lexer.lex(); // a temporary one is provided in the main.cpp file 
```

Then, you must feed the `std::shared_ptr<karma_lang::source_token_list>` to the token_analyzer, to make sure all the tokens that were lexed were correctly formed (no malformed identifiers).
```c++
karma_lang::token_analyzer(std::make_shared<karma_lang::lexer>(karma_lang_lexer)).analyze_source_token_list();
```

Now comes the construction of the actual parser and parsing the `karma_lang::source_token_list`. You need to construct a `karma_lang::root_node` and a `karma_lang::parser` object. (The `karma_lang::parser` object is not important, but the `karma_lang::root_node` one is).
```c++
std::shared_ptr<karma_lang::root_node> root = std::make_shared<karma_lang::root_node>(std::make_shared<karma_lang::lexer>(karma_lang_lexer), std::make_shared<karma_lang::diagnostics_reporter>(karma_lang_lexer.get_source_token_list()));
std::shared_ptr<karma_lang::parser> parse = std::make_shared<karma_lang::parser>(root);
std::vector<std::shared_ptr<karma_lang::statement>> stmt_list = root->parse_program(parse);
```

The entire abstract syntax tree (AST), is now contained within the statement node. Finally, now comes the semantic analysis.
```c++
karma_lang::analyze_ast aa(root);
aa.perform_semantic_analysis();
```

As of now, the `karma_lang::analyze_ast::perform_semantic_analysis()` method returns a boolean, and to access the abstract syntax tree (AST) annotated with types, you need to retrieve the `karma_lang::annotated_root_node` object, which will contain a `std::vector` of `karma_lang::annotated_statement` objects.
```c++
std::shared_ptr<karma_lang::annotated_root_node> ann_root_node = aa.get_annotated_root_node();
std::vector<std::shared_ptr<karma_lang::annotated_statement>> astmt_list = ann_root_node->get_annotated_statement_list();
```

More information will be added once code generation support has been added to the language.
