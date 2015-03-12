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
This project is under active development; feature suggestions are welcome.

STATUS:
	Currently, the parser and semantic analysis have been completed. More bugfixes to these will be coming. The next step is code-generation.*

* The next step will be code-generation. It is currently up in the air, choosing between, generating code to C, native-code (x86_64 for linux, most likely NASM if this is the option being chosen), or creating a VM to interpret the code. Once a decision has been made, the README.md file will be updated and then the coding will begin.
