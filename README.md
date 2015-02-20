This is the karma language.

Once you have downloaded your repository, this is how you get the build running:
	1) Create a build directory: mkdir build
	2) Change into the build directory: cd build
	3) Create the Makefile: cmake -G "Unix Makefiles" ..
	4) Run karma: ./karma

Note: this build will only run on Linux

Once you run the program, you can type program instructions into the standard output; the program will compile the input and show the lexed output.
If correct, the program will just show the lexed output. If incorrect, the program will also show an error message with a caret and colored output (if a terminal is unsupported, it will just show garbage).

Thank you for downloading and contributing to karma-lang!
This repository is under active development; feature suggestions are welcome.

STATUS:
	Currently, the parser is working for the basic language; type detection and code generation to this basic language is being worked on.
